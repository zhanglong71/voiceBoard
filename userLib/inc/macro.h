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
#define	MisDevinfo(idx)  ((idx) == 0)    /** the index in table commandKeyArr **/
#define	MisDevinfoRespOk(idx, len, body)  (MisDevinfo(idx) && ((len) == 2) && (strstr((body), "ok") != NULL))
#define	MisDevinfoRespErr(idx, len, body)  (MisDevinfo(idx) && ((len) == 5) && (strstr((body), "error") != NULL)) 

#define	MisReportSerice(idx)  ((idx) == 4)   /** the index in table commandKeyArr **/
#define	MisReportSericeRespOk(idx, len, body)  (MisReportSerice(idx) && ((len) == 2) && (strstr((body), "ok") != NULL))
#define	MisReportSericeRespErr(idx, len, body)  (MisReportSerice(idx) && ((len) == 5) && (strstr((body), "error") != NULL)) 

#define	MisHeartbeatCommand(idx)  ((idx) == 1)   /** the index in table commandKeyArr **/

#define	MisPutChar(idx)  ((idx) == 2)    /** the index in table commandKeyArr **/

#define	MisGetChar(idx)  ((idx) == 3)    /** the index in table commandKeyArr **/
#define	MisGetCharMopStatus(idx, len, body)  (MisGetChar(idx) && ((len) == 3) && (strstr((body), "mop") != NULL))    /** the index in table commandKeyArr **/

#define	MisgetWifiStatus(idx)  ((idx) == 5)    /** the index in table commandKeyArr **/

#define	MisTestWifiResponse(idx, len, body)  (((idx) == 6 /** CTestWIFIkeyIdx **/) &&  ((len) == 2) && (strlen(body) == 2))

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
