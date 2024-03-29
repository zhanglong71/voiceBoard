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
    int flag;
    u8 len = 0;
    u8 i = 0;

    switch(((msg_t *)pMsg)->msgType)
    {
    case CMSG_UART2RX:
        if(u8FIFOisEmpty(&g_uart2RxQue) != TRUE) {
            /** do something to uart2 data **/
            u8Data_t u8Data;
            objType_t objtype = sm_receiveData(buf);
            if (objtype == obj_none) {
                /** nothing **/
            } else if ((objtype == obj_key) || (objtype == obj_len) || (objtype == obj_body)) {
                /** do something **/
                /** what ? **/
            } else if (objtype == obj_SSID) {
                memset(KVarr, 0, sizeof(KVarr));
                JsonParseL0(buf, KVarr);
                for (u8 i = 0; ((i < MTABSIZE(KVarr)) && (KVarr[i].KVIndex > 0)); i++) {
                    if (strstr(KVarr[i].key, "status") && strstr(KVarr[i].value, "1")) {
                        flag |= (1 << 1);
                    }
                    if (strstr(KVarr[i].key, "ssid")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.ssid, KVarr[i].value, sizeof(g_netInfo.ssid));
                    }
                }
                if ((flag & 0x60) == 0x60) {
                    g_netInfo.flag |= (1 << 0);
                }
            } else if (objtype == obj_IP) {
                memset(KVarr, 0, sizeof(KVarr));
                JsonParseL0(buf, KVarr);
                for (u8 i = 0; ((i < MTABSIZE(KVarr)) && (KVarr[i].KVIndex > 0)); i++) {
                    if (strstr(KVarr[i].key, "status") && strstr(KVarr[i].value, "1")) {
                        flag |= (1 << 1);
                    }
                    if (strstr(KVarr[i].key, "ip")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.ip, KVarr[i].value, sizeof(g_netInfo.ip));
                    }
                }
                if ((flag & 0x60) == 0x60) {
                    g_netInfo.flag |= (1 << 1);
                }
            } else if (objtype == obj_MAC) {
                memset(KVarr, 0, sizeof(KVarr));
                JsonParseL0(buf, KVarr);
                for (u8 i = 0; ((i < MTABSIZE(KVarr)) && (KVarr[i].KVIndex > 0)); i++) {
                    if (strstr(KVarr[i].key, "status") && strstr(KVarr[i].value, "1")) {
                        flag |= (1 << 1);
                    }
                    if (strstr(KVarr[i].key, "mac")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.mac, KVarr[i].value, sizeof(g_netInfo.mac));
                    }
                }
                if ((flag & 0x60) == 0x60) {
                    g_netInfo.flag |= (1 << 2);
                }
            } else if (objtype == obj_RSSI) {
                g_netInfo.rssi = atoi(buf);
                g_netInfo.flag |= (1 << 3);
            }
        }

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
        /** nothing **/
        
        break;

    case CGET_CHAR:
    #if 0
        (void)reportGetCharCmd(NULL);
    #endif
        break;
        
    case CGETCHAR_MOP:
        (void)checkAndAckGetCharWorkMode();
        break;
    case CGETCHAR_ROLLER:
        (void)checkAndAckGetCharRollerStatus();
        break;
    case CGETCHAR_CLEARWATER:
        (void)checkAndAckGetCharClearWaterStatus();
        break;
    case CGETCHAR_PUMP:
        (void)checkAndAckGetCharPumpStatus();
        break;
    case CGETCHAR_BATTERY:
        (void)checkAndAckGetCharBatteryStatus();
        break;
    case CGETCHAR_CHARGE:
        (void)checkAndAckGetCharChargeStatus();
        break;
    case CGETCHAR_NETINFO:
        checkAndAckGetCharNetInfo();
        break;
    case CGETCHAR_UPDATE:
        checkAndAckGetCharUpdate();
        break;
    case CPUT_SYNC:
        reportAckPutSync();
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

/*************************************************************************************/
int checkWorkModeChange(u8* status)  // !!!!!!!!!!!!!!!!
{
    return FALSE;
}

void checkAndReportWorkMode(void)
{
    u8 status = 0;
    if (checkWorkModeChange(&status) == TRUE) {
        (void)reportComponentStatus(status);
    }
}

void checkAndAckGetCharWorkMode(void)
{
    u8 status = 0;
    if (checkWorkModeChange(&status) == TRUE) {
        (void)getCharAckComponentStatus(status);
    }
}
/*********************roller***********************************************************************/
int checkRollerStatusChange(u8* status)
{
    u8 static roller_status_last = 0;
    
    if (status == NULL ) { /** something wrong**/
        return FALSE;
    }
 
    if (roller_status_last != *status) {
        roller_status_last = *status;
        /** report **/
        return TRUE;
    }
    return FALSE;
}

void checkAndReportRollerStatus(void)
{
    u8 roller_status = 0;

    if (checkRollerStatusChange(&roller_status) == TRUE) {
        /** report **/
        (void)reportComponentStatus(roller_status);
    }
}

void checkAndAckGetCharRollerStatus(void)
{
    u8 roller_status = 0;

    if (checkRollerStatusChange(&roller_status) == TRUE) {
        /** report **/
        (void)getCharAckComponentStatus(roller_status);
    }
}


/************************** pump *****************************************************************/
int checkPumpStatusChange(u8* status)
{
    return FALSE;
}

/**
 * the g_componentStatus.pump state get from IDs_Judge() only
 **/
void checkAndReportPumpStatus(void)
{
}

void checkAndAckGetCharPumpStatus(void)
{
}
/**************************************************************************************************/
int checkBatteryChange(u8* status)
{
    u8 static battery_status_last = 0;

    if (status == NULL) {
        return FALSE;
    }
    
    if (battery_status_last != *status) {
        battery_status_last = *status;
        /** report **/
        return TRUE;
    }
    return FALSE;
}

void checkAndReportBatteryStatus(void)
{
    u8 battery_status = 0;
    
    if (checkBatteryChange(&battery_status) == TRUE) {
        /** report **/
        (void)reportComponentStatus(battery_status);
    }
}

void checkAndAckGetCharBatteryStatus(void)
{
    u8 battery_status = 0;
    
    if (checkBatteryChange(&battery_status) == TRUE) {
        /** report **/
        (void)getCharAckComponentStatus(battery_status);
    }
}

/**************************************************************************************************/
int checkChargeChange(u8* status)
{
    u8 static charge_status_last = 0;

    if (charge_status_last != *status) {
        charge_status_last = *status;
        /** report **/
        return TRUE;
    }
    return FALSE;
}

void checkAndReportChargeStatus(void)
{
    u8 charge_status = 0;

    if (checkChargeChange(&charge_status) == TRUE) {
        /** report **/
        (void)reportComponentStatus(charge_status);
    } else {
        if (charge_status == CINDEX_UNCHARGED) {
            // (void)reportBatteryLevel(sysvar.BAT_soc);
        } else {
            reportComponentStatus(charge_status);
        }
    }
}

void checkAndAckGetCharChargeStatus(void)
{
    u8 charge_status = 0;

    if (checkChargeChange(&charge_status) == TRUE) {
        /** report **/
        (void)getCharAckComponentStatus(charge_status);
    }
}
/**************************************************************************************************/
int checClearWaterChange(u8* status)
{
    u8 static clear_status_last = 0;

    if (status == NULL) {
        return FALSE;
    }
    
    if (clear_status_last != *status) {
        clear_status_last = *status;
        /** report **/
        return TRUE; // !!!!!!!!!!!!!!
    }
    return FALSE;
}

void checkAndReportClearWaterStatus(void)
{
    u8 clear_status = 0;

    if (checClearWaterChange(&clear_status) == TRUE) {
        /** report **/
        (void)reportComponentStatus(clear_status);
    }
}

void checkAndAckGetCharClearWaterStatus(void)
{
    u8 clear_status = 0;
    if (checClearWaterChange(&clear_status) == TRUE) {
        /** report **/
        (void)getCharAckComponentStatus(clear_status);
    }
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharNetInfo(void)
{
    reportgetCharNetInfo(&g_netInfo);
}

void netInfoData_init(void)
{
    g_netInfo.flag = 0;
    g_netInfo.rssi = 0;
    strcpy(g_netInfo.ssid, "ssid");
    strcpy(g_netInfo.ip, "0.0.0.0");
    strcpy(g_netInfo.mac, "0:0:0:0:0:0");
}

void checkAndAckGetCharUpdate(void)
{
    (void)getCharAckComponentStatus(CINDEX_UPDATE);
}

/*****************************************************************************/
void setStatus(void)
{
    return;
}
/*****************************************************************************/
pair_u8vpv_t command2status[] = {
	{vopIdx_ConnectNo, setStatus},   //=1, //未连接设备//共用//
	{vopIdx_Disconnect, setStatus},  // =2,//设备已断开
	// {vopIdx_Install, setStatus},  // =3,//进入设置模式
	// {vopIdx_VoiceOpen, setStatus},  // =4,//开启语音
	// {vopIdx_VoiceClose, setStatus},  // =5,//关闭语音
	// {vopIdx_WifiReset, setStatus},  // =6,//网络复位成功
	// {vopIdx_WifiConnecting, setStatus},  // =7,//网络正在连接
	// {vopIdx_WifiOk, setStatus},  // =8,//网络连接成功
	
	{vopIdx_CHing, setStatus},  // =9,//开始充电
	{vopIdx_CHcomplete, setStatus},  // =10,//充电已完成
	{vopIdx_Choff, setStatus},  // =11,//充电中断
	{vopIdx_standard, setStatus},  // =12,//进入标准模式

	#if 0	
	{vopIdx_RUNm2, setStatus},  // =13,//进入强力模式
	// {vopIdx_nop2, setStatus},  // =14,//大水冲洗模式
	// {vopIdx_RUNCL, setStatus},  // =15,//进入自清洗模式
	// {vopIdx_RunclOver, setStatus},  // =16,//自清洗已完成
	{vopIdx_RUNover, setStatus},  // =17,//运行结束，请放回底座自清洗
	{vopIdx_RUNOFF, setStatus},  // =18,//运行结束
	{vopIdx_Chlowing, setStatus},  // =19,//电量不足，请及时充电
	{nop3, setStatus},  // =20,//电量不足，请立即充电
	{nop4, setStatus},  // =21,//电量不足，请充电后继续
	{vopIdx_CHErr, setStatus},  // =22,//充电异常，请检查充电器
	{vopIdx_ConnectDragLala, setStatus},  // =23,//洗地机已连接
	//{vopIdx_CisternOk, setStatus},  // =24,//水箱已安装
	//{vopIdx_CisternNo, setStatus},  // =25,//水箱已取出
	//{vopIdx_CisternTake, setStatus},  // =26,//水箱已取出，进入大水冲洗模式
	//{vopIdx_sewageErr, setStatus},  // =27,//污水箱已满，请清理污水箱
	{vopIdx_ClearErr, setStatus},  // =28,//请加入清水
	{vopIdx_PumpErr, setStatus},  // =29,//水泵电机异常
	{nop5, setStatus},  // =30,//水泵电机未安装
	{vopIdx_RollerErr, setStatus},  // =31,//请检查滚筒
#endif
};

