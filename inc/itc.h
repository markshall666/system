#ifndef ITC_H
#define ITC_H

#include <stdint.h>

#define ITC_MAX_MSG_SIZE 1024
#define ITC_MESSAGE_HEADER 8

struct internalMessage
{
	uint32_t size;
	uint32_t senderFd;
	uint32_t msgNo;
};

bool initItc(const char* name, int* fd);

bool sendData(int sender, const char* receiver, union itcMsg* msg);

union itcMsg* receiveData(int sock);

void terminateItc(int fd);

union itcMsg* itcAlloc(size_t bufSize, uint32_t msgNo);

void itcPrintMsg(union itcMsg* msg);

void itcFree(union itcMsg* msg);

#endif
