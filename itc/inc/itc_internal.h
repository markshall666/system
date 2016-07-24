/*
 * itc_internal.h
 *
 *  Created on: 16-07-2016
 *      Author: marek
 */
#include <pthread.h>
#include <bits/pthreadtypes.h>
#include <stdint.h>

#ifndef ITC_INTERNAL_H_
#define ITC_INTERNAL_H_

#define ITC_MAX_MSG_SIZE 1024
#define ITC_MESSAGE_HEADER 8
#define MAX_NO_APP 16


struct internalMsg
{
	uint32_t size;
	uint32_t senderTId;
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

struct internalMsg* getInternalMsg(union itcMsg* msg);

struct threadData* getThreadDataPtr(int sock);

struct threadData*** getThreadsData();

#endif /* ITC_INTERNAL_H_ */
