#include "encoder.h"
#include "delay.h"
#include "stdio.h"


extern int TargetVelocity_1;//���һĿ���ٶ� //���һ ʵ���ϰ�����D2
extern PID_TypeDef motor_pid_1;//���һPID
extern int TargetVelocity_2;//���һĿ���ٶ� //���һ ʵ���ϰ�����D2
extern PID_TypeDef motor_pid_2;//���һPID
extern int TargetVelocity_3;//���һĿ���ٶ� //����� D3
extern PID_TypeDef motor_pid_3;//���һPID
int encoder1;//���һ�ļ���ֵ
float v1;
int encoder2;//���һ�ļ���ֵ
float v2;
int encoder3;//���3�ļ���ֵ
float v3;

#define ENCODER_TIM_PSC  0x0          /*��������Ƶ*/
#define ENCODER_TIM_PERIOD  65535   /*���������ֵ*/ 
#define CNT_INIT 0                  /*��������ֵ*/

#define ENCODER_RESOLUTION 11    /*������һȦ������������*/
#define ENCODER_MULTIPLE 4       /*��������Ƶ��ͨ����ʱ���ı�����ģʽ����*/
#define MOTOR_REDUCTION_RATIO 30 /*����ļ��ٱ�*/
/*���תһȦ�ܵ�������(��ʱ���ܶ�����������) = ����������������*��������Ƶ*������ٱ� */
#define TOTAL_RESOLUTION ( ENCODER_RESOLUTION*ENCODER_MULTIPLE*MOTOR_REDUCTION_RATIO ) 
#define ABS(x) ((x)>=0?(x):-(x))


 float Encoder_Get_CNT(u8 TIMX)
{
  float encoder_CNT;
	switch(TIMX)
	{
		case 1:
			encoder_CNT = (short)TIM1 -> CNT;
			TIM1 -> CNT = 0;
			break;
		case 2:
			encoder_CNT = (short)TIM2 -> CNT;
			TIM2 -> CNT = 0;
			break;
		case 3:
			encoder_CNT = (short)TIM3 -> CNT;
			TIM3 -> CNT = 0;
			break;
		case 4:
			encoder_CNT = (short)TIM4 -> CNT;
			TIM4 -> CNT = 0;
			break;
		case 5:
			encoder_CNT = (short)TIM5 -> CNT;
			TIM5 -> CNT = 0;
			break;
		case 6:
			encoder_CNT = (short)TIM6 -> CNT;
			TIM6 -> CNT = 0;
			break;
		case 7:
			encoder_CNT = (short)TIM7 -> CNT;
			TIM7 -> CNT = 0;
			break;
		case 8:
			encoder_CNT = (short)TIM8 -> CNT;
			TIM8 -> CNT = 0;
			break;
		case 9:
			encoder_CNT = (short)TIM9 -> CNT;
			TIM9 -> CNT = 0;
			break;
	}
    return encoder_CNT;
}


float calc_motor_rotate_speed(u8 TIMX)
{
	int encoderNum = 0;
	float rotateSpeed = 0;
	
	/*��ȡ��������ֵ������������ת����*/
	encoderNum = Encoder_Get_CNT(TIMX);//�����ı�������ֵ
	/* ת��(1����ת����Ȧ)=��λʱ���ڵļ���ֵ/�ֱܷ���*ʱ��ϵ�� */
	rotateSpeed = (float)encoderNum/(TOTAL_RESOLUTION*0.1); 
	
	return rotateSpeed;
}


	/**************************************************************************
�������ܣ� һ�ŵ���ı�������ȡ��ջ�������TIM5 CH1--PA0 CH2--PA1)
��ڲ����� ��
���� ֵ��  ��
**************************************************************************/
void TIM2_ENCODER_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*ʱ��*/
	TIM_ICInitTypeDef TIM_ICInitStruct;          /*����ͨ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   /*ʹ�ܶ�ʱ��ʱ�� APB1*/
	TIM_DeInit(TIM2);  //ʹ��ʱ�Ӻ���
    /*GPIO��ʼ��*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); /*ʹ��GPIOʱ�� AHB1*/                    
	GPIO_StructInit(&GPIO_InitStruct);        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF;         
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
	

	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC  ;       /*Ԥ��Ƶ */        
	TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*����(��װ��ֵ)*/ 
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up ;  /*�������ϼ���ģʽ*/    
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct); 
 
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
	        
	TIM_ICInitStruct.TIM_ICFilter = 10;   /*����ͨ�����˲�����*/
	TIM_ICStructInit(&TIM_ICInitStruct); //��������Ĭ�ϵĲ���
	TIM_ICInit(TIM2, &TIM_ICInitStruct); /*����ͨ����ʼ��*/
	

	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); /*�ж�ʹ��*/
	TIM_ClearFlag(TIM2,TIM_IT_Update);   /*�жϱ�־��0*/
	TIM_SetCounter(TIM2, CNT_INIT);      /*CNT���ֵ*/ 
	TIM_Cmd(TIM2,ENABLE);                
		

	
}
	

	
/**************************************************************************
�������ܣ� ���ŵ���ı�������ȡ��ջ�������TIM4 CH1--PD12 CH2--PD13)
��ڲ����� ��
���� ֵ��  ��
**************************************************************************/
void TIM4_ENCODER_Init(void)	//���ñ�����ģʽ   
{ 
	GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*ʱ��*/
	TIM_ICInitTypeDef TIM_ICInitStruct;          /*����ͨ��*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   /*ʹ�ܶ�ʱ��ʱ�� APB1*/
	TIM_DeInit(TIM4);  //ʹ��ʱ�Ӻ���
    /*GPIO��ʼ��*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); /*ʹ��GPIOʱ�� AHB1*/                    
	GPIO_StructInit(&GPIO_InitStruct);        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; 
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF;         
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	
	/*ʱ����ʼ��*/
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC  ;       /*Ԥ��Ƶ */        
	TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*����(��װ��ֵ)*/ 
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up ;  /*�������ϼ���ģʽ*/    
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct); 

	/*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/  
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
	        
	TIM_ICInitStruct.TIM_ICFilter = 10;   /*����ͨ�����˲�����*/
	TIM_ICStructInit(&TIM_ICInitStruct); //��������Ĭ�ϵĲ���
	TIM_ICInit(TIM4, &TIM_ICInitStruct); /*����ͨ����ʼ��*/
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); /*�ж�ʹ��*/
	TIM_ClearFlag(TIM4,TIM_IT_Update);   /*�жϱ�־��0*/
	TIM_SetCounter(TIM4, CNT_INIT);      /*CNT���ֵ*/ 
	TIM_Cmd(TIM4,ENABLE);                /*ʹ��CR�Ĵ���*/
}

/**************************************************************************
�������ܣ� TIM4 �жϷ�����
��ڲ����� ��
���� ֵ�� ��
**************************************************************************/

void EncoderRead_TIM5(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //����һ����ʱ�жϵĽṹ��
	NVIC_InitTypeDef NVIC_InitStrue; //����һ���ж����ȼ���ʼ���Ľṹ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʹ��ͨ�ö�ʱ�� 2 ʱ��
	TIM_TimeBaseInitStrue.TIM_Period=arr;
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ���� ����ÿһ��������ʱ��
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //����ģʽ�� ���ϼ���
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //һ�㲻ʹ�ã� Ĭ��TIM_CKD_DIV1
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStrue); //���� TIM_TimeBaseInitStrue�Ĳ�����ʼ����ʱ�� TIM4
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE); //ʹ�ܶ�ʱ�� TIM4
	NVIC_InitStrue.NVIC_IRQChannel=TIM5_IRQn; //���� TIM2 �ж�
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1; //��ռ���ȼ�Ϊ 1 ���� ֵԽС���ȼ�Խ�ߣ� 0 �����ȼ����
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1; //��Ӧ���ȼ�Ϊ 1 ���� ֵԽС���ȼ�Խ�ߣ� 0 �����ȼ����
  NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE; //�ж�ʹ��
	NVIC_Init(&NVIC_InitStrue); //���� NVIC_InitStrue �Ĳ�����ʼ�� VIC �Ĵ����� ���� TIM4 �ж�
}

void TIM5_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM5, TIM_IT_Update)==1) //�������ж�ʱ״̬�Ĵ���(TIMx_SR)�� bit0 �ᱻӲ���� 1
		{
			encoder1=-(short) Encoder_Get_CNT(2);TIM_SetCounter(TIM2,0);
			encoder2=-(short) Encoder_Get_CNT(4);TIM_SetCounter(TIM4,0);
			encoder3=-(short) Encoder_Get_CNT(1);TIM_SetCounter(TIM1,0);
			//(int)((int16_t)//������ģʽ��ͨ����ʱ�� Encoder_Get_CNT(4)
			v1=encoder1/(TOTAL_RESOLUTION*0.01);
			v2=encoder2/(TOTAL_RESOLUTION*0.01);
			v3=encoder3/(TOTAL_RESOLUTION*0.01);
			PID_Incremental_PID_Calculation(&motor_pid_1,encoder1 ,TargetVelocity_1 );//���һ��Ŀ���ٶ�
			PID_Incremental_PID_Calculation(&motor_pid_2,encoder2 ,TargetVelocity_2 );//�������Ŀ���ٶ�
			PID_Incremental_PID_Calculation(&motor_pid_3,encoder3 ,TargetVelocity_3 );//�������Ŀ���ٶ�
			SetPWM1(motor_pid_1.Output); //���� PWM
			SetPWM2(motor_pid_2.Output); //���� PWM
			SetPWM3(motor_pid_3.Output); //���� PWM
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //״̬�Ĵ���(TIMx_SR)�� bit0�� 0
		}	
	}

/**************************************************************************
�������ܣ� ���ŵ���ı�������ȡ��ջ�������TIM1 CH1--PA8 CH2--PA9)
��ڲ����� ��
���� ֵ��  ��
**************************************************************************/
void TIM1_ENCODER_Init(void)	//���ñ�����ģʽ   
{ 
	GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*ʱ��*/
	TIM_ICInitTypeDef TIM_ICInitStruct;          /*����ͨ��*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   /*ʹ�ܶ�ʱ��ʱ�� APB2*/
	TIM_DeInit(TIM1);  //ʹ��ʱ�Ӻ���
    /*GPIO��ʼ��*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); /*ʹ��GPIOʱ�� AHB1*/                    
	GPIO_StructInit(&GPIO_InitStruct);        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF;         
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
	
	/*ʱ����ʼ��*/
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC  ;       /*Ԥ��Ƶ */        
	TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*����(��װ��ֵ)*/ 
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up ;  /*�������ϼ���ģʽ*/  
  TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;//�ظ���������  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct); 
	
	TIM_ClearFlag(TIM1,TIM_IT_Update);   /*�жϱ�־��0*/
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); /*�ж�ʹ��*/   //|TIM_IT_Trigger 

	/*������ģʽ���ã�ͬʱ����ͨ��1��ͨ��2(��4��Ƶ)�����Ծ�ΪRising*/  
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
	        
	TIM_ICInitStruct.TIM_ICFilter = 10;   /*����ͨ�����˲�����*/
	TIM_ICStructInit(&TIM_ICInitStruct); //��������Ĭ�ϵĲ���
	TIM_ICInit(TIM1, &TIM_ICInitStruct); /*����ͨ����ʼ��*/
	
	
	
	
	TIM_SetCounter(TIM1, CNT_INIT);      /*CNT���ֵ*/ 
	TIM_Cmd(TIM1,ENABLE);                /*ʹ��CR�Ĵ���*/
	

}

/**************************************************************************
�������ܣ� TIM1 �жϷ�����
��ڲ����� ��
���� ֵ�� ��
**************************************************************************/

