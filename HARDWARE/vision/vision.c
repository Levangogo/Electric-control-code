#include "vision.h"

	//OPENMV二维码接收的数据


int Judge_state(int qr)
{
		int Flag;

	if(qr/10==2&&qr%10==2)
	{
		Flag=4;
	}
	
	else if(qr/10==2&&qr%10==1)
	{
		Flag=3;
	}

	else if(qr/10==1&&qr%10==2)
	{
		Flag=2;
	}
	
	else if(qr/10==1&&qr%10==1)
	{
		Flag=1;
	}
	  return Flag;

}
	
