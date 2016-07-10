#ifndef ITC_H
#define ITC_H

#include <stdint.h>
//#include <stdbool.h>
#include <pthread.h>

#define ITC_MAX_MSG_SIZE 1024
#define ITC_MESSAGE_HEADER 8
#define MAX_NO_APP 16

struct internalMsg
{
	uint32_t size;
	uint32_t senderFd;
	uint32_t msgNo;
};

struct threadData
{
	int fd;
	char name[16];
	pthread_t tId;
	void* buf;
};

union itcMsg;


bool initItc(const char* name, int* fd);

bool sendData(int sender, const char* receiver, union itcMsg* msg);

union itcMsg* receiveData();

void terminateItc(int fd);

union itcMsg* itcAlloc(size_t bufSize, uint32_t msgNo);

void itcPrintMsg(union itcMsg* msg);

void itcFree(union itcMsg* msg);

struct internalMsg* getInternalMsg(union itcMsg* msg);

struct threadData* getThreadDataPtr(int sock);

#endif
