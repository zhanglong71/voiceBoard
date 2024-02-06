#include "hk32f0301mxxc_it.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"

#include "l_arch.h"
#include "l_u8FIFO.h"
#include "l_rs485.h"
/*******************************************************************************/
//void rs485_stor(u8Data_t *u8Data)
void rs485_stor_irq(u8Data_t *u8Data)
{
    if (rs485transX.sm_status == SM_RS485_IDLE) { 
        u8FIFOin_irq(rs485transX.pfifo, u8Data);
    } else {
        /** too many or error !!! **/
    }
}

#if 0
void rs485Init(rs485transX_t *q, Timer_t *timer, msgType_t stepMsgType, msgType_t overMsgType)
{
	q->pfifo = &g_uart1TxQue;
    q->ptimer = &(g_ustimer[0]);
    q->sm_status = SM_RS485_IDLE;
    q->stepMsgType = C485_TOUT;
    q->overMsgType = C485_OVER;
    ClrTimer_irq(q->ptimer);
}
#endif

void rs485Init(void)
{
	rs485transX.pfifo = &g_uart1TxQue;
    rs485transX.ptimer = &(g_ustimer[0]);
    rs485transX.sm_status = SM_RS485_IDLE;
    rs485transX.stepMsgType = C485_TOUT;
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
            //M485TR_T("发送前置高");
            SetTimer_irq(rs485transX.ptimer, TIMER_2UNIT, rs485transX.stepMsgType);
        }
        break;
    case SM_RS485_START:
        /** do nothing(DE=H 50us... only wait timer) **/
        break;
        
    case SM_RS485_SEND:
        //rs485_deamon_send();
        if(u8FIFOisEmpty(rs485transX.pfifo) == TRUE) { /** 发完成后回发空闲状态 **/
            rs485transX.sm_status = SM_RS485_IDLE;
        }
        break;
                    
    default:
        break;
	}
}

#if 0
void rs485_deamon_send(void)
{
        msg_t msg;
        u8Data_t u8Data;
        if(u8FIFOisEmpty(rs485transX.pfifo) == TRUE) {
            /****/
            return;
        }
        //if(UART_GetFlagStatus(UART1, UART_FLAG_TC) == RESET) {
        if(UART_GetFlagStatus(UART1, UART_FLAG_TXE) == RESET) {
            return;
        }
        
        if(u8FIFOout_irq(rs485transX.pfifo, &u8Data) == TRUE) {
            M485TR_T("发送前置高");  // ???????????????
            UART_SendData(UART1, u8Data.u8Val);
            /** 如果队列已空，数据发送完成 **/
            if(u8FIFOisEmpty(rs485transX.pfifo) == TRUE) {
                //msg.msgType = CMSG_RS485TX;
                //msgq_in_irq(&g_msgq, &msg); 
                SetTimer_irq(rs485transX.ptimer, TIMER_2UNIT, rs485transX.overMsgType);
            }
        } 
}
#endif

#if 1
void rs485_send_start(void)
{
#if 0
        msg_t msg;
        u8Data_t u8Data;

        if(u8FIFOout_irq(rs485transX.pfifo, &u8Data) == TRUE) {
            UART_SendData(UART1, u8Data.u8Val);
        }
#endif
        /* Enable transfer empty interrupt */
        UART_ITConfig(UART1, UART_IT_TXE, ENABLE);

        /* Enable transfer complete interrupt */
        UART_ITConfig(UART1, UART_IT_TC, DISABLE);
}
#endif


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
        ClrTimer_irq(rs485transX.ptimer);
        //rs485_deamon_send();
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
}


#if 0
smStatus_t rs485NextStep(smStatus_t cur_status)
{
#if 1
    if (cur_status == SM_RS485_IDLE) {
        return SM_RS485_START;
    } else if (cur_status == SM_RS485_START) {
        return SM_RS485_SEND;
    }
#else
    const static rs485pair_t statusrs485step[] = {
        {sm_init, sm_step},
        {sm_idle, sm_step},
        {sm_step, sm_end},
    };

    for (int i = 0; i < MTABSIZE(statusrs485step); i++) {
        if ((u8)(statusrs485step[i].smStatus_curr) == cur_status) {
            return (smStatus_t)(statusrs485step[i].smStatus_next);
        }
    }
#endif
    return SM_RS485_ERROR;
}
#endif

