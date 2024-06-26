#include "hk32f0301mxxc_it.h"
#include <string.h>
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "version.h"

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
    // rs485transX.ptimer = &(g_ustimer[0]);
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

/**********************************************************************************************/
RetStatus reportVersion(void)
{
    u8 i = 0;
    u8Data_t u8Data;
    char buf[32];
    RetStatus retStatus = POK;
    
    //sprintf(buf, "voice softVER:202403201423.%s" CVERSION);
    sprintf(buf, "%s.%s.%s.%s", CBOARD, CWARE, CDATETIME, CVERSION);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        retStatus = rs485_stor_irq(&u8Data);
        if (retStatus != POK) {
            return retStatus;
        }
    }
    return retStatus;
}

/**********************************************************************************************/

/*********************************************
 * {"voi":xxx,"ver":vxxxx}
 *********************************************/
void generateVoiceAckVer(char* to, u8* ver)
{
    u8 i = 0;
    u8Data_t u8Data;
    char buf[32];
    
    sprintf(buf, "{\"voi\":%s,\"ver\":%s}", to, ver);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        rs485_stor_irq(&u8Data);
    }
}

/*********************************************
 * {"voi":xxxx,"PLY":OK}
 * {"voi":xxxx,"PLY":err}
 *********************************************/
 void generateVoiceAckOk(char* to, u8 seq)
{
    u8 i = 0;
    u8Data_t u8Data;
    char buf[64];
    
    sprintf(buf, "{\"voi\":%s,\"PLY\":OK,\"SEQ\":%d}", to, seq);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        //u8FIFOin_irq(&g_uart1TxQue, &u8Data);
        rs485_stor_irq(&u8Data);
    }
}

 void generateVoiceAckErr(char* to, u8 seq)
{
    u8 i = 0;
    u8Data_t u8Data;
    char buf[64];
    
    sprintf(buf, "{\"voi\":%s,\"PLY\":err,\"SEQ\":%d}", to, seq);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        //u8FIFOin_irq(&g_uart1TxQue, &u8Data);
        rs485_stor_irq(&u8Data);
    }
}

/*******************************************************************************
 * prase json body(JsonParseL0)
 * 
 * example:
 * {key1:value1, key2:value2, ... ,keyn:valuen}
 *******************************************************************************/
unsigned char JsonParseL0(char* jsonstrbuf, kv_t* jsonstcarr)
{
    u8 j_u8,k_u8,i_u8 = 0;
    char *p[CMAX1_COUPLE * 2];
    char *pChar = jsonstrbuf;
    jsonstcarr[0].KVIndex = 0;
    // u8Data_t u8Data;

    /** 1. simily as '{ ... }'! **/
    if((jsonstrbuf[0] != '{') || (jsonstrbuf[strlen(jsonstrbuf) - 1] != '}')) {
        return(0);
    }

    jsonstrbuf[strlen(jsonstrbuf) - 1] = '\0';            /** overwrite the start '}' ! **/
    for(j_u8 = 0; j_u8 < strlen(jsonstrbuf); j_u8++) {    /** overwrite the end '{' ! **/
        jsonstrbuf[j_u8] = jsonstrbuf[j_u8 + 1];
    }
    
    while((p[i_u8]  = strtok(pChar, ":,")) != NULL) {    /** split the string ... **/
        i_u8++;
        pChar = NULL;
    }
 
    for(j_u8 = 0; ((j_u8 < i_u8/2) && (j_u8 < CMAX1_COUPLE)); j_u8++) {
        jsonstcarr[j_u8].KVIndex = i_u8/2 - j_u8;
        strcpy(jsonstcarr[j_u8].key, p[j_u8 * 2]);
        strcpy(jsonstcarr[j_u8].value, p[j_u8 * 2 + 1]);

        //(void)strim_quot(jsonstcarr[j_u8].key);
        jsonstcarr[j_u8].key[strlen(jsonstcarr[j_u8].key) - 1] = '\0';           /** overwrite the start '"' ? **/
        for(k_u8 = 0; k_u8 < strlen(jsonstcarr[j_u8].key); k_u8++)                 /** overwrite the end '"' ? **/
        {
            jsonstcarr[j_u8].key[k_u8] = jsonstcarr[j_u8].key[k_u8 + 1];
        }
    }

    return (1);
}


