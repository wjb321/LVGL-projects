#ifndef CAN_CONFIG_H
#define CAN_CONFIG_H

#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h" 

/*CAN configurations for all the nodes*/
void CanInit(void);
void CAN_Master_Configuration(void);
void CAN_ABSCONTROL_Configuration(void);


/*configure all the filters for different nodes*/

void CanFilter_ABS_Configure(void);
void NodeMesTransmit(uint16_t ID);
void CanWriteData(uint16_t ID);
void SendRef(uint16_t ID, uint16_t TOTNumBC, uint16_t NUMSlot, uint16_t SLOTTime);
void NVIC0_Configuration(void);
void NVIC1_Configuration(void);
void GPIO_Configuration(void);

#define mes1_ID 0x120
#define mes2_ID 0x121
#define mes3_ID 0x122
#define mes4_ID 0x123

#define mes5_ID 0x124
#define mes6_ID 0x125
#define mes7_ID 0x126
#define mes8_ID 0x127

#define arb1_ID 0x130
#define arb2_ID 0x131
#define arb3_ID 0x132
#define arb4_ID 0x133
#define arb5_ID 0x134
#define arb6_ID 0x135
#endif 
