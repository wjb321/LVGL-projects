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
for the introduction [ltdc+dma2d](https://www.jianshu.com/p/216388df4d4d), [ltdc+dma2d(GPU)](https://www.eet-china.com/mp/a60976.html),[difference beteween mcu lcd and rgb lcd](https://mcu.eetrend.com/blog/2020/100048344.html)
there are 2 ways of using sram:  
>1. put the memory at external sram that lvgl manages (not recommanded) in lv_conf.h only

```
    #define LV_MEM_SIZE (200U * 1024U)   **can be reduced to 100U or increased d to 300U, casue it uses external SRAM**
    #define LV_MEM_ADR  0x68000000  //0  /*0: unused*/
```

>2. the buffer for drawing the pic is put in external SRAM (internal memory is not enough)(**does not work**)
```
#define LV_MEM_ADR  0  /*0: unused*/
static lv_color_t buf_1[MY_DISP_HOR_RES * MY_DISP_VER_RES] __attribute__((at(0x68000000))); //0x68000000 start add of sram
lv_disp_draw_buf_init(&draw_buf_dsc_1, buf_1, NULL, MY_DISP_HOR_RES * MY_DISP_VER_RES); 
```
[bilibili reference:external sram](https://www.bilibili.com/video/BV1CG4y157Px/?p=12&spm_id_from=pageDriver&vd_source=bfafe33e0d30866f120602a52781ae5e) 

**malloc for memory management is not done yet**


