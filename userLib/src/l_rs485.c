#include "hk32f0301mxxc_it.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"

#include "l_arch.h"
#include "l_u8FIFO.h"
#include "l_rs485.h"
/*******************************************************************************
 * rs485 translate status:
 * SM_RS485_IDLE  --- wait for start transmit
 * SM_RS485_START --- prepare(prefix) before transmit(maybe do nothing)
 * SM_RS485_SEND  --- doing transmit
 * SM_RS485_OVER  --- over(postfix) after transmit(maybe do nothing)
 *******************************************************************************/
RetStatus rs485_stor_irq(u8Data_t *u8Data)
{
    if (rs485transX.sm_status == SM_RS485_IDLE) { 
        u8FIFOin_irq(rs485transX.pfifo, u8Data);
    } else {  // busy
        return PERROR;
    }
    return POK;
}

void rs485Init(void)
{
	rs485transX.pfifo = &g_uart1TxQue;
    rs485transX.ptimer = &(g_ustimer[0]);
    rs485transX.sm_status = SM_RS485_IDLE;
    rs485transX.stepMsgType = C485_STEP;
    rs485transX.overMsgType = C485_OVER;
    ClrTimer_irq(rs485transX.ptimer);
}

void rs485TransXDoing(void)
{
    switch(rs485transX.sm_status)
    {
    case SM_RS485_IDLE:
        if(u8FIFOisEmpty(rs485transX.pfifo) != TRUE) {
            rs485transX.sm_status = SM_RS485_START;
            // SetTimer_irq(rs485transX.ptimer, TIMER_2UNIT, rs485transX.stepMsgType);
            msg_t msg;
            msg.msgType = rs485transX.stepMsgType;
            msgq_in_irq(&g_msgq, &msg);
        }
        break;
    case SM_RS485_START:
        /** do nothing(DE=H 50us... only wait timer) **/
        break;
        
    case SM_RS485_SEND:
        if(u8FIFOisEmpty(rs485transX.pfifo) == TRUE) { /** 发完成后回发空闲状态 **/
            rs485transX.sm_status = SM_RS485_IDLE;
        }
        break;

    default:
        break;
	}
}

void rs485_send_start(void)
{
    /* Enable transfer empty interrupt */
    UART_ITConfig(UART1, UART_IT_TXE, ENABLE);

    /* Enable transfer complete interrupt */
    UART_ITConfig(UART1, UART_IT_TC, DISABLE);
}

/*****************************************************************************
 * process the "C485_TOUT" only
 *****************************************************************************/
void rs485actProcess(void)
{
    switch(rs485transX.sm_status)
    {
    case SM_RS485_IDLE:
        /** do nothing **/
        break;
        
    case SM_RS485_START:
        rs485transX.sm_status = SM_RS485_SEND;
        //ClrTimer_irq(rs485transX.ptimer);
        M485TR_T("发送前置高");
        rs485_send_start();
        break;
        
    case SM_RS485_SEND:
        /** do nothing **/
        break;
                    
    default:
        break;
	}
}

/*****************************************************************************
 * process the "C485_OVER"
 *****************************************************************************/
void rs485actOver(void)
{
#if 0
    switch(rs485transX.sm_status)
    {
    case SM_RS485_IDLE:
    case SM_RS485_START:
        /** do nothing **/
        break;
        
    case SM_RS485_SEND:  // send complete
        rs485transX.sm_status = SM_RS485_IDLE;
        ClrTimer_irq(rs485transX.ptimer);
        //M485TR_R("发送完成，进入等待接收状态");
        break;
                    
    default:
        break;
	}
#endif
}

