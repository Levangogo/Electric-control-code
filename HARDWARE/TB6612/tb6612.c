#include "tb6612.h"
#include "stdio.h"

void TB6612_1_Init(int arr, int psc)//定时器8  CH3  PC8 高级定时器注意
	//PD8 PD9  电机1
{
	
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体  时基
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //定义一个 PWM 输出的结构体
	
	//使能时钟1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //使能 GPIOA 时钟， GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);                            
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //使能通用定时器 12 时钟
	
	
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	
	//TB6612 控制方向引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //引脚模式为输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); //根据上面设置好的 GPIO_InitStructure参数，初始化引脚

	GPIO_ResetBits(GPIOD, GPIO_Pin_8); //初始化设置引脚低电平
	GPIO_ResetBits(GPIOD, GPIO_Pin_9); //初始化设置引脚低电平

	//TB6612PWM 初始化PWM输出引脚
  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;//引脚 15
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz ;      //GPIO_High_Speed
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化引脚 GPIOB15
	TIM_CtrlPWMOutputs(TIM8, ENABLE); //高级定时器要使能
	
 //设置并初始化定时器TIM12 PB15 对应TIM12 CH2
TIM_TimeBaseInitStrue.TIM_Period=arr; //计数模式为向上计数时， 定时器从 0 开始计数， 计数超过到 arr 时触发定时中断服务函数//即重新计数
TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数， 决定每一个计数的时长
TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数模式： 向上计数
TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //一般不使用， 默认TIM_CKD_DIV1

TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStrue); //根据 TIM_TimeBaseInitStrue
	
TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM 模式 1， 当定时器计数小
TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //输出有效电平为高于 TIM_Pulse 时， 定时器对应 IO 输出有效电平
TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //使能 PWM 输出
TIM_OCInitTypeStrue.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值

TIM_OC3Init(TIM8, &TIM_OCInitTypeStrue); //根 TIM_OCInitTypeStrue 参数初始化定时器 12 通道 2

TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable); //CH2 预装载使能， 使能后改变 TIM_Pulse(即 PWM)的值立刻生效， 不使能则下个周期生效


TIM_ARRPreloadConfig(TIM8, ENABLE); //TIM12 预装载使能
TIM_Cmd(TIM8, ENABLE); //使能定时器 TIM12  定时器记得改



}


/**************************************************************************
函数功能： 设置 TIM8 通道 3 PWM 值   PD8 PD9
入口参数： PWM 值
返回 值： 无
**************************************************************************/

//电机控制函数， 通过控制方向引脚和 PWM 引脚实现：

void SetPWM1(int pwm)//前8.9
{
	if(pwm>=0)//pwm>=0 (BIN1, BIN2)=(0, 1) 正转 逆时针
	{
	PDout(8)=0; //BIN1=0
	PDout(9)=1; //BIN2=1
	TIM8->CCR3=pwm;
	TIM_SetCompare3(TIM8, pwm);  //可改变TIM_OCInitTypeStrue.TIM_Pulse
	}
	else if(pwm<0)//pwm<0 (BIN1, BIN2)=(1, 0) 反转 顺时针
	{
	PDout(8)=1; //BIN1=1   //引脚记得改
	PDout(9)=0; //BIN2=0
	TIM8->CCR3=-pwm;   //通道记得改
	TIM_SetCompare3(TIM8, -pwm);
	}
}



/**************************************************************************
函数功能： TB6612_2 初始化函数  电机2
入口参数： 定时器8  CH4
IN1 PA4 IN2 PD4  定时器PC9
返回 值： 无
**************************************************************************/
void TB6612_2_Init(int arr, int psc)//电机2
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体  时基
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //定义一个 PWM 输出的结构体
	
	//使能时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能 GPIOA 时钟， GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//时钟别忘记打开
                                                        //挂载在 AHB1 时钟下  看中文参考手册
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE); //使能通用定时器 8 时钟
	
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	
	//TB6612 控制方向引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //引脚模式为输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); //根据上面设置好的 GPIO_InitStructure参数，初始化引脚

GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //引脚模式为输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); //根据上面设置好的 GPIO_InitStructure参数，初始化引脚

	GPIO_ResetBits(GPIOA, GPIO_Pin_4); //初始化设置引脚低电平
	GPIO_ResetBits(GPIOD, GPIO_Pin_4);
	
	//TB6612PWM 初始化PWM输出引脚
  
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//引脚 9
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz ;      //GPIO_High_Speed
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化引脚 GPIOC9
	TIM_CtrlPWMOutputs(TIM8, ENABLE);
	
	
	

 //设置并初始化定时器TIM8 PC9 对应TIM8 CH4
TIM_TimeBaseInitStrue.TIM_Period=arr; //计数模式为向上计数时， 定时器从 0 开始计数， 计数超过到 arr 时触发定时中断服务函数
//即重新计数
TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数， 决定每一个计数的时长
TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数模式： 向上计数
//TB6612_Init(0xffff, 0);

TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //一般不使用， 默认TIM_CKD_DIV1

TIM_TimeBaseInit(TIM8, &TIM_TimeBaseInitStrue); //根据 TIM_TimeBaseInitStrue
	
	TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM 模式 1， 当定时器计数小

TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //输出有效电平为高于 TIM_Pulse 时， 定时器对应 IO 输出有效电平
TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //使能 PWM 输出
TIM_OCInitTypeStrue.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
TIM_OC4Init(TIM8, &TIM_OCInitTypeStrue); //根 TIM_OCInitTypeStrue 参数初始化定时器 12 通道 4

TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable); //CH2 预装载使能， 使能后改变 TIM_Pulse(即 PWM)的值立刻生效， 不使能则下个周期生效

TIM_ARRPreloadConfig(TIM8, ENABLE); //TIM12 预装载使能
TIM_Cmd(TIM8, ENABLE); //使能定时器 TIM12  定时器记得改


}

/**************************************************************************
函数功能： 设置 TIM8 通道 4 PWM 值
入口参数： PWM 值
返回 值： 无
**************************************************************************/

//电机控制函数， 通过控制方向引脚和 PWM 引脚实现：

void SetPWM2(int pwm)
{
	if(pwm>=0)//pwm>=0 (BIN1, BIN2)=(0, 1) 正转 逆时针
	{
	PAout(4)=0; //BIN1=0
	PDout(4)=1; //BIN2=1
	TIM8->CCR4=pwm;
	TIM_SetCompare4(TIM8, pwm);  //可改变TIM_OCInitTypeStrue.TIM_Pulse
	} 
	else if(pwm<0)//pwm<0 (BIN1, BIN2)=(1, 0) 反转 顺时针
	{
	PAout(4)=1; //BIN1=1   //引脚记得改
	PDout(4)=0; //BIN2=0
	TIM8->CCR4=-pwm;   //通道记得改
	TIM_SetCompare4(TIM8, -pwm);
	}
}

/**************************************************************************
函数功能： TB6612_3 初始化函数--电机3
入口参数： 定时器12  CH2                
IN1 PE11 IN2 PE13  定时器PB15 电机3       
返回 值： 无
**************************************************************************/
void TB6612_3_Init(int arr, int psc)
{
	//定义结构体
	GPIO_InitTypeDef GPIO_InitStructure; //定义一个引脚初始化的结构体
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue; //定义一个定时中断的结构体  时基
	TIM_OCInitTypeDef TIM_OCInitTypeStrue; //定义一个 PWM 输出的结构体
	
	//使能时钟1
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); //使能 GPIOA 时钟， GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);                            
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); //使能通用定时器 12 时钟
	
	
//	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	
	//TB6612 控制方向引脚
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //引脚模式为输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_100MHz; 
	GPIO_Init(GPIOE, &GPIO_InitStructure); //根据上面设置好的 GPIO_InitStructure参数，初始化引脚

	GPIO_ResetBits(GPIOE, GPIO_Pin_11); //初始化设置引脚低电平
	GPIO_ResetBits(GPIOE, GPIO_Pin_13); //初始化设置引脚低电平

	//TB6612PWM 初始化PWM输出引脚
  
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_TIM12);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_15;//引脚 15
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz ;      //GPIO_High_Speed
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化引脚 GPIOB15
	//TIM_CtrlPWMOutputs(TIM8, ENABLE); 高级定时器要使能
	
 //设置并初始化定时器TIM12 PB15 对应TIM12 CH2
TIM_TimeBaseInitStrue.TIM_Period=arr; //计数模式为向上计数时， 定时器从 0 开始计数， 计数超过到 arr 时触发定时中断服务函数//即重新计数
TIM_TimeBaseInitStrue.TIM_Prescaler=psc; //预分频系数， 决定每一个计数的时长
TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up; //计数模式： 向上计数
TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1; //一般不使用， 默认TIM_CKD_DIV1

TIM_TimeBaseInit(TIM12, &TIM_TimeBaseInitStrue); //根据 TIM_TimeBaseInitStrue
	
TIM_OCInitTypeStrue.TIM_OCMode=TIM_OCMode_PWM1; //PWM 模式 1， 当定时器计数小
TIM_OCInitTypeStrue.TIM_OCPolarity=TIM_OCNPolarity_High; //输出有效电平为高于 TIM_Pulse 时， 定时器对应 IO 输出有效电平
TIM_OCInitTypeStrue.TIM_OutputState=TIM_OutputState_Enable; //使能 PWM 输出
TIM_OCInitTypeStrue.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值

TIM_OC2Init(TIM12, &TIM_OCInitTypeStrue); //根 TIM_OCInitTypeStrue 参数初始化定时器 12 通道 2

TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable); //CH2 预装载使能， 使能后改变 TIM_Pulse(即 PWM)的值立刻生效， 不使能则下个周期生效


TIM_ARRPreloadConfig(TIM12, ENABLE); //TIM12 预装载使能
TIM_Cmd(TIM12, ENABLE); //使能定时器 TIM12  定时器记得改



}



/**************************************************************************
函数功能： 设置 TIM12 通道 2 PWM 值
入口参数： PWM 值
返回 值： 无
**************************************************************************/

//电机控制函数， 通过控制方向引脚和 PWM 引脚实现：

void SetPWM3(int pwm)
{
	if(pwm>=0)//pwm>=0 (BIN1, BIN2)=(0, 1) 正转 逆时针
	{
	PEout(13)=0; //BIN1=0
	PEout(11)=1; //BIN2=1
	TIM12->CCR2=pwm;
	TIM_SetCompare2(TIM12, pwm);  //可改变TIM_OCInitTypeStrue.TIM_Pulse
	} 
	else if(pwm<0)//pwm<0 (BIN1, BIN2)=(1, 0) 反转 顺时针
	{
	PEout(13)=1; //BIN1=1   //引脚记得改
	PEout(11)=0; //BIN2=0
	TIM12->CCR2=-pwm;   //通道记得改
	TIM_SetCompare2(TIM12, -pwm);
	}
}
