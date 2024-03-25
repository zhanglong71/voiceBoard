#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "version.h"
#include "l_arch.h"
#include <string.h>
#include <stdlib.h>
#include "l_u8FIFO.h"
#include "l_jsonTL.h"
#include "l_timer.h"
#include "l_gpio.h"
#include "l_actionFIFO.h"
#include "l_sysProcess.h"
#include "l_rs485.h"
#include "hk32f0301mxxc.h"

int sysProcess(void *pMsg)
{
    int iRet = TRUE;
    u8 buf[U8FIFOSIZE];
    u8Data_t u8Data;
    //u8 pCh = NULL;
    kv_t KVarr[CKVTABSIZE];
    // u8 len = 0;
    u8 i = 0;

    switch(((msg_t *)pMsg)->msgType)
    {
    case CMSG_UART2RX:
	    #if 1
        if(u8FIFOisEmpty(&g_uart2RxQue) != TRUE) {
            //u8 len = 0;
            u8Data_t u8Data;
            while (u8FIFOout_irq(&g_uart2RxQue, &u8Data) == TRUE) {
                //u8FIFOin_irq(&g_uart2TxQue, &u8Data); //????????????????????????????
            } 
        }
        #else
        if(u8FIFOisEmpty(&g_uart2RxQue) != TRUE) {
            /** do something to uart3 data **/
            //???????????????????????????????????????
            
            u8 len = 0;
            u8Data_t u8Data;
            objType_t objtype = sm_receiveData(buf);
            if ((objtype == obj_key) || (objtype == obj_len) || (objtype == obj_body)) {
                #if 0
            	len = strlen(buf);
                for(int i = 0; i < len; i++) {
                    u8Data.u8Val = buf[i];
                    u8FIFOin_irq(&g_uart2TxQue, &u8Data);
                }
                #endif
            }
            //???????????????????????????????????????
        }
       #endif

        break;
    //case CMSG_MASTER:
    case CUART1_TOUT:
        /************************************
         * receive: {"xxxx":voi,"PLY":xxx,"SEQ":num}
         * ack:
         *    {"voi":xxxx,"PLY":OK,"SEQ":num}
         *    {"voi":xxxx,"PLY":err,"SEQ":num}
         *
         * receive: {"user":voi,"ask":ver}
         * ack:     {"voi":user,"ver":v001}
         ************************************/
        memset(buf, 0, sizeof(buf));
        memset(KVarr, 0, sizeof(KVarr));
        
 #if 1
        for(i = 0; ((i < MTABSIZE(buf)) && (u8FIFOout_irq(&g_uart1RxQue, &u8Data) == TRUE)); i++) {
            buf[i] = u8Data.u8Val;
        }
        if (i < MTABSIZE(buf)) {
            buf[i] = '\0';
        } else {
            u8FIFOinit_irq(&g_uart1RxQue);  /** !!!!!!!!!!!! **/
            /** something wrong happened **/
            #if 1
            u8Data.u8Val = 'f';
            u8FIFOin_irq(&g_uart1TxQue, &u8Data);
            u8Data.u8Val = 'f';
            u8FIFOin_irq(&g_uart1TxQue, &u8Data);
            #endif
        }
 #endif
 #if 0
        for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
            u8Data.u8Val = buf[i];
            rs485_stor_irq(&u8Data);
        }
 #else
        if (JsonParseL0(buf, KVarr)) {
            int voi = 0;
            u8 flag = 0;
            u8 src_idx = 0;
            u8 u8Seq = 0;
            static u8 u8Seq_last = 0xff;
            //u8 voi_idx = 0;
            for (u8 i = 0; ((i < MTABSIZE(KVarr)) && (KVarr[i].KVIndex > 0)); i++) {
                 if (strstr(KVarr[i].value, "voi")) {
                     Mset_bit(flag,1);
                     src_idx = i;
                 }
                 if (strstr(KVarr[i].key, "PLY")) {
                     Mset_bit(flag,2);
                     voi = atoi(KVarr[i].value);
                 }
                 if (strstr(KVarr[i].key, "SEQ")) {
                     Mset_bit(flag,3);
                     u8Seq = atoi(KVarr[i].value);
                 }
                 if (strstr(KVarr[i].key, "ask") && strstr(KVarr[i].value, "ver")) {
                     Mset_bit(flag,4);
                 }
            }
            if (Mget_bit(flag, 1) && Mget_bit(flag, 2) && Mget_bit(flag, 3)) {
                if (voi < 100) {
                    if (u8Seq_last != u8Seq) {
                        u8Seq_last = u8Seq;
                        vp_stop1();
                        vp_stor(voi);
                    }
                    
                    /** construct ack**/
                    generateVoiceAckOk(KVarr[src_idx].key, u8Seq_last);
                } else {
                    generateVoiceAckErr(KVarr[src_idx].key, u8Seq_last);
                }
            }
            
            if (Mget_bit(flag, 1) && Mget_bit(flag, 4)) {
                generateVoiceAckVer(KVarr[src_idx].key, CVERSION);
            }
        };
#endif
        break;
        
    case CMSG_UART1RX:
	    #if 0
        if(u8FIFOisEmpty(&g_uart1RxQue) != TRUE) {
            //u8 len = 0;
            
            //while (u8FIFO_last(&g_uart1RxQue, &u8Data) == TRUE) {
            while (u8FIFOout_irq(&g_uart1RxQue, &u8Data) == TRUE) {
                u8FIFOin_irq(&g_uart2TxQue, &u8Data);//????????????????????????????
            } 
        }
        #endif
        break;
        
    case CMSG_UART2TX:
    #if 1
        sm_sendData(NULL);
    #endif
        break;
    
    case CMSG_UART1TX:
        /** no what ? **/
        
        break;

    case CGET_CHAR:
    #if 1
        (void)reportGetCharCmd(NULL);
    #endif
        break;
		
    case CPUT_CHAR:
         #if 0  //?????????????????? for test only
			(void)JsonParse(NULL, NULL);
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }
        #endif
			break;
        
    case CGETDEVINFO_REQ:
    case CGETDEVINFO_RSPERROR:
    #if 1
        (void)reportDevInfo(NULL);
    #endif
        break;
        
    case CREPORT_RSPERROR:   /** report Services error, try again **/
    case CGETDEVINFO_RSPOK:  /** according protocal report Services after devInfo reported ok **/
        #if 1
        (void)reportService(0);
        #endif
        break;
        
    case CREPORT_RSPOK:
        /** do nothing **/
        #if 0  //?????????????????? for test only
        u8Data.u8Val = 't';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 'e';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 's';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        u8Data.u8Val = 't';
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
        break;

    case CWIFI_STATUS:
        /** do what ? **/
        #if 0
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }
        #endif
        break;    
        
    case CHEART_BEAT:
        #if 1
        (void)reportHeartbeat(NULL);
        #endif
        break;

    case CWIFI_TEST:
        /** do test only! 2-byte data !!! **/
        #if 0
             len = strlen(buf);
             for(int i = 0; i < len; i++) {
                  u8Data.u8Val = buf[i];
                  u8FIFOin_irq(&g_uart3TxQue, &u8Data);
             }
        #endif
        break;
        
    case CPMT_TOUT:        /** 一段提示动作完成 **/
		actProcess(&g_promptQueue);
		break;
        
    case C485_STEP:
        rs485actProcess();
        break;
    
    case C485_OVER:    
        rs485actOver();
        break;
                
    default:
        iRet = FALSE;
        break;	
	}
    return  iRet;   
}
/**********************************************************************************************************/

