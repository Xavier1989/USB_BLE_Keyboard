#ifndef _BSP_USART_FIFO_H_
#define _BSP_USART_FIFO_H_

#include "bsp.h"
	#define	UART1_FIFO_EN	1
	#define	UART2_FIFO_EN	0

/* ����˿ں� */
typedef enum
{
	COM_BLE = 0,	/* USART1  PA9, PA10 ��  PB6, PB7*/
	COM2 = 1	/* USART2, PD5,PD6 �� PA2, PA3 */
}COM_PORT_E;

/* ���崮�ڲ����ʺ�FIFO��������С����Ϊ���ͻ������ͽ��ջ�����, ֧��ȫ˫�� */
#if UART1_FIFO_EN == 1
	#define UART1_BAUD			115200
	#define UART1_TX_BUF_SIZE	1*512
	#define UART1_RX_BUF_SIZE	1*128
#endif

#if UART2_FIFO_EN == 1
	#define UART2_BAUD			9600
	#define UART2_TX_BUF_SIZE	1*128
	#define UART2_RX_BUF_SIZE	1*128
#endif
/* �����豸�ṹ�� */
typedef struct
{
	USART_TypeDef *uart;		/* STM32�ڲ������豸ָ�� */
	uint8_t *pTxBuf;			/* ���ͻ����� */
	uint8_t *pRxBuf;			/* ���ջ����� */
	uint16_t usTxBufSize;		/* ���ͻ�������С */
	uint16_t usRxBufSize;		/* ���ջ�������С */
	uint16_t usTxWrite;			/* ���ͻ�����дָ�� */
	uint16_t usTxRead;			/* ���ͻ�������ָ�� */
	uint16_t usTxCount;			/* �ȴ����͵����ݸ��� */

	uint16_t usRxWrite;			/* ���ջ�����дָ�� */
	uint16_t usRxRead;			/* ���ջ�������ָ�� */
	uint16_t usRxCount;			/* ��δ��ȡ�������ݸ��� */

	void (*SendBefor)(void); 	/* ��ʼ����֮ǰ�Ļص�����ָ�루��Ҫ����RS485�л�������ģʽ�� */
	void (*SendOver)(void); 	/* ������ϵĻص�����ָ�루��Ҫ����RS485������ģʽ�л�Ϊ����ģʽ�� */
	void (*ReciveNew)(void);	/* �����յ����ݵĻص�����ָ�� */
}UART_T;

void bsp_InitUart(void);
void comSendBuf(COM_PORT_E _ucPort, uint8_t *_ucaBuf, uint16_t _usLen);
void comSendChar(COM_PORT_E _ucPort, uint8_t _ucByte);
uint8_t comGetChar(COM_PORT_E _ucPort, uint8_t *_pByte);

void comClearTxFifo(COM_PORT_E _ucPort);
void comClearRxFifo(COM_PORT_E _ucPort);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
