#ifndef ITC_H
#define ITC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "itc_internal.h"


bool initItc(const char* name, int* fd);

bool sendData(const char* receiver, union itcMsg* msg);

union itcMsg* receiveData();

void terminateItc(int* fd);

union itcMsg* itcAlloc(size_t bufSize, uint32_t msgNo);

void itcFree(union itcMsg* msg);

pthread_t getSenderTId(union itcMsg* msg);

#ifdef __cplusplus
}
#endif

#endif
