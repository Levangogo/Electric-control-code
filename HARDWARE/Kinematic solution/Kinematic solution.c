#include "Kinematic solution.h"
#include "math.h"
#include "delay.h"
 int TargetVelocity_1;   // 13.2v=e   13 0.985m/s
 int TargetVelocity_2;
 int TargetVelocity_3;



/**************************************************************************
�������ܣ�ͨ��С��������ٶ����ÿ�����ӵ��ٶ�  ��ʱδ��Ͻ��ٶ�
��ڲ�����double Vx, double Vy,double W
����  ֵ����
**************************************************************************/
int calculate(float Vx, float Vy,int g)//������ֵ
{
	int V1, V2, V3,V;//��λΪm/s
	float angle=PI/6;
	V1=(int)-Vy;//-a*w;
  V2=(int)(cos(angle)*Vx+sin(angle)*Vy);  //+a*w  aΪ����
  V3=(int)(Vy*sin(angle)-Vx*cos(angle));  //-a*w
	
	switch(g)//ȡ�ٶ�
  {
  case 1: V=V1;break;
  case 2: V=V2;break;
  case 3: V=V3;break;
  default: V=0;break;
  }
  return V;

}

void Go_to_Trafficlight(void)  //ֱ�ߵ����̵�
{
	
	TargetVelocity_1=calculate(0,18,1);
	TargetVelocity_2=calculate(0,18,2);
	TargetVelocity_3=calculate(0,18,3);
	 delay_ms(700);
	Stop();
	
}	

void Stop(void)
{
  TargetVelocity_1=calculate(0,0,1);
	TargetVelocity_2=calculate(0,0,2);
	TargetVelocity_3=calculate(0,0,3);

}	

void Go_left(void)
{
	
	TargetVelocity_1=calculate(-18,0,1);
	TargetVelocity_2=calculate(-18,0,2);
	TargetVelocity_3=calculate(-18,0,3);
	delay_ms(500);
	Stop();
}

void Go_right(void)
{
	
	TargetVelocity_1=calculate(18,0,1);
	TargetVelocity_2=calculate(18,0,2);
	TargetVelocity_3=calculate(18,0,3);
	delay_ms(500);
  Stop();

}

void Go_backward(void)
{
	
  TargetVelocity_1=calculate(0,-18,1);
	TargetVelocity_2=calculate(0,-18,2);
	TargetVelocity_3=calculate(0,-18,3);
	delay_ms(910);
  Stop();


}


void Go_Straight(void)//ֱ�߹�����
	
{
	
  TargetVelocity_1=calculate(0,18,1);
	TargetVelocity_2=calculate(0,18,2);
	TargetVelocity_3=calculate(0,18,3);
	delay_ms(210);
  Stop();

}
