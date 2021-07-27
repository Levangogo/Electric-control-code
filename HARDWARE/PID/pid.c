#include "pid.h"

/*��ʼ��PID*/
void PID_Init(PID_TypeDef *pid, float P, float I, float D, float Limit)
{
	pid->P = P;
	pid->I = I;
	pid->D = D;
	pid->Limit = Limit;
	pid->Current_Error = 0.0;
	pid->Last_Error = 0.0;
	pid->Previous_Error = 0.0;
	pid->Output = 0.0;
	pid->Sum_Error = 0.0;
}

/*����PID*/
void PID_Clear(PID_TypeDef *pid)
{
	pid->Current_Error = 0.0;
	pid->Last_Error = 0.0;
	pid->Previous_Error = 0.0;
	pid->Output = 0.0;
	pid->Sum_Error = 0.0;
}


/*����ʽPID����*/
void PID_Incremental_PID_Calculation(PID_TypeDef *pid, float NowPlace, float Point)
{
    pid->Current_Error = Point - NowPlace;//���㵱ǰ���
    pid->Output += pid->P * (pid->Current_Error - pid->Last_Error)\
	+ pid->I * pid->Current_Error + pid->D * (pid->Current_Error - 2.0 * pid->Last_Error +\
	pid->Previous_Error);

    //����޷�
    if(pid->Output > pid->Limit) pid->Output = pid->Limit;
    else if (pid->Output < -pid->Limit) pid->Output = -pid->Limit;

    //�������
    pid->Previous_Error = pid->Last_Error;
    pid->Last_Error = pid->Current_Error;
}


/*λ��ʽPID����*/ //δ�õ�
void PID_position_PID_calculation(PID_TypeDef *pid, float NowPlace, float Point)
{
	pid->Current_Error = Point - NowPlace;//���㵱ǰ���
	pid->Sum_Error += pid->Current_Error;				 //����ۻ�
	pid->Output = pid->P * pid->Current_Error + pid->I * pid->Sum_Error + pid->D * (pid->Current_Error - pid->Last_Error);
	
	//����޷�
  if(pid->Output > pid->Limit) pid->Output = pid->Limit;
	else if (pid->Output < -pid->Limit) pid->Output = -pid->Limit;
	
//	//����
//	if(pid->Output < 300.0 && pid->Output > 0) pid->Output = 300.0;
//	else if (pid->Output > -300.0 && pid->Output < 0) pid->Output = -300.0;

	//�������
  pid->Last_Error = pid->Current_Error;
}
