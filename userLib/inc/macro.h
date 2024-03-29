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

#define	MisDevinfo(idx)  ((idx) == 0)    /** the index in table commandKeyArr **/
#define	MisDevinfoRespOk(idx, len, body)  (MisDevinfo(idx) && (MisRespOk(len, body)))
#define	MisDevinfoRespErr(idx, len, body)  (MisDevinfo(idx) && (MisRespErr(len, body))) 

#define	MisReportSerice(idx)  ((idx) == 4)   /** the index in table commandKeyArr **/
#define	MisReportSericeRespOk(idx, len, body)  (MisReportSerice(idx) && (MisRespOk(len, body)))
#define	MisReportSericeRespErr(idx, len, body)  (MisReportSerice(idx) && (MisRespErr(len, body))) 

#define	MisHeartbeatCommand(idx)  ((idx) == 1)   /** the index in table commandKeyArr **/

#define	MisPutChar(idx)  ((idx) == 2)    /** the index in table commandKeyArr **/

#define	MisGetChar(idx)  ((idx) == 3)    /** the index in table commandKeyArr **/
#define	MisGetCharMopStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 3) && (strstr((body), "mop") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharRollerStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strstr((body), "roller") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharClearWaterStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 10) && (strstr((body), "clearWater") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharPumpStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 4) && (strstr((body), "pump") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharBatteryStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 13) && (strstr((body), "batterystatus") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharChargeStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 6) && (strstr((body), "charge") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharNetInfoStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 7) && (strstr((body), "netInfo") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharUpdateStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 7) && (strstr((body), "update") != NULL))    /** the index in table commandKeyArr **/
#define	MisGetCharStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 7) && (strstr((body), "status") != NULL))    /** the index in table commandKeyArr **/

#define	MisScanWifi(idx)  ((idx) == 5)       /** the index in table commandKeyArr **/
#define	MisScanWifiRespFail(idx, len, body) (MisScanWifi(idx) && (MisRespFail(len, body)))

#define	MisconnectWifi(idx)  ((idx) == 6)       /** the index in table commandKeyArr **/
#define	MisconnectWifiRespOk(idx, len, body)  (MisconnectWifi(idx) && (MisRespOk(len, body))) 
#define	MisconnectWifiRespFail(idx, len, body) (MisconnectWifi(idx) && (MisRespFail(len, body)))

#define	MisputWifiStatus(idx)  ((idx) == 8)    /** the index in table commandKeyArr **/
#define	MisgetWifiStatus(idx)  ((idx) == 9)    /** the index in table commandKeyArr **/

#define	MisResetNet(idx)  ((idx) == 10)       /** the index in table commandKeyArr **/
#define	MisResetNetRespOk(idx, len, body)   (MisResetNet(idx) && (MisRespOk(len, body))) 
#define	MisResetNetRespFail(idx, len, body) (MisResetNet(idx) && (MisRespFail(len, body)))

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
