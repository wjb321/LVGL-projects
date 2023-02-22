#ifndef __SRAM_H
#define __SRAM_H															    
#include "sys.h" 

											  
void FSMC_SRAM_Init(void);
void FSMC_SRAM_WriteBuffer(u8* pBuffer,u32 WriteAddr,u32 NumHalfwordToWrite);
void FSMC_SRAM_ReadBuffer(u8* pBuffer,u32 ReadAddr,u32 NumHalfwordToRead);


#define SRAM_WR_GPIO_PORT               GPIOD
#define SRAM_WR_GPIO_PIN                GPIO_PIN_5
#define SRAM_WR_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

#define SRAM_RD_GPIO_PORT               GPIOD
#define SRAM_RD_GPIO_PIN                GPIO_PIN_4
#define SRAM_RD_GPIO_CLK_ENABLE()       do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

/* SRAM_CS(��Ҫ����SRAM_FSMC_NEX������ȷ��IO��) ���� ���� */
#define SRAM_CS_GPIO_PORT                GPIOG
#define SRAM_CS_GPIO_PIN                 GPIO_PIN_10
#define SRAM_CS_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* ����IO��ʱ��ʹ�� */

/* FSMC��ز��� ���� 
 * ע��: ����Ĭ����ͨ��FSMC��3������SRAM, ��1��4��Ƭѡ: FSMC_NE1~4
 *
 * �޸�SRAM_FSMC_NEX, ��Ӧ��SRAM_CS_GPIO�������Ҳ�ø�
 */
#define SRAM_FSMC_NEX           3         /* ʹ��FSMC_NE3��SRAM_CS,ȡֵ��Χֻ����: 1~4 */

#define SRAM_FSMC_BCRX          FSMC_Bank1->BTCR[(SRAM_FSMC_NEX - 1) * 2]       /* BCR�Ĵ���,����SRAM_FSMC_NEX�Զ����� */
#define SRAM_FSMC_BTRX          FSMC_Bank1->BTCR[(SRAM_FSMC_NEX - 1) * 2 + 1]   /* BTR�Ĵ���,����SRAM_FSMC_NEX�Զ����� */
#define SRAM_FSMC_BWTRX         FSMC_Bank1E->BWTR[(SRAM_FSMC_NEX - 1) * 2]      /* BWTR�Ĵ���,����SRAM_FSMC_NEX�Զ����� */

#define SRAM_BASE_ADDR         (0X60000000 + (0X4000000 * (SRAM_FSMC_NEX - 1)))
void fsmc_sram_test_write(u8 data,u32 addr);
u8 fsmc_sram_test_read(u32 addr);

#endif

