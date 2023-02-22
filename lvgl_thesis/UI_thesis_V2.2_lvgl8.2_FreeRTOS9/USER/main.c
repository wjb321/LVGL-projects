#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "touch.h"
#include "timer.h"
#include "sram.h"
#include "led.h"
#include "can_config.h"
#include "lvgl_ttcan.h"
#include "malloc.h"
#include "beep.h"

#if !SYSTEM_SUPPORT_OS
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demo_keypad_encoder.h"
#endif


static int arrValue    = 199;
static int pscValue    = 71;
int Pulse         = 62;
int TIM1_Enable   = ENABLE;


//extern lv_coord_t series1_y[POINT_COUNT];


int main(void)
{
  delay_init();	    	 		//延时函数初始化
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
  uart_init(115200);	 		//串口初始化为115200
  LED_Init();			     		//LED端口初始化
  BEEP_Init();						//蜂鸣器初始化
  FSMC_SRAM_Init();				//外部1MB的sram初始化
//  tp_dev.init();
//	my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
//  my_mem_init(SRAMEX);                /* 初始化外部SRAM内存池 */
	CAN_ABSCONTROL_Configuration();
	TIM1_Int_Init(99, 7199, TIM1_Enable);
  TIM2_Int_Init(arrValue,pscValue,ENABLE);
  TIM3_PWM_Init(1399,0);	 //不分频。PWM频率=72000000/900=80Khz
	TIM4_EncoderMode_Config(Pulse);
	 //TIM6_Int_Init(9999,7199, ENABLE);
	
#if !SYSTEM_SUPPORT_OS
	TIM5_Int_Init(999,71);	//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节拍
  lv_init();					
  lv_port_disp_init();
  lv_port_indev_init();	
	lv_demo_keypad_encoder();
	
    while(1)
    { 
      lv_timer_handler(); 
			delay_ms(5);
		}
#else
  lvgl_ttcan();
#endif
}
// tp_dev.init();		
//      xTaskCreate((TaskFunction_t )start_task,            //任务函数
//                (const char*    )"start_task",          //任务名称
//                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
//                (void*          )NULL,                  //传递给任务函数的参数
//                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
//                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
//    vTaskStartScheduler();          //开启任务调度


//void start_task(void *pvParameters)
//{  
//	 pvParameters = pvParameters;
//    taskENTER_CRITICAL();           //进入临界区
//    //创建TASK1任务
//    xTaskCreate((TaskFunction_t )LVGL_task,             
//                (const char*    )"LVGL_task",           
//                (uint16_t       )LVGL_STK_SIZE,        
//                (void*          )NULL,                  
//                (UBaseType_t    )LVGL_TASK_PRIO,        
//                (TaskHandle_t*  )&LVGL_Task_Handler);   
//    //创建TASK2任务
//    xTaskCreate((TaskFunction_t )task2_task,     
//                (const char*    )"task2_task",   
//                (uint16_t       )TASK2_STK_SIZE,
//                (void*          )NULL,
//                (UBaseType_t    )TASK2_TASK_PRIO,
//                (TaskHandle_t*  )&Task2Task_Handler); 
//    taskEXIT_CRITICAL();            //退出临界区
//	  vTaskDelete(StartTask_Handler); //删除开始任务
//}


//task2任务函数
//void task2_task(void *pvParameters)
//{
//	while(1)
//	{
//	       LED1=!LED1;
//        vTaskDelay(1000);                          
//	}
//}

//task1任务函数
//void LVGL_task(void *pvParameters)
//{  vu8 key=0;
////	pvParameters = pvParameters;
////	  lv_demo_stress();
////	
//  while(1)
//    { 
////			   lv_timer_handler(); /* LVGL计时器 */
////        vTaskDelay(5);
////      tp_dev.scan(0);
////      lv_task_handler();
////      key=KEY_Scan(0);	//得到键值
////      if(key)
////        {
////          switch(key)
////            {
////            case KEY2_PRES:  //change the current trend:celeration-> deceleration or vice versa
////              CeleOrDeceFlag = !CeleOrDeceFlag;
////              printf("CeleOrDeceFlag is %d\n",CeleOrDeceFlag);
////              break;

////            case KEY1_PRES:  // initial the speed either highest or lowest
////              InitPWMFlag =!InitPWMFlag;
////              if(InitPWMFlag) led0pwmval = 30;
////              else led0pwmval = 1079;
////              printf("led0pwmval is %d\n",led0pwmval);
////              break;

////            case KEY0_PRES:  // deceleration or celeration rate change
////              PWMChangeFlag =!PWMChangeFlag;
////              if(PWMChangeFlag) SpeedDecrease = 6;
////              else SpeedDecrease = 3;
////              printf("SpeedDecrease is %d\n",SpeedDecrease);
////              break;

////            case WKUP_PRES:  // keep current speed or keep the previous trend or or off timer
////              ConstantOrChangeSpeed = !ConstantOrChangeSpeed;
////              TIM_Cmd(TIM1, ConstantOrChangeSpeed);
////              printf("ConstantOrChangeSpeed is %d\n",ConstantOrChangeSpeed);
////              break;
////            }
////        }
////      switch(CAN_Speedflag)
////        {
////        case 1:
////          if(led0pwmval <= 30) CeleOrDeceFlag = 0;
////          else if(led0pwmval >= 1079) CeleOrDeceFlag = 1;
////          switch(CeleOrDeceFlag)
////            {
////            case 0:
////              led0pwmval +=SpeedDecrease; // decelation
////						  //BEEP = !BEEP;
////						  //printf("led0pwmval is %d\n",led0pwmval);
////              break;
////            case 1:
////              led0pwmval -=SpeedDecrease; // celelation
////              break;
////            default:
////              break;
////            }
////          CAN_Speedflag = 0;
////          break;
////        case 2:
////					 switch(Rx1_DATA0)
////					 {
////						 case 1:
////							 SpeedDecrease = 6;
////						   led0pwmval -= SpeedDecrease;
////						   BEEP = !BEEP;
////							 //printf("blocked\n");
////							 break;
////						 case 2:
////							 printf("celeration\n");
////							 break;
////						 case 3:
////							 printf("celeration\n");
////							 break;
////						 case 4:
////							 printf("constant\n");
////							 break;
////						 default:
////							 break;
////					 }
////					 CAN_Speedflag = 0;
////          break;
////        default:
////          break;
////        }
////      TIM_SetCompare2(TIM3,led0pwmval);
////		//		printf("hello the world'\r\n");
////					//delay_ms(500);
////		//lv_timer_handler();
////  		   LED0=!LED0;
////        vTaskDelay(1000);
//   }

//}



