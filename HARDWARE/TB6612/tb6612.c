#include "tb6612.h"
#include "stdio.h"

void TB6612_1_Init(int arr, int psc)//��ʱ��8  CH3  PC8 �߼���ʱ��ע��
	//PD8 PD9  ���1
{
	
	GPIO_InitTypeDef GPIO_InitStructure; //����һ�����ų�ʼ���Ľṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //����һ����ʱ�жϵĽṹ��  ʱ��
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //����һ�� PWM ����Ľṹ��
	
	//ʹ��ʱ��1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //ʹ�� GPIOA ʱ�ӣ� GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);                            
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //ʹ��ͨ�ö�ʱ�� 12 ʱ��
	
	
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	
	//TB6612 ���Ʒ�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //����ģʽΪ���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); //�����������úõ� GPIO_InitStructure��������ʼ������

	GPIO_ResetBits(GPIOD, GPIO_Pin_8); //��ʼ���������ŵ͵�ƽ
	GPIO_ResetBits(GPIOD, GPIO_Pin_9); //��ʼ���������ŵ͵�ƽ

	//TB6612PWM ��ʼ��PWM�������
  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//���� 15
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz ;      //GPIO_High_Speed
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ������ GPIOB15
	TIM_CtrlPWMOutputs(TIM8, ENABLE); //�߼���ʱ��Ҫʹ��
	
 //���ò���ʼ����ʱ��TIM12 PB15 ��ӦTIM12 CH2
TIM_TimeBaseInitStrue.TIM_Period=arr; //����ģʽΪ���ϼ���ʱ�� ��ʱ���� 0 ��ʼ������ ���������� arr ʱ������ʱ�жϷ�����//�����¼���
TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ���� ����ÿһ��������ʱ��
TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //����ģʽ�� ���ϼ���
TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //һ�㲻ʹ�ã� Ĭ��TIM_CKD_DIV1

TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStrue); //���� TIM_TimeBaseInitStrue
	
TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM ģʽ 1�� ����ʱ������С
TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //�����Ч��ƽΪ���� TIM_Pulse ʱ�� ��ʱ����Ӧ IO �����Ч��ƽ
TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //ʹ�� PWM ���
TIM_OCInitTypeStrue.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ

TIM_OC3Init(TIM8, &TIM_OCInitTypeStrue); //�� TIM_OCInitTypeStrue ������ʼ����ʱ�� 12 ͨ�� 2

TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //CH2 Ԥװ��ʹ�ܣ� ʹ�ܺ�ı� TIM_Pulse(�� PWM)��ֵ������Ч�� ��ʹ�����¸�������Ч


TIM_ARRPreloadConfig(TIM8, ENABLE); //TIM12 Ԥװ��ʹ��
TIM_Cmd(TIM8, ENABLE); //ʹ�ܶ�ʱ�� TIM12  ��ʱ���ǵø�



}


/**************************************************************************
�������ܣ� ���� TIM8 ͨ�� 3 PWM ֵ   PD8 PD9
��ڲ����� PWM ֵ
���� ֵ�� ��
**************************************************************************/

//������ƺ����� ͨ�����Ʒ������ź� PWM ����ʵ�֣�

void SetPWM1(int pwm)//ǰ8.9
{
	if(pwm>=0)//pwm>=0 (BIN1, BIN2)=(0, 1) ��ת ��ʱ��
	{
	PDout(8)=0; //BIN1=0
	PDout(9)=1; //BIN2=1
	TIM8->CCR3=pwm;
	TIM_SetCompare3(TIM8, pwm);  //�ɸı�TIM_OCInitTypeStrue.TIM_Pulse
	}
	else if(pwm<0)//pwm<0 (BIN1, BIN2)=(1, 0) ��ת ˳ʱ��
	{
	PDout(8)=1; //BIN1=1   //���żǵø�
	PDout(9)=0; //BIN2=0
	TIM8->CCR3=-pwm;   //ͨ���ǵø�
	TIM_SetCompare3(TIM8, -pwm);
	}
}



/**************************************************************************
�������ܣ� TB6612_2 ��ʼ������  ���2
��ڲ����� ��ʱ��8  CH4
IN1 PA4 IN2 PD4  ��ʱ��PC9
���� ֵ�� ��
**************************************************************************/
void TB6612_2_Init(int arr, int psc)//���2
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure; //����һ�����ų�ʼ���Ľṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //����һ����ʱ�жϵĽṹ��  ʱ��
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //����һ�� PWM ����Ľṹ��
	
	//ʹ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //ʹ�� GPIOA ʱ�ӣ� GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//ʱ�ӱ����Ǵ�
                                                        //������ AHB1 ʱ����  �����Ĳο��ֲ�
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //ʹ��ͨ�ö�ʱ�� 8 ʱ��
	
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	
	//TB6612 ���Ʒ�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //����ģʽΪ���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); //�����������úõ� GPIO_InitStructure��������ʼ������

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //����ģʽΪ���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); //�����������úõ� GPIO_InitStructure��������ʼ������

	GPIO_ResetBits(GPIOA, GPIO_Pin_4); //��ʼ���������ŵ͵�ƽ
	GPIO_ResetBits(GPIOD, GPIO_Pin_4);
	
	//TB6612PWM ��ʼ��PWM�������
  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//���� 9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz ;      //GPIO_High_Speed
	GPIO_Init(GPIOC, &GPIO_InitStructure); //��ʼ������ GPIOC9
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	
	
	

 //���ò���ʼ����ʱ��TIM8 PC9 ��ӦTIM8 CH4
TIM_TimeBaseInitStrue.TIM_Period=arr; //����ģʽΪ���ϼ���ʱ�� ��ʱ���� 0 ��ʼ������ ���������� arr ʱ������ʱ�жϷ�����
//�����¼���
TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ���� ����ÿһ��������ʱ��
TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //����ģʽ�� ���ϼ���
//TB6612_Init(0xffff, 0);

TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //һ�㲻ʹ�ã� Ĭ��TIM_CKD_DIV1

TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStrue); //���� TIM_TimeBaseInitStrue
	
	TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM ģʽ 1�� ����ʱ������С

TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //�����Ч��ƽΪ���� TIM_Pulse ʱ�� ��ʱ����Ӧ IO �����Ч��ƽ
TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //ʹ�� PWM ���
TIM_OCInitTypeStrue.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
TIM_OC4Init(TIM8, &TIM_OCInitTypeStrue); //�� TIM_OCInitTypeStrue ������ʼ����ʱ�� 12 ͨ�� 4

TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); //CH2 Ԥװ��ʹ�ܣ� ʹ�ܺ�ı� TIM_Pulse(�� PWM)��ֵ������Ч�� ��ʹ�����¸�������Ч

TIM_ARRPreloadConfig(TIM8, ENABLE); //TIM12 Ԥװ��ʹ��
TIM_Cmd(TIM8, ENABLE); //ʹ�ܶ�ʱ�� TIM12  ��ʱ���ǵø�


}

/**************************************************************************
�������ܣ� ���� TIM8 ͨ�� 4 PWM ֵ
��ڲ����� PWM ֵ
���� ֵ�� ��
**************************************************************************/

//������ƺ����� ͨ�����Ʒ������ź� PWM ����ʵ�֣�

void SetPWM2(int pwm)
{
	if(pwm>=0)//pwm>=0 (BIN1, BIN2)=(0, 1) ��ת ��ʱ��
	{
	PAout(4)=0; //BIN1=0
	PDout(4)=1; //BIN2=1
	TIM8->CCR4=pwm;
	TIM_SetCompare4(TIM8, pwm);  //�ɸı�TIM_OCInitTypeStrue.TIM_Pulse
	} 
	else if(pwm<0)//pwm<0 (BIN1, BIN2)=(1, 0) ��ת ˳ʱ��
	{
	PAout(4)=1; //BIN1=1   //���żǵø�
	PDout(4)=0; //BIN2=0
	TIM8->CCR4=-pwm;   //ͨ���ǵø�
	TIM_SetCompare4(TIM8, -pwm);
	}
}

/**************************************************************************
�������ܣ� TB6612_3 ��ʼ������--���3
��ڲ����� ��ʱ��12  CH2                
IN1 PE11 IN2 PE13  ��ʱ��PB15 ���3       
���� ֵ�� ��
**************************************************************************/
void TB6612_3_Init(int arr, int psc)
{
	//����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure; //����һ�����ų�ʼ���Ľṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //����һ����ʱ�жϵĽṹ��  ʱ��
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //����һ�� PWM ����Ľṹ��
	
	//ʹ��ʱ��1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //ʹ�� GPIOA ʱ�ӣ� GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);                            
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); //ʹ��ͨ�ö�ʱ�� 12 ʱ��
	
	
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	
	//TB6612 ���Ʒ�������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //����ģʽΪ���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); //�����������úõ� GPIO_InitStructure��������ʼ������

	GPIO_ResetBits(GPIOE, GPIO_Pin_11); //��ʼ���������ŵ͵�ƽ
	GPIO_ResetBits(GPIOE, GPIO_Pin_13); //��ʼ���������ŵ͵�ƽ

	//TB6612PWM ��ʼ��PWM�������
  
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//���� 15
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz ;      //GPIO_High_Speed
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ������ GPIOB15
	//TIM_CtrlPWMOutputs(TIM8, ENABLE); �߼���ʱ��Ҫʹ��
	
 //���ò���ʼ����ʱ��TIM12 PB15 ��ӦTIM12 CH2
TIM_TimeBaseInitStrue.TIM_Period=arr; //����ģʽΪ���ϼ���ʱ�� ��ʱ���� 0 ��ʼ������ ���������� arr ʱ������ʱ�жϷ�����//�����¼���
TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //Ԥ��Ƶϵ���� ����ÿһ��������ʱ��
TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //����ģʽ�� ���ϼ���
TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //һ�㲻ʹ�ã� Ĭ��TIM_CKD_DIV1

TIM_TimeBaseInit(TIM12, &TIM_TimeBaseInitStrue); //���� TIM_TimeBaseInitStrue
	
TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM ģʽ 1�� ����ʱ������С
TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //�����Ч��ƽΪ���� TIM_Pulse ʱ�� ��ʱ����Ӧ IO �����Ч��ƽ
TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //ʹ�� PWM ���
TIM_OCInitTypeStrue.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ

TIM_OC2Init(TIM12, &TIM_OCInitTypeStrue); //�� TIM_OCInitTypeStrue ������ʼ����ʱ�� 12 ͨ�� 2

TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable); //CH2 Ԥװ��ʹ�ܣ� ʹ�ܺ�ı� TIM_Pulse(�� PWM)��ֵ������Ч�� ��ʹ�����¸�������Ч


TIM_ARRPreloadConfig(TIM12, ENABLE); //TIM12 Ԥװ��ʹ��
TIM_Cmd(TIM12, ENABLE); //ʹ�ܶ�ʱ�� TIM12  ��ʱ���ǵø�



}



/**************************************************************************
�������ܣ� ���� TIM12 ͨ�� 2 PWM ֵ
��ڲ����� PWM ֵ
���� ֵ�� ��
**************************************************************************/

//������ƺ����� ͨ�����Ʒ������ź� PWM ����ʵ�֣�

void SetPWM3(int pwm)
{
	if(pwm>=0)//pwm>=0 (BIN1, BIN2)=(0, 1) ��ת ��ʱ��
	{
	PEout(13)=0; //BIN1=0
	PEout(11)=1; //BIN2=1
	TIM12->CCR2=pwm;
	TIM_SetCompare2(TIM12, pwm);  //�ɸı�TIM_OCInitTypeStrue.TIM_Pulse
	} 
	else if(pwm<0)//pwm<0 (BIN1, BIN2)=(1, 0) ��ת ˳ʱ��
	{
	PEout(13)=1; //BIN1=1   //���żǵø�
	PEout(11)=0; //BIN2=0
	TIM12->CCR2=-pwm;   //ͨ���ǵø�
	TIM_SetCompare2(TIM12, -pwm);
	}
}
