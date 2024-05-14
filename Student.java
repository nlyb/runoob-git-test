package hello;

public class Student{
	int sNO;
	String sName;
	char sSex;
	int sAge;
	double sJava;
	int getSNO() {
		return sNO;
	}
	void setSNO(int sNO) {
		this.sNO = sNO;
	}
	String getSName() {
		return sName;	
	}
	void setSName(String sName) {
		this.sName = sName;
	}
	char getSSex() {
		return sSex;	
	}
	void setSsex(char sSex) {
		this.sSex = sSex;
	}
	int getSAge() {
		return sAge;	
	}
	void setSAge(int sAge) {
		this.sAge = sAge;
	}
	double getSJava() {
		return sJava;	
	}
	void setSJava(double sJava) {
		this.sJava = sJava;
	}
	void xueShengXingXi() {
		System.out.println("学号:"+sNO+" 姓名："+sName+" 性别:"+sSex+" 年龄"+sAge+" Java课程成绩"+sJava);
	}
	public static void main(String[] args) {
		Student stu = new Student();
		stu.setSNO(10609);    
		stu.setSName("罗鑫");
		stu.setSsex('男');
		stu.setSAge(18);
		stu.setSJava(70.0);
		stu.xueShengXingXi();
	}
}