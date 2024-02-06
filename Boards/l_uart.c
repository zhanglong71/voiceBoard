#include "hk32f0301mxxc_it.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "l_arch.h"
#include "l_u8FIFO.h"
#include "l_uart.h"

void deamon_uart2_recive(u8 ch)
{
	#if 0 	/** the function execute in interupt subroutine **/
    u8Data_t u8data;
    u8data.u8Val = ch;
    u8FIFOin(&g_uart3RxQue, &u8data);
    
    msg_t msg;
    msg.msgType = CMSG_UART3RX;     /** message CMSG_UART3RX **/
    msgq_in(&g_msgq, &msg);
	#endif
}

void deamon_uart1_send(void)
{
#if 0
    msg_t msg;
    u8Data_t u8Data;
    if(u8FIFOisEmpty(&g_uart1TxQue) == TRUE) {
        if (UART_GetFlagStatus(UART1, UART_FLAG_TC) == SET) {
            M485TR_R("发送完成，进入接收状态");
        }
        return;
    }
    if(UART_GetFlagStatus(UART1, UART_FLAG_TC) == RESET) {
        return;
    }
    
    if(u8FIFOout_irq(&g_uart1TxQue, &u8Data) == TRUE) {
        M485TR_T("发送前置高");
        UART_SendData(UART1, u8Data.u8Val);
        /** 如果队列已空，数据发送完成 **/
        if(u8FIFOisEmpty(&g_uart1TxQue) == TRUE) {
            msg.msgType = CMSG_UART1TX;
            msgq_in_irq(&g_msgq, &msg); 
        }
    }
#endif
}

void deamon_uart2_send(void)
{
    msg_t msg;
    u8Data_t u8Data;
    if (u8FIFOisEmpty(&g_uart2TxQue) == TRUE) {
        return;
    }
    if(UART_GetFlagStatus(UART2, UART_FLAG_TC) == RESET) {
        return;
    }
      
    if(u8FIFOout_irq(&g_uart2TxQue, &u8Data) == TRUE) {
        //USART_SendData(USART3, u8Data.u8Val);
        UART_SendData(UART2, u8Data.u8Val);
        /** 如果队列已空，数据发送完成 **/
        if(u8FIFOisEmpty(&g_uart2TxQue) == TRUE) {
            msg.msgType = CMSG_UART2TX;		/** message CMSG_UART2TX **/
            msgq_in_irq(&g_msgq, &msg);
        }
		//		GPIO_led_blink(); //?????????????????????????
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration4uart(void)
{
  /* Enable GPIO clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);

  /* Enable UART1 clock */
  RCC_APBPeriph2ClockCmd(RCC_APBPeriph2_UART1, ENABLE);

  /* Enable UART2 clock */
  RCC_APBPeriph1ClockCmd(RCC_APBPeriph1_UART2, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration4uart(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*
    UART1_TX:PA3  AF1
    UART1_RX:PA2  AF1

    UART2_TX:PC3   AF6
    UART2_RX:PC4   AF6
  */

  /* UART1 Pins configuration ************************************************/
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);

  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  /* RX Pull-Up can filter noise*/
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* UART2 Pins configuration ************************************************/
  /* Connect pin to Periph */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_6);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_6);

  /* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;   /* RX Pull-Up can filter noise*/
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void UART1_Configuration(void)
{
  /* UART1 configuration --------------------------------------------------*/
  /*configured as follow:
  - BaudRate = 9600 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Receive and transmit enabled
  */
  UART_InitTypeDef UART_InitStructure;
  //UART_InitStructure.UART_BaudRate = 9600;
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;

  UART_Init(UART1, &UART_InitStructure);

  /* Enable UART1 Receive and IDLE interrupts */
  UART_ITConfig(UART1, UART_IT_RXNE, ENABLE);
  UART_ITConfig(UART1, UART_IT_IDLE, DISABLE);

  //Uart1Rx.Index = 0;
  //Uart1Rx.Len = 0;
  //Uart1Rx.FinishedFlag = RESET;

  /* Enable the UART1 */
  UART_Cmd(UART1, ENABLE);
}

void UART2_Configuration(void)
{
	
  /* UART2 configuration --------------------------------------------------*/
  /*configured as follow:
  - BaudRate = 9600 baud
  - Word Length = 8 Bits
  - One Stop Bit
  - No parity
  - Receive and transmit enabled
  */
  UART_InitTypeDef UART_InitStructure;
  //UART_InitStructure.UART_BaudRate = 9600;
  UART_InitStructure.UART_BaudRate = 115200;
  UART_InitStructure.UART_WordLength = UART_WordLength_8b;
  UART_InitStructure.UART_StopBits = UART_StopBits_1;
  UART_InitStructure.UART_Parity = UART_Parity_No;
  UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
  UART_Init(UART2, &UART_InitStructure);

  /* Enable UART2 Receive and IDLE interrupts */
  UART_ITConfig(UART2, UART_IT_RXNE, ENABLE);
  UART_ITConfig(UART2, UART_IT_IDLE, DISABLE);

  //Uart2Rx.Index = 0;
  //Uart2Rx.Len = 0;
  //Uart2Rx.FinishedFlag = RESET;

  /* Enable the UART2 */
  UART_Cmd(UART2, ENABLE);
}
/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration4uart(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* UART1 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = UART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* UART2 IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = UART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

