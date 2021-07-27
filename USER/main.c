#include "main.h"

  PID_TypeDef motor_pid_1;//电机一PID
	PID_TypeDef motor_pid_2;//电机2PID
	PID_TypeDef motor_pid_3;//电机3PID
	//调节pid代码，记得注释
	unsigned char i; //计数变量
	unsigned char Send_Count; //串口需要发送的数据个数
	extern int encoder1;
	extern int encoder2;
	extern int encoder3;
	extern float v1;//单位m/s
	extern float v2;//单位m/s
  extern float v3;//单位m/s
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168); //延时初始化
	usart6_init(115200);//openmv  对应PCB usart2
	usart1_init(9600); //语音播报 初始化为112800  
	
	// 500开100关
  TIM13_PWM_Init(5000-1,168-1);
  TIM14_PWM_Init(5000-1,168-1);  //比较好用的占空比

TIM2_ENCODER_Init();//电机1
TIM4_ENCODER_Init();//电机2代码  
TIM1_ENCODER_Init();//电机3代码

TB6612_1_Init(65535-1, 0); //电机1驱动外设初始化 使用定时器 12 从0计数到65535
TB6612_2_Init(65535-1, 0); //电机2驱动外设初始化 使用定时器 12 从0计数到65535
TB6612_3_Init(65535-1, 0); //电机2驱动外设初始化 使用定时器 12 从0计数到65535

PID_Init(&motor_pid_1,50,30 ,20, 65535);//pid1初始化  //136  30 3 65535  2、400 0.5 50 65535
PID_Init(&motor_pid_2,50,30 ,20, 65535);  //50
PID_Init(&motor_pid_3,50,30 ,20, 65535);  //50  //差不多  1000  10 30
EncoderRead_TIM5(33600-1, 5-1);

//剪切代码
while(1)  //判断逻辑
{
	int state;
	state=PThread();
	
	switch(state)
	{
		
		case 0: Stop();break;
		
		case 1: //对应11
		{
			
			//已经扫描到了最初的二维码
		  TIM_SetCompare1(TIM13,500);//打开舵机
			delay_ms(200);//延时时间不确定
		  Send_data(6,voice_5);//播报：扫描完成，关闭仓门
			delay_ms(200);//延时时间不确定
			TIM_SetCompare1(TIM13,100);//关闭舵机
			delay_ms(200);
			Go_to_Trafficlight();	
			//判断绿灯否 不是则一直等 
			while(1)
			{ 
				 if(VFlag!=49)
			   Stop();
				 if(VFlag==49)//绿灯
					break; 
			}	
			Go_Straight();
      Go_left();//衔接过程要不要延时，延时多久
			//这里要不要延时
		Send_data(6,voice_1);	

//等待扫描二维码			
			while(1)
			{ 
				 if(VFlag!=51)
			   Stop();
				 if(VFlag==51)
					break; 
			}	
			
			TIM_SetCompare1(TIM13,500);//打开舵机
			delay_ms(100);//延时时间不确定
			
			Send_data(6,voice_3);//播报：扫描完成，关闭仓门
      delay_ms(400);//延时时间不确定
			TIM_SetCompare1(TIM13,100);//关闭舵机
			delay_ms(200);
			
			Go_right();
			Go_backward();
			state=0;
			
			break;
			}//case 1
		
			
		case 2://对应12
			{
			
			//已经扫描到了最初的二维码
		  TIM_SetCompare1(TIM13,500);//打开舵机
			delay_ms(200);//延时时间不确定
		  Send_data(6,voice_5);//播报：扫描完成，关闭仓门 //改  一号物料
			delay_ms(200);//延时时间不确定
			TIM_SetCompare1(TIM13,100);//关闭舵机
			delay_ms(200);
			Go_to_Trafficlight();	
			//判断绿灯否 不是则一直等 
			while(1)
			{ 
				 if(VFlag!=49)
			   Stop();
				 if(VFlag==49)//绿灯
					break; 
			}	
			Go_Straight();
      Go_right();//衔接过程要不要延时，延时多久  //改
			//这里要不要延时
		Send_data(6,voice_2);	  //改

//等待扫描二维码			
			while(1)
			{ 
				 if(VFlag!=50)  //改
			   Stop();
				 if(VFlag==50)  //改
					break; 
			}	
			
			TIM_SetCompare1(TIM13,500);//打开舵机
			delay_ms(100);//延时时间不确定
			
			Send_data(6,voice_3);//播报：扫描完成，关闭仓门  //改  请取出一号物料并关闭仓门
      delay_ms(400);//延时时间不确定
			TIM_SetCompare1(TIM13,100);//关闭舵机
			delay_ms(200);
			
			Go_left();  //改
			Go_backward();
			state=0;
			
			break;
			}//case 2
			
			
			case 3://对应21--52
			{
			
			//已经扫描到了最初的二维码
		  TIM_SetCompare1(TIM14,500);//打开舵机
			delay_ms(200);//延时时间不确定
		  Send_data(6,voice_6);//播报：扫描完成，关闭仓门 //改  2号物料
			delay_ms(600);//延时时间不确定
			TIM_SetCompare1(TIM14,100);//关闭舵机
			delay_ms(200);
			Go_to_Trafficlight();	
			//判断绿灯否 不是则一直等 
			while(1)
			{ 
				 if(VFlag!=49)
			   Stop();
				 if(VFlag==49)//绿灯
					break; 
			}	
			Go_Straight();
      Go_left();//衔接过程要不要延时，延时多久  //改
			//这里要不要延时
		Send_data(6,voice_1);	  //改

//等待扫描二维码			
			while(1)
			{ 
				 if(VFlag!=52)  //改
			   Stop();
				 if(VFlag==52)  //改
					break; 
			}	
			
			TIM_SetCompare1(TIM14,500);//打开舵机
			delay_ms(100);//延时时间不确定
			
			Send_data(6,voice_4);//播报：扫描完成，关闭仓门  //改  取出二号物料
      delay_ms(400);//延时时间不确定
			TIM_SetCompare1(TIM14,100);//关闭舵机
			delay_ms(200);
			
			Go_right();  //改
			Go_backward();
			state=0;
			
			break;
			}//case 3
			
			
			case 4://
			{
			
			//已经扫描到了最初的二维码
		  TIM_SetCompare1(TIM14,500);//打开舵机
			delay_ms(200);//延时时间不确定
		  Send_data(6,voice_6);//播报：扫描完成，关闭仓门 //改  2号物料
			delay_ms(200);//延时时间不确定
			TIM_SetCompare1(TIM14,100);//关闭舵机
			delay_ms(200);
			Go_to_Trafficlight();	
			//判断绿灯否 不是则一直等 
			while(1)
			{ 
				 if(VFlag!=49)
			   Stop();
				 if(VFlag==49)//绿灯
					break; 
			}	
			Go_Straight();
      Go_right();//衔接过程要不要延时，延时多久  //改
			//这里要不要延时
		Send_data(6,voice_2);	  //改

//等待扫描二维码			
			while(1)
			{ 
				 if(VFlag!=53)  //改
			   Stop();
				 if(VFlag==53)  //改
					break; 
			}	
			
			TIM_SetCompare1(TIM14,500);//打开舵机
			delay_ms(100);//延时时间不确定
			
			Send_data(6,voice_4);//播报：扫描完成，关闭仓门  //改  取出二号物料
      delay_ms(400);//延时时间不确定
			TIM_SetCompare1(TIM14,100);//关闭舵机
			delay_ms(200);
			
			Go_left();  //改
			Go_backward();
			state=0;
			
			break;
			}//case 4
			
			default: state=0;
			
			
		}
		delay_ms(20);
		}//while(1)
  
	
	}//main
	
	
	

	



