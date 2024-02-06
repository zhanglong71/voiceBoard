/**
  ******************************************************************************
  * @file    hk32f0301mxxc_it.c 
  * @author  Alexander
  * @version V1.0.0
  * @date    30-Jan-2023
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 HKMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
	
/* Includes ------------------------------------------------------------------*/
#include "hk32f0301mxxc_it.h"

#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "l_arch.h"
#include "l_u8FIFO.h"
#include "l_timer.h"
/** @addtogroup HK32F0301MxxC_StdPeriph_Examples
  * @{
  */

/** @addtogroup UART_2xUARTOnBoard
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
    TimingDelay_Increment();
}

/******************************************************************************/
/*             HK32F0301MxxC Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (KEIL_startup_hk32f0301mxxc.s).                                      */
/******************************************************************************/

/**
  * @brief  This function handles UART1 global interrupt request.
  * @param  None
  * @retval None
  */
void UART1_IRQHandler(void)
{
    u8Data_t u8data;
	if( UART_GetITStatus(UART1, UART_IT_RXNE) != RESET )
	{
		UART_ClearITPendingBit(UART1, UART_IT_RXNE);
        g_IT_uart1_tmr = 0;

        u8data.u8Val = UART_ReceiveData(UART1);
        u8FIFOin(&g_uart1RxQue, &u8data);

        #if 1   // ???????????
        msg_t msg;
        msg.msgType = CMSG_UART1RX;
        msgq_in(&g_msgq, &msg);
        #endif
	}

	/* UART1 transfer empty interrupt handle */
	if( UART_GetITStatus(UART1, UART_IT_TXE) != RESET )
	{
		UART_ClearITPendingBit(UART1, UART_IT_TXE);

        if(u8FIFOout(rs485transX.pfifo, &u8data) == TRUE) {
           UART1->TDR = u8data.u8Val;
            
        }
		else
		{
			UART_ITConfig(UART1, UART_IT_TXE, DISABLE);  
			UART_ITConfig(UART1, UART_IT_TC, ENABLE);
		}
	}
    
	/* UART1 transfer complete interrupt handle */
	if( UART_GetITStatus(UART1, UART_IT_TC) != RESET )
	{
		UART_ClearITPendingBit(UART1, UART_IT_TC);
		UART_ITConfig(UART1, UART_IT_TC, DISABLE);
        M485TR_R("发送结束，设置成为接收");
	}

	#if 0
	if(UART_GetITStatus(UART1, UART_IT_IDLE) != RESET)
	{
		UART_ClearITPendingBit(UART1, UART_IT_IDLE);
		UART_ITConfig(UART1, UART_IT_IDLE, DISABLE);
		Uart1Rx.Len = Uart1Rx.Index;
		Uart1Rx.Index = 0;
		Uart1Rx.FinishedFlag = SET;
	}
	#endif
}

/**
  * @brief  This function handles UART2 global interrupt request.
  * @param  None
  * @retval None
  */
void UART2_IRQHandler(void)
{
	if( UART_GetITStatus(UART2, UART_IT_RXNE) != RESET )
	{
		UART_ClearITPendingBit(UART2, UART_IT_RXNE);

        u8Data_t u8data;
        u8data.u8Val = UART_ReceiveData(UART2);
        u8FIFOin(&g_uart2RxQue, &u8data);
    
        msg_t msg;
        msg.msgType = CMSG_UART2RX;     /** message CMSG_UART3RX **/
        msgq_in(&g_msgq, &msg);

    #if 0  	
    /* receive data */
    Uart2Rx.Data[Uart2Rx.Index++] = UART_ReceiveData(UART2);
		
		if( Uart2Rx.Index == 1 )
		{
			UART_ITConfig(UART2, UART_IT_IDLE, ENABLE);
		}
		#endif
	}

  #if 0  	
	if(UART_GetITStatus(UART2, UART_IT_IDLE) != RESET)
	{
		UART_ClearITPendingBit(UART2, UART_IT_IDLE);
		UART_ITConfig(UART2, UART_IT_IDLE, DISABLE);
		Uart2Rx.Len = Uart2Rx.Index;
		Uart2Rx.Index = 0;
		Uart2Rx.FinishedFlag = SET;
	}
	#endif
}

/**
  * @brief  This function handles TIM2 global interrupt request.
  * @param  None
  * @retval None
  */
  uint16_t capture = 0;
void TIM2_IRQHandler(void)
{
#if 1
  if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

    /*vop control frequency = 4k Hz */
    vop_sendByPulse();
    
    capture = TIM_GetCapture1(TIM2);
    TIM_SetCompare1(TIM2, capture + CCR1_Val);
  } else if (TIM_GetITStatus(TIM2, TIM_IT_CC2) != RESET) {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

    /* 485 with frequency = 10k Hz */
    TimingDelayUS_Decrement();
    capture = TIM_GetCapture2(TIM2);
    TIM_SetCompare2(TIM2, capture + CCR2_Val);
  }
  
  #if 0
  else if (TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3);

    /* LED3 toggling with frequency = 219.7 Hz */
    //HK_EVAL_LEDToggle(LED3);
    capture = TIM_GetCapture3(TIM2);
    TIM_SetCompare3(TIM2, capture + CCR3_Val);
  }
  else
  {
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC4);

    /* LED4 toggling with frequency = 439.4 Hz */
    capture = TIM_GetCapture4(TIM2);
    TIM_SetCompare4(TIM2, capture + CCR4_Val);
  }
  #endif
#endif
}


/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */
