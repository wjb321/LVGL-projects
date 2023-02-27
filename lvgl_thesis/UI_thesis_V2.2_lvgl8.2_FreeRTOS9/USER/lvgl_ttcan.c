#include "lvgl_ttcan.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
//#include "lv_demo_stress.h"
#include "beep.h"
#include "timer.h"
#include "led.h"
#include "key.h"
#include "lvgl_test.h"
#include "lv_mainstart.h"

#if SYSTEM_SUPPORT_OS
#include "FreeRTOS.h"
#include "task.h"

#include "delay.h"
#include "lv_demo_keypad_encoder.h"

#define START_TASK_PRIO     1
#define START_STK_SIZE      128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);


#define LV_DEMO_TASK_PRIO   3
#define LV_DEMO_STK_SIZE   1024
TaskHandle_t LV_DEMOTask_Handler;
void lv_demo_task(void *pvParameters);


#define LED_TASK_PRIO       6
#define LED_STK_SIZE        128
TaskHandle_t LEDTask_Handler;
void led_task(void *pvParameters);


void lvgl_ttcan(void)
{
  KEY_Init();
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

  xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
              (const char*    )"start_task",          /* 任务名称 */
              (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
              (void*          )NULL,                  /* 传递给任务函数的参数 */
              (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
              (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */


  vTaskStartScheduler();
}

void start_task(void *pvParameters)
{
  taskENTER_CRITICAL();

  xTaskCreate((TaskFunction_t )lv_demo_task,
              (const char*    )"lv_demo_task",
              (uint16_t       )LV_DEMO_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )LV_DEMO_TASK_PRIO,
              (TaskHandle_t*  )&LV_DEMOTask_Handler);


  xTaskCreate((TaskFunction_t )led_task,
              (const char*    )"led_task",
              (uint16_t       )LED_STK_SIZE,
              (void*          )NULL,
              (UBaseType_t    )LED_TASK_PRIO,
              (TaskHandle_t*  )&LEDTask_Handler);

  taskEXIT_CRITICAL();
  vTaskDelete(StartTask_Handler);
}


void lv_demo_task(void *pvParameters)
{ 
	//my_event_test3();
	lv_label_test();
  //lv_demo_stress();
  //lv_demo_keypad_encoder();
  while(1)
    {
      lv_timer_handler();
      vTaskDelay(5);

    }
}

extern int Tem_NumHigh2Freq;
static int arrValue    = 199;
static int pscValue    = 71;

void led_task(void *pvParameters)
{ 
	int SpeedDecrease = 20;
  int update_chart_Array = 0;
  int ConstantOrChangeSpeed = 0;
  int PWMChangeFlag = 0;
  int CeleOrDeceFlag = 0;
  int InitPWMFlag = 0;
  int led0pwmval    = 20;
	extern int CAN_Speedflag;
	extern uint8_t Rx1_DATA0,Rx1_DATA1,Rx1_DATA2,Rx1_DATA3,Rx1_DATA4,Rx1_DATA5,Rx1_DATA6,Rx1_DATA7;
  extern int updateChart;
  vu8 key=0;
  while(1)
    {
      key=KEY_Scan(0);
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
              BEEP = !BEEP;
              //printf("blocked\n");
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
						case 3:
							TSpeed( arrValue, pscValue, Tem_NumHigh2Freq );
						CAN_Speedflag = 0;
						break;
        default:
          break;
        }
      TIM_SetCompare2(TIM3,led0pwmval);
      //printf("hello the world'\r\n");
      //delay_ms(500);
      //lv_timer_handler();
      //LED0=!LED0;
      vTaskDelay(1);
    }
}

#endif

