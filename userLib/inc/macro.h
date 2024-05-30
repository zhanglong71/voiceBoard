#ifndef __MACRO_H__
#define __MACRO_H__
/*******************************************************************************/
/***************************** 数组大小 ****************************************/
#define	MTABSIZE(x)	(sizeof(x)/sizeof(x[0]))
/*******************************************************************************/
#define	MIRQ_enable()	__enable_irq()		//中断使能
#define	MIRQ_disable()	__disable_irq()		//中断禁止
/*********************************************************************************/
#define MVopData_L(x)  do{GPIO_ResetBits(GPIOD, GPIO_Pin_2);}while(0)
#define MVopData_H(x) do{GPIO_SetBits(GPIOD, GPIO_Pin_2);}while(0)

#define MVopPower_on(x) // do{GPIO_SetBits(GPIOD, GPIO_Pin_3);}while(0)
#define MVopPower_off(x) // do{GPIO_ResetBits(GPIOD, GPIO_Pin_3);}while(0)
#define MVopReadBusy_status(x) GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)
/*********************************************************************************/
#define MWifi_enable(x) do{GPIO_SetBits(GPIOC, GPIO_Pin_5);}while(0)
#define MWifi_disable(x) do{GPIO_ResetBits(GPIOC, GPIO_Pin_5);}while(0)
/*********************************************************************************/
#define M485TR_T(x) do{GPIO_SetBits(GPIOB, GPIO_Pin_4);}while(0)
#define M485TR_R(x) do{GPIO_ResetBits(GPIOB, GPIO_Pin_4);}while(0)
//#define M485TR_R(x) do{GPIO_SetBits(GPIOB, GPIO_Pin_4);}while(0)    // ????????????
/*********************************************************************************/
#define Mset_bit(var, offset) do{	\
        ((var) |= (1 << (offset))); \
    }while(0)
    
#define Mreset_bit(var, offset) do{	\
    ((var) &= ~(1 << (offset))); \
    }while(0)

#define Mget_bit(var, offset) \
    !(!((var) & (1 << (offset))))
/*********************************************************************************/

/*********************************************************************************/
#define	MisRespOk(len, body)  (((len) == 2) && (strstr((body), "ok") != NULL)) 
#define	MisRespFail(len, body)  (((len) == 4) && (strstr((body), "fail") != NULL))
#define	MisRespErr(len, body)  (((len) == 5) && (strstr((body), "error")!= NULL))
/********************************/

#define	MisDevinfo(idx)  ((idx) == CKEYINDEX_GETDEVINFO)    /** the index in table commandKeyArr **/
#define	MisDevinfoRespOk(idx, len, body)  (MisDevinfo(idx) && (MisRespOk(len, body)))
#define	MisDevinfoRespErr(idx, len, body)  (MisDevinfo(idx) && (MisRespErr(len, body))) 

#define	MisReportSerice(idx)  ((idx) == CKEYINDEX_REPORTSERVICE)   /** the index in table commandKeyArr **/
#define	MisReportSericeRespOk(idx, len, body)  (MisReportSerice(idx) && (MisRespOk(len, body)))
#define	MisReportSericeRespErr(idx, len, body)  (MisReportSerice(idx) && (MisRespErr(len, body))) 

#define	MisHeartbeatCommand(idx)  ((idx) == CKEYINDEX_HEARTBEAT)   /** the index in table commandKeyArr **/

#define	MisPutChar(idx)  ((idx) == CKEYINDEX_PUTCHAR)    /** the index in table commandKeyArr **/

#define	MisGetChar(idx)  ((idx) == CKEYINDEX_GETCHAR)    /** the index in table commandKeyArr **/
#define	MisGetCharMopStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 3) && (strstr((body), "mop") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharRollerStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strstr((body), "roller") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharClearWaterStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 10) && (strstr((body), "clearWater") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharPumpStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 4) && (strstr((body), "pump") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharBatteryStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 13) && (strstr((body), "batterystatus") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharChargeStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strstr((body), "charge") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharNetInfoStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 7) && (strstr((body), "netInfo") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharUpdateStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strstr((body), "update") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strstr((body), "status") != NULL))    /** the index in table commandKeyArr **/

#define	MisScanWifi(idx)  ((idx) == CKEYINDEX_SCANWIFI)       /** the index in table commandKeyArr **/
#define	MisScanWifiRespFail(idx, len, body) (MisScanWifi(idx) && (MisRespFail(len, body)))

#define	MisconnectWifi(idx)  ((idx) == CKEYINDEX_CONNECTWIFI)       /** the index in table commandKeyArr **/
#define	MisconnectWifiRespOk(idx, len, body)  (MisconnectWifi(idx) && (MisRespOk(len, body))) 
#define	MisconnectWifiRespFail(idx, len, body) (MisconnectWifi(idx) && (MisRespFail(len, body)))

#define	MisputWifiStatus(idx)  ((idx) == CKEYINDEX_PUTWIFISTATUS)    /** the index in table commandKeyArr **/
#define	MisgetWifiStatus(idx)  ((idx) == CKEYINDEX_GETWIFISTATUS)    /** the index in table commandKeyArr **/
#define	MisgetWifiStatusExpect(idx, len, value, expected)  ((MisgetWifiStatus(idx)) && ((len) == 1) && ((value) == (expected)))   /** the index in table commandKeyArr **/

#define	MisResetNet(idx)  ((idx) == CKEYINDEX_RESETNET)       /** the index in table commandKeyArr **/
#define	MisResetNetRespOk(idx, len, body)   (MisResetNet(idx) && (MisRespOk(len, body))) 
#define	MisResetNetRespFail(idx, len, body) (MisResetNet(idx) && (MisRespFail(len, body)))

#define	MisGetSsid(idx)  ((idx) == CKEYINDEX_GETSSID)       /** the index in table commandKeyArr **/
#define	MisGetIp(idx)    ((idx) == CKEYINDEX_GETIP)       /** the index in table commandKeyArr **/
#define	MisGetMac(idx)   ((idx) == CKEYINDEX_GETMAC)       /** the index in table commandKeyArr **/

#define	MisGetRssi(idx)  ((idx) == CKEYINDEX_GETRSSI)       /** the index in table commandKeyArr **/
#define	MisGetRssiFail(idx, len, body)   (MisGetRssi(idx) && (MisRespFail(len, body)))       /** the index in table commandKeyArr **/

#define	MisPutSync(idx)  ((idx) == CKEYINDEX_PUTSYNC)       /** the index in table commandKeyArr **/

// #define	MisTestWifiResponse(idx, len, body)  (((idx) == CTestWIFIkeyIdx) &&  ((len) == 2) && (strlen(body) == 2))

/*******************************************************************************/

#define	MCOMMENT(x)
/*******************************************************************************/
#if 0
#define Mprintf(q, buf, len)  do{
        u8Data_t u8Data;                    \
        for(u8 j = 0; j < len); j++) {     \
            u8Data.u8Val = buf[i];          \
            u8FIFOin_irq(&q, &u8Data);      \
        }                                   \
    }while(0)
#endif
/*******************************************************************************/

#endif
