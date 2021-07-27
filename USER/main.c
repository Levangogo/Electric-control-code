#include "main.h"

  PID_TypeDef motor_pid_1;//���һPID
	PID_TypeDef motor_pid_2;//���2PID
	PID_TypeDef motor_pid_3;//���3PID
	//����pid���룬�ǵ�ע��
	unsigned char i; //��������
	unsigned char Send_Count; //������Ҫ���͵����ݸ���
	extern int encoder1;
	extern int encoder2;
	extern int encoder3;
	extern float v1;//��λm/s
	extern float v2;//��λm/s
  extern float v3;//��λm/s
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init(168); //��ʱ��ʼ��
	usart6_init(115200);//openmv  ��ӦPCB usart2
	usart1_init(9600); //�������� ��ʼ��Ϊ112800  
	
	// 500��100��
  TIM13_PWM_Init(5000-1,168-1);
  TIM14_PWM_Init(5000-1,168-1);  //�ȽϺ��õ�ռ�ձ�

TIM2_ENCODER_Init();//���1
TIM4_ENCODER_Init();//���2����  
TIM1_ENCODER_Init();//���3����

TB6612_1_Init(65535-1, 0); //���1���������ʼ�� ʹ�ö�ʱ�� 12 ��0������65535
TB6612_2_Init(65535-1, 0); //���2���������ʼ�� ʹ�ö�ʱ�� 12 ��0������65535
TB6612_3_Init(65535-1, 0); //���2���������ʼ�� ʹ�ö�ʱ�� 12 ��0������65535

PID_Init(&motor_pid_1,50,30 ,20, 65535);//pid1��ʼ��  //136  30 3 65535  2��400 0.5 50 65535
PID_Init(&motor_pid_2,50,30 ,20, 65535);  //50
PID_Init(&motor_pid_3,50,30 ,20, 65535);  //50  //���  1000  10 30
EncoderRead_TIM5(33600-1, 5-1);

//���д���
while(1)  //�ж��߼�
{
	int state;
	state=PThread();
	
	switch(state)
	{
		
		case 0: Stop();break;
		
		case 1: //��Ӧ11
		{
			
			//�Ѿ�ɨ�赽������Ķ�ά��
		  TIM_SetCompare1(TIM13,500);//�򿪶��
			delay_ms(200);//��ʱʱ�䲻ȷ��
		  Send_data(6,voice_5);//������ɨ����ɣ��رղ���
			delay_ms(200);//��ʱʱ�䲻ȷ��
			TIM_SetCompare1(TIM13,100);//�رն��
			delay_ms(200);
			Go_to_Trafficlight();	
			//�ж��̵Ʒ� ������һֱ�� 
			while(1)
			{ 
				 if(VFlag!=49)
			   Stop();
				 if(VFlag==49)//�̵�
					break; 
			}	
			Go_Straight();
      Go_left();//�νӹ���Ҫ��Ҫ��ʱ����ʱ���
			//����Ҫ��Ҫ��ʱ
		Send_data(6,voice_1);	

//�ȴ�ɨ���ά��			
			while(1)
			{ 
				 if(VFlag!=51)
			   Stop();
				 if(VFlag==51)
					break; 
			}	
			
			TIM_SetCompare1(TIM13,500);//�򿪶��
			delay_ms(100);//��ʱʱ�䲻ȷ��
			
			Send_data(6,voice_3);//������ɨ����ɣ��رղ���
      delay_ms(400);//��ʱʱ�䲻ȷ��
			TIM_SetCompare1(TIM13,100);//�رն��
			delay_ms(200);
			
			Go_right();
			Go_backward();
			state=0;
			
			break;
			}//case 1
		
			
		case 2://��Ӧ12
			{
			
			//�Ѿ�ɨ�赽������Ķ�ά��
		  TIM_SetCompare1(TIM13,500);//�򿪶��
			delay_ms(200);//��ʱʱ�䲻ȷ��
		  Send_data(6,voice_5);//������ɨ����ɣ��رղ��� //��  һ������
			delay_ms(200);//��ʱʱ�䲻ȷ��
			TIM_SetCompare1(TIM13,100);//�رն��
			delay_ms(200);
			Go_to_Trafficlight();	
			//�ж��̵Ʒ� ������һֱ�� 
			while(1)
			{ 
				 if(VFlag!=49)
			   Stop();
				 if(VFlag==49)//�̵�
					break; 
			}	
			Go_Straight();
      Go_right();//�νӹ���Ҫ��Ҫ��ʱ����ʱ���  //��
			//����Ҫ��Ҫ��ʱ
		Send_data(6,voice_2);	  //��

//�ȴ�ɨ���ά��			
			while(1)
			{ 
				 if(VFlag!=50)  //��
			   Stop();
				 if(VFlag==50)  //��
					break; 
			}	
			
			TIM_SetCompare1(TIM13,500);//�򿪶��
			delay_ms(100);//��ʱʱ�䲻ȷ��
			
			Send_data(6,voice_3);//������ɨ����ɣ��رղ���  //��  ��ȡ��һ�����ϲ��رղ���
      delay_ms(400);//��ʱʱ�䲻ȷ��
			TIM_SetCompare1(TIM13,100);//�رն��
			delay_ms(200);
			
			Go_left();  //��
			Go_backward();
			state=0;
			
			break;
			}//case 2
			
			
			case 3://��Ӧ21--52
			{
			
			//�Ѿ�ɨ�赽������Ķ�ά��
		  TIM_SetCompare1(TIM14,500);//�򿪶��
			delay_ms(200);//��ʱʱ�䲻ȷ��
		  Send_data(6,voice_6);//������ɨ����ɣ��رղ��� //��  2������
			delay_ms(600);//��ʱʱ�䲻ȷ��
			TIM_SetCompare1(TIM14,100);//�رն��
			delay_ms(200);
			Go_to_Trafficlight();	
			//�ж��̵Ʒ� ������һֱ�� 
			while(1)
			{ 
				 if(VFlag!=49)
			   Stop();
				 if(VFlag==49)//�̵�
					break; 
			}	
			Go_Straight();
      Go_left();//�νӹ���Ҫ��Ҫ��ʱ����ʱ���  //��
			//����Ҫ��Ҫ��ʱ
		Send_data(6,voice_1);	  //��

//�ȴ�ɨ���ά��			
			while(1)
			{ 
				 if(VFlag!=52)  //��
			   Stop();
				 if(VFlag==52)  //��
					break; 
			}	
			
			TIM_SetCompare1(TIM14,500);//�򿪶��
			delay_ms(100);//��ʱʱ�䲻ȷ��
			
			Send_data(6,voice_4);//������ɨ����ɣ��رղ���  //��  ȡ����������
      delay_ms(400);//��ʱʱ�䲻ȷ��
			TIM_SetCompare1(TIM14,100);//�رն��
			delay_ms(200);
			
			Go_right();  //��
			Go_backward();
			state=0;
			
			break;
			}//case 3
			
			
			case 4://
			{
			
			//�Ѿ�ɨ�赽������Ķ�ά��
		  TIM_SetCompare1(TIM14,500);//�򿪶��
			delay_ms(200);//��ʱʱ�䲻ȷ��
		  Send_data(6,voice_6);//������ɨ����ɣ��رղ��� //��  2������
			delay_ms(200);//��ʱʱ�䲻ȷ��
			TIM_SetCompare1(TIM14,100);//�رն��
			delay_ms(200);
			Go_to_Trafficlight();	
			//�ж��̵Ʒ� ������һֱ�� 
			while(1)
			{ 
				 if(VFlag!=49)
			   Stop();
				 if(VFlag==49)//�̵�
					break; 
			}	
			Go_Straight();
      Go_right();//�νӹ���Ҫ��Ҫ��ʱ����ʱ���  //��
			//����Ҫ��Ҫ��ʱ
		Send_data(6,voice_2);	  //��

//�ȴ�ɨ���ά��			
			while(1)
			{ 
				 if(VFlag!=53)  //��
			   Stop();
				 if(VFlag==53)  //��
					break; 
			}	
			
			TIM_SetCompare1(TIM14,500);//�򿪶��
			delay_ms(100);//��ʱʱ�䲻ȷ��
			
			Send_data(6,voice_4);//������ɨ����ɣ��رղ���  //��  ȡ����������
      delay_ms(400);//��ʱʱ�䲻ȷ��
			TIM_SetCompare1(TIM14,100);//�رն��
			delay_ms(200);
			
			Go_left();  //��
			Go_backward();
			state=0;
			
			break;
			}//case 4
			
			default: state=0;
			
			
		}
		delay_ms(20);
		}//while(1)
  
	
	}//main
	
	
	

	



