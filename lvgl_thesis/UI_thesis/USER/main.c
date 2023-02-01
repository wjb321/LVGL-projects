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
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_apps\demo\demo.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_1.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_2.h"
#include "gui_app.h"


int arrValue = 199;
int pscValue = 71;
int Pulse = 62;
int led0pwmval=70;
int TIM1_Enable = ENABLE;
int SpeedDecrease = 0;
int main(void)
{	 
  vu8 key=0;
  u8 dir=1;	
	delay_init();	    	 		//延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为115200
 	LED_Init();			     		//LED端口初始化
	KEY_Init();	 						//按键初始化
	BEEP_Init();						//蜂鸣器初始化
	TIM5_Int_Init(999,71);	//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
	FSMC_SRAM_Init();				//外部1MB的sram初始化
	LCD_Init();							//LCD初始化			
 	tp_dev.init();					//触摸屏初始化
	TIM1_Int_Init(6999, 7199, TIM1_Enable);
	TIM2_Int_Init(arrValue,pscValue,ENABLE); 
  TIM3_PWM_Init(1399,0);	 //不分频。PWM频率=72000000/900=80Khz
	TIM4_EncoderMode_Config(Pulse);
	
	lv_init();							//lvgl系统初始化
	lv_port_disp_init();		//lvgl显示接口初始化
	lv_port_indev_init();		//lvgl输入接口初始化
	
	
	gui_app_start(); 	//运行例程
	
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
		 key=KEY_Scan(0);	//得到键值
      if(key)
        {
          switch(key)
            {
            case KEY2_PRES: // decelerate
							  SpeedDecrease = 40;
//							led0pwmval+= 20;
//						  if(led0pwmval >=1079) led0pwmval =1079;
//						  //else if (74 <=led0pwmval && led0pwmval<= 83) led0pwmval = 84;
//						  printf("led0pwmval %d \r\n", led0pwmval);
              break;

            case KEY1_PRES:  // decelerate
							  SpeedDecrease = 20;
//							led0pwmval +=40;
//						  if(led0pwmval >=1079) led0pwmval =1079;
//						  printf("led0pwmval %d \r\n", led0pwmval);
              break;

            case KEY0_PRES:  // celerate
							TIM_Cmd(TIM1, ENABLE); 
//							led0pwmval -=20;
//						  if(led0pwmval <=30) led0pwmval =30;
//						  printf("led0pwmval %d \r\n", led0pwmval);
              break;
						 
						case WKUP_PRES:  // mid high
							TIM_Cmd(TIM1, DISABLE); 
//						 led0pwmval -=40;
//						 if(led0pwmval <=30) led0pwmval =30;
//						 printf("led0pwmval %d \r\n", led0pwmval);
              break;
            }
        }
				TIM_SetCompare2(TIM3,led0pwmval);
				if(led0pwmval <= 30) led0pwmval =1079;

				
	}
}


