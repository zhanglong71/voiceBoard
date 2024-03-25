#ifndef __L_JSONTL_H__
#define __L_JSONTL_H__

#define Mu8FIFO_bufLen(q) (MTABSIZE((q)->buf))

jsonTL_t* getReportCmdbyMode(u8 mode);
jsonTL_t* getGetCharCmdbyMode(u8 mode);

jsonTL_t* getDevInfo(u8 idx);
jsonTL_t* getHeartbeat(void);

jsonTL_t* getService(u8 idx);
jsonTL_t* getConnectWifi(u8 idx);

void sm_sendData(jsonTL_t* p);
void sm_sendData_once(jsonTL_t* jp);
objType_t sm_receiveData(u8 *data);

u8 getCommandKeyArrLen(void);
jsonTL_t* getCommandKey(u8 idx);

u8 getIdxbyMode(u8 mode);

int reportHeartbeat(unsigned *arg);
int reportDevInfo(unsigned *arg);
int reportService(unsigned *arg);
int reportGetCharCmd(void);
int reportReportCharCmd(void);
int reportResetNet(u8 arg);
int reportScanWifi(void *arg);
int reportConnectWifi(void *arg);
int reportBatteryLevel(u8 arg);
int reportComponentStatus(u8 statusIndex);

int getCharAckComponentStatus(u8 statusIndex);

// void reportTest(void);

#endif
