#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "touch.h"
#include "timer.h"
#include "lvgl.h"
#include "sram.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_apps\demo\demo.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_1.h" 
#include "lv_tests\lv_test_theme\lv_test_theme_2.h"
/************************************************
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/



int main(void)
{	 		    
	delay_init();	    	 		//延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 		//串口初始化为115200
 	LED_Init();			     		//LED端口初始化
	KEY_Init();	 						//按键初始化
	TIM3_Int_Init(999,71);	//定时器初始化(1ms中断),用于给lvgl提供1ms的心跳节
	FSMC_SRAM_Init();
	LCD_Init();							//LCD初始化, why it did not work properly someone can tell me?
 	tp_dev.init();					//触摸屏初始化
	lv_init();							//lvgl系统初始化
	lv_port_disp_init();		//lvgl显示接口初始化
	lv_port_indev_init();		//lvgl输入接口初始化
	
	//demo_create();					//开始运行demo
	//lv_test_theme_1(lv_theme_night_init(210, NULL));
	lv_test_theme_2();
	
	
	
	while(1)
	{
		tp_dev.scan(0);
		lv_task_handler();
		/*
		ensure there are not so many delay function, which wastes lots of time
		*/
	}
}


