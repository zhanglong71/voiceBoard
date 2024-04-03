
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"

#include "l_arch.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "l_u8FIFO.h"
#include "l_jsonTL.h"
#include "l_timer.h"
#include "l_gpio.h"
#include "l_jsonTL.h"
#include "l_rs485.h"
// #include "main.h"

int reportDevInfo(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getDevInfo(0);
    sm_sendData(p);
    return (TRUE);
}


int reportService(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getService(0);
    sm_sendData(p);
    return (TRUE);
}

int reportHeartbeat(void)
{
    u8 buf[] = "heartbeat,0\n";
    reportNobodyInfo(buf, strlen(buf));
}

int reportResetNet(void)
{
    u8 buf[] = "resetNet,2,AP\n";
    reportNobodyInfo(buf, strlen(buf));
}

int reportScanWifi(void)
{
    u8 buf[] = "scanWifi,0\n";
    reportNobodyInfo(buf, strlen(buf));
}

void reportAckPutSync(void)
{
    u8 buf[] = "putSync,0\n";
    reportNobodyInfo(buf, strlen(buf));
}

void reportNobodyInfo(char* data, int len)
{
    u8Data_t u8Data;
    if ((data == NULL) || (len <= 0)) {
        return;
    }
    for (int i = 0; ((i < strlen(data)) && (i < len)); i++) {
        u8Data.u8Val = data[i];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }
}

#if 0   // !!! 
int reportConnectWifi(void *arg)
{
    (void)arg;
    jsonTL_t* p = getConnectWifi(0);
    sm_sendData(p);
    return (TRUE);
}
#endif

static const reportStatusBody_t reportStatusBodyArr[] = {
    // { CINDEX_UNKNOW,              "{\"mop\":{\"status\":0}}"},             // unknow 未知状态(主机断开及其它)
    { CINDEX_STANDBY,             "{\"mop\":{\"status\":1}}"},             // standby 待机
    { CINDEX_STANDARD,            "{\"mop\":{\"status\":2}}"},                   // standard 标准模式
    { CINDEX_HIGHPOWER,           "{\"mop\":{\"status\":3}}"},                   // highPower强力模式
    // { CINDEX_RINSE,               "{\"mop\":{\"status\":4}}"},                   // rinse 大水冲洗模式
    // { CINDEX_CLEANING,            "{\"mop\":{\"status\":5}}"},                   // cleaning 自清洗模式
    
    { CINDEX_ROLLERNORMAL,        "{\"roller\":{\"status\":1}}"},                // normal 滚筒电机正常
    { CINDEX_ROLLEROVERLOAD,      "{\"roller\":{\"status\":2}}"},                // error 滚筒电机故障
    
    { CINDEX_CLEARWATERNORMAL,    "{\"clearWater\":{\"status\":1}}"},             // clear water normal 清水正常
    { CINDEX_CLEARWATERSHORTAGE,  "{\"clearWater\":{\"status\":2}}"},             // clear water shortage 清水不足
    
    // { CINDEX_PUMPNORMAL,          "{\"pump\":{\"status\":1}}"},                  // 水泵正常     pumpNormal
    // { CINDEX_PUMPOVERLOAD,        "{\"pump\":{\"status\":2}}"},                  // 水泵过载     pumpOverload
    // { CINDEX_PUMPCURRENTSMALL,    "{\"pump\":{\"status\":3}}"},                  // 水泵电流过小 pumpCurrentTooSmall
    
    { CINDEX_BATTERYNORMAL,       "{\"batterystatus\":{\"status\":1}}"},      // 电池电压在正常范围
    { CINDEX_BATTERYLOW,          "{\"batterystatus\":{\"status\":2}}"},      // 电池电压过低
    { CINDEX_BATTERYLEVEL,        "{\"batterystatus\":{\"level\":%u}}"},
    
    { CINDEX_UNCHARGED,           "{\"charge\":{\"status\":1}}"},         // 没充电
    { CINDEX_CHARGING,            "{\"charge\":{\"status\":2}}"},         // 正在充电
    { CINDEX_CHARGECOMPLETE,      "{\"charge\":{\"status\":3}}"},         // 充电完成(区别不充电场景)
    { CINDEX_CHARGEFAULT,         "{\"charge\":{\"status\":4}}"},         // 充电故障

    { CINDEX_NETINFO,             "{\"netInfo\":{\"IP\":%s,\"RSSI\":%d,\"SSID\":%s}}"},         // 网络信息
    { CINDEX_UPDATE,              "{\"update\":{\"versoin\":1.0.1,\"introduction\":newest,\"progress\":100,\"bootTime\":60}}"},         // 升级信息

};

int reportBatteryLevel(u8 arg)
{
    jsonTL_t jsonTypeTx;
    u8 buf[U8FIFOSIZE]; 
    u8 len = 0;
    u8Data_t u8Data;
    u8 idx = 0;

    for (idx = 0; idx < MTABSIZE(reportStatusBodyArr); idx++) {
        if (reportStatusBodyArr[idx].index == CINDEX_BATTERYLEVEL) {
            break;
        }
    }
    if (idx >= MTABSIZE(reportStatusBodyArr)) {
        return (MTABSIZE(reportStatusBodyArr));
    }

    jsonTypeTx.jHead = "reportChar";
    sprintf(buf, reportStatusBodyArr[idx].body, arg);
    jsonTypeTx.jBody =buf;
    jsonTypeTx.jLen = strlen(jsonTypeTx.jBody);

#if 1
    sm_sendData_once(&jsonTypeTx);
#else
    /** hhhhhhhhh head **/
    len = strlen(jsonTypeTx.jHead);
    for (int i = 0; i < len; i++) {
        u8Data.u8Val = jsonTypeTx.jHead[i];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }
    u8Data.u8Val = ',';
    u8FIFOin_irq(&g_uart2TxQue, &u8Data); 
    
    /** lllllllll length **/
    sprintf(buf, reportStatusBodyArr[idx].body, arg);
    len = strlen(buf);
    if (sprintf(buf, "%d", len)) {
        for (int i = 0; i < strlen(buf); i++) {
            u8Data.u8Val = buf[i];
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        }
    }
    u8Data.u8Val = ',';
    u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    
    /** bbbbbbbbb body **/
    sprintf(buf, reportStatusBodyArr[idx].body, arg);
    for (int i = 0; i < strlen(buf); i++) {
        u8Data.u8Val = buf[i];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }

    u8Data.u8Val = '\n';
    u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    return (TRUE);
#endif
}

int reportgetCharNetInfo(NetInfo_t* netInfo)
{
    jsonTL_t jsonTypeTx;
    u8 buf[U8FIFOSIZE]; 
    u8 len = 0;
    u8Data_t u8Data;
    u8 idx = 0;

    if (netInfo == NULL) {
        return (FALSE);
    }

    for (idx = 0; idx < MTABSIZE(reportStatusBodyArr); idx++) {
        if (reportStatusBodyArr[idx].index == CINDEX_NETINFO) {
            break;
        }
    }
    if (idx >= MTABSIZE(reportStatusBodyArr)) {
        return (MTABSIZE(reportStatusBodyArr));
    }

    jsonTypeTx.jHead = "getChar";
    sprintf(buf, reportStatusBodyArr[idx].body, netInfo->ip, netInfo->rssi, netInfo->ssid);
    jsonTypeTx.jBody = buf;
    jsonTypeTx.jLen = strlen(jsonTypeTx.jBody);
#if 1
    sm_sendData_once(&jsonTypeTx);
#else
    /** hhhhhhhhh head **/
    len = strlen(jsonTypeTx.jHead);
    for (int i = 0; i < len; i++) {
        u8Data.u8Val = jsonTypeTx.jHead[i];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }
    u8Data.u8Val = ',';
    u8FIFOin_irq(&g_uart2TxQue, &u8Data); 
    
    /** lllllllll length **/
    
    len = strlen(buf);
    if (sprintf(buf, "%d", len)) {
        for (int i = 0; i < strlen(buf); i++) {
            u8Data.u8Val = buf[i];
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        }
    }
    u8Data.u8Val = ',';
    u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    
    /** bbbbbbbbb body **/
    sprintf(buf, reportStatusBodyArr[idx].body, netInfo->ip, netInfo->rssi, netInfo->ssid);
    for (int i = 0; i < strlen(buf); i++) {
        u8Data.u8Val = buf[i];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }

    u8Data.u8Val = '\n';
    u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    return (TRUE);
#endif
}

/***********************************************************************
 * moto/pump/battery/clear/charge
 ***********************************************************************/
int reportComponentStatus(u8 statusIndex)
{
    static jsonTL_t jsonTypeTx;
    u8 idx = 0;

    for (idx = 0; idx < MTABSIZE(reportStatusBodyArr); idx++) {
        if (reportStatusBodyArr[idx].index == statusIndex) {
            break;
        }
    }

    if (idx >= MTABSIZE(reportStatusBodyArr)) {
        return (MTABSIZE(reportStatusBodyArr));
    }

    jsonTypeTx.jHead = "reportChar";
    jsonTypeTx.jBody = reportStatusBodyArr[idx].body;
    jsonTypeTx.jLen = strlen(jsonTypeTx.jBody);
    
    sm_sendData_once(&jsonTypeTx);
    return 0;
}

int getCharAckComponentStatus(u8 statusIndex)
{
    static jsonTL_t jsonTypeTx;
    u8 idx = 0;

    for (idx = 0; idx < MTABSIZE(reportStatusBodyArr); idx++) {
        if (reportStatusBodyArr[idx].index == statusIndex) {
            break;
        }
    }

    if (idx >= MTABSIZE(reportStatusBodyArr)) {
        return (MTABSIZE(reportStatusBodyArr));
    }

    jsonTypeTx.jHead = "getChar";
    jsonTypeTx.jBody = reportStatusBodyArr[idx].body;
    jsonTypeTx.jLen = strlen(jsonTypeTx.jBody);
    
    sm_sendData_once(&jsonTypeTx);
    return 0;
}

/*********************************************************************/
jsonTL_t* getDevInfo(u8 idx)
{
    static jsonTL_t jsonTypeDevInfo[] = {
        {
            "getDevInfo", 0,
            "{\"v\":\"1.0.1\","
            "\"dv\":\"1.0.0\","
            "\"prodId\":\"2NPQ\","
            "\"deviceTypeId\":\"19F\","
            "\"manufacturerId\":\"hlp\","
            "\"deviceModel\":\"DM6\","
            "\"deviceTypeNameEn\":\"Scrubber\","
            "\"manufacturerNameEn\":\"DIISEA\","
            "\"networkType\":\"AP\","
            "\"acKey\":\"2B5F3377287C4920506E604B326D5A6479F44A6942B1FE3C86CAD3E3A5F9654D6BC810E9D216466D843A0385A723CC8E\","
            "\"productSeries\":\"DM6\","
            "\"productKey\":\"f2b80c7c77b840e4b7017029baab9bf6\","
            "\"marketName\":\"滴水洗地机DM6\","
            //"\"marketName\":\"DIISEA-DM6\","
            "\"brand\":\"滴水科技\"}"
        },
        #if 0
        {
            "getDevInfo", 0,
            "{\"v\":\"1.0.1\","
            "\"dv\":\"1.0.0\","
            "\"prodId\":\"2NPQ\","
            "\"deviceTypeId\":\"19F\","
            "\"manufacturerId\":\"hlp\","
            "\"deviceModel\":\"DM6\","
            "\"deviceTypeNameEn\":\"Scrubber\","
            "\"manufacturerNameEn\":\"DIISEA\","
            "\"networkType\":\"BLE\","
            "\"acKey\":\"2B5F3377287C4920506E604B326D5A6479F44A6942B1FE3C86CAD3E3A5F9654D6BC810E9D216466D843A0385A723CC8E\","
            "\"productSeries\":\"DM6\","
            "\"productKey\":\"f2b80c7c77b840e4b7017029baab9bf6\","
            "\"marketName\":\"滴水洗地机DM6\","
            //"\"marketName\":\"DIISEA-DM6\","
            "\"brand\":\"滴水科技\"}"
        },

        {
            "getDevInfo", 0,
            "{\"v\":\"1.0.1\","
            "\"dv\":\"1.0.0\","
            "\"prodId\":\"2NPQ\","
            "\"deviceTypeId\":\"19F\","
            "\"manufacturerId\":\"hlp\","
            "\"deviceModel\":\"DM6\","
            "\"deviceTypeNameEn\":\"Scrubber\","
            "\"manufacturerNameEn\":\"DIISEA\","
            "\"networkType\":\"BLE\","
            "\"near\":{"
                "\"type\":1,"
                "\"trp\":-8"
            "},"
            "\"acKey\":\"2B5F3377287C4920506E604B326D5A6479F44A6942B1FE3C86CAD3E3A5F9654D6BC810E9D216466D843A0385A723CC8E\","
            "\"productSeries\":\"DM6\","
            "\"productKey\":\"f2b80c7c77b840e4b7017029baab9bf6\","
            "\"marketName\":\"滴水洗地机DM6\","
            "\"brand\":\"滴水科技\"}"
        }
        #endif
    };

    if (idx >= MTABSIZE(jsonTypeDevInfo)) {
        return (NULL);
    }
	return (&jsonTypeDevInfo[idx]);
}

jsonTL_t* getService(u8 idx)
{
    static jsonTL_t ServiceArr[] = {
        {
            "reportService", 0,
            "{"
                "\"sId\":["
                    "\"status\","
                    "\"charge\","
                    "\"clearWater\","
                    "\"roller\","
                    "\"batterystatus\","
                    "\"mop\","
                    "\"netInfo\","
                    "\"update\""
                    "],"
                "\"sType\":["
                    "\"status\","
                    "\"charge\","
                    "\"clearWater\","
                    "\"roller\","
                    "\"batterystatus\","
                    "\"mop\","
                    "\"netInfo\","
                    "\"update\""
                "]"
            "}"
        }
       // {"reportService", 0, ""}
    };

    if (idx >= MTABSIZE(ServiceArr)) {
        return (NULL);
    }
    return (&ServiceArr[idx]);
}

#if 0
jsonTL_t* getConnectWifi(u8 idx)
{
    static jsonTL_t ConnectWifiArr[] = {
        {
            "connectWifi", 0,
            "{"
            "\"ssid\":\"hilink_production_test\","
            "\"password\":\"12345678\""
            "}"
        }
    };
    if (idx >= MTABSIZE(ConnectWifiArr)) {
        return (NULL);
    }

    return (&ConnectWifiArr[idx]);
}
#endif

/**
 * ��sprintf()ת�����ֵ��ַ���ʱ���ֹ���! ԭ��δ֪! �ô˺������
 **/
RetStatus digit2ascii(int from, char* to)
{
    if ((from > 999) || (from < 0) || (to == NULL)) {
        return PERROR;
    }
    #if 1
    if (from > 99) {
        to[0] = '0' + (from / 100);
        to[1] = '0' + ((from / 10) % 10);
        to[2] = '0' + (from % 10);
        to[3] = '\0';
    } else if (from > 9) {
        to[0] = '0' + (from / 10);
        to[1] = '0' + (from % 10);
        to[2] = '\0';
    } else {
        to[0] = '0' + from;
        to[1] = '\0';
    }
    #endif
    return POK;
}

/**
 * send all data at once 
 * make sure the length of string is less than length of FIFO buff
 **/
void sm_sendData_once(jsonTL_t* jp)
{
    u8Data_t u8Data;
    u8 len_buf[8];
    int len = 0;
    int send_count = 0;
    // int i = 0;

    if (jp == NULL) {
        return;
    }

    /** hhhhhhhhh head **/
    len = strlen(jp->jHead);
    for (int i = 0; i < len; i++) {
        u8Data.u8Val = jp->jHead[i];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }
    u8Data.u8Val = ',';
    u8FIFOin_irq(&g_uart2TxQue, &u8Data); 

    /** lllllllll length **/
    len = strlen(jp->jBody);
    if (strchr(jp->jBody, '\n')) {
        len--;
    }
    #if 0
    if (sprintf(len_buf, "%d", len)) {
    #else
    if (digit2ascii(len, len_buf) == POK) {
    #endif
        for (int i = 0; i < strlen(len_buf); i++) {
            u8Data.u8Val = len_buf[i];
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        }
    }

    if (len > 0) {
        u8Data.u8Val = ',';
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    } else { /** len == 0, nobody need transmit **/
        u8Data.u8Val = '\n';
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        return;
    }
    /** bbbbbbbbb body **/
    for (send_count = 0; ((send_count < len) && (send_count < Mu8FIFO_bufLen(&g_uart2TxQue))); send_count++) {
        u8Data.u8Val = jp->jBody[send_count];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }

    if (send_count >= len) {               /** the last transmit part **/
        u8Data.u8Val = '\n';
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }
}


/**
 * state machine
 *
 * sm_init =(jp!=NULL)=> sm_step =(p!=NULL)=> sm_end ==>sm_init
 * 
 * sm_init: send data include head+len+head of body(< 40 byte)
 * sm_step: send data body(< 60 byte) again and again(CMSG_UART3TX)
 * sm_end: send over
 *
 * note: send data the length of g_uart2TxQue buf
 **/
void sm_sendData(jsonTL_t* jp)
{
    static smStatus_t s_smStatus = sm_init;
    static jsonTL_t* p = NULL;
    static u16 total = 0;
    u8Data_t u8Data;
    u8 buf[8];
    int len = 0;

    if (s_smStatus == sm_init) {   /** first enter the send process **/
        if (jp == NULL) {
            return;
        }
        s_smStatus = sm_step;
        p = jp;
        total = 0;
        
        /** hhhhhhhhh head **/
        len = strlen(p->jHead);
        for (int i = 0; i < len; i++) {
            u8Data.u8Val = p->jHead[i];
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        }
        u8Data.u8Val = ',';
        u8FIFOin_irq(&g_uart2TxQue, &u8Data); 
        
        /** lllllllll length **/
        len = strlen(p->jBody);
        if (strchr(p->jBody, '\n')) {
            len--;
        }
        if (sprintf(buf, "%d", len)) {
            for (int i = 0; i < strlen(buf); i++) {
                u8Data.u8Val = buf[i];
                u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            }
        }
        
        if (len > 0) {
            u8Data.u8Val = ',';
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        } else { /** len == 0, nobody need transmit **/
            u8Data.u8Val = '\n';
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            s_smStatus = sm_init;
            return;
        }
        /** bbbbbbbbb body **/
        for (int i = 0; ((i < len) && (i < (Mu8FIFO_bufLen(&g_uart2TxQue) - 32))); i++, total++) {
            u8Data.u8Val = p->jBody[i];
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        }
    } else if (s_smStatus == sm_step) {   /**  enter the send process again **/
        /** body **/
        if (p == NULL) {
            s_smStatus = sm_end;
            return; 
        }
        len = strlen(p->jBody);
        for (int i = 0; ((total < len) && (i < Mu8FIFO_bufLen(&g_uart2TxQue) - 1)); i++, total++) {
            u8Data.u8Val = p->jBody[total];
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        }

        if (total >= len) {               /** the last transmit part **/
            u8Data.u8Val = '\n';
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            s_smStatus = sm_end;
        }
    } else if (s_smStatus == sm_end) {   /** transmit over **/
        s_smStatus = sm_init;
        p = NULL;
    } else { /** unrecognize **/
        s_smStatus = sm_init;
        p = NULL;
    }
}

/********************************
 * enrolled key 
 ********************************/
 const pair_u8s8p_t commandKeyArr[] = {
    {CINDEX_GETDEVINFO, "getDevInfo"},
    {CINDEX_HEARTBEAT, "heartbeat"},
    {CINDEX_PUTCHAR, "putChar"},         /** 命令下发！长度不定 **/
    {CINDEX_GETCHAR, "getChar"},         /** 查询单个服务状态！ 长度不定  **/
    {CINDEX_REPORTSERVICE, "reportService,"},  /** 查询单个服务状态！ 长度不定  **/
    {CINDEX_SCANWIFI, "scanWifi"},        /** production test ack of scanwifi **/
    {CINDEX_CONNECTWIFI, "connectWifi"},     /** production test ack of connect action **/
    {CINDEX_GETRSSI, "getRssi"},         /** production test ack of getRssi action **/
    {CINDEX_PUTWIFISTATUS, "putWifiStatus"},   /** the command description !!! **/
    {CINDEX_GETWIFISTATUS, "getWifiStatus"},   /** the command description !!! **/
    {CINDEX_RESETNET, "resetNet"},        /** reset net and configure net !!! **/
    {CINDEX_GETSSID, "getSsid"},         /** netInfo ssid !!! **/
    {CINDEX_GETIP, "getIp"},           /** netInfo ip !!! **/
    {CINDEX_GETMAC, "getMac"},          /** netInfo mac !!! **/
    {CINDEX_GETRSSI, "getRssi"},         /** netInfo rssi !!! **/
    {CINDEX_PUTSYNC, "putSync"},          /** netInfo mac !!! **/
    // {"\xa5\x5a\x01\x10\x00\x06\x00\x0A\x00\x02", 0, NULL, NULL},
    // {"\"getDevInfo\"", 0},   /**  **/
    // {"\"heartbeat\"", 0},    /** 下发心跳！长度为0 **/

#define CTestWIFIkeyIdx (MTABSIZE(commandKeyArr))
};

pair_u8s8p_t* getCommandKey(u8 idx)
{ 
    if (idx >= MTABSIZE(commandKeyArr)) {
        return (NULL);
    }
	return (&commandKeyArr[idx]);
}

u8 getCommandKeyArrLen(void)
{
    return MTABSIZE(commandKeyArr);
}

/**
 * "key",length,"{body}"
 * if receive key/length/body, then return true!
 **/
objType_t sm_receiveData(char *data)
{
    static smStatus_t s_smStatus = sm_init;
    static u16 s_bodyLen = 0;
    static u8 s_keyIdx = 0;
    static u8 offset = 0;
    u8Data_t u8Data;
    msg_t msg;
    u8 chData;
    u8 i;

    if(u8FIFOisEmpty(&g_uart2RxQue) == TRUE) { /** no data !!! **/
        return obj_none;
    }
    (void)u8FIFO_last(&g_uart2RxQue, &chData);

    if (s_smStatus == sm_init) {   /** identifing key **/
        if (chData != ',') {
            offset = 0;
            return obj_none;
        }
        if (u8FIFO_get(&g_uart2RxQue, 0, data) != TRUE) {
            return obj_none;
        }

        for(i = 0; i < getCommandKeyArrLen(); i++) {
            // if (strstr(data, getCommandKey(i)->jHead) != NULL) {   /** key **/
            if (strstr(data, getCommandKey(i)->second) != NULL) {   /** key **/
                break;
            }
        }

        if(i < getCommandKeyArrLen()) {
            // s_keyIdx = i;
            s_keyIdx = getCommandKey(i)->first;
            s_bodyLen = 0 ;
            
            /** 
             * ������key����ʶ�𣬲���Ҫ sm_receiveLenStep1 ״̬
             * �� sm_receiveLenStep2 ״̬ʶ�𳤶�
             **/
            s_smStatus = sm_receiveLenStep2;
            return obj_key;
        }else {
            #if 0
            /** !!! wifi module working verify!!! **/
            if (checkResponseTest(data)) {
                s_smStatus = sm_receiveBody;
                s_bodyLen = 2;
                s_keyIdx = CTestWIFIkeyIdx;   /** ȡһ����Чֵ ���⴦�� **/
            }
            #endif
        }
    } else if (s_smStatus == sm_receiveLenStep2) {    /** identifing length  **/
        if ((chData == ' ') || (chData == '\t')) { // ignore the blank and tab
            return obj_none;
        } else if (isdigit(chData)) {   // the valid number
            s_bodyLen = (s_bodyLen * 10) + (chData - '0');
        } else if (chData == ',') {
            #if 1
            digit2ascii(s_bodyLen, data);
            #else
            sprintf(data, "%d", s_bodyLen);  // δ֪ԭ�������ʧ�� !!!!!!
            #endif
            s_smStatus = sm_receiveBody;
            offset = u8FIFOlength(&g_uart2RxQue);
            return obj_len;
        } else if (chData == '\n') {
            u8FIFOinit_irq(&g_uart2RxQue);
            s_smStatus = sm_init;   // over
            
            #if 1
            if (commandIdx2Message(s_keyIdx, &(msg.msgType)) != PERROR) {
                msgq_in_irq(&g_msgq, &msg);
            }
            #else
            if (MisDevinfo(s_keyIdx) == TRUE) {
                /** get devinfo **/
                msg.msgType = CGETDEVINFO_REQ;
                msgq_in_irq(&g_msgq, &msg);
                
                return obj_none;
            } else if (MisHeartbeatCommand(s_keyIdx) == TRUE) {
                msg.msgType = CHEART_BEAT;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if (MisPutSync(s_keyIdx) == TRUE) {
                msg.msgType = CPUT_SYNC;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else {
                /** end transmit but invalid data **/
            }
            #endif
        }
        return obj_none;
    } else if (s_smStatus == sm_receiveBody) {    /** identifing body **/
        // ??????????????????????????????
        #if 0
            u8Data.u8Val = '!';
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            u8Data.u8Val = chData;
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        #endif
        // ??????????????????????????????
        /** offset: start of head, end of len **/
        if (u8FIFO_get(&g_uart2RxQue, offset, data) != TRUE) {
            return obj_none;
        }
        if (chData == '\n') {
            ClrTimer_irq(&g_timer[1]);
            u8FIFOinit_irq(&g_uart2RxQue);
            s_smStatus = sm_init;   // over 
           
            if (MisDevinfoRespOk(s_keyIdx, s_bodyLen, data)) {
                /** response devinfo **/
                msg.msgType = CGETDEVINFO_RSPOK;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if (MisDevinfoRespErr(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETDEVINFO_RSPERROR;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisReportSericeRespOk(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CREPORT_RSPOK;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisReportSericeRespErr(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CREPORT_RSPERROR;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisGetCharMopStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_MOP;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;

            } else if(MisGetCharStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_STATUS;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisGetCharRollerStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_ROLLER;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisGetCharClearWaterStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_CLEARWATER;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
                #if 0  /** D7 no pump status **/
            } else if(MisGetCharPumpStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_PUMP;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
                #endif
            } else if(MisGetCharBatteryStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_BATTERY;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisGetCharChargeStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_CHARGE;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisGetCharNetInfoStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_NETINFO;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisGetCharUpdateStatus(s_keyIdx, s_bodyLen, data)) {
                msg.msgType = CGETCHAR_UPDATE;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;

            } else if(MisPutChar(s_keyIdx)) {
                msg.msgType = CPUT_CHAR;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisgetWifiStatus(s_keyIdx)) {
                // msg.msgType = CWIFI_STATUS;
                // msgq_in_irq(&g_msgq, &msg);
                i = atoi(data);
                if(MisgetWifiStatusExpect(s_keyIdx, s_bodyLen, i, 1)) {
                    msg.msgType = CCONN_CLOUD;
                    msgq_in_irq(&g_msgq, &msg);
                } else if(MisgetWifiStatusExpect(s_keyIdx, s_bodyLen, i, 8)) {
                    msg.msgType = CCONN_ROUTE;
                    msgq_in_irq(&g_msgq, &msg);
                } else {
                    msg.msgType = CDISCONN_CLOUD;
                    msgq_in_irq(&g_msgq, &msg);
                }
                return obj_none;
            } else if(MisScanWifi(s_keyIdx)) {
                /** need one global variety store the rssi **/
                msg.msgValue = s_bodyLen;
                msg.msgType = CSCAN_WIFI;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisconnectWifi(s_keyIdx)) {
                /** need one global variety store the connection status **/
                msg.msgType = CCONN_WIFI;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisResetNet(s_keyIdx)) {
                if (MisRespOk(s_bodyLen, data)) {
                    msg.msgType = CRESETNET_RSPOK;
                    msgq_in_irq(&g_msgq, &msg);
                    return obj_none;
                } else if (MisRespFail(s_bodyLen, data)) {
                    msg.msgType = CRESETNET_RSPFAIL;
                    msgq_in_irq(&g_msgq, &msg);
                    return obj_none;
                }
            } else if (MisGetSsid(s_keyIdx)) {
                //if (strlen(data) == s_bodyLen) {
                //}
                return obj_SSID;
            } else if (MisGetIp(s_keyIdx)) {
                return obj_IP;
            } else if (MisGetMac(s_keyIdx)) {
                return obj_MAC;
            } else if (MisGetRssi(s_keyIdx)) {
                if (!(MisGetRssiFail(s_keyIdx, s_bodyLen, data))) {
                    return obj_RSSI;
                } else { 
                    // return obj_RSSI;
                }
            } else {
            }
            return obj_body;
        }

        if (u8FIFOisFull(&g_uart2RxQue) == TRUE) {
            /** warning...  buf full **/
            u8Data.u8Val = 'F';
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        } else {
            // ??????????????????????????????
            #if 0
            u8Data.u8Val = '!';
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            u8Data.u8Val = chData;
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            u8Data.u8Val = g_uart2RxQue.in;
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            u8Data.u8Val = g_uart2RxQue.out;
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            #endif
           // ??????????????????????????????
        }
        if (u8FIFOlength(&g_uart2RxQue) >= (s_bodyLen + offset + 1)      /** strlen(",x,") + the "" end of body + the \n end of body **/) {
            u8FIFOinit_irq(&g_uart2RxQue);
            s_smStatus = sm_init;   // over 
            return obj_body;
        }
    } else if  (s_smStatus == sm_end) {           /** identifing end **/
        s_smStatus = sm_init;
    } else {
        /** !!! **/
    }

    return obj_none;
}

#if 1
RetStatus commandIdx2Message(char index, msgType_t* msg)
{
    int i = 0;
    pair_u8msgType_t const char2msgType[] = {
        {CINDEX_GETDEVINFO, CGETDEVINFO_REQ},
        {CINDEX_HEARTBEAT, CHEART_BEAT},
        {CINDEX_PUTSYNC, CPUT_SYNC},
    };
    for (i = 0; i < MTABSIZE(char2msgType); i++) {
        if (char2msgType[i].first == index) {
            *msg = char2msgType[i].second;
            return POK;
        }
    }
    return PERROR;
}
#endif

