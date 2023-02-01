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
#include "chart_win.h"

int arrValue = 199;
int pscValue = 71;
//extern int arrValue ;
//extern int pscValue;
int Pulse = 62;
int led0pwmval=70;
int TIM1_Enable = ENABLE;
int SpeedDecrease = 0;
int speedflag = 0;
extern lv_coord_t series1_y[POINT_COUNT];
int update_chart_Array = 0;
extern int updateChart;

int main(void)
{	 
  vu8 key=0;
  u8 dir=1;	
	delay_init();	    	 		//��ʱ������ʼ��	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     		//LED�˿ڳ�ʼ��
	KEY_Init();	 						//������ʼ��
	BEEP_Init();						//��������ʼ��
	FSMC_SRAM_Init();				//�ⲿ1MB��sram��ʼ��
	LCD_Init();							//LCD��ʼ��			
 	tp_dev.init();					//��������ʼ��
	TIM1_Int_Init(2999, 7199, TIM1_Enable);
	TIM2_Int_Init(arrValue,pscValue,ENABLE); 
  TIM3_PWM_Init(1399,0);	 //����Ƶ��PWMƵ��=72000000/900=80Khz
	TIM5_Int_Init(999,71);	//��ʱ����ʼ��(1ms�ж�),���ڸ�lvgl�ṩ1ms����������
	TIM6_Int_Init(9999,7199, ENABLE);
	
	TIM4_EncoderMode_Config(Pulse);
	
	lv_init();							//lvglϵͳ��ʼ��
	lv_port_disp_init();		//lvgl��ʾ�ӿڳ�ʼ��
	lv_port_indev_init();		//lvgl����ӿڳ�ʼ��
	
	
	gui_app_start(); 	//��������
	
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
		 key=KEY_Scan(0);	//�õ���ֵ
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
     if(speedflag == 1)
		 { 		 

			 led0pwmval -=SpeedDecrease;	
			speedflag = 0;
		 }
				
	}
}


