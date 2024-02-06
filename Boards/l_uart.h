#ifndef __L_UART_H_
#define __L_UART_H_

void deamon_uart1_send(void);
void deamon_uart2_send(void);

void UART1_Configuration(void);
void UART2_Configuration(void);

void RCC_Configuration4uart(void);
void GPIO_Configuration4uart(void);
void NVIC_Configuration4uart(void);


#endif
