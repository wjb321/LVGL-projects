#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
//修正UCOSIII支持时的2个bug：
//delay_tickspersec改为：delay_ostickspersec
//delay_intnesting改为：delay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 
	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_xms(u32 nms);
#endif






























