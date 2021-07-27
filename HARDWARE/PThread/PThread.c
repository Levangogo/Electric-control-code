#include "PThread.h"
extern u8 VFlag;

/**************************************************************************
函数功能： 根据VFlag的值得到不同的状态
入口参数： 无
返回 值：  无
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


