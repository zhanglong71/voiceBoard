#ifndef __CONST_H__
#define __CONST_H__

/*******************************************************************************/

/*******************************************************************************
 *
 * constant define here
 *
 *******************************************************************************/
#define	STACKSIZE	8
#define	QUEUESIZE	8

#define U8FIFOSIZE   128
//#define	OK	0
//#define	ERROR	-1

#define CKVTABSIZE (6)


#define	TRUE	1
#define	FALSE	0

#define CSEND_BUF_NUM	8	//16	 //缓冲区个数
#define CSEND_BUF_SIZE	16	//16	 //缓冲区大小

#define TIMER_NUM	4       /** 定时器个数 **/
#define TIMER_NUM_40US	1       /** 200us Timer **/
/** 
 * 0 - 计时计数
 * 1 - 开/闭锁
 * 2 - VOP语音
 * 3 - led blink
 * 4 - ADC采集周期
 */
#define TIMER_0 	0		/** 100Hz定时, 0等于关闭时钟 **/
#define TIMER_UNIT 	(1)		/** 100Hz定时, 0等于关闭时钟 **/
#define TIMER_2UNIT 	(2)		/** 100Hz定时, 0等于关闭时钟 **/
#define TIMER_3UNIT 	(3)		/** 100Hz定时, 0等于关闭时钟 **/
#define TIMER_10MS  (TIMER_UNIT)

#define TIMER_50MS	(TIMER_10MS * 5)
#define TIMER_100MS (TIMER_10MS * 10)

#define TIMER_200MS (TIMER_10MS * 20)
#define TIMER_400MS (TIMER_10MS * 40)
#define TIMER_800MS (TIMER_10MS * 80)
#define TIMER_1SEC  (TIMER_10MS * 100)

#define TIMER_2SEC  (TIMER_10MS * 200)
#define TIMER_3SEC  (TIMER_10MS * 300)
#define TIMER_5SEC  (TIMER_10MS * 500)
#define TIMER_6SEC  (TIMER_10MS * 600)
#define TIMER_10SEC (TIMER_10MS * 1000)
/*******************************************************************************/
#define TIMER_50US 	1		/** about 16Khz(62.6us) **/

#define TIMER_100US	  (TIMER_50US * 2)
#define TIMER_1000US   (TIMER_50US * 20)
#define TIMER_5000US   (TIMER_50US * 50)

/*******************************************************************************/
#define CCR4_Val (40961)
#define CCR3_Val (27309)
//#define CCR2_Val (350)  // 17.11KHz
#define CCR2_Val (700)  // 8.55KHz
#define CCR1_Val (1500)    // 4.16KHz
//#define CCR1_Val (375)    // 16.18KHz
//#define CCR1_Val (350)  //17.11KHz

/*******************************************************************************/
#define	CVOP_VOL0		0xe0
//...
#define	CVOP_VOL7		0xe7
#define	CVOP_VOL15		0xef

#define	CVOP_ALLLOOP		0xf1	//play voice again and again
#define	CVOP_ONELOOP		0xf2	//play one voice again and again
#define	CVOP_STOP		0xFE	//stop playing

/*******************************************************************************/
#define	CMAX1_COUPLE		6    // the max number of kv couple in one object between master and slave (uart1 communicate)

/*******************************************************************************/
/**  index for sendto wifi ֻҪ����ͬ���� **/
#define CINDEX_UNKNOW                 (0)

#define CINDEX_STANDBY                (1)
#define CINDEX_STANDARD               (2)
#define CINDEX_HIGHPOWER              (3)
#define CINDEX_CLEANING               (4)
#define CINDEX_RINSE                  (5)

#define CINDEX_ROLLEROVERLOAD         (6)
#define CINDEX_ROLLERNORMAL           (7)

#define CINDEX_CLEARWATERNORMAL       (8)
#define CINDEX_CLEARWATERSHORTAGE     (9)

#define CINDEX_PUMPNORMAL             (10)
#define CINDEX_PUMPOVERLOAD           (11)
#define CINDEX_PUMPCURRENTSMALL       (12)

#define CINDEX_BATTERYNORMAL          (13)
#define CINDEX_BATTERYLOW             (14)
#define CINDEX_BATTERYLEVEL           (15)

#define CINDEX_UNCHARGED              (16)
#define CINDEX_CHARGING               (17)
#define CINDEX_CHARGECOMPLETE         (18)
#define CINDEX_CHARGEFAULT            (19)

#define CINDEX_CONNECTED              (20)
#define CINDEX_DISCONNECTED           (21)
#define CINDEX_NETINFO                (22)
#define CINDEX_UPDATE                (23)

#define CINDEX_TANKINPLACE            (31)
#define CINDEX_TANKNOTINPLACE         (32)
#define CINDEX_DORMANCY               (33)
#define CINDEX_SCREENSHUTDOWN         (34)
#define CINDEX_NEUTRAL                (35)
#define CINDEX_CONNECTION             (36)
#define CINDEX_FLUSHING               (37) 
#define CINDEX_FULLSEWAGE             (38)
#define CINDEX_SETUP                  (39)

#define CINDEX_INVALID (0xff)

/*******************************************************************************/
/**  index for from wifi ֻҪ����ͬ���� **/

#if 0
#define CINDEX_GETDEVINFO             (0)
#define CINDEX_HEARTBEAT              (1)  
#define CINDEX_PUTCHAR                (2)
#define CINDEX_GETCHAR                (3)
#define CINDEX_REPORTSERVICE          (4)

#define CINDEX_SCANWIFI               (5)
#define CINDEX_CONNECTWIFI            (6)

#define CINDEX_GETRSSI                (7)
#define CINDEX_PUTWIFISTATUS          (8)
#define CINDEX_GETWIFISTATUS          (9)

#define CINDEX_RESETNET               (10)
#define CINDEX_GETSSID                (11)

#define CINDEX_GETIP                  (12)
#define CINDEX_GETMAC                 (13)
#define CINDEX_PUTSYNC                (14)
#endif

#define CKEYINDEX_NONE                   (0)
#define CKEYINDEX_HEARTBEAT              (1)
#define CKEYINDEX_PUTCHAR                (2)
#define CKEYINDEX_GETCHAR                (3)
#define CKEYINDEX_REPORTSERVICE          (4)

#define CKEYINDEX_SCANWIFI               (5)
#define CKEYINDEX_CONNECTWIFI            (6)

#define CKEYINDEX_GETRSSI                (7)
#define CKEYINDEX_PUTWIFISTATUS          (8)
#define CKEYINDEX_GETWIFISTATUS          (9)

#define CKEYINDEX_RESETNET               (10)
#define CKEYINDEX_GETSSID                (11)

#define CKEYINDEX_GETIP                  (12)
#define CKEYINDEX_GETMAC                 (13)
#define CKEYINDEX_PUTSYNC                (14)
#define CKEYINDEX_GETDEVINFO             (15)

/*******************************************************************************/
#define CBODYINDEX_NONE              (0)
#define CBODYINDEX_FAIL              (1)
#define CBODYINDEX_ERROR             (2)
#define CBODYINDEX_MOP               (3)
#define CBODYINDEX_ROLLER            (4)
#define CBODYINDEX_CLEARWATER        (5)
#define CBODYINDEX_PUMP              (6)
#define CBODYINDEX_BATTERYSTATUS     (7)
#define CBODYINDEX_CHARGE            (8)
#define CBODYINDEX_NETINFO           (9)
#define CBODYINDEX_UPDATE            (10)
#define CBODYINDEX_STATUS            (11)

#define CBODYINDEX_0                 (12)
#define CBODYINDEX_1                 (13)
#define CBODYINDEX_8                 (14)
#define CBODYINDEX_OK                (15)


/*******************************************************************************/
#endif
