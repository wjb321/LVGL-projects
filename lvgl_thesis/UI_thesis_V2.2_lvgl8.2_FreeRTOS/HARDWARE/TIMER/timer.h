#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM1_Int_Init(int arr, int psc, int timer_enable_flag);
void TIM5_Int_Init(u16 arr,u16 psc);
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM3_PWM_Init(u16 arr,u16 psc);
void TIM4_EncoderMode_Config(int Period);
void TIM2_Int_Init(u16 arr,u16 psc, u8 timer_enable_flag);
float TSpeed(int arr, int psc );
float Mspeed(int arr, int psc);
int read_encoder(void); //static 
void TIM6_Int_Init(u16 arr,u16 psc, u8 timer_enable_flag);

#endif
