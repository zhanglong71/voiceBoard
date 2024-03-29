#ifndef __L_SYSPROCESS_H__
#define __L_SYSPROCESS_H__

int sysProcess(void *data);

void checkAndAckGetCharWorkMode(void);
void checkAndAckGetCharRollerStatus(void);
void checkAndAckGetCharPumpStatus(void);
void checkAndAckGetCharBatteryStatus(void);
void checkAndAckGetCharChargeStatus(void);
void checkAndAckGetCharClearWaterStatus(void);
void checkAndAckGetCharNetInfo(void);
void checkAndAckGetCharUpdate(void);

void netInfoData_init(void);

#endif
