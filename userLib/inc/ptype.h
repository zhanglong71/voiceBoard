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
 * 重新拟定消息命名规则
 * 1. 普通以CMSG_xxx形式命名
 * 2. 以CSYS_xxx的形式命令在sys中处理的消息
 *************/    
 
    CSYS_INIT,  	//
    CSYS_INITS1,  	//
    CSYS_INITS2,  	//
    CSYS_INITS3,  	//
    
    CMSG_INIT,  	//初始化动作(进入选择)
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
 //   CMSG_DLAY, 	/** 时钟 **/
 //   CMSG_NEXT, 	/** 进行入下一阶段 **/
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

	C485_STEP,		/** 动作超时 **/
	C485_OVER,		/** 动作完成 **/
	
//	CACT_TOUT,		/** 动作超时 **/
//	CACT_OVER,		/** 动作完成 **/
	
	CPMT_TOUT,		/** 提示动作超时 **/
	CPMT_OVER,		/** 提示动作完成 **/

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
    u8 jLen;    /** ָ������! �����0�����ǲ�ȷ�� **/
    u8 *jBody;
    // void* arg; /** �յ���Ӧ�������Ӧ���� �� ���� **/
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

typedef enum {              /** 用于控制com数据的发送 **/
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
    u16 actionTime;	/** 持续时间 **/
    u16 actionPara;	/** 动作需要的其它参数，如VPidx索引号 **/
    u8 actionType;	/** 动作类型. 似与前面的参数有重叠之嫌，是一个区别动作的依据 **/
    // u8 __pad1;		/** 字节对齐 **/
} action_t;

typedef struct actionQueue_s {
    u8 head;
    u8 tail;
    u8 flag;
    msgType_t stepMsgType;			/** 阶段事件完成消息 **/
    msgType_t overMsgType;			/** 队列事件全部完成消息 **/
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
	vopIdx_ConnectNo=1,//δ�����豸//����//
	vopIdx_Disconnect=2,//�豸�ѶϿ�
	vopIdx_Install=3,//��������ģʽ
	vopIdx_VoiceOpen=4,//��������
	vopIdx_VoiceClose=5,//�ر�����
	vopIdx_WifiReset=6,//���縴λ�ɹ�
	vopIdx_WifiConnecting=7,//������������
	vopIdx_WifiOk=8,//�������ӳɹ�
	vopIdx_CHing=9,//��ʼ���
	vopIdx_CHcomplete=10,//��������
	vopIdx_Choff=11,//����ж�
	vopIdx_standard=12,//�����׼ģʽ
	vopIdx_RUNm2=13,//����ǿ��ģʽ
	vopIdx_nop2=14,//��ˮ��ϴģʽ
	vopIdx_RUNCL=15,//��������ϴģʽ
	vopIdx_RunclOver=16,//����ϴ�����
	vopIdx_RUNover=17,//���н�������Żص�������ϴ
	vopIdx_RUNOFF=18,//���н���
	vopIdx_Chlowing=19,//�������㣬�뼰ʱ���
	nop3=20,//�������㣬���������
	nop4=21,//�������㣬��������
	vopIdx_CHErr=22,//����쳣����������
	vopIdx_ConnectDragLala=23,//ϴ�ػ�������
	vopIdx_CisternOk=24,//ˮ���Ѱ�װ
	vopIdx_CisternNo=25,//ˮ����ȡ��
	vopIdx_CisternTake=26,//ˮ����ȡ���������ˮ��ϴģʽ
	vopIdx_sewageErr=27,//��ˮ����������������ˮ��
	vopIdx_ClearErr=28,//�������ˮ
	vopIdx_PumpErr=29,//ˮ�õ���쳣
	nop5=30,//ˮ�õ��δ��װ
	vopIdx_RollerErr=31,//�����Ͳ
	vopIdx_ConnectAspiration=32,//������������
	vopIdx_DeviceErr=33,//����������
	vopIdx_ConnectFeather=34,//�綯���ˢ������
	nop6=35,//�綯���ˢ����
	nop7=36,//ϴ����������
	nop8=37,//ϴ��������
	vopIdx_ConnectSpray=38,//����������������
	vopIdx_RUN3=39,//�������ɱ��ģʽ(��ʪģʽ)
	vopIdx_40=40,//��������������

	vopIdx_STOP = 0xFE,					/*/ֹͣ��*/
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
