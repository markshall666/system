#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "itc.h"

bool initItc(const char* name, int* fd)
{
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
  return true;
}

bool sendData(int sender, const char* receiver, union itcMsg* msg)
{
  uint32_t* msgPtr = (uint32_t*)msg;
  uint32_t size = *(msgPtr-2) + ITC_MESSAGE_HEADER;
  struct sockaddr_un receiverAddr;
  receiverAddr.sun_family = AF_UNIX;
  strcpy(receiverAddr.sun_path, receiver);
  printf("sending message to %s len = %d\n", receiver, size - ITC_MESSAGE_HEADER); //debug
  if (sendto(sender, msgPtr-2, size, 0, (struct sockaddr *) &receiverAddr, sizeof(struct sockaddr_un)) != size)
  {
    perror("failed to send");
    return false;
  }
  itcFree(msg);
  return true;
}

union itcMsg* receiveData(int sock)
{
  void* buf = malloc(ITC_MAX_MSG_SIZE);
  uint32_t recBytes = recv(sock, buf, ITC_MAX_MSG_SIZE, 0);
  struct internalMessage* msgPtr;
  msgPtr = (struct internalMessage*)buf;
  return (union itcMsg*)&(msgPtr->msgNo);
}

void terminateItc(int fd)
{
  close(fd);
}

union itcMsg* itcAlloc(size_t bufSize, uint32_t msgNo)
{
	struct internalMessage* msgPtr; 
	msgPtr = (struct internalMessage*)malloc(bufSize + ITC_MESSAGE_HEADER);
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
	uint32_t* msgPtr = (uint32_t*)msg;
	free(msgPtr-2);
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
