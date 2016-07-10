#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "itc.h"

struct threadData** threadDataPtr = NULL;

bool initItc(const char* name, int* fd)
{
  static uint32_t noThd = 0;
  if (noThd > MAX_NO_APP)
  {
    perror("Number of application exceeded");
	return false;
  }
  
  struct sockaddr_un server;
  int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sock < 0) 
  {
    perror("opening dgram socket");
    return false;
  }
  
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, name);
  unlink(name);
  if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)))
  {
    perror("binding dgram socket");
    return false;   
  }
  
  *fd = sock;
  struct threadData* thread = (struct threadData*)malloc(sizeof(struct threadData));
  thread->fd = *fd;
  thread->tId = pthread_self();
  strcpy(thread->name, name);
  thread->buf = malloc(ITC_MAX_MSG_SIZE);
  if (threadDataPtr == NULL)
  {
	threadDataPtr = (struct threadData**)malloc(sizeof(struct threadData*) * MAX_NO_APP);
	for (uint32_t i = 0; i < MAX_NO_APP; ++i)
	{
	  threadDataPtr[i] = NULL;
	}
  }
  threadDataPtr[noThd] = thread;
  ++noThd;
  printf("no threads = %d\n", noThd); //debug
  printf("fd = %d, tId = 0x%x, name = %s\n", thread->fd, (uint32_t)thread->tId, thread->name); //debug
  return true;
}

bool sendData(int sender, const char* receiver, union itcMsg* msg)
{
  struct internalMsg* msgInt = getInternalMsg(msg);
  uint32_t size = msgInt->size + ITC_MESSAGE_HEADER;
  struct sockaddr_un receiverAddr;
  receiverAddr.sun_family = AF_UNIX;
  strcpy(receiverAddr.sun_path, receiver);
  printf("sending message to %s len = %d\n", receiver, size - ITC_MESSAGE_HEADER); //debug
  if (sendto(sender, msgInt, size, 0, (struct sockaddr *) &receiverAddr, sizeof(struct sockaddr_un)) != size)
  {
    perror("failed to send");
    return false;
  }
  itcFree(msg);
  return true;
}

union itcMsg* receiveData()
{
  struct threadData* thDataPtr = getThreadDataPtr(0);
  memset(thDataPtr->buf, 0, ITC_MAX_MSG_SIZE);
  uint32_t recBytes = recv(thDataPtr->fd, thDataPtr->buf, ITC_MAX_MSG_SIZE, 0);
  void* buf = malloc(recBytes);
  memcpy(buf, thDataPtr->buf, recBytes);
  struct internalMsg* msgPtr;
  msgPtr = (struct internalMsg*)buf;
  return (union itcMsg*)&(msgPtr->msgNo);
}

void terminateItc(int fd)
{
	close(fd);
}

union itcMsg* itcAlloc(size_t bufSize, uint32_t msgNo)
{
	struct internalMsg* msgPtr; 
	msgPtr = (struct internalMsg*)malloc(bufSize + ITC_MESSAGE_HEADER);
	if (msgPtr == NULL)
	{
		perror("Cannot allocate message");
	}
	msgPtr->size = bufSize;
	msgPtr->senderFd = 0;
	msgPtr->msgNo = msgNo;
	printf("alloc %d bytes\n", msgPtr->size); //debug
	return (union itcMsg*)&(msgPtr->msgNo); 
}

void itcFree(union itcMsg* msg)
{
	struct internalMsg* intMsg = getInternalMsg(msg);
	free(intMsg);
}

void itcPrintMsg(union itcMsg* msg)
{
	const unsigned char* const bytes = (const unsigned char* const)msg;
	uint32_t* msgPtr = (uint32_t*)msg;
	
	printf("[");
	for(size_t i = 0; i < *(msgPtr-2); i++)
	{
		printf("%02x ", bytes[i]);
	}
	printf("]\n");
}

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
		while (threadDataPtr[index]->tId != currentTId)
			{
				++index;
			}
	}
	else
	{
		while (threadDataPtr[index]->fd != sock)
		{
			++index;
		}
	}
	return threadDataPtr[index];
}
