#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
//����UCOSIII֧��ʱ��2��bug��
//delay_tickspersec��Ϊ��delay_ostickspersec
//delay_intnesting��Ϊ��delay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 
	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
void delay_xms(u32 nms);
#endif






























