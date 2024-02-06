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
#endif
