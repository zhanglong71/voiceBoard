/**
  ******************************************************************************
  * @file    main.h 
  * @author  Alexander
  * @version V1.0.0
  * @date    30-Jan-2023
  * @brief   Header for main.c module
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "hk32f0301mxxc.h"

#ifdef USE_HK32F0301MF4P7C_EVAL
//#include "hk32f0301mf4p7c_eval.h"
#endif /* USE_HK32F0301MF4P7C_EVAL */

/* Exported types ------------------------------------------------------------*/
typedef struct 
{
	uint8_t Data[256];
	uint8_t Index;
	uint8_t Len;
	uint8_t FinishedFlag;
}UartRxBufDef;

extern UartRxBufDef Uart1Rx,Uart2Rx;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

