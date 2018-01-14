/************************************************************
    *   @file:      XX.c
    *   @brief:     XX
    *   @author:    XIET
    *   @version:   V1.0
    *   @data:      201x-xx-xx
    *   @modify:    data            remarks
**************************************************************/
#include "bsp.h"

static void HC595_SPI_Init(void);       //SPI ³õÊ¼»¯
static void HC595_GPIO_Config(void);    //SPI IOÅäÖÃ
/*============================================================
    *   @func:      bsp_74hc595_Init
    *   @brief:     595³õÊ¼»¯£¬IOÅäÖÃ
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void bsp_74hc595_Init(void)
{
    HC595_GPIO_Config();
    HC595_SPI_Init();
}
/*============================================================
    *   @func:      HC595_GPIO_Config
    *   @brief:     HC595¿ØÖÆIO³õÊ¼»¯£¬ÕâÀïÖ÷Òª³õÊ¼»¯LATÒý½Å
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void HC595_GPIO_Config(void)
{
    LL_GPIO_InitTypeDef  GPIO_InitStructure;
    /**注：IO端口时钟在前面已经使能 */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

    /* Configure 74hc595 pins: SCK */
    GPIO_InitStructure.Pin = GPIO_PIN_HC595_CLK;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull  = LL_GPIO_PULL_NO;
    GPIO_InitStructure.Alternate = LL_GPIO_AF_0;
    LL_GPIO_Init(GPIO_PORT_HC595, &GPIO_InitStructure);

    /* Configure 74hc595 pins: MOSI */
    GPIO_InitStructure.Pin = GPIO_PIN_HC595_DATA;
    LL_GPIO_Init(GPIO_PORT_HC595, &GPIO_InitStructure);

    /* Configure 74hc595 pins: CS */
    HC595_LAT_LOW();
    GPIO_InitStructure.Pin = GPIO_PIN_HC595_LAT;
    GPIO_InitStructure.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStructure.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStructure.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIO_PORT_HC595_LAT, &GPIO_InitStructure);
}
/*============================================================
    *   @func:      HC595_SPI_Init
    *   @brief:     SPI³õÊ¼»¯£¬Ó²¼þSPI
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
static void HC595_SPI_Init(void)
{
   LL_SPI_InitTypeDef   SPI_InitStructure;
  SPI_InitStructure.TransferDirection = LL_SPI_HALF_DUPLEX_TX;
  SPI_InitStructure.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStructure.DataWidth = LL_SPI_DATAWIDTH_16BIT;
  SPI_InitStructure.ClockPolarity = LL_SPI_POLARITY_LOW;
  SPI_InitStructure.ClockPhase = LL_SPI_PHASE_1EDGE;
  SPI_InitStructure.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
  SPI_InitStructure.BitOrder = LL_SPI_LSB_FIRST;
  SPI_InitStructure.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStructure.CRCPoly = 7;
  LL_SPI_Init(SPI1, &SPI_InitStructure);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
	LL_SPI_Enable(SPI1);
}
/*============================================================
    *   @func:      HC595_SendByte
    *   @brief:     Í¨¹ýSPI·¢ËÍÒ»×Ö½ÚÊý¾Ý
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void HC595_Send2Byte(uint16_t byte)
{
    while (LL_SPI_IsActiveFlag_TXE(SPI1) == RESET);
    LL_SPI_TransmitData16(SPI1, byte);
}
/*============================================================
    *   @func:      HC595_SendData
    *   @brief:     Í¨¹ýSPI·¢ËÍÒ»×éÊý¾Ý
    *   @param:     NA
    *   @retval:    NA
    *   @modify:    data            remarks
=============================================================*/
void HC595_SendData(uint16_t *pbuf, uint8_t len)
{
    while(len--)
    {
        HC595_Send2Byte(*(pbuf++));
    }
}
