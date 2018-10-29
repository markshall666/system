#ifndef ITC_H
#define ITC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "itc_internal.h"

#define MY_MBOX 0
typedef pthread_t mboxId;

mboxId initItc(const char* name);
bool sendData(const char* receiver, union itcMsg* msg);
bool sendData2(mboxId receiver, mboxId sender, union itcMsg* msg);
union itcMsg* receiveData();
bool terminateItc(mboxId mbox);
union itcMsg* itcAlloc(size_t bufSize, uint32_t msgNo);
union itcMsg* itcAllocProto(int32_t protoSize,  uint32_t msgNo);
void itcFree(union itcMsg* msg);
pthread_t getSenderTId(union itcMsg* msg);
int getFileDescriptor(mboxId mbox);
int itcGetProtoSize(union itcMsg* msg);

#ifdef __cplusplus
}
#endif

#endif
