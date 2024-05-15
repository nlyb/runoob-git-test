#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<ctype.h>
typedef struct token {
    enum {
        VAR, NUM, OP
    } type;
    char str[32];
} Token;
Token tokens[1024];
typedef struct value {
    enum {
        INT,
        FLOAT,
        ERROR
    } type;
    union {
        int iVal;
        double fVal;
    } val;
} Value;
typedef struct assignment {
    char name[32];
    Value val;
} Assignment;
Assignment vars[128];
int varsLen = 0;
bool isInt(char num[]);
bool isFloat(char num[]);
bool isOp(const char str[]);
bool isAssignment(char var[]);
int findIndex(char var[]);
Value eval(int l, int r);
bool check_parentheses(int l, int r);
bool check_minus(int l, int r);
int getop(int l, int r);
Value meetValue(Value v1, Value v2, int op);
Value minus(Value v);
int findEqualIndex(int l, int r);
void save(Assignment var, Value val);
Value evalAssign(int l, int r);
void printValue(Value v);
int main() {
    int n = 0;
    while (scanf("%s", tokens[n++].str) != EOF) {
        if (isOp(tokens[n - 1].str)) tokens[n - 1].type = OP;
        else if (isInt(tokens[n - 1].str) || isFloat(tokens[n - 1].str)) tokens[n - 1].type = NUM;
        else if (isAssignment(tokens[n - 1].str) && !isInt(tokens[n - 1].str)) tokens[n - 1].type = VAR;
        char c = (char) getchar();
        if (c == '\n') {
            printValue(evalAssign(0, n - 1));
            n = 0;
        }
    }
    return 0;
}
bool isInt(char num[]) {
    int len = (int) strlen(num);
    if (len > 1 && num[0] == '0') return false;
    for (int i = 0; i < len; i++) 
	{
        if (isdigit(num[i]) == 0) return false;
    }
    return true;
}
bool isFloat(char num[]) {
    int len = (int) strlen(num);
    if (strchr(num, '.') == NULL || num[0] == '.' || num[len - 1] == '.') return false;
    int count = 0;
    for (int i = 0; i < len; i++) {
        if (num[i] == '.') count++;
    }
    if (num[0] == '0') {
        if (num[1] != '.') return false;
    }
    if (count == 1) return true;
    else return false;
}
bool isOp(const char str[]) {//"+" "-" "*" "/"
    char op = str[0];
    if (op == '+' || op == '-' || op == '*' || op == '/') return true;
    return false;
}
bool isAssignment(char var[]) {//用于赋值的变量名是否合法
    int len = (int) strlen(var);
    if (var[0] >= '0' && var[0] <= '9') return false;//不能以数字开头
    return (len == strspn(var, "_0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"));
}
int findIndex(char var[]) {//找变量名的索引，找不到就返回-1
    int index = -1;
    for (int i = 0; i < varsLen; i++) {
        if (strcmp(vars[i].name, var) == 0) index = i;
    }
    return index;
}
Value eval(int l, int r) {
    Value error;
    error.type = ERROR;
    Value res;
    if (l > r) return error;
    else if (l == r) {
        if (tokens[l].type == OP) return error;
        int index = findIndex(tokens[l].str);
        if (index != -1) return vars[index].val;
        else if (isFloat(tokens[l].str)) {
            double d;
            sscanf(tokens[l].str, "%lf", &d);
            res.type = FLOAT;
            res.val.fVal = d;
            return res;
        }
        else if (isInt(tokens[l].str)) {
            int i;
            sscanf(tokens[l].str, "%d", &i);
            res.type = INT;
            res.val.iVal = i;
            return res;
        }
        else return error;
    }
    else if (check_parentheses(l, r)) {
        return eval(l + 1, r - 1);//去除两侧括号
    }
    else if (check_minus(l, r)) {
        return minus(eval(l + 1, r));//解引用负号
    }
    else {
        int op = getop(l, r);//如果找不到也要返回error
        if (op == -1) return error;
        Value v1 = eval(l, op - 1);
        Value v2 = eval(op + 1, r);
        return meetValue(v1, v2, op);
    }
}
bool check_parentheses(int l, int r) {//判断是否需要去掉表达式两侧的括号
    //最左边、最右边有一个不是括号就返回false
    if (strcmp(tokens[l].str, "(") != 0 || strcmp(tokens[r].str, ")") != 0) return false;
    int top = 0;
    for (int i = l; i <= r; i++) {//去掉最左边最右边括号如果左右不匹配返回false
        if (strcmp(tokens[l].str, "(") == 0) {
            top++;
            for (int j = i + 1; j <= r; j++) {
                if (strcmp(tokens[j].str, "(") == 0) top++;
                if (strcmp(tokens[j].str, ")") == 0) top--;
                if (top == 0) return (j == r) ? true : false;
            }
        }
    }
    return false;
}
bool check_minus(int l, int r) {//判断是否需要解引用负号
    if (strcmp(tokens[l].str, "-") != 0) {
        return false;//不是以 - 开头 返回false
    }
    int op = getop(l, r);//扫描所有token，尝试寻找主运算符，找到返回false op == -1:找不到返回true
    return (op == -1) ? true : false;
}
Value minus(Value v) {//判断出来需要解引用负号后加上负号
    v.val.iVal = -v.val.iVal;
    v.val.fVal = -v.val.fVal;
    return v;
}
int getop(int l, int r) {//找主运算符的索引
    int op = -1;
    int level = 2;
    int top = 0;//模拟栈顶指针 遇到(++ 遇到)--:top>0表示在一对括号中
    for (int i = l; i <= r; i++) {
        if (strcmp(tokens[i].str, "(") == 0) {
            top++;
            for (int j = i + 1; j <= r; j++) {
                if (strcmp(tokens[j].str, "(") == 0) top++;
                if (strcmp(tokens[j].str, ")") == 0) top--;
                if (top == 0) {//遍历过一对括号
                    i = j;
                    break;
                }
            }
        }
        if (strcmp(tokens[i].str, "+") == 0) op = i;
        if (strcmp(tokens[i].str, "-") == 0) {//不是负号就是减号
            if (i == l || strcmp(tokens[i - 1].str, "(") == 0 || strcmp(tokens[i - 1].str, "+") == 0 || strcmp(tokens[i - 1].str, "-") == 0
            || strcmp(tokens[i - 1].str, "*") == 0 || strcmp(tokens[i - 1].str, "/") == 0) {
                continue;
            }
            else op = i;
        }
        if (strcmp(tokens[i].str, "+") == 0 || strcmp(tokens[i].str, "-") == 0) level = 1;
        if (level == 2) {
            if (strcmp(tokens[i].str, "*") == 0 || strcmp(tokens[i].str, "/") == 0) op = i;
        }
    }
    return op;
}
int findEqualIndex(int l, int r) {
    for (int i = l; i <= r; i++) {
        if (strcmp(tokens[i].str, "=") == 0) return i;
    }
    return -1;
}
void save(Assignment var, Value val) {
    for (int i = 0; i < varsLen; i++) {
        if (strcmp(var.name, vars[i].name) == 0) {//更新赋值
            vars[i].val = val;
            break;
        }
    }
    var.val = val;
    vars[varsLen++] = var;
}
Value evalAssign(int l, int r) {//统一处理了赋值、计算两种语句
    Value error;
    error.type = ERROR;
    int firstEqualIndex = findEqualIndex(l, r);
    if (firstEqualIndex != -1) {//赋值语句
        Assignment var;
        if (!isAssignment(tokens[l].str)) return error;
        if (strcmp(tokens[firstEqualIndex + 2].str, "=") != 0 && tokens[firstEqualIndex + 1].type == VAR && tokens[firstEqualIndex + 2].type == OP
        && tokens[firstEqualIndex + 3].type == VAR && tokens[r].type == NUM) return error;
        Value val = evalAssign(l + 2, r);
        //要初始化var
        strcpy(var.name, tokens[l].str);
        save(var, val);
        return val;
    }
    return eval(l, r);
}
Value meetValue(Value v1, Value v2, int op) {
    if (v1.type == ERROR || v2.type == ERROR) {
        Value error;
        error.type = ERROR;
        return error;
    }
    if (v1.type != v2.type) {//类型提升（强制类型转换）
        if (v1.type == INT) {
            v1.type = FLOAT;
            v1.val.fVal = (double) v1.val.iVal;
        }
        else {
            v2.type = FLOAT;
            v2.val.fVal = (double) v2.val.iVal;
        }
    }
    char c = tokens[op].str[0];
    Value res = v1;
    switch (c) {
        case '+': {
            if (res.type == INT) res.val.iVal += v2.val.iVal;
            else res.val.fVal += v2.val.fVal;
            break;
        }
        case '-': {
            if (res.type == INT) res.val.iVal -= v2.val.iVal;
            else res.val.fVal -= v2.val.fVal;
            break;
        }
        case '*': {
            if (res.type == INT) res.val.iVal *= v2.val.iVal;
            else res.val.fVal *= v2.val.fVal;
            break;
        }
        case '/': {
            if (res.type == INT) res.val.iVal /= v2.val.iVal;
            else res.val.fVal /= v2.val.fVal;
            break;
        }
        default : break;
    }
    return res;
}
void printValue(Value v) {
    switch (v.type) {
        case INT: {
            printf("%d\n", v.val.iVal);
            break;
        }
        case FLOAT: {
            printf("%f\n", v.val.fVal);
            break;
        }
        case ERROR: {
            printf("Error\n");
            break;
        }
    }
}
