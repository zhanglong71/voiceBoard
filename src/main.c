/**
  ******************************************************************************
  * @file    main.c
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
#include "main.h"

#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "l_arch.h"
#include "l_u8FIFO.h"
#include "l_actionFIFO.h"
#include "l_timer.h"
#include "l_uart.h"
#include "l_gpio.h"
#include "l_rs485.h"
#include "f_idle.h"
#include "l_sysProcess.h"


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
//UartRxBufDef Uart1Rx, Uart2Rx;
//UART_TypeDef* UartInstance[2] = {UART1, UART2};
//uint8_t UartIndex;

/* Private functions ---------------------------------------------------------*/
//static void RCC_Configuration(void);
//static void GPIO_Configuration(void);
//static void NVIC_Configuration(void);
//static void Uart1SendDatas(uint8_t *p, uint8_t len);
//static void Uart2SendDatas(uint8_t *p, uint8_t len);
/* global variables ----------------------------------------------------------*/

 fstack_t g_fstack;
 msgq_t g_msgq;
 msg_t msg;
 func_t func;
 u16 g_tick;			/** 定时器计数 **/
 u16 g_flag;
 Timer_t g_timer[TIMER_NUM];		
 /**
  * g_timer[0] for global, 
  * g_timer[1] for uart2 receive
  * g_timer[2] for g_promptQueue
  * g_timer[3] for key/getNetInfo
  **/
// Timer_t g_ustimer[TIMER_NUM_40US];		/** for 485 transmit only **/
//u8FIFO_t g_uart3TxQue;
//u8FIFO_t g_uart3RxQue;
u8FIFO_t g_uart2TxQue;
u8FIFO_t g_uart2RxQue;
u8FIFO_t g_uart1TxQue;
u8FIFO_t g_uart1RxQue;

rs485transX_t rs485transX;

actionQueue_t g_promptQueue;
u8 g_IT_uart1_tmr = 0; // used uart1 received timeover
/******************************************************************************/
NetInfo_t g_netInfo;

/**
 * record wifi net information
 **/
/******************************************************************************/
ComponentField_t g_componentStatus = {CINDEX_STANDBY,  // mop --- standby
                                      CINDEX_ROLLERNORMAL,  // roller --- 滚筒正常
                                      CINDEX_PUMPNORMAL,  // pump --- 水泵正常
                                      CINDEX_BATTERYNORMAL,  // battery --- 电池正常
                                      CINDEX_UNCHARGED,  // charge --- 未充电
                                      CINDEX_CLEARWATERNORMAL,   // clearWater --- 清水正常
                                      CINDEX_CONNECTED   // connected
                                      };
/******************************************************************************/
char g_buf[U8FIFOSIZE];    // Temporary variables for uart received data resolve
kv_t g_KVarr[CKVTABSIZE];  // Temporary variables for uart received data resolve
/******************************************************************************/
/**
 * UART1: GPIOA.2/GPIOA.3
 * UART2: GPIOC.3/GPIOC.4
 * VOP data : GPIOD.2
 * VOP Power: GPIOD.3
 *
 * Key:       GPIOC.6
 * wifi_en:   GPIOC.5
 **/
/******************************************************************************/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /*!< At this stage the microcontroller clock setting is already configured,
       this is done through SystemInit() function which is called from startup
       file (KEIL_startup_hk32f0301mxxc.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_hk32f0301mxxc.c file
     */
    /* Configure clock GPIO, UARTs */
    RCC_Configuration4uart();

    /* Configure GPIO Pin Tx/Rx for Uart communication */
    GPIO_Configuration4uart();

    /* Configure NVIC */
    NVIC_Configuration4uart();

    UART1_Configuration();
    UART2_Configuration();
    // SysTick_Config(SystemCoreClock / 1000);        // 1ms
    SysTick_Config(SystemCoreClock / 100);       // 10ms
    TIM_Config();
    GPIO_init4led();
    GPIO_PC06_K11INPUT();
    
    GPIO_initVOPPort();
    GPIO_init485();
    vp_init();
    watchDog_init();
    /*********************************/
    promptInit();
    rs485Init();
    
    fstack_init(&g_fstack);
    //func.func = f_init;
    func.func = f_idle;
    fstack_push(&g_fstack, &func);
    
    //for(int i = 0; i < MTABSIZE(g_ustimer); i++) {
    //    ClrTimer(&g_ustimer[i]);
    //}
    for(int i = 0; i < MTABSIZE(g_timer); i++) {
        ClrTimer(&g_timer[i]);
    }
    SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);

    u8FIFOinit(&g_uart1TxQue);
    u8FIFOinit(&g_uart1RxQue);
    u8FIFOinit(&g_uart2TxQue);
    u8FIFOinit(&g_uart2RxQue);
  
    msgq_init(&g_msgq);
    msg.msgType = CSYS_INIT;
    msgq_in_irq(&g_msgq, &msg);
    
    /* Infinite Loop */
    while(1)
    {
        IWDG_ReloadCounter();  // feed dog(Reload IWDG counter)
        //deamon_uart1_send();
        deamon_uart2_send();
        actionDoing(&g_promptQueue);
        rs485TransXDoing();
        
        if(msgq_out_irq(&g_msgq, &msg) == FALSE) {
            continue;
        }
        if(sysProcess(&msg) == TRUE) {
            continue;
        }

        if(fstack_top(&g_fstack, &func) == FALSE) {
            /** something wrong happend, Power Down or recover it **/
            fstack_init(&g_fstack);
            func.func = f_idle;
            fstack_push(&g_fstack, &func);
	
            g_tick = 0;
            SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
            continue;
        }
        func.func((unsigned *)&msg);
    }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */
