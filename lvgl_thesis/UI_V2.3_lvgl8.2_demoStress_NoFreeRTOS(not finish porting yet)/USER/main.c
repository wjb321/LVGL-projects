#include "led.h"
#include "delay.h"
#include "key.h"
#include "beep.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "touch.h"
#include "timer.h"
#include "sram.h"
#include "lvgl.h"
#include "can_config.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_keypad_encoder.h"
#include "lv_demo_stress.h"

int arrValue = 199;
int pscValue = 71;
int Pulse         = 62;
int led0pwmval    = 20;
int TIM1_Enable   = ENABLE;
int SpeedDecrease = 20;
int update_chart_Array = 0;
int ConstantOrChangeSpeed = 0;
int PWMChangeFlag = 0;
int CeleOrDeceFlag = 0;
int InitPWMFlag = 0;


extern int CAN_Speedflag;
//extern lv_coord_t series1_y[POINT_COUNT];
extern uint8_t Rx1_DATA0,Rx1_DATA1,Rx1_DATA2,Rx1_DATA3,Rx1_DATA4,Rx1_DATA5,Rx1_DATA6,Rx1_DATA7;
extern int updateChart;

int main(void)
{
  delay_init();	    	 		
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  uart_init(115200);	 		
  LED_Init();			     		
  KEY_Init();	 					
  BEEP_Init();					
  FSMC_SRAM_Init();				
  CAN_ABSCONTROL_Configuration();
  TIM1_Int_Init(99, 7199, TIM1_Enable);
  TIM2_Int_Init(arrValue,pscValue,ENABLE);
  TIM3_PWM_Init(1399,0);	
  TIM5_Int_Init(999,71);	
  TIM6_Int_Init(9999,7199, ENABLE);
  TIM4_EncoderMode_Config(Pulse);

  //LCD_Init();					
  lv_init();					
  lv_port_disp_init();
  lv_port_indev_init();		
  lv_demo_stress();
  while(1)
   {
     lv_timer_handler();
     delay_ms(5);
    }
}
