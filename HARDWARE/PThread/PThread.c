#include "PThread.h"
extern u8 VFlag;

/**************************************************************************
�������ܣ� ����VFlag��ֵ�õ���ͬ��״̬
��ڲ����� ��
���� ֵ��  ��
**************************************************************************/

int PThread(void)
{
int pthread;

	
if(VFlag==51)//11
{
    pthread=1;
}
else if(VFlag==50)//12
{
      pthread=2;
}

else if(VFlag==52)//21
{
  pthread=3;
}
	else if(VFlag==53)//22
	{
	pthread=4;
	}

	else if(VFlag==49)
	{
	  pthread=5;
	}
	
//	else
//	{
//	  pthread=0;
//	}
return pthread;
	  }


