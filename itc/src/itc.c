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


mboxId initItc(const char* name)
{
  if (noThd == MAX_NO_THS)
  {
    TRACE_ERROR("Max number of threads (%d) exceeded", MAX_NO_THS);
    return 0;
  }
  
  if (!checkName(name))
  {
    TRACE_ERROR("Name %s already registered or is too long", name);
    return 0;
  }

  struct sockaddr_un server;
  int sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (sock < 0) 
  {
    TRACE_PERROR("opening dgram socket");
    return 0;
  }
  
  server.sun_family = AF_UNIX;
  strcpy(server.sun_path, name);
  unlink(name);
  if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)))
  {
    TRACE_PERROR("binding dgram socket");
    return 0;
  }

  struct threadData* thread = (struct threadData*)malloc(sizeof(struct threadData));
  thread->fd = sock;
  thread->tId = pthread_self();
  strcpy(thread->name, name);
  thread->buf = malloc(ITC_MAX_MSG_SIZE);

  storeThreadData(thread);

  TRACE_DEBUG("no threads = %d", noThd);
  TRACE_DEBUG("fd = %d, tId = 0x%x, name = %s", thread->fd, (uint32_t)thread->tId, thread->name);
  return thread->tId;
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
  TRACE_MSG(msg);
  if (sendto(thDataPtr->fd, msgInt, size, 0, (struct sockaddr *) &receiverAddr, sizeof(struct sockaddr_un)) != size)
  {
    TRACE_PERROR("failed to send");
    itcFree(msg);
    return false;
  }
  itcFree(msg);
  return true;
}

/*bool sendData2(mboxId receiver, union itcMsg* msg)
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
}*/

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
  union itcMsg* msg = (union itcMsg*)&(msgPtr->msgNo);
  TRACE_MSG(msg);
  return msg;
}

bool terminateItc(mboxId mbox)
{
  struct threadData* thDataPtr = getThreadDataPtr(mbox);
  if (thDataPtr == NULL)
  {
    TRACE_ERROR("failed to terminate, Itc not initialized");
    return false;
  }

  close(thDataPtr->fd);
  unlink(thDataPtr->name);
  removeThreadData(thDataPtr);
  return true;
}

union itcMsg* itcAlloc(size_t bufSize, uint32_t msgNo)
{
  struct internalMsg* msgPtr = NULL;
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

union itcMsg* itcAllocProto(int32_t protoSize,  uint32_t msgNo)
{
  struct internalMsg* msgPtr =  NULL;
  msgPtr = (struct internalMsg*)malloc(protoSize + 4 + ITC_MESSAGE_HEADER);
  if (msgPtr == NULL)
  {
    TRACE_PERROR("Cannot allocate message");
  }
  memset(msgPtr, 0, protoSize + 4 + ITC_MESSAGE_HEADER);
  msgPtr->size = protoSize + 4;
  msgPtr->msgNo = msgNo;
  TRACE_DEBUG("alloc %d bytes", msgPtr->size);
  return (union itcMsg*)&(msgPtr->msgNo);
}

int itcGetProtoSize(union itcMsg* msg)
{
  struct internalMsg* intMsg = getInternalMsg(msg);
  return (intMsg->size) - 4;
}

void itcFree(union itcMsg* msg)
{
  struct internalMsg* intMsg = getInternalMsg(msg);
  free(intMsg);
}

pthread_t getSenderTId(union itcMsg* msg)
{
  struct internalMsg* intMsg = getInternalMsg(msg);
  return intMsg->senderTId;
}

int getFileDescriptor(mboxId mbox)
{
  struct threadData* thDataPtr = getThreadDataPtr(mbox);
  if (thDataPtr == NULL)
  {
    TRACE_ERROR("getFileDescriptor failed, mbox not initialized!");
    return 0;
  }

  return thDataPtr->fd;
}
