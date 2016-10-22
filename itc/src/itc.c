#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "itc.h"
#include "itc_internal.h"
#include "trace.h"


bool initItc(const char* name, int* fd)
{
  if (noThd == MAX_NO_APP)
  {
    TRACE_ERROR("Number of application exceeded");
	return false;
  }
  
  struct sockaddr_un server;
  int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sock < 0) 
  {
    TRACE_PERROR("opening dgram socket");
    return false;
  }
  
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, name);
  unlink(name);
  if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)))
  {
	TRACE_PERROR("binding dgram socket");
    return false;   
  }

  *fd = sock;
  struct threadData* thread = (struct threadData*)malloc(sizeof(struct threadData));
  thread->fd = *fd;
  thread->tId = pthread_self();
  strcpy(thread->name, name);
  thread->buf = malloc(ITC_MAX_MSG_SIZE);

  storeThreadData(thread);

  TRACE_DEBUG("no threads = %d", noThd);
  TRACE_DEBUG("fd = %d, tId = 0x%x, name = %s", thread->fd, (uint32_t)thread->tId, thread->name);
  return true;
}

bool sendData(const char* receiver, union itcMsg* msg)
{
  struct threadData* thDataPtr = getThreadDataPtr(0);
  if (thDataPtr == NULL)
  {
	  TRACE_ERROR("failed to send, Itc not initialized");
	  itcFree(msg);
	  return false;
  }
  struct internalMsg* msgInt = getInternalMsg(msg);
  msgInt->senderTId = pthread_self();
  uint32_t size = msgInt->size + ITC_MESSAGE_HEADER;
  struct sockaddr_un receiverAddr;
  receiverAddr.sun_family = AF_UNIX;
  strcpy(receiverAddr.sun_path, receiver);
  TRACE_DEBUG("sending message to %s len = %d", receiver, size - ITC_MESSAGE_HEADER);
  if (sendto(thDataPtr->fd, msgInt, size, 0, (struct sockaddr *) &receiverAddr, sizeof(struct sockaddr_un)) != size)
  {
    TRACE_PERROR("failed to send");
    itcFree(msg);
    return false;
  }
  itcFree(msg);
  return true;
}

union itcMsg* receiveData()
{
  struct threadData* thDataPtr = getThreadDataPtr(0);
  if (thDataPtr == NULL)
  {
	  TRACE_ERROR("failed to receive, Itc not initialized");
	  return NULL;
  }
  memset(thDataPtr->buf, 0, ITC_MAX_MSG_SIZE);
  uint32_t recBytes = recv(thDataPtr->fd, thDataPtr->buf, ITC_MAX_MSG_SIZE, 0);
  void* buf = malloc(recBytes);
  memcpy(buf, thDataPtr->buf, recBytes);
  struct internalMsg* msgPtr;
  msgPtr = (struct internalMsg*)buf;
  return (union itcMsg*)&(msgPtr->msgNo);
}

void terminateItc(int* fd)
{
  struct threadData* thDataPtr = getThreadDataPtr(0);
  if (thDataPtr == NULL)
  {
    TRACE_ERROR("failed to terminate, Itc not initialized");
    return;
  }

  close(*fd);
  *fd = 0;
  removeThreadData(thDataPtr);
}

union itcMsg* itcAlloc(size_t bufSize, uint32_t msgNo)
{
  struct internalMsg* msgPtr;
  msgPtr = (struct internalMsg*)malloc(bufSize + ITC_MESSAGE_HEADER);
  if (msgPtr == NULL)
  {
    TRACE_PERROR("Cannot allocate message");
  }
  memset(msgPtr, 0, bufSize + ITC_MESSAGE_HEADER);
  msgPtr->size = bufSize;
  msgPtr->msgNo = msgNo;
  TRACE_DEBUG("alloc %d bytes", msgPtr->size);
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
