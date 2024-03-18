# include <stdio.h>
# include <windows.h>
# include <malloc.h>

int main(void)
{
  int c;
  int i;
  
  nihao:
	printf("1:自动关机\n");
	printf("2:自动死机\n");//通过自动弹出打量窗口，让系统耗尽资源而死机！
	printf("请选择：");
	scanf("%d", &c);
  
	if(c==1)
	{
	  system("shutdown -s -t 60");//系统执行自动关机的DOS命令
	}
	else if(c==2)
	{
	  printf("你太坏了，我要惩罚你！\n");
	  for(i=0; i<10; i++)//10这个值表示系统将会自动弹出的窗口数（这个值过大，就会死机！）
	  system("start");//系统执行弹出窗口的DOS命令
	}
	else
	{
	  printf("你敲错了，请重新输入！\n");
	  goto nihao;
	}

  return 0;
}