# include <stdio.h>
# include <windows.h>
# include <malloc.h>

int main(void)
{
  int c;
  int i;
  
  nihao:
	printf("1:�Զ��ػ�\n");
	printf("2:�Զ�����\n");//ͨ���Զ������������ڣ���ϵͳ�ľ���Դ��������
	printf("��ѡ��");
	scanf("%d", &c);
  
	if(c==1)
	{
	  system("shutdown -s -t 60");//ϵͳִ���Զ��ػ���DOS����
	}
	else if(c==2)
	{
	  printf("��̫���ˣ���Ҫ�ͷ��㣡\n");
	  for(i=0; i<10; i++)//10���ֵ��ʾϵͳ�����Զ������Ĵ����������ֵ���󣬾ͻ���������
	  system("start");//ϵͳִ�е������ڵ�DOS����
	}
	else
	{
	  printf("���ô��ˣ����������룡\n");
	  goto nihao;
	}

  return 0;
}