
#include "CONST.h"
#include "ptype.h"
#include "macro.h"
#include "global.h"
#include "version.h"

#include "l_arch.h"
#include <string.h>
#include <stdio.h>

#include "l_u8FIFO.h"
#include "l_jsonTL.h"
#include "l_timer.h"
#include "l_gpio.h"
#include "l_jsonTL.h"
#include "l_rs485.h"
// #include "main.h"
/**********************************************************************************************/
RetStatus reportVersion(void)
{
    u8 i = 0;
    u8Data_t u8Data;
    u8 buf[32];
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
int doNothing(unsigned *arg)
{
    (void)arg;
    return 0;
}

int constructGetCharCmd(u8 status)
{
    //jsonTL_t* p = getDevInfo(0);
    //sm_sendData(p);
    return 0;
}

int reportDevInfo(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getDevInfo(0);
    sm_sendData(p);
    return 0;
}

int reportHeartbeat(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getHeartbeat();
    sm_sendData(p);
    return 0;
}

int reportService(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getService(0);
    sm_sendData(p);
    return 0;
}

int reportResetNet(void *arg)
{
    (void)arg;
    u8Data_t u8Data;
    u8 buf[] = "resetNet,2,AP\n";
    for (int i = 0; i < strlen(buf); i++) {
        u8Data.u8Val = buf[i];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }

    return 0;
}

#if 1
const static u8* statusBodyArr[] = {
#define CINDEX_CHARGING (0)
    "{\"mop\":{\"status\":\"charging\"}}",
#define CINDEX_CHARGECOMPLETE (1)
    "{\"mop\":{\"status\":\"chargeComplete\"}}",
#define CINDEX_CHARGEFAULT (2)
    "{\"mop\":{\"status\":\"chargeFault\"}}",
#define CINDEX_LOWBATTERY (3)
    "{\"mop\":{\"status\":\"lowBattery\"}}",
#define CINDEX_DORMANCY (4)
    "{\"mop\":{\"status\":\"dormancy\"}}",
#define CINDEX_SCREENSHUTDOWN (5)
    "{\"mop\":{\"status\":\"ScreenShutdown\"}}",
#define CINDEX_STANDBY (6)
    "{\"mop\":{\"status\":\"standby\"}}",
#define CINDEX_NEUTRAL (7)
    "{\"mop\":{\"status\":\"neutral\"}}",                 // 空档模式
#define CINDEX_SETUP (8)
    "{\"mop\":{\"status\":\"setup\"}}",                      // 设置模式 setup mode
#define CINDEX_SETUP2 (9)
    "{\"mop\":{\"status\":\"setup\"}}",                      // 设置语音 setup voice prompt
#define CINDEX_SETUP3 (10)
    "{\"mop\":{\"status\":\"setup\"}}",                     // wifi复位 wifiReset
    
#define CINDEX_CONNECTION (11)                          
    "{\"mop\":{\"status\":\"connection\"}}",               // 主机接驳
#define CINDEX_STANDARD (12)  
    "{\"mop\":{\"status\":\"standard\"}}",                 // 标准模式
#define CINDEX_HIGHPOWER (13)
    "{\"mop\":{\"status\":\"highpower\"}}",                 // 强力模式
#define CINDEX_CLEANING (14)
    "{\"mop\":{\"status\":\"cleaning\"}}",                 // 自清洗模式
#define CINDEX_FLUSHING (15)
    "{\"mop\":{\"status\":\"flushing\"}}",                   // 大水冲洗模式
#define CINDEX_TANKINPLACE (16)
    "{\"mop\":{\"status\":\"tankInPlace\"}}",                // 水箱在位识别
#define CINDEX_TANKNOTINPLACE (17)
    "{\"mop\":{\"status\":\"tankNotInPlace\"}}",           // 水箱不在位识别
#define CINDEX_FULLSEWAGE (18)
    "{\"mop\":{\"status\":\"fullSewage\"}}",                // 污水满
#define CINDEX_INSUFFICIENTWATER (19)
    "{\"mop\":{\"status\":\"InsufficientWater\"}}",         // 没有清水
#define CINDEX_PUMPOVERLOAD (20)
    "{\"mop\":{\"status\":\"pumpOverLoad\"}}",            // 水泵过载     pumpOverload
#define CINDEX_PUMPCURRENTSMALL (21)
    "{\"mop\":{\"status\":\"pumpCurrentSmall\"}}",            // 水泵电流过小 pumpCurrentToosmall
#define CINDEX_MOTOROVERLOAD (22)
    "{\"mop\":{\"status\":\"motorOverLoad\"}}",                 // 电机故障
#define CINDEX_INVALID (0xff)
    ""
};


static u8 getIdxbyMode(u8 mode)
{
    const static pair_t statusNum2IdxArr[] = {
        {(void *)1, (void *)CINDEX_STANDARD},
        {(void *)2, (void *)CINDEX_HIGHPOWER},
    };

    for (int i = 0; i < MTABSIZE(statusNum2IdxArr); i++) {
        if ((u8)(statusNum2IdxArr[i].var1) == mode) {
            return (u8)statusNum2IdxArr[i].var2;
        }
    }
    return CINDEX_INVALID;
}
#endif

jsonTL_t* getGetCharCmdbyMode(u8 mode)
{
    static jsonTL_t jsonTypeTx;
    
    u8 idx = getIdxbyMode(mode); 
    if (idx >= MTABSIZE(statusBodyArr)) {
        return (NULL);
    }

    jsonTypeTx.jHead = "getChar";
    jsonTypeTx.jBody = statusBodyArr[idx];
    jsonTypeTx.jLen = strlen(statusBodyArr[idx]);
    
	return (&jsonTypeTx);
}

int reportGetCharCmd(unsigned *arg)
{
    (void)arg;
    jsonTL_t* p = getGetCharCmdbyMode(1);  //????????????????????????????
    //jsonTL_t* p = getGetCharCmdbyMode(sysvar.Modes);
    sm_sendData(p);
    return 0;
}

/** 上报命令 **/
#if 1
jsonTL_t* getReportCmdbyMode(u8 mode)
{
    static jsonTL_t jsonTypeTx;
    u8 idx = getIdxbyMode(mode); 
    if (idx >= MTABSIZE(statusBodyArr)) {
        return (NULL);
    }

    jsonTypeTx.jHead = "reportChar";
    jsonTypeTx.jBody = statusBodyArr[idx];
    jsonTypeTx.jLen = strlen(statusBodyArr[idx]);
    
	return (&jsonTypeTx);
}
#endif

jsonTL_t* getDevInfo(u8 idx)
{
    static jsonTL_t jsonTypeDevInfo[] = {
        {
            "getDevInfo", 0,
            "{\"v\": \"1.0.1\","
            "\"dv\": \"1.0.0\","
            "\"prodId\": \"DM6\","
            "\"deviceTypeId\": \"01D\","
            "\"manufacturerId\": \"416\","
            "\"deviceModel\": \"OHOS002\","
            "\"deviceTypeNameEn\": \"mop\","
            "\"manufacturerNameEn\": \"DIISEA\","
            "\"networkType\": \"AP\","
            "\"acKey\": \"702B20206F2468203956502B3A6A673A734AE2CC6BE3B7A4CDD9BA27DD889661DB9D818EED21F46CD159B2AAAAAC16C8\","
            "\"productSeries\": \"Certificate\","
            "\"productKey\": \"f2b80c7c77b840e4b7017029baab9bf6\","
            // "\"marketName\": \"多功能水尘洗地机\","
            "\"marketName\": \"DIISEA-DM6\","
            "\"brand\": \"c-chip\"}"
        },

        {
            "getDevInfo", 0,
            "{\"v\": \"1.0.1\","
            "\"dv\": \"1.0.0\","
            "\"prodId\": \"DM6\","
            "\"deviceTypeId\": \"01D\","
            "\"manufacturerId\": \"416\","
            "\"deviceModel\": \"OHOS002\","
            "\"deviceTypeNameEn\": \"mop\","
            "\"manufacturerNameEn\": \"DIISEA\","
            "\"networkType\": \"BLE\","
            "\"acKey\": \"702B20206F2468203956502B3A6A673A734AE2CC6BE3B7A4CDD9BA27DD889661DB9D818EED21F46CD159B2AAAAAC16C8\","
            "\"productSeries\": \"Certificate\","
            "\"productKey\": \"f2b80c7c77b840e4b7017029baab9bf6\","
            "\"marketName\": \"多功能水尘洗地机\","
            "\"brand\": \"c-chip\"}"
        },

        {
            "getDevInfo", 0,
            "{\"v\": \"1.0.1\","
            "\"dv\": \"1.0.0\","
            "\"prodId\": \"DM6\","
            "\"deviceTypeId\": \"01D\","
            "\"manufacturerId\": \"416\","
            "\"deviceModel\": \"OHOS002\","
            "\"deviceTypeNameEn\": \"mop\","
            "\"manufacturerNameEn\": \"DIISEA\","
            "\"networkType\": \"BLE\","
            "\"near\": {"
                "\"type\": 1,"
                "\"trp\": -8"
            "},"
            "\"acKey\": \"702B20206F2468203956502B3A6A673A734AE2CC6BE3B7A4CDD9BA27DD889661DB9D818EED21F46CD159B2AAAAAC16C8\","
            "\"productSeries\": \"Certificate\","
            "\"productKey\": \"f2b80c7c77b840e4b7017029baab9bf6\","
            "\"marketName\": \"多功能水尘洗地机\","
            "\"brand\": \"c-chip\"}"
        }
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
                "\"sId\": ["
                    "\"master\","
                    "\"mop\","
                    "\"sparyer\","
                    "\"aspirator\","
                    "\"brush\""
                    "],"
                "\"sType\": ["
                    "\"master\","
                    "\"mop\","
                    "\"sparyer\","
                    "\"aspirator\","
                    "\"brush\""
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

jsonTL_t* getHeartbeat(void)
{
    static jsonTL_t heartbeat[] = {
        {"\"heartbeat\"",0,""}
    };
    return (&heartbeat[0]);
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
 * note: send data the length of g_uart3TxQue buf
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
         #if 1
        len = strlen(p->jBody);
        if (sprintf(buf, "%d", len)) {
            for (int i = 0; i < strlen(buf); i++) {
                u8Data.u8Val = buf[i];
                u8FIFOin_irq(&g_uart2TxQue, &u8Data);
            }
        }
        #else
        if( (len >> 8) & 0xff) {   /** over 255, then high 8-bit first **/
            u8Data.u8Val = (len >> 8) & 0xff;
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        }
        u8Data.u8Val = len & 0xff;
        u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
        
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
        for (int i = 0; ((i < len) && (i < (U8FIFOSIZE - 32))); i++, total++) {
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
        for (int i = 0; ((total < len) && (i < U8FIFOSIZE)); i++, total++) {
            u8Data.u8Val = p->jBody[total];
            u8FIFOin_irq(&g_uart2TxQue, &u8Data);
        }

        if (total >= len) {               /** the last transmit part **/
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

#if 0
void reportTest(void)
{
    u8 buf[] = {0xA5, 0x5A, 0x01, 0x13, 0x00, 0x05, 0x00, 0xA, 0x00, 0x01, 0x00, 0x23}; /** C8138 module test req(12-bytes) **/

    u8Data_t u8Data;
    for (int i = 0; i < MTABSIZE(buf); i++) {
        u8Data.u8Val = buf[i];
        u8FIFOin_irq(&g_uart2TxQue, &u8Data);
    }

}

int checkResponseTest(u8* str)
{
	  int i = 0;
    u8 buf[] = {0x5A, 0xA5, 0x01, 0x10, 0x00, 0x06, 0x00, 0xA, 0x00, 0x02, 0x00, 0x00}; /** C8138 module test response(12-bytes) **/
    for (i = 0; i < MTABSIZE(buf); i++) {
        if (buf[i] != str[i]) {
            break;
        }
    }

    if (i >= MTABSIZE(buf)) {
        return TRUE;
    }

    return FALSE;
}
#endif

/********************************
 * enrolled key 
 ********************************/
const jsonTL_t commandKeyArr[] = {
    {"getDevInfo", 0, NULL, NULL},
    {"heartbeat", 0, NULL, NULL},
    {"putChar", 0, NULL, NULL},      /** 命令下发！长度不定 **/
    {"getChar", 0, NULL, NULL},      /** 查询单个服务状态！ 长度不定  **/
    {"reportService,", 0, NULL, NULL},      /** 查询单个服务状态！ 长度不定  **/
    {"getWifiStatus,", 1, NULL, NULL},
    // {"\xa5\x5a\x01\x10\x00\x06\x00\x0A\x00\x02", 0, NULL, NULL},
    // {"\"getDevInfo\"", 0},   /**  **/
    // {"\"heartbeat\"", 0},    /** 下发心跳！长度为0 **/

#define CTestWIFIkeyIdx (MTABSIZE(commandKeyArr))
};

u8 getCommandKeyArrLen(void)
{
    return MTABSIZE(commandKeyArr);
}

jsonTL_t* getCommandKey(u8 idx)
{ 
    if (idx >= MTABSIZE(commandKeyArr)) {
        return (NULL);
    }
	return (&commandKeyArr[idx]);
}

/**
 * "key",length,"{body}"
 * if receive key/length/body, then return true!
 **/
objType_t sm_receiveData(u8 *data)
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
            if (strstr(data, getCommandKey(i)->jHead) != NULL) {   /** key **/
                break;
            }
        }

        if(i < getCommandKeyArrLen()) {
            s_keyIdx = i;
            s_bodyLen = 0 ;
            
            /** 
             * ������key����ʶ�𣬲���Ҫ sm_receiveLenStep1 ״̬
             * �� sm_receiveLenStep2 ״̬ʶ�𳤶�
             **/
            s_smStatus = sm_receiveLenStep2;
            return obj_key;
        } else {
            /** !!! wifi module working verify!!! **/
            #if 0  // ????????????????????????
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
        } else if (isdigit(chData)) {
            s_bodyLen = (s_bodyLen * 10) + (chData - '0');
        } else if (chData == ',') {
            sprintf(data, "%d", s_bodyLen);
            s_smStatus = sm_receiveBody;
            offset = u8FIFOlength(&g_uart2RxQue);
            return obj_len;
        } else if (chData == '\n') {
            u8FIFOinit_irq(&g_uart2RxQue);
            s_smStatus = sm_init;   // over
            
            if (MisDevinfo(s_keyIdx) == TRUE) {
                /** get devinfo **/
                msg.msgType = CGETDEVINFO_REQ;
                msgq_in_irq(&g_msgq, &msg);
                
                return obj_none;
            } else if (MisHeartbeatCommand(s_keyIdx) == TRUE) {
                msg.msgType = CHEART_BEAT;
                msgq_in_irq(&g_msgq, &msg);
                
                return obj_none;
            }
        }
        return obj_none;
    } else if (s_smStatus == sm_receiveBody) {    /** identifing body **/
        // ??????????????????????????????
        #if 0
            u8Data.u8Val = chData;
            u8FIFOin_irq(&g_uart3TxQue, &u8Data);
        #endif
        // ??????????????????????????????
        /** offset: start of head, end of len **/
        if (u8FIFO_get(&g_uart2RxQue, offset, data) != TRUE) {
            return obj_none;
        }
        if (chData == '\n') {
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
                msg.msgType = CGET_CHAR;
                msgq_in_irq(&g_msgq, &msg);
                return obj_none;
            } else if(MisPutChar(s_keyIdx)) {
                msg.msgType = CPUT_CHAR;
                msgq_in_irq(&g_msgq, &msg);
            } else if(MisgetWifiStatus(s_keyIdx)) {
                msg.msgType = CWIFI_STATUS;
                msgq_in_irq(&g_msgq, &msg);
                
            }
            return obj_body;
        }

        if (MisTestWifiResponse(s_keyIdx, s_bodyLen, data)) {
            msg.msgType = CWIFI_TEST;
            msgq_in_irq(&g_msgq, &msg);    
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

int sm_receiveDataLen(const u8FIFO_t *que, u8 *data)
{
    return 0;
}

void CmdProcess(u8* CommId)
{
    for(int i = 0; i < MTABSIZE(commandKeyArr); i++) {
        //if (strstr(jsonTypeArr[i].jsonKey, CommId)) {
			
        //}
    }
}

/*******************************************************************************
 * prase json body(JsonParseL0)
 * 
 * example:
 * {key1:value1, key2:value2, ... ,keyn:valuen}
 *******************************************************************************/
unsigned char JsonParseL0(unsigned char* jsonstrbuf, kv_t* jsonstcarr)
{
    u8 j_u8,k_u8,i_u8 = 0;
    char *p[CMAX1_COUPLE * 2];
    char *pChar = jsonstrbuf;
    jsonstcarr[0].KVIndex = 0;
    u8Data_t u8Data;

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
        /**********/
        #if 0
        for(k_u8 = 0; k_u8 < strlen(jsonstcarr[j_u8].value); k_u8++)                 /** overwrite the end '"' ! **/
        {
            u8Data.u8Val = jsonstcarr[j_u8].value[k_u8];
            u8FIFOin_irq(&g_uart1TxQue, &u8Data);//????????????????????????????
        }
	    #endif
        /**********/ 

        //(void)strim_quot(jsonstcarr[j_u8].key);
        jsonstcarr[j_u8].key[strlen(jsonstcarr[j_u8].key) - 1] = '\0';           /** overwrite the start '"' ? **/
        for(k_u8 = 0; k_u8 < strlen(jsonstcarr[j_u8].key); k_u8++)                 /** overwrite the end '"' ? **/
        {
            jsonstcarr[j_u8].key[k_u8] = jsonstcarr[j_u8].key[k_u8 + 1];
        }
    }
    /**********/
    #if 0
    j_u8 = 1;
    for(k_u8 = 0; k_u8 < 10; k_u8++)                 /** overwrite the end '"' ! **/
    {
        u8Data.u8Val = jsonstcarr[j_u8].value[k_u8];
        u8FIFOin_irq(&g_uart1TxQue, &u8Data);//????????????????????????????
    }

    #endif
    /**********/ 
    return (1);
}

/*********************************************
 * {"voi":xxx,"ver":vxxxx}
 *********************************************/
void generateVoiceAckVer(u8* to, u8* ver)
{
    u8 i = 0;
    u8Data_t u8Data;
    u8 buf[32];
    
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
 #if 0
void generateVoiceAckOk(u8* to)
{
    u8 i = 0;
    u8Data_t u8Data;
    u8 buf[32];
    
    sprintf(buf, "{\"voi\":%s,\"PLY\":OK}", to);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        //u8FIFOin_irq(&g_uart1TxQue, &u8Data);
        rs485_stor_irq(&u8Data);
    }
}
 #else
 void generateVoiceAckOk(char* to, u8 seq)
{
    u8 i = 0;
    u8Data_t u8Data;
    u8 buf[64];
    
    sprintf(buf, "{\"voi\":%s,\"PLY\":OK,\"SEQ\":%d}", to, seq);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        //u8FIFOin_irq(&g_uart1TxQue, &u8Data);
        rs485_stor_irq(&u8Data);
    }
}
 #endif

 #if 0
void generateVoiceAckErr(u8* to)
{
    u8 i = 0;
    u8Data_t u8Data;
    u8 buf[32];
    
    sprintf(buf, "{\"voi\":%s,\"PLY\":err}", to);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        //u8FIFOin_irq(&g_uart1TxQue, &u8Data);
        rs485_stor_irq(&u8Data);
    }
}
 #else
 void generateVoiceAckErr(char* to, u8 seq)
{
    u8 i = 0;
    u8Data_t u8Data;
    u8 buf[64];
    
    sprintf(buf, "{\"voi\":%s,\"PLY\":err,\"SEQ\":%d}", to, seq);

    for (i = 0; ((i < strlen(buf)) && (i < MTABSIZE(buf))); i++) {
        u8Data.u8Val = buf[i];
        //u8FIFOin_irq(&g_uart1TxQue, &u8Data);
        rs485_stor_irq(&u8Data);
    }
}
 #endif

/******************************************************************************
 * check kv_t array
 ******************************************************************************/
u8 isPlayVoiceCommand(kv_t* kv_arr, u8* voi_idx)
{
	#if 0
    u8 flag = 0;
    for (u8 i = 0; (kv_arr[i]->KVIndex > 0); i++) {
         if (strstr(kv_arr[i]->value, "voi")) {
             Mset_bit(flag,1);
         }
         if (strstr(kv_arr[i]->key, "PLY")) {
             Mset_bit(flag,2);
             *voi_idx = atoi(kv_arr[i]->value);
         }
    }

    if (Mget_bit(flag, 1) && Mget_bit(flag, 2)) {
        return TRUE;
    } else {
        return FALSE;
    }
	#endif
}

/*******************************************************************************
 * prase json body(JsonParseL0V2)
 * 
 * example:
 * {key1:value1, key2:value2, ... ,keyn:valuen}
 *******************************************************************************/
 #if 0
unsigned char JsonParseL0V2(unsigned char* jsonstrbuf, JsonArr* jsonstcarr)
{
    u8 j_u8,k_u8,i_u8 = 0;
    char *p[12];
    char *pChar = jsonstrbuf;
    jsonstcarr[0].jIndexN = 0;

    /** 1. simily as '{ ... }'! **/
    if((jsonstrbuf[0] != '{') || (jsonstrbuf[strlen(jsonstrbuf) - 1] != '}')) {
        return(0);
    }
    
    jsonstrbuf[strlen(jsonstrbuf) - 1] = '\0';            /** overwrite the start '}' ! **/
    for(j_u8 = 0; j_u8 < strlen(jsonstrbuf); j_u8++) {    /** overwrite the end '{' ! **/
        jsonstrbuf[j_u8] = jsonstrbuf[j_u8 + 1];
    }
    
    while((p[i_u8]  = strtok(pChar, ",")) != NULL) {    /** split the string by ',' **/
        i_u8 += 2;
        pChar = NULL;
    }
    for(j_u8 = 0; j_u8 < i_u8/2; j_u8++) {
        pChar = p[j_u8 * 2];
        while ((p[j_u8 * 2 + 1]  = strtok(pChar, ":")) != NULL) {
            pChar = NULL;
        }
        
        jsonstcarr[j_u8].jIndexN = i_u8/2 - j_u8;
        strcpy(jsonstcarr[j_u8].jName, p[j_u8 * 2]);
        strcpy(jsonstcarr[j_u8].jValue, p[j_u8 * 2 + 1]);

        jsonstcarr[j_u8].jName[strlen(jsonstcarr[j_u8].jName) - 1] = '\0';           /** overwrite the start '"' ! **/
        for(k_u8 = 0; k_u8 < strlen(jsonstcarr[j_u8].jName); k_u8++)                 /** overwrite the end '"' ! **/
        {
            jsonstcarr[j_u8].jName[k_u8] = jsonstcarr[j_u8].jName[k_u8 + 1];
        }
    }
    return (1);
}
 #endif

