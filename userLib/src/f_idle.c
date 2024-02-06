#include "hk32f0301mxxc_it.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "l_arch.h"
#include "l_gpio.h"
#include "l_u8FIFO.h"
#include "l_jsonTL.h"
#include "l_actionFIFO.h"
#include "l_sysProcess.h"
#include "f_idle.h"

int f_idle(void *pMsg)
{
    switch(((msg_t *)pMsg)->msgType) 
    {
    case CMSG_TMR:
        g_tick++;
        /********************/
        //GPIO_led2_blink();
        //u8Data_t u8Data;
        //u8Data.u8Val = g_tick & 0x7f;
        //u8FIFOin_irq(&g_uart2TxQue, &u8Data); //????????????????????????????
        //u8FIFOin_irq(&g_uart1TxQue, &u8Data); //????????????????????????????
       
        // vp_stor((g_tick & 0x1f));        //????????????????????????????????
        
        //reportVersion();  // ???????????????????????????
        /********************/
        break;
        
    case CMSG_INIT:
        g_tick = 0;
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
	    break;
          
    case CPMT_OVER:
        GPIO_VOPPWR_off();
        break;
        
    default:
        break;
	}  

    return  0;
}

int f_init(void *pMsg)
{
    func_t func;
    msg_t msg;
    switch(((msg_t *)pMsg)->msgType) 
    {
    case CMSG_TMR:
        g_tick++;
        break;
        
    case CSYS_INIT:        // step1
        //M485TR_T("set H before send data!!!");  //??????????????
        //GPIO_VOPPWR_on();  //????????
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CSYS_INITS1);
	    break;
 
    case CSYS_INITS1:      // step2
        reportVersion();
        //vp_stor(vopIdx_STOP);
        //promptInit();
        //vp_stop();
        // vp_setDefaultVolume();
        // vp_stor(vopIdx_standard);
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
	    break;
        
    case CPMT_OVER:       // over
        GPIO_VOPPWR_off();
        fstack_init(&g_fstack);
        func.func = f_idle;
        fstack_push(&g_fstack, &func);

        msgq_init(&g_msgq);
        msg.msgType = CSYS_INIT;
        msgq_in_irq(&g_msgq, &msg);
        g_tick = 0;
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
        break;
        
    default:
		break;
	}  

    return  0;
}


