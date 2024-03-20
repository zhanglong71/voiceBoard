#ifndef __L_GPIO_H_
#define __L_GPIO_H_

void GPIO_initVOPPort(void);
void GPIO_init485(void);

void GPIO_VOPPWR_off(void);
void GPIO_VOPPWR_on(void);


void GPIO_init4led(void);
void GPIO_led_blink(void);
void GPIO_led1_blink(void);
void GPIO_led2_blink(void);

void watchDog_init(void);

void GPIO_PC06_K11INPUT(void);
void test_outPC05byPC06(void);


#endif
