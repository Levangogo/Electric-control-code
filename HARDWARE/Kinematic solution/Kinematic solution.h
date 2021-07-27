#ifndef __KINEMATIC_SOLUTION_H
#define __KINEMATIC_SOLUTION_H
#include "sys.h"

#define PI 3.1415926
extern int TargetVelocity_1;//电机一目标速度 
extern int TargetVelocity_2;//电机二目标速度 
extern int TargetVelocity_3;//电机三目标速度 //电机一 实际上板子上D2
int calculate(float Vx, float Vy,int g);
void Go_to_Trafficlight(void);
void Stop(void);
void Go_left(void);
void Go_right(void);
void Go_Straight(void);

void Go_backward(void);




#endif


