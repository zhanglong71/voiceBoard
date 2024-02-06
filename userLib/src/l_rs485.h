#ifndef __L_RSA85_H__
#define __L_RSA85_H__

void rs485Init(void);
//void rs485_stor(u8Data_t *u8Data);
void rs485_stor_irq(u8Data_t *u8Data);

void rs485actProcess(void);
void rs485TransXDoing(void);
smStatus_t rs485NextStep(smStatus_t cur_status);


#endif
