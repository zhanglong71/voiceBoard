#include "hk32f0301mxxc_it.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "l_arch.h"
#include "l_gpio.h"

void GPIO_initVOPPort(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOD Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);

  /* Configure PD02() in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
    
  /* Configure PD03(busy) in input pushpull mode */
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  MVopData_H("init status: H");
  // GPIO_ResetBits(GPIOD, GPIO_Pin_3);
  MVopPower_off("init power status: power off");
}

void GPIO_init485(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* GPIOB Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* Configure PB04() in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  M485TR_R("不发送时，485处于接收状态");
}

#if 0
void GPIO_VOPPWR_on(void)
{
    GPIO_SetBits(GPIOD, GPIO_Pin_3);
}

void GPIO_VOPPWR_off(void)
{
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);
}
#endif

void watchDog_init(void)
{
    /* IWDG timeout equal to 250 ms (the timeout may varies due to LSI frequency
       dispersion) */
    /* Enable write access to IWDG_PR and IWDG_RLR registers */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    
    /* IWDG counter clock: LSI/32 */
    IWDG_SetPrescaler(IWDG_Prescaler_8);
    
    /* Set counter reload value to obtain 250ms IWDG TimeOut.
       Counter Reload Value = 250ms/IWDG counter clock period
                            = 250ms / (LSI/8)
                            = 0.25s / (LsiFreq/8)
                            = LsiFreq/(8 * 4)
                            = LsiFreq/32
     */
    IWDG_SetReload(/*LsiFreq = */ 60000 / 32);
    
    /* Reload IWDG counter */
    IWDG_ReloadCounter();
    
    /* Enable IWDG (the LSI oscillator will be enabled by hardware) */
    IWDG_Enable();
}

/**********************
 * for output pin 
 **********************/
void GPIO_init4led(void)
{
  GPIO_InitTypeDef        GPIO_InitStructure;
  /* GPIOD Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC | RCC_AHBPeriph_GPIOD, ENABLE);

  /* Configure PD01 in output pushpull mode,  wifi enable  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    // GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_SetBits(GPIOD, GPIO_Pin_1);

  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  /** test only **/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void GPIO_led1_blink(void)
{
    GPIO_Toggle(GPIOC, GPIO_Pin_3);
}

void GPIO_led2_blink(void)
{
    GPIO_Toggle(GPIOC, GPIO_Pin_4);
}

void GPIO_led_blink(void)
{
#if 0
    if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_7) == Bit_RESET) { \
        GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);    \
    } else {                                          \
        GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);  \
    }
    
    if(GPIO_ReadOutputDataBit(GPIOD, GPIO_Pin_1) == Bit_RESET) { \
        GPIO_WriteBit(GPIOD, GPIO_Pin_1, Bit_SET);    \
    } else {                                          \
        GPIO_WriteBit(GPIOD, GPIO_Pin_1, Bit_RESET);  \
    }
#else
    GPIO_Toggle(GPIOC, GPIO_Pin_3);
    GPIO_Toggle(GPIOC, GPIO_Pin_4);
#endif
}

void GPIO_PC06_K11INPUT(void) // for test ??????????????????????????????????
{
    GPIO_InitTypeDef        GPIO_InitStructure;
   
    /* Enable GPIOC clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
    
    /* Configure PC7 pin as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

#if 0
void test_outPC05byPC06(void)  // used wifi_EN pin for test ??????????????????????????????????
{
#if 1
    GPIO_Toggle(GPIOC, GPIO_Pin_5);
    for(int i = 0; i < 100; i++);
#else
    if (GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)) {
        GPIO_SetBits(GPIOC, GPIO_Pin_5);
    } else {
        GPIO_ResetBits(GPIOC, GPIO_Pin_5);
    }
#endif
}
#endif

