/**
  ******************************************************************************
  * @file    GPIO/IOToggle/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "usart.h"
uint8_t Rx1_DATA0,Rx1_DATA1,Rx1_DATA2,Rx1_DATA3,Rx1_DATA4,Rx1_DATA5,Rx1_DATA6,Rx1_DATA7;
int CAN_Speedflag = 0;
uint16_t CANRx_ID;

void CAN1_RX1_IRQHandler(void)
{

  CanRxMsg RxMessage1;
  CAN_Receive(CAN1,CAN_FIFO1, &RxMessage1);  
  CANRx_ID =RxMessage1.StdId;
  Rx1_DATA0=RxMessage1.Data[0];
  Rx1_DATA1=RxMessage1.Data[1]; 
  Rx1_DATA2=RxMessage1.Data[2];
  Rx1_DATA3=RxMessage1.Data[3]; 
  Rx1_DATA4=RxMessage1.Data[4]; 

  CAN_ClearITPendingBit(CAN1,CAN_IT_FMP1);  /* Çå³ý¹ÒÆðÖÐ¶Ï */
  CAN_Speedflag = 2;
 //CanRxFlag = ENABLE;
 // printf("receive %#x,%d,%d,%d,%d\n", CANRx_ID,Rx1_DATA0,Rx1_DATA1,Rx1_DATA2,Rx1_DATA3);
}


 
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
//void SVC_Handler(void)
//{
//}
// 
void DebugMon_Handler(void)
{
}
 
//void PendSV_Handler(void)
//{
//}
 
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
