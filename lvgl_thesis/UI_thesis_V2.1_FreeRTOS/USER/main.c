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
#include "can_config.h"

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
extern lv_coord_t series1_y[POINT_COUNT];
extern uint8_t Rx1_DATA0,Rx1_DATA1,Rx1_DATA2,Rx1_DATA3,Rx1_DATA4,Rx1_DATA5,Rx1_DATA6,Rx1_DATA7;
extern int updateChart;

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
  FSMC_SRAM_Init();				//外部1MB的sram初始化
	CAN_ABSCONTROL_Configuration();
  LCD_Init();							//LCD初始化
  tp_dev.init();					//触摸屏初始化

  TIM1_Int_Init(99, 7199, TIM1_Enable);
  TIM2_Int_Init(arrValue,pscValue,ENABLE);
  TIM3_PWM_Init(1399,0);	 //不分频。PWM频率=72000000/900=80Khz
  TIM5_Int_Init(999,71);	//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
  TIM6_Int_Init(9999,7199, ENABLE);
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
            case KEY2_PRES:  //change the current trend:celeration-> deceleration or vice versa
              CeleOrDeceFlag = !CeleOrDeceFlag;
              printf("CeleOrDeceFlag is %d\n",CeleOrDeceFlag);
              break;

            case KEY1_PRES:  // initial the speed either highest or lowest
              InitPWMFlag =!InitPWMFlag;
              if(InitPWMFlag) led0pwmval = 30;
              else led0pwmval = 1079;
              printf("led0pwmval is %d\n",led0pwmval);
              break;

            case KEY0_PRES:  // deceleration or celeration rate change
              PWMChangeFlag =!PWMChangeFlag;
              if(PWMChangeFlag) SpeedDecrease = 6;
              else SpeedDecrease = 3;
              printf("SpeedDecrease is %d\n",SpeedDecrease);
              break;

            case WKUP_PRES:  // keep current speed or keep the previous trend or or off timer
              ConstantOrChangeSpeed = !ConstantOrChangeSpeed;
              TIM_Cmd(TIM1, ConstantOrChangeSpeed);
              printf("ConstantOrChangeSpeed is %d\n",ConstantOrChangeSpeed);
              break;
            }
        }
      switch(CAN_Speedflag)
        {
        case 1:
          if(led0pwmval <= 30) CeleOrDeceFlag = 0;
          else if(led0pwmval >= 1079) CeleOrDeceFlag = 1;
          switch(CeleOrDeceFlag)
            {
            case 0:
              led0pwmval +=SpeedDecrease; // decelation
						  //BEEP = !BEEP;
						  //printf("led0pwmval is %d\n",led0pwmval);
              break;
            case 1:
              led0pwmval -=SpeedDecrease; // celelation
              break;
            default:
              break;
            }
          CAN_Speedflag = 0;
          break;
        case 2:
					 switch(Rx1_DATA0)
					 {
						 case 1:
							 SpeedDecrease = 6;
						   led0pwmval -= SpeedDecrease;
							 printf("blocked\n");
							 break;
						 case 2:
							 printf("celeration\n");
							 break;
						 case 3:
							 printf("celeration\n");
							 break;
						 case 4:
							 printf("constant\n");
							 break;
						 default:
							 break;
					 }
					 CAN_Speedflag = 0;
          break;
        default:
          break;
        }
      TIM_SetCompare2(TIM3,led0pwmval);
    }
}


