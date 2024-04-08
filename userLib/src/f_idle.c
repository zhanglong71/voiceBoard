#include "hk32f0301mxxc_it.h"
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "l_arch.h"
#include "l_gpio.h"
#include "l_u8FIFO.h"
#include "l_rs485.h"
#include "l_jsonTL.h"
#include "l_actionFIFO.h"
#include "l_sysProcess.h"
#include "f_idle.h"

int f_idle(void *pMsg)
{
	  // func_t func;
    msg_t msg;

    switch(((msg_t *)pMsg)->msgType) 
    {
    case CMSG_TMR:
        g_tick++;
        reportBatteryLevel(g_tick % 100);  // ?????????????????????????????????? for debug only
        break;
        
    case CMSG_INIT:
        g_tick = 0;
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
	    break;
          
    case CPMT_OVER:
        // GPIO_VOPPWR_off();
        break;

    case CCONN_ROUTE:
        //fstack_init(&g_fstack);
        // func.func = f_getNetInfo;
        //fstack_push(&g_fstack, &func);
        
        msg.msgType = CMSG_INIT;
        msgq_in_irq(&g_msgq, &msg);
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
    RetStatus retStatus = POK;
    
    switch(((msg_t *)pMsg)->msgType) 
    {
    case CMSG_TMR:
        g_tick++;
        break;
        
    case CSYS_INIT:        // step1
        retStatus = reportVersion();
        if (retStatus != POK) {  // busy! try again later; giveup the 
            SetTimer_irq(&g_timer[0], TIMER_100MS, CSYS_INIT);
        } else {
            SetTimer_irq(&g_timer[0], TIMER_1SEC, CSYS_INITS1);
        }
	    break;
 
    case CSYS_INITS1:      // step2
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
        actionDelay(TIMER_50MS);
	    break;
        
    case CPMT_OVER:       // over
        // GPIO_VOPPWR_off();
        fstack_init(&g_fstack);
        func.func = f_idle;
        fstack_push(&g_fstack, &func);

        // msgq_init(&g_msgq);
        msg.msgType = CMSG_INIT;
        msgq_in_irq(&g_msgq, &msg);
        g_tick = 0;
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
        break;
        
    default:
		break;
	}  

    return  0;
}

/** get wifi netinfo **/
int f_getNetInfo(unsigned *pMsg)
{
    func_t func;
    msg_t msg;
    RetStatus_pfunc_void_t funcArr[] = {
        doNothing,
        reportgetSsid,
        reportgetIp,
        reportgetMac,
        reportgetRssi,
    };

    switch(((msg_t *)pMsg)->msgType)
    {
    case CMSG_INIT:
        g_tick = 0;
        SetTimer_irq(&g_timer[0], TIMER_1SEC, CMSG_TMR);
        g_netInfo.flag = 0;
		break;
    
    case CMSG_TMR:
        g_tick++;
        if (g_tick >= MTABSIZE(funcArr)) {
            fstack_init(&g_fstack);
            func.func = f_idle;
            fstack_push(&g_fstack, &func);
            msg.msgType = CMSG_INIT;
            msgq_in_irq(&g_msgq, &msg);
        } else {
            (void)funcArr[g_tick]();
        }
        break;
        
    default:
        break;
    }
	
    return  0;
}



