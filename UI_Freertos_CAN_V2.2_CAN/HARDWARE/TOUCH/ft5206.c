/**
 ****************************************************************************************************
 * @file        ft5206.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-25
 * @brief       7����ݴ�����-FT5206/FT5426 ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200425
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "string.h"
#include "lcd.h"
#include "touch.h"
#include "ctiic.h"
#include "ft5206.h"
#include "usart.h"
#include "delay.h"


/**
 * @brief       ��FT5206д��һ������
 * @param       reg : ��ʼ�Ĵ�����ַ
 * @param       buf : ���ݻ�������
 * @param       len : д���ݳ���
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ft5206_wr_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT5206_CMD_WR);    /* ����д���� */
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF);       /* ���͵�8λ��ַ */
    CT_IIC_Wait_Ack();

    for (i = 0; i < len; i++)
    {
        CT_IIC_Send_Byte(buf[i]);       /* ������ */
        ret = CT_IIC_Wait_Ack();

        if (ret)break;
    }

    CT_IIC_Stop();  /* ����һ��ֹͣ���� */
    return ret;
}

/**
 * @brief       ��FT5206����һ������
 * @param       reg : ��ʼ�Ĵ�����ַ
 * @param       buf : ���ݻ�������
 * @param       len : �����ݳ���
 * @retval      ��
 */
void ft5206_rd_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT5206_CMD_WR);    /* ����д���� */
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF);       /* ���͵�8λ��ַ */
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte(FT5206_CMD_RD);    /* ���Ͷ����� */
    CT_IIC_Wait_Ack();

    for (i = 0; i < len; i++)
    {
        buf[i] = CT_IIC_Read_Byte(i == (len - 1) ? 0 : 1);  /* ������ */
    }

    CT_IIC_Stop();  /* ����һ��ֹͣ���� */
}

/**
 * @brief       ��ʼ��FT5206������
 * @param       ��
 * @retval      0, ��ʼ���ɹ�; 1, ��ʼ��ʧ��;
 */
uint8_t ft5206_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    uint8_t temp[2];

    FT5206_RST_GPIO_CLK_ENABLE();   /* RST����ʱ��ʹ�� */
    FT5206_INT_GPIO_CLK_ENABLE();   /* INT����ʱ��ʹ�� */

    gpio_init_struct.GPIO_Pin = FT5206_RST_GPIO_PIN;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;            /* ������� */
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;          /* ���� */
    GPIO_Init(FT5206_RST_GPIO_PORT, &gpio_init_struct); /* ��ʼ��RST���� */

    gpio_init_struct.GPIO_Pin = FT5206_INT_GPIO_PIN;
    gpio_init_struct.GPIO_Mode = GPIO_Mode_IPU;                /* ���� */
    gpio_init_struct.GPIO_Speed = GPIO_Speed_50MHz;          /* ���� */
    GPIO_Init(FT5206_INT_GPIO_PORT, &gpio_init_struct); /* ��ʼ��INT���� */

    CT_IIC_Init();      /* ��ʼ����������I2C���� */
    FT5206_RST(0);      /* ��λ */
    delay_ms(20);
    FT5206_RST(1);      /* �ͷŸ�λ */
    delay_ms(50);
    temp[0] = 0;
    ft5206_wr_reg(FT5206_DEVIDE_MODE, temp, 1); /* ������������ģʽ */
    ft5206_wr_reg(FT5206_ID_G_MODE, temp, 1);   /* ��ѯģʽ */
    temp[0] = 22;                               /* ������Чֵ��22��ԽСԽ���� */
    ft5206_wr_reg(FT5206_ID_G_THGROUP, temp, 1);/* ���ô�����Чֵ */
    temp[0] = 12;                               /* �������ڣ�����С��12�����14 */
    ft5206_wr_reg(FT5206_ID_G_PERIODACTIVE, temp, 1);
    
    /* ��ȡ�汾�ţ��ο�ֵ��0x3003 */
    ft5206_rd_reg(FT5206_ID_G_LIB_VERSION, &temp[0], 2);

    if ((temp[0] == 0X30 && temp[1] == 0X03) || temp[1] == 0X01 || temp[1] == 0X02)   /* �汾:0X3003/0X0001/0X0002 */
    {
        printf("CTP ID:%x\r\n", ((uint16_t)temp[0] << 8) + temp[1]);
        return 0;
    }

    return 1;
}

/* FT5206 5�������� ��Ӧ�ļĴ����� */
const uint16_t FT5206_TPX_TBL[5] = {FT5206_TP1_REG, FT5206_TP2_REG, FT5206_TP3_REG, FT5206_TP4_REG, FT5206_TP5_REG};

/**
 * @brief       ɨ�败����(���ò�ѯ��ʽ)
 * @param       mode : ������δ�õ��β���, Ϊ�˼��ݵ�����
 * @retval      ��ǰ����״̬
 *   @arg       0, �����޴���; 
 *   @arg       1, �����д���;
 */
uint8_t ft5206_scan(uint8_t mode)
{
    uint8_t sta = 0;
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t res = 0;
    uint16_t temp;
    static uint8_t t = 0;   /* ���Ʋ�ѯ���,�Ӷ�����CPUռ���� */
    
    t++;
    
    if ((t % 10) == 0 || t < 10)   /* ����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ���� */
    {
        ft5206_rd_reg(FT5206_REG_NUM_FINGER, &sta, 1);  /* ��ȡ�������״̬ */
        delay_ms(4);

        if ((sta & 0XF) && ((sta & 0XF) < 6))
        {
            temp = 0XFFFF << (sta & 0XF);           /* ����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta���� */
            tp_dev.sta = (~temp) | TP_PRES_DOWN | TP_CATH_PRES;

            for (i = 0; i < 5; i++)
            {
                if (tp_dev.sta & (1 << i))          /* ������Ч? */
                {
                    ft5206_rd_reg(FT5206_TPX_TBL[i], buf, 4);   /* ��ȡXY����ֵ */

                    if (tp_dev.touchtype & 0X01)    /* ���� */
                    {
                        tp_dev.y[i] = ((uint16_t)(buf[0] & 0X0F) << 8) + buf[1];
                        tp_dev.x[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
                    }
                    else
                    {
                        tp_dev.x[i] = lcddev.width - (((uint16_t)(buf[0] & 0X0F) << 8) + buf[1]);
                        tp_dev.y[i] = ((uint16_t)(buf[2] & 0X0F) << 8) + buf[3];
                    }

                    if ((buf[0] & 0XF0) != 0X80)tp_dev.x[i] = tp_dev.y[i] = 0; /* ������contact�¼�������Ϊ��Ч */

                    //printf("x[%d]:%d,y[%d]:%d\r\n", i, tp_dev.x[i], i, tp_dev.y[i]);
                }
            }

            res = 1;

            if (tp_dev.x[0] == 0 && tp_dev.y[0] == 0)sta = 0;   /* ���������ݶ���0,����Դ˴����� */

            t = 0;  /* ����һ��,��������������10��,�Ӷ���������� */
        }
    }

    if ((sta & 0X1F) == 0)  /* �޴����㰴�� */
    {
        if (tp_dev.sta & TP_PRES_DOWN)      /* ֮ǰ�Ǳ����µ� */
        {
            tp_dev.sta &= ~TP_PRES_DOWN;    /* ��ǰ����ɿ� */
        }
        else    /* ֮ǰ��û�б����� */
        {
            tp_dev.x[0] = 0xffff;
            tp_dev.y[0] = 0xffff;
            tp_dev.sta &= 0XE000;           /* �������Ч��� */
        }
    }

    if (t > 240)t = 10; /* ���´�10��ʼ���� */

    return res;
}




























