#include "encoder.h"
#include "delay.h"
#include "stdio.h"


extern int TargetVelocity_1;//电机一目标速度 //电机一 实际上板子上D2
extern PID_TypeDef motor_pid_1;//电机一PID
extern int TargetVelocity_2;//电机一目标速度 //电机一 实际上板子上D2
extern PID_TypeDef motor_pid_2;//电机一PID
extern int TargetVelocity_3;//电机一目标速度 //电机三 D3
extern PID_TypeDef motor_pid_3;//电机一PID
int encoder1;//电机一的计数值
float v1;
int encoder2;//电机一的计数值
float v2;
int encoder3;//电机3的计数值
float v3;

#define ENCODER_TIM_PSC  0x0          /*计数器分频*/
#define ENCODER_TIM_PERIOD  65535   /*计数器最大值*/ 
#define CNT_INIT 0                  /*计数器初值*/

#define ENCODER_RESOLUTION 11    /*编码器一圈的物理脉冲数*/
#define ENCODER_MULTIPLE 4       /*编码器倍频，通过定时器的编码器模式设置*/
#define MOTOR_REDUCTION_RATIO 30 /*电机的减速比*/
/*电机转一圈总的脉冲数(定时器能读到的脉冲数) = 编码器物理脉冲数*编码器倍频*电机减速比 */
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
	
	/*读取编码器的值，正负代表旋转方向*/
	encoderNum = Encoder_Get_CNT(TIMX);//读到的编码器的值
	/* 转速(1秒钟转多少圈)=单位时间内的计数值/总分辨率*时间系数 */
	rotateSpeed = (float)encoderNum/(TOTAL_RESOLUTION*0.1); 
	
	return rotateSpeed;
}


	/**************************************************************************
函数功能： 一号电机的编码器读取与闭环反馈（TIM5 CH1--PA0 CH2--PA1)
入口参数： 无
返回 值：  无
**************************************************************************/
void TIM2_ENCODER_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*时基*/
	TIM_ICInitTypeDef TIM_ICInitStruct;          /*输入通道*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   /*使能定时器时钟 APB1*/
	TIM_DeInit(TIM2);  //使能时钟函数
    /*GPIO初始化*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); /*使能GPIO时钟 AHB1*/                    
	GPIO_StructInit(&GPIO_InitStruct);        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; 
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF;         
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM2);
	

	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC  ;       /*预分频 */        
	TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*周期(重装载值)*/ 
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up ;  /*连续向上计数模式*/    
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStruct); 
 
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
	        
	TIM_ICInitStruct.TIM_ICFilter = 10;   /*输入通道的滤波参数*/
	TIM_ICStructInit(&TIM_ICInitStruct); //这里是用默认的参数
	TIM_ICInit(TIM2, &TIM_ICInitStruct); /*输入通道初始化*/
	

	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); /*中断使能*/
	TIM_ClearFlag(TIM2,TIM_IT_Update);   /*中断标志清0*/
	TIM_SetCounter(TIM2, CNT_INIT);      /*CNT设初值*/ 
	TIM_Cmd(TIM2,ENABLE);                
		

	
}
	

	
/**************************************************************************
函数功能： 二号电机的编码器读取与闭环反馈（TIM4 CH1--PD12 CH2--PD13)
入口参数： 无
返回 值：  无
**************************************************************************/
void TIM4_ENCODER_Init(void)	//配置编码器模式   
{ 
	GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*时基*/
	TIM_ICInitTypeDef TIM_ICInitStruct;          /*输入通道*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   /*使能定时器时钟 APB1*/
	TIM_DeInit(TIM4);  //使能时钟函数
    /*GPIO初始化*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); /*使能GPIO时钟 AHB1*/                    
	GPIO_StructInit(&GPIO_InitStruct);        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13; 
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF;         
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_Init(GPIOD, &GPIO_InitStruct); 
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);
	
	/*时基初始化*/
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC  ;       /*预分频 */        
	TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*周期(重装载值)*/ 
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up ;  /*连续向上计数模式*/    
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStruct); 

	/*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/  
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
	        
	TIM_ICInitStruct.TIM_ICFilter = 10;   /*输入通道的滤波参数*/
	TIM_ICStructInit(&TIM_ICInitStruct); //这里是用默认的参数
	TIM_ICInit(TIM4, &TIM_ICInitStruct); /*输入通道初始化*/
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); /*中断使能*/
	TIM_ClearFlag(TIM4,TIM_IT_Update);   /*中断标志清0*/
	TIM_SetCounter(TIM4, CNT_INIT);      /*CNT设初值*/ 
	TIM_Cmd(TIM4,ENABLE);                /*使能CR寄存器*/
}

/**************************************************************************
函数功能： TIM4 中断服务函数
入口参数： 无
返回 值： 无
**************************************************************************/

void EncoderRead_TIM5(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体
	NVIC_InitTypeDef NVIC_InitStrue; //定义一个中断优先级初始化的结构体
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //使能通用定时器 2 时钟
	TIM_TimeBaseInitStrue.TIM_Period=arr;
	TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数， 决定每一个计数的时长
	TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数模式： 向上计数
	TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //一般不使用， 默认TIM_CKD_DIV1
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStrue); //根据 TIM_TimeBaseInitStrue的参数初始化定时器 TIM4
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE); //使能定时器 TIM4
	NVIC_InitStrue.NVIC_IRQChannel=TIM5_IRQn; //属于 TIM2 中断
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority=1; //抢占优先级为 1 级， 值越小优先级越高， 0 级优先级最高
	NVIC_InitStrue.NVIC_IRQChannelSubPriority=1; //响应优先级为 1 级， 值越小优先级越高， 0 级优先级最高
  NVIC_InitStrue.NVIC_IRQChannelCmd=ENABLE; //中断使能
	NVIC_Init(&NVIC_InitStrue); //根据 NVIC_InitStrue 的参数初始化 VIC 寄存器， 设置 TIM4 中断
}

void TIM5_IRQHandler(void)
{	
	if(TIM_GetITStatus(TIM5, TIM_IT_Update)==1) //当发生中断时状态寄存器(TIMx_SR)的 bit0 会被硬件置 1
		{
			encoder1=-(short) Encoder_Get_CNT(2);TIM_SetCounter(TIM2,0);
			encoder2=-(short) Encoder_Get_CNT(4);TIM_SetCounter(TIM4,0);
			encoder3=-(short) Encoder_Get_CNT(1);TIM_SetCounter(TIM1,0);
			//(int)((int16_t)//编码器模式的通道定时器 Encoder_Get_CNT(4)
			v1=encoder1/(TOTAL_RESOLUTION*0.01);
			v2=encoder2/(TOTAL_RESOLUTION*0.01);
			v3=encoder3/(TOTAL_RESOLUTION*0.01);
			PID_Incremental_PID_Calculation(&motor_pid_1,encoder1 ,TargetVelocity_1 );//电机一的目标速度
			PID_Incremental_PID_Calculation(&motor_pid_2,encoder2 ,TargetVelocity_2 );//电机二的目标速度
			PID_Incremental_PID_Calculation(&motor_pid_3,encoder3 ,TargetVelocity_3 );//电机三的目标速度
			SetPWM1(motor_pid_1.Output); //设置 PWM
			SetPWM2(motor_pid_2.Output); //设置 PWM
			SetPWM3(motor_pid_3.Output); //设置 PWM
			TIM_ClearITPendingBit(TIM5, TIM_IT_Update); //状态寄存器(TIMx_SR)的 bit0置 0
		}	
	}

/**************************************************************************
函数功能： 三号电机的编码器读取与闭环反馈（TIM1 CH1--PA8 CH2--PA9)
入口参数： 无
返回 值：  无
**************************************************************************/
void TIM1_ENCODER_Init(void)	//配置编码器模式   
{ 
	GPIO_InitTypeDef GPIO_InitStruct;            /*GPIO*/
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStruct; /*时基*/
	TIM_ICInitTypeDef TIM_ICInitStruct;          /*输入通道*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);   /*使能定时器时钟 APB2*/
	TIM_DeInit(TIM1);  //使能时钟函数
    /*GPIO初始化*/    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); /*使能GPIO时钟 AHB1*/                    
	GPIO_StructInit(&GPIO_InitStruct);        
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; 
	GPIO_InitStruct.GPIO_Mode =GPIO_Mode_AF;         
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;	 
	GPIO_Init(GPIOA, &GPIO_InitStruct); 
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1);
	
	/*时基初始化*/
	
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStruct);    
	TIM_TimeBaseStruct.TIM_Prescaler = ENCODER_TIM_PSC  ;       /*预分频 */        
	TIM_TimeBaseStruct.TIM_Period = ENCODER_TIM_PERIOD;       /*周期(重装载值)*/ 
	TIM_TimeBaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;      
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up ;  /*连续向上计数模式*/  
  TIM_TimeBaseStruct.TIM_RepetitionCounter = 0;//重复计数设置  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStruct); 
	
	TIM_ClearFlag(TIM1,TIM_IT_Update);   /*中断标志清0*/
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE); /*中断使能*/   //|TIM_IT_Trigger 

	/*编码器模式配置：同时捕获通道1与通道2(即4倍频)，极性均为Rising*/  
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising); 
	        
	TIM_ICInitStruct.TIM_ICFilter = 10;   /*输入通道的滤波参数*/
	TIM_ICStructInit(&TIM_ICInitStruct); //这里是用默认的参数
	TIM_ICInit(TIM1, &TIM_ICInitStruct); /*输入通道初始化*/
	
	
	
	
	TIM_SetCounter(TIM1, CNT_INIT);      /*CNT设初值*/ 
	TIM_Cmd(TIM1,ENABLE);                /*使能CR寄存器*/
	

}

/**************************************************************************
函数功能： TIM1 中断服务函数
入口参数： 无
返回 值： 无
**************************************************************************/

