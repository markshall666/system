/*
 * itc_internal.c
 *
 *  Created on: 16-07-2016
 *      Author: marek
 */

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include "itc_internal.h"

struct threadData** threadsData = NULL;
int noThd = 0;

struct internalMsg* getInternalMsg(union itcMsg* msg)
{
	uint32_t* msgPtr = (uint32_t*)msg;
	struct internalMsg* intMsgPtr = NULL;
	return (struct internalMsg*)(msgPtr - (&(intMsgPtr->msgNo) - &(intMsgPtr->size)));
}

void storeThreadData(struct threadData* data)
{
	if (threadsData == NULL)
	{
		threadsData = (struct threadData**)malloc(sizeof(struct threadData*) * MAX_NO_APP);
		for (uint32_t i = 0; i < MAX_NO_APP; ++i)
		{
		  threadsData[i] = NULL;
		}
	}

	for (int i = 0; i <= MAX_NO_APP; i++)
	{
		if (threadsData[i] == NULL)
		{
			threadsData[i] = data;
			break;
		}
	}
	noThd++;
}

void removeThreadData(struct threadData* data)
{
	free(data->buf);
	free(data);
	for (int i = 0; i <= MAX_NO_APP; i++)
	{
		if (threadsData[i] == data)
		{
			threadsData[i] = NULL;
			break;
		}
	}
	noThd--;

	if (noThd == 0)
	{
		free(threadsData);
		threadsData = NULL;
	}
}

struct threadData* getThreadDataPtr(int sock)
{
	uint32_t index = 0;
	if (!sock)
	{
		pthread_t currentTId = pthread_self();
		while (threadsData[index] == NULL || threadsData[index]->tId != currentTId)
		{
			++index;
		}
	}
	else
	{
		while (threadsData[index] == NULL || threadsData[index]->fd != sock)
		{
			++index;
		}
	}

	return threadsData[index];
}
