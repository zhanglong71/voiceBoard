#ifndef __PTYPE_H__
#define __PTYPE_H__

/*******************************************************************************
 *
 * All data type define here
 *
 *******************************************************************************/
 #include <const.h>
 //#include <stdint.h>
 
#ifndef NULL
#define NULL ((void *)0)
#endif

typedef unsigned char u8;
typedef unsigned short u16;
/*******************************************************************************/
typedef enum {
    PERROR = -1,
    POK = 0,
} RetStatus;

typedef	int	 (*pfunc_t)(void* arg);
typedef	void (*void_paction_void_t)(void);
//typedef	void (*paction_t_0)(void);

typedef struct func_s {
    pfunc_t func;       /** function **/
    unsigned int  *arg;    /** argument **/
} func_t;

typedef struct func_stack{
	unsigned char top;
	func_t func[STACKSIZE]; 
}fstack_t;

typedef enum {
    CMSG_NONE = 0, 
/*************
 * é‡æ–°æ‹Ÿå®šæ¶ˆæ¯å‘½åè§„åˆ™
 * 1. æ™®é€šä»¥CMSG_xxxå½¢å¼å‘½å
 * 2. ä»¥CSYS_xxxçš„å½¢å¼å‘½ä»¤åœ¨sysä¸­å¤„ç†çš„æ¶ˆæ¯
 *************/    
 
    CSYS_INIT,  	//
    CSYS_INITS1,  	//
    CSYS_INITS2,  	//
    CSYS_INITS3,  	//
    
    CMSG_INIT,  	//åˆå§‹åŒ–åŠ¨ä½œ(è¿›å…¥é€‰æ‹©)
    CRESP_INFO,
    
    CGETDEVINFO_REQ,
    CGETDEVINFO_RSPOK,
    CGETDEVINFO_RSPERROR,
    
    CREPORT_RSPOK,
    CREPORT_RSPERROR,
    // CGET_CHAR,
    CGETCHAR_STATUS,
    CGETCHAR_MOP,
    CGETCHAR_ROLLER,
    CGETCHAR_CLEARWATER,
    CGETCHAR_PUMP,
    CGETCHAR_BATTERY,
    CGETCHAR_CHARGE,
    CGETCHAR_NETINFO,
    CGETCHAR_UPDATE,
    CPUT_CHAR,
    CPUT_SYNC,
    CHEART_BEAT,
    CWIFI_STATUS,
    CCONN_ROUTE,
    CCONN_CLOUD,
    CDISCONN_CLOUD,
    CSCAN_WIFI,
    
    //CCONN_WIFI,
    CWIFI_TEST,
    
    CCONN_WIFI,
    CCONNWIFI_RSPOK,
    CCONNWIFI_RSPFAIL,
       
    CRESET_NET,
    CRESETNET_RSPOK,
    CRESETNET_RSPFAIL,
    
    CMSG_TMR,
    CMSG_USTMR,
 //   CMSG_DLAY, 	/** æ—¶é’Ÿ **/
 //   CMSG_NEXT, 	/** è¿›è¡Œå…¥ä¸‹ä¸€é˜¶æ®µ **/
    CMSG_DKEY,
    CMSG_UKEY,
    
    CUART1_TOUT,
    CMSG_UART1RX,
    CMSG_UART1TX,
    CMSG_UART2RX,
    CMSG_UART2TX,
    //CMSG_UART3RX,
    //CMSG_UART3TX,
    
	/**  **/
	//CVOP_STRT,		/** voice prompt start **/
	//CVOP_OVER,

	C485_STEP,		/** åŠ¨ä½œè¶…æ—¶ **/
	C485_OVER,		/** åŠ¨ä½œå®Œæˆ **/
	
//	CACT_TOUT,		/** åŠ¨ä½œè¶…æ—¶ **/
//	CACT_OVER,		/** åŠ¨ä½œå®Œæˆ **/
	
	CPMT_TOUT,		/** æç¤ºåŠ¨ä½œè¶…æ—¶ **/
	CPMT_OVER,		/** æç¤ºåŠ¨ä½œå®Œæˆ **/

} msgType_t;

typedef struct msg_s {
	msgType_t msgType;
	u8	msgValue;
}msg_t;

typedef struct msg_queue{
	u8	head;
	u8	tail;
	msg_t msg[QUEUESIZE];
} msgq_t;

typedef struct Timer_s {
    u16 tick_bak;
    u16 tick;
    msgType_t msgType;                      /** the type of msg when timeout **/
    //unsigned int count;                   /** Is it necessary ? **/
} Timer_t; 

typedef struct jsonTL_s {
	u8 *jHead;
    u8 jLen;    /** Ö¸¶¨³¤¶È! Èç¹ûÊÇ0£¬¾ÍÊÇ²»È·¶¨ **/
    u8 *jBody;
    // void* arg; /** ÊÕµ½¶ÔÓ¦ÃüÁîµÄÏìÓ¦¶¯×÷ »ò ÆäËü **/
} jsonTL_t;

typedef struct {
	void *var1;
    void *var2;
} pair_t;

typedef struct {
   u8 first;
   u8 second;
} pair_u8u8_t;

typedef struct {
   u8 first;
   void_paction_void_t second;
} pair_u8vpv_t;

typedef struct {
   u8  idx;
   u8* ptr;
   u8  value;
} triplet_u8u8pu8_t;

typedef struct {
   u8  idx;
   u8  status_idx;
   u8* ptr;
   u8  value;
} Quadruple_u8u8u8pu8_t;

typedef struct {
   u8 first;
   char* second;
} pair_u8s8p_t;

typedef struct {
   u8 first;
   msgType_t second;
} pair_u8msgType_t;

typedef struct {
   msgType_t first;
   u8* second;
} pair_msgType2u8ptr_t;


#if 0
typedef struct bitmap_s {
    //unsigned int bitmap;		    /** 32 bits **/
    u8 bitmap[40];				/** 40 * 8 = 320bits **/
} bitmap_t;

typedef enum {              /** ç”¨äºæ§åˆ¶comæ•°æ®çš„å‘é€ **/
    CTRL_START = 0,         /** 0 - start transmission **/
    CTRL_CONTI = 1,         /** 1 - continue transmission  **/
                            /** 2 reserved. for other control **/
    CTRL_TMR = 5,           /** 3..200 for delay **/
    CTRL_STOP = 255,        /** 255 - stop transmission**/
} ctrlFlag_t;
#endif

typedef struct u8Data_s{
	u8 u8Val;
} u8Data_t;

typedef struct u8FIFO_s {
    u8 in;
    u8 out;
    //u8 len;
	//u8 __pad1;
    u8 buf[U8FIFOSIZE];     /** 16 bytes **/
} u8FIFO_t;

typedef struct {
    u8FIFO_t* pfifo;
    Timer_t* ptimer;
    
    u8 sm_status;
    msgType_t stepMsgType;
    msgType_t overMsgType;
} rs485transX_t;

#if 1
typedef struct action_s {
	/*****************
	 * MOT-ON
	 * MOT-OFF
	 * LED-ON
	 * LED-OFF
	 * VOP-ON
	 * VOP-OFF
	 * Delay
	 *****************/
#define CACT_MOTONORM	0xff
#define CACT_MOTOREVE	0xfe
#define CACT_MOTOWAIT	0xfd
  
#define CACT_LEDPMTON	0xfb
#define CACT_LEDPMTOFF	0xfa
#define CACT_LEDPMTWAT	0xf9
  
#define CACT_LEDWRNON	0xf7
#define CACT_LEDWRNOFF	0xf6
#define CACT_LEDWRNWAT	0xf5

#define CACT_RS485	0xf4
#define CACT_VOPON	0xf3
//#define CACT_VOPOFF	0xf2
#define CACT_VOPWAT	0xf1
#define CACT_DELAY	0xf0
  
#define CVOP_PWRON	0xef
#define CVOP_PWROFF 0xee


#define CACT_MISC	0xeb


    // paction_t_0		func;
    u16 actionTime;	/** æŒç»­æ—¶é—´ **/
    u16 actionPara;	/** åŠ¨ä½œéœ€è¦çš„å…¶å®ƒå‚æ•°ï¼Œå¦‚VPidxç´¢å¼•å· **/
    u8 actionType;	/** åŠ¨ä½œç±»å‹. ä¼¼ä¸å‰é¢çš„å‚æ•°æœ‰é‡å ä¹‹å«Œï¼Œæ˜¯ä¸€ä¸ªåŒºåˆ«åŠ¨ä½œçš„ä¾æ® **/
    // u8 __pad1;		/** å­—èŠ‚å¯¹é½ **/
} action_t;

typedef struct actionQueue_s {
    u8 head;
    u8 tail;
    u8 flag;
    msgType_t stepMsgType;			/** é˜¶æ®µäº‹ä»¶å®Œæˆæ¶ˆæ¯ **/
    msgType_t overMsgType;			/** é˜Ÿåˆ—äº‹ä»¶å…¨éƒ¨å®Œæˆæ¶ˆæ¯ **/
    Timer_t *timer;
        #define ACTIONQUEUESIZE 8
    action_t buf[ACTIONQUEUESIZE];     /** 16 bytes **/
} actionQueue_t;
#endif
/*******************************************************************************/

/*******************************************************************************/

/*******************************************************************************/

typedef enum {
    sm_none = 0,
    sm_init, 
    sm_idle,
    sm_step,
    sm_key,
    sm_sendBody,
    sm_receiveLen,
    sm_receiveLenStep1,
    sm_receiveLenStep2,
    sm_receiveLenStep3,
    sm_receiveBody,
    sm_send,
    sm_end,
    sm_error
} smStatus_t;

typedef enum {
    obj_none = 0,
    obj_key,
    obj_len,
    obj_body,
    obj_SSID,
    obj_IP,
    obj_MAC,
    obj_RSSI,
} objType_t;
/*******************************************************************************/ 
typedef struct _JsonArr
{
    char jName[10];
    char jValue[10];
    unsigned char jIndexN;
} JsonArr;

typedef struct kv_s
{
    char key[10];
    char value[10];
    unsigned char KVIndex;
} kv_t;

/*******************************************************************************/
typedef enum {
	vopIdx_ConnectNo=1,//Î´Á¬½ÓÉè±¸//¹²ÓÃ//
	vopIdx_Disconnect=2,//Éè±¸ÒÑ¶Ï¿ª
	vopIdx_Install=3,//½øÈëÉèÖÃÄ£Ê½
	vopIdx_VoiceOpen=4,//¿ªÆôÓïÒô
	vopIdx_VoiceClose=5,//¹Ø±ÕÓïÒô
	vopIdx_WifiReset=6,//ÍøÂç¸´Î»³É¹¦
	vopIdx_WifiConnecting=7,//ÍøÂçÕıÔÚÁ¬½Ó
	vopIdx_WifiOk=8,//ÍøÂçÁ¬½Ó³É¹¦
	vopIdx_CHing=9,//¿ªÊ¼³äµç
	vopIdx_CHcomplete=10,//³äµçÒÑÍê³É
	vopIdx_Choff=11,//³äµçÖĞ¶Ï
	vopIdx_standard=12,//½øÈë±ê×¼Ä£Ê½
	vopIdx_RUNm2=13,//½øÈëÇ¿Á¦Ä£Ê½
	vopIdx_nop2=14,//´óË®³åÏ´Ä£Ê½
	vopIdx_RUNCL=15,//½øÈë×ÔÇåÏ´Ä£Ê½
	vopIdx_RunclOver=16,//×ÔÇåÏ´ÒÑÍê³É
	vopIdx_RUNover=17,//ÔËĞĞ½áÊø£¬Çë·Å»Øµ××ù×ÔÇåÏ´
	vopIdx_RUNOFF=18,//ÔËĞĞ½áÊø
	vopIdx_Chlowing=19,//µçÁ¿²»×ã£¬Çë¼°Ê±³äµç
	nop3=20,//µçÁ¿²»×ã£¬ÇëÁ¢¼´³äµç
	nop4=21,//µçÁ¿²»×ã£¬Çë³äµçºó¼ÌĞø
	vopIdx_CHErr=22,//³äµçÒì³££¬Çë¼ì²é³äµçÆ÷
	vopIdx_ConnectDragLala=23,//Ï´µØ»úÒÑÁ¬½Ó
	vopIdx_CisternOk=24,//Ë®ÏäÒÑ°²×°
	vopIdx_CisternNo=25,//Ë®ÏäÒÑÈ¡³ö
	vopIdx_CisternTake=26,//Ë®ÏäÒÑÈ¡³ö£¬½øÈë´óË®³åÏ´Ä£Ê½
	vopIdx_sewageErr=27,//ÎÛË®ÏäÒÑÂú£¬ÇëÇåÀíÎÛË®Ïä
	vopIdx_ClearErr=28,//Çë¼ÓÈëÇåË®
	vopIdx_PumpErr=29,//Ë®±Ãµç»úÒì³£
	nop5=30,//Ë®±Ãµç»úÎ´°²×°
	vopIdx_RollerErr=31,//Çë¼ì²é¹öÍ²
	vopIdx_ConnectAspiration=32,//Îü³¾Æ÷ÒÑÁ¬½Ó
	vopIdx_DeviceErr=33,//Îü³¾Æ÷¹ÊÕÏ
	vopIdx_ConnectFeather=34,//µç¶¯Çå½àË¢ÒÑÁ¬½Ó
	nop6=35,//µç¶¯Çå½àË¢¹ÊÕÏ
	nop7=36,//Ï´³µ»úÒÑÁ¬½Ó
	nop8=37,//Ï´³µ»ú¹ÊÕÏ
	vopIdx_ConnectSpray=38,//»îÑõÅçÎíÆ÷ÒÑÁ¬½Ó
	vopIdx_RUN3=39,//½øÈë»îÑõÉ±¾úÄ£Ê½(¼ÓÊªÄ£Ê½)
	vopIdx_40=40,//»îÑõÅçÎíÆ÷¹ÊÕÏ

	vopIdx_STOP = 0xFE,					/*/Í£Ö¹Ïî*/
}vopIdx_t;

typedef enum {
    SM_RS485_IDLE = 0,
    SM_RS485_START = 1,
    SM_RS485_SEND = 2,
    SM_RS485_OVER = 3,
    SM_RS485_ERROR = 0xF1,
} rs485sm_t;

/*******************************************************************************/
typedef struct ComponentField_s{
    u8 mop;
	u8 roller;
	// u8 pump;
	u8 battery;
	u8 charge;
	u8 clearWater;
} ComponentField_t;

typedef struct NetInfo_s{
    char ssid[36];
    char ip[20];
    char mac[20];
    int rssi;
    int flag;
    /** 
     * flag: updated or not
     * bit 0: ssid
     * bit 1: ip
     * bit 2: mac
     * bit 3: rssi
     **/
} NetInfo_t;

typedef struct reportStatusBody_s{
	u8 index;
	u8* body;
} reportStatusBody_t;
/*******************************************************************************/

/*******************************************************************************/
#endif /** ifndef end **/
