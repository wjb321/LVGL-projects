# LVGL-projects  
will do some projects based on LVGL:  
basic for testing the lvgl lib in order to be used in the future for MCU as an UI  
In this [project](https://github.com/wjb321/LVGL-projects/tree/main/lvgl_thesis/UI_thesis_V2.2_lvgl8.2_FreeRTOS9)

> sys.h:  :joy_cat: 
 
configure:  
```
#define SYSTEM_SUPPORT_OS		1  		
```  
to configure if need run freertos or not, this will change the configurations in  
>lv_conf.h  
timer.h  
port.c  
freeRTOSCfig.h
sys.c  
delay.c  
usart.c  

where there are some parts for supporting **OS** or not
the maximum heap_size for the given board(stm32f1ze):  
```
#define configTOTAL_HEAP_SIZE					((size_t)(40*1024))
```
**SRAM support**  
configure the system to support external sram, this series is stm32f1, so it does not support lcd tft display controller(LTDC), so the configure in lv_conf.h only needs the head address of sram, it it is stm32f429, it needs the offset from ltdc
```
#define LV_MEM_ADR      Bank1_SRAM3_ADDR(sram.c)
```
for the introduction [ltdc+dma2d](https://www.jianshu.com/p/216388df4d4d), 

