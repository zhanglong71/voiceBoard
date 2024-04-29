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
// #include "f_idle.h"
#include "hk32f0301mxxc.h"

int sysProcess(void *pMsg)
{
    //func_t func;
    //msg_t msg;

    int iRet = TRUE;
    // char buf[U8FIFOSIZE];
    u8Data_t u8Data;
    //u8 pCh = NULL;
    // kv_t KVarr[CKVTABSIZE];
    int flag;
    // u8 len = 0;
    u8 i = 0;

    switch(((msg_t *)pMsg)->msgType)
    {
    case CMSG_UART2RX:
        if(u8FIFOisEmpty(&g_uart2RxQue) != TRUE) {
            /** do something to uart2 data **/
            objType_t objtype = sm_receiveData(g_buf);
            if ((objtype == obj_SSID) || (objtype == obj_IP) || (objtype == obj_MAC)) {
                memset(g_KVarr, 0, sizeof(g_KVarr));
                strim(g_buf);
                if (JsonParseL0(g_buf, g_KVarr) == 0) {
                    /** do what? nothing! **/
                }
                for (i = 0; ((i < MTABSIZE(g_KVarr)) && (g_KVarr[i].KVIndex > 0)); i++) {
                    if (strstr(g_KVarr[i].key, "status") && strstr(g_KVarr[i].value, "1")) {
                        flag |= (1 << 1);
                    }
                    if (strstr(g_KVarr[i].key, "ssid")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.ssid, g_KVarr[i].value, sizeof(g_netInfo.ssid));  /** δ֪���ȣ����Ƴ�������**/
                    }
                    if (strstr(g_KVarr[i].key, "ip")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.ip, g_KVarr[i].value, sizeof(g_netInfo.ip));
                    }
                    if (strstr(g_KVarr[i].key, "mac")) {
                        flag |= (1 << 2);
                        strncpy(g_netInfo.mac, g_KVarr[i].value, sizeof(g_netInfo.mac));
                    }
                }
                if ((flag & 0x60) == 0x60) { /** make sure the status==1 **/
                    g_netInfo.flag |= (1 << 0);
                }
            } else if (objtype == obj_RSSI) {
                strncpy(g_netInfo.rssi, g_buf, sizeof(g_netInfo.rssi));
                g_netInfo.flag |= (1 << 3);
            } else { /** (objtype == obj_key) || (objtype == obj_len) || (objtype == obj_body) || (objtype == obj_none) **/
                /** do nothing **/
            }
        }
        break;

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
        memset(g_buf, 0, sizeof(g_buf));
        memset(g_KVarr, 0, sizeof(g_KVarr));

        for(i = 0; ((i < MTABSIZE(g_buf)) && (u8FIFOout_irq(&g_uart1RxQue, &u8Data) == TRUE)); i++) {
            g_buf[i] = u8Data.u8Val;
        }
        if (i < MTABSIZE(g_buf)) {
            g_buf[i] = '\0';
        } else {
            u8FIFOinit_irq(&g_uart1RxQue);  /** !!!!!!!!!!!! **/
            /** something wrong happened **/
        }

        if (JsonParseL0(g_buf, g_KVarr)) {
            int voi = 0;
            u8 flag = 0;
            u8 src_idx = 0;
            u8 u8Seq = 0;
            static u8 u8Seq_last = 0xff;
            //u8 voi_idx = 0;
            for (u8 i = 0; ((i < MTABSIZE(g_KVarr)) && (g_KVarr[i].KVIndex > 0)); i++) {
                 if (strstr(g_KVarr[i].value, "voi")) {
                     Mset_bit(flag,1);
                     src_idx = i;
                 }
                 if (strstr(g_KVarr[i].key, "PLY")) {
                     Mset_bit(flag,2);
                     voi = atoi(g_KVarr[i].value);
                 }
                 if (strstr(g_KVarr[i].key, "SEQ")) {
                     Mset_bit(flag,3);
                     u8Seq = atoi(g_KVarr[i].value);
                 }
                 if (strstr(g_KVarr[i].key, "ask") && strstr(g_KVarr[i].value, "ver")) {
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
                    setStatusByvoiceIdx((u8)voi);
                    reportStatusByvoiceIdx((u8)voi);
                    /** construct ack**/
                    generateVoiceAckOk(g_KVarr[src_idx].key, u8Seq_last);
                } else {
                    generateVoiceAckErr(g_KVarr[src_idx].key, u8Seq_last);
                }
            }
            
            if (Mget_bit(flag, 1) && Mget_bit(flag, 4)) {
                generateVoiceAckVer(g_KVarr[src_idx].key, CVERSION);
            }
        }
        break;
        
    case CMSG_UART1RX:
        break;
        
    case CMSG_UART2TX:
        sm_sendData(NULL);
        break;
    
    case CMSG_UART1TX:
        /** nothing **/
        break;

    case CGETCHAR_MOP:
    case CGETCHAR_ROLLER:
    case CGETCHAR_CLEARWATER:
    case CGETCHAR_PUMP:
    case CGETCHAR_BATTERY:
    case CGETCHAR_CHARGE:
    case CGETCHAR_STATUS:
        AckgetCharStatusByMsgType(((msg_t *)pMsg)->msgType);
        break;

    case CGETCHAR_NETINFO:
        checkAndAckGetCharNetInfo();
        break;
    case CGETCHAR_UPDATE:
        checkAndAckGetCharUpdate();
        break;
    case CPUT_SYNC:
        (void)reportAckPutSync();
        break;

    case CPUT_CHAR:
			break;
        
    case CGETDEVINFO_REQ:
    case CGETDEVINFO_RSPERROR:
        (void)reportDevInfo(NULL);
        break;
        
    case CREPORT_RSPERROR:   /** report Services error, try again **/
    case CGETDEVINFO_RSPOK:  /** according protocal report Services after devInfo reported ok **/
        (void)reportService(0);
        break;
        
    case CREPORT_RSPOK:
        /** do nothing **/
        break;

    case CWIFI_STATUS:
    case CDISCONN_CLOUD:
    case CCONN_CLOUD:
        break;

    case CHEART_BEAT:
        (void)reportHeartbeat();
        break;

    case CWIFI_TEST:
        /** do test only! 2-byte data !!! **/
        break;
        
    case CPMT_TOUT:        /** һ����ʾ������� **/
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
/**************************************************************************************************/
char* pstrcpy(char* dest, char* src)
{
    if ((dest == NULL) || (src == NULL)) {
        return NULL;
    }
    while((*dest++ = *src++) != '\0') {
        ;
    }
    return dest;
}

/**************************************************************************************************/
void netInfoData_init(void)
{
    g_netInfo.flag = 0;

    #if 1
    strcpy(g_netInfo.rssi, "-13");
    strcpy(g_netInfo.ssid, "DIISEA-ssid");
    strcpy(g_netInfo.ip,  "10.23.45.67");
    strcpy(g_netInfo.mac, "ab:cd:ef:01:23:45");
    #endif
}

/** Ack for query; no matter status changed or not **/
void checkAndAckGetCharNetInfo(void)
{
    // netInfoData_init();   // ???????????????????? for test only
    (void)reportgetCharNetInfo(&g_netInfo);
}
/**************************************************************************************************/
void checkAndAckGetCharUpdate(void)
{
    (void)getCharAckComponentStatus(CINDEX_UPDATE);
}

/*****************************************************************************/
/**
 * ���ذ���Ҫ���������֪��ǰ����״̬! ��ǰ������ֱ�������ذ巢�������������Ϊ����״̬ͨ��
 **/
 #if 1
 Triplet_u8u8pu8_t const voiceIdx2status[] = {
	// {vopIdx_ConnectNo, CINDEX_CHARGING, &(g_componentStatus.mop)},   //=1, //δ�����豸//����//
	// {vopIdx_Disconnect, CINDEX_CHARGING, &(g_componentStatus.mop)},  // =2,//�豸�ѶϿ�
	// {vopIdx_Install, CINDEX_CHARGING, &(g_componentStatus.mop)},  // =3,//��������ģʽ
	// {vopIdx_VoiceOpen, CINDEX_CHARGING,  &(g_componentStatus.mop)},  // =4,//��������
	// {vopIdx_VoiceClose, CINDEX_CHARGING, &(g_componentStatus.mop)},  // =5,//�ر�����
	// {vopIdx_WifiReset, CINDEX_CHARGING, &(g_componentStatus.mop)},  // =6,//���縴λ�ɹ�
	// {vopIdx_WifiConnecting, CINDEX_CHARGING, &(g_componentStatus.mop)},  // =7,//������������
	// {vopIdx_WifiOk, CINDEX_CHARGING, &(g_componentStatus.mop)},  // =8,//�������ӳɹ�
	
	{vopIdx_CHing, CINDEX_CHARGING, &(g_componentStatus.charge)},  // =9,//��ʼ���
	{vopIdx_CHcomplete, CINDEX_CHARGECOMPLETE, &(g_componentStatus.charge)},  // =10,//��������
	{vopIdx_Choff, CINDEX_UNCHARGED, &(g_componentStatus.charge)},  // =11,//����ж�
	
	{vopIdx_standard, CINDEX_STANDARD, &(g_componentStatus.mop)},  // =12,//�����׼ģʽ
	{vopIdx_RUNm2, CINDEX_HIGHPOWER, &(g_componentStatus.mop)},  // =13,//����ǿ��ģʽ
	// {vopIdx_nop2, CINDEX_STANDBY, &(g_componentStatus.mop},  // =14,//��ˮ��ϴģʽ
	// {vopIdx_RUNCL, CINDEX_STANDBY, &(g_componentStatus.mop},  // =15,//��������ϴģʽ
	// {vopIdx_RunclOver, CINDEX_STANDBY, &(g_componentStatus.mop},  // =16,//����ϴ�����
	
	{vopIdx_RUNover, CINDEX_STANDBY, &(g_componentStatus.mop)},  // =17,//���н�������Żص�������ϴ	

	// {vopIdx_RUNOFF, CINDEX_STANDBY, &(g_componentStatus.mop)},  // =18,//���н���
	{vopIdx_Chlowing, CINDEX_BATTERYLOW,  &(g_componentStatus.battery)},  // =19,//�������㣬�뼰ʱ���
	// {nop3, CINDEX_STANDBY, &(g_componentStatus.battery)},  // =20,//�������㣬���������
	// {nop4, CINDEX_STANDBY, &(g_componentStatus.battery)},  // =21,//�������㣬��������
	{vopIdx_CHErr, CINDEX_CHARGEFAULT, &(g_componentStatus.charge)},  // =22,//����쳣����������
	//{vopIdx_ConnectDragLala, CINDEX_STANDBY, &(g_componentStatus.battery)},  // =23,//ϴ�ػ�������
	//{vopIdx_CisternOk, CINDEX_STANDBY, &(g_componentStatus.battery)},  // =24,//ˮ���Ѱ�װ
	//{vopIdx_CisternNo, CINDEX_STANDBY, &(g_componentStatus.battery)},  // =25,//ˮ����ȡ��
	//{vopIdx_CisternTake,CINDEX_STANDBY,  &(g_componentStatus.battery)},  // =26,//ˮ����ȡ���������ˮ��ϴģʽ
	{vopIdx_sewageErr, CINDEX_CLEARWATERSHORTAGE, &(g_componentStatus.clearWater)},  // =27,//��ˮ����������������ˮ��
	{vopIdx_ClearErr, CINDEX_CLEARWATERSHORTAGE, &(g_componentStatus.clearWater)},  // =28,//�������ˮ
	// {vopIdx_PumpErr, CINDEX_STANDBY, &(g_componentStatus.clearWater)},  // =29,//ˮ�õ���쳣
	// {nop5, CINDEX_STANDBY,  &(g_componentStatus.clearWater)},  // =30,//ˮ�õ��δ��װ

	{vopIdx_RollerErr, CINDEX_ROLLEROVERLOAD, &(g_componentStatus.roller)},  // =31,//�����Ͳ
};
 #else
Quadruple_u8u8u8pu8_t  voiceIdx2status[] = {
	// {vopIdx_ConnectNo, CINDEX_CHARGING, &(g_componentStatus.mop), 2},   //=1, //δ�����豸//����//
	// {vopIdx_Disconnect, CINDEX_CHARGING, &(g_componentStatus.mop), 2},  // =2,//�豸�ѶϿ�
	// {vopIdx_Install, CINDEX_CHARGING, &(g_componentStatus.mop), 2},  // =3,//��������ģʽ
	// {vopIdx_VoiceOpen, CINDEX_CHARGING,  &(g_componentStatus.mop), 2},  // =4,//��������
	// {vopIdx_VoiceClose, CINDEX_CHARGING, &(g_componentStatus.mop), 2},  // =5,//�ر�����
	// {vopIdx_WifiReset, CINDEX_CHARGING, &(g_componentStatus.mop), 2},  // =6,//���縴λ�ɹ�
	// {vopIdx_WifiConnecting, CINDEX_CHARGING, &(g_componentStatus.mop), 2},  // =7,//������������
	// {vopIdx_WifiOk, CINDEX_CHARGING, &(g_componentStatus.mop), 2},  // =8,//�������ӳɹ�
	
	{vopIdx_CHing, CINDEX_CHARGING, &(g_componentStatus.charge), 2},  // =9,//��ʼ���
	{vopIdx_CHcomplete, CINDEX_CHARGECOMPLETE, &(g_componentStatus.charge), 3},  // =10,//��������
	{vopIdx_Choff, CINDEX_UNCHARGED, &(g_componentStatus.charge), 1},  // =11,//����ж�
	
	{vopIdx_standard, CINDEX_STANDARD, &(g_componentStatus.mop), 2},  // =12,//�����׼ģʽ
	{vopIdx_RUNm2, CINDEX_HIGHPOWER, &(g_componentStatus.mop), 3},  // =13,//����ǿ��ģʽ
	// {vopIdx_nop2, CINDEX_STANDBY, &(g_componentStatus.mop, 1},  // =14,//��ˮ��ϴģʽ
	// {vopIdx_RUNCL, CINDEX_STANDBY, &(g_componentStatus.mop, 1},  // =15,//��������ϴģʽ
	// {vopIdx_RunclOver, CINDEX_STANDBY, &(g_componentStatus.mop, 1},  // =16,//����ϴ�����
	
	{vopIdx_RUNover, CINDEX_STANDBY, &(g_componentStatus.mop), 1},  // =17,//���н�������Żص�������ϴ	

	// {vopIdx_RUNOFF, CINDEX_STANDBY, &(g_componentStatus.mop), 1},  // =18,//���н���
	{vopIdx_Chlowing, CINDEX_BATTERYLOW,  &(g_componentStatus.battery), 1},  // =19,//�������㣬�뼰ʱ���
	// {nop3, CINDEX_STANDBY, &(g_componentStatus.battery), 1},  // =20,//�������㣬���������
	// {nop4, CINDEX_STANDBY, &(g_componentStatus.battery), 1},  // =21,//�������㣬��������
	{vopIdx_CHErr, CINDEX_CHARGEFAULT, &(g_componentStatus.charge), 4},  // =22,//����쳣����������
	//{vopIdx_ConnectDragLala, CINDEX_STANDBY, &(g_componentStatus.battery), 2},  // =23,//ϴ�ػ�������
	//{vopIdx_CisternOk, CINDEX_STANDBY, &(g_componentStatus.battery), 2},  // =24,//ˮ���Ѱ�װ
	//{vopIdx_CisternNo, CINDEX_STANDBY, &(g_componentStatus.battery), 2},  // =25,//ˮ����ȡ��
	//{vopIdx_CisternTake,CINDEX_STANDBY,  &(g_componentStatus.battery), 2},  // =26,//ˮ����ȡ���������ˮ��ϴģʽ
	{vopIdx_sewageErr, CINDEX_CLEARWATERSHORTAGE, &(g_componentStatus.clearWater), 2},  // =27,//��ˮ����������������ˮ��
	{vopIdx_ClearErr, CINDEX_CLEARWATERSHORTAGE, &(g_componentStatus.clearWater), 2},  // =28,//�������ˮ
	// {vopIdx_PumpErr, CINDEX_STANDBY, &(g_componentStatus.clearWater), 2},  // =29,//ˮ�õ���쳣
	// {nop5, CINDEX_STANDBY,  &(g_componentStatus.clearWater), 2},  // =30,//ˮ�õ��δ��װ

	{vopIdx_RollerErr, CINDEX_ROLLEROVERLOAD, &(g_componentStatus.roller), 2},  // =31,//�����Ͳ
};
#endif
/*****************************************************************************/
RetStatus setStatusByvoiceIdx(u8 idx)
{
    int i = 0;
    for (i = 0; i < MTABSIZE(voiceIdx2status); i++) {
        if (voiceIdx2status[i].idx == idx) {
            *(voiceIdx2status[i].ptr) = voiceIdx2status[i].status_idx;
            return POK;
        }
    }
    return PERROR;
}

RetStatus reportStatusByvoiceIdx(u8 idx)
{
    int i = 0;
    for (i = 0; i < MTABSIZE(voiceIdx2status); i++) {
        if (voiceIdx2status[i].idx == idx) {
            reportComponentStatus(voiceIdx2status[i].status_idx);
            return POK;
        }
    }

    return PERROR;
}

/*****************************************************************************/
static const pair_msgType2u8ptr_t msgType2u8ptr[] = {
    {CGETCHAR_MOP,        &(g_componentStatus.mop)},
    {CGETCHAR_ROLLER,     &(g_componentStatus.roller)},
    {CGETCHAR_PUMP,       &(g_componentStatus.pump)},
    {CGETCHAR_CLEARWATER, &(g_componentStatus.clearWater)},
    {CGETCHAR_BATTERY,    &(g_componentStatus.battery)},
    {CGETCHAR_CHARGE,     &(g_componentStatus.charge)},
    {CGETCHAR_STATUS,     &(g_componentStatus.status)},
    // {CGETCHAR_NETINFO,    &(g_componentStatus.charge)},
    // {CGETCHAR_UPDATE,     &(g_componentStatus.charge)},
};

RetStatus AckgetCharStatusByMsgType(msgType_t msgType)
{
    int i = 0;
    for (i = 0; i < MTABSIZE(msgType2u8ptr); i++) {
        if (msgType2u8ptr[i].first == msgType) {
            getCharAckComponentStatus(*(msgType2u8ptr[i].second));
            return POK;
        }
    }
    return PERROR;
}
/*****************************************************************************/

