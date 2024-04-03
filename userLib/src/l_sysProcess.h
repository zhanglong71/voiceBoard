#ifndef __L_SYSPROCESS_H__
#define __L_SYSPROCESS_H__

int sysProcess(void *data);
#if 0
void AckGetCharWorkMode(void);
void AckGetCharRollerStatus(void);
void AckGetCharPumpStatus(void);
void AckGetCharBatteryStatus(void);
void AckGetCharChargeStatus(void);
void AckGetCharClearWaterStatus(void);
#endif
void checkAndAckGetCharNetInfo(void);
void checkAndAckGetCharUpdate(void);

void netInfoData_init(void);

RetStatus setStatusByvoiceIdx(u8 idx);
RetStatus reportStatusByvoiceIdx(u8 idx);
RetStatus AckgetCharStatusByMsgType(msgType_t msgType);


#endif
