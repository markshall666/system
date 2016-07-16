/*
 * itc_internal.c
 *
 *  Created on: 16-07-2016
 *      Author: marek
 */

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include "itc_internal.h"

struct threadData** threadsData =  NULL;

struct internalMsg* getInternalMsg(union itcMsg* msg)
{
	uint32_t* msgPtr = (uint32_t*)msg;
	struct internalMsg* intMsgPtr = NULL;
	return (struct internalMsg*)(msgPtr - (&(intMsgPtr->msgNo) - &(intMsgPtr->size)));
}

struct threadData* getThreadDataPtr(int sock)
{
	uint32_t index = 0;
	if (!sock)
	{
		pthread_t currentTId = pthread_self();
		while (threadsData[index]->tId != currentTId)
			{
				++index;
			}
	}
	else
	{
		while (threadsData[index]->fd != sock)
		{
			++index;
		}
	}
	return threadsData[index];
}

struct threadData*** getThreadsData()
{
	return &threadsData;
}
