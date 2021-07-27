#ifndef __ENCODER_H
#define __ENCODER_H
#include "sys.h"
#include "tb6612.h"
#include "pid.h"
extern int encoder1;
extern int encoder2;
extern int encoder3;
extern float v1;//��λm/s
extern float v2;//��λm/s
extern float v3;//��λm/s
//���һ
void TIM4_ENCODER_Init(void);  
void EncoderRead_TIM5(u16 arr, u16 psc); 
//�����
void TIM2_ENCODER_Init(void);  
void EncoderRead_TIM9(u16 arr, u16 psc);
//�����
void TIM1_ENCODER_Init(void);  
//void EncoderRead_TIM3(u16 arr, u16 psc); 

float Encoder_Get_CNT(u8 TIMX);
float calc_motor_rotate_speed(u8 TIMX); 
#endif

