#ifndef __TB6612_H
#define __TB6612_H
#include "sys.h"


void TB6612_1_Init(int arr, int psc);
void TB6612_2_Init(int arr, int psc);
void TB6612_3_Init(int arr, int psc);

void SetPWM1(int pwm); //���1 ��Ӧ����D2
void SetPWM2(int pwm);
void SetPWM3(int pwm); //��Ӧd3
#endif
