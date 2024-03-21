#ifndef __L_JSONTL_H__
#define __L_JSONTL_H__

RetStatus reportVersion(void);
jsonTL_t* getReportCmdbyMode(u8 mode);
jsonTL_t* getGetCharCmdbyMode(u8 mode);

jsonTL_t* getDevInfo(u8 idx);
jsonTL_t* getHeartbeat(void);

jsonTL_t* getService(u8 idx);

void sm_sendData(jsonTL_t* p);
objType_t sm_receiveData(u8 *data);

u8 getCommandKeyArrLen(void);
jsonTL_t* getCommandKey(u8 idx);

int reportHeartbeat(unsigned *arg);
int reportDevInfo(unsigned *arg);
int reportService(unsigned *arg);
int reportGetCharCmd(unsigned *arg);
int reportResetNet(void *arg);

// void reportTest(void);

/***********************************************/
unsigned char JsonParseL0(unsigned char* jsonstrbuf, kv_t* jsonstcarr);
void generateVoiceAckVer(u8* to, u8* ver);
void generateVoiceAckOk(char* to, u8 seq);
void generateVoiceAckErr(char* to, u8 seq);
u8 isPlayVoiceCommand(kv_t* kv_arr, u8* voi_idx);


/***********************************************/

#endif
