#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
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

bool sendData(int sender, const char* receiver, void* buf, int numOfBytes)
{
  struct sockaddr_un receiverAddr;
  receiverAddr.sun_family = AF_UNIX;
  strcpy(receiverAddr.sun_path, receiver);
  if (sendto(sender, buf, numOfBytes, 0, (struct sockaddr *) &receiverAddr, sizeof(struct sockaddr_un)) != numOfBytes) 
  {
    perror("failed to send");
    return false;
  }
  return true;
}

int receiveData(int sock, void* buf)
{
  return recvfrom(sock, buf, 1024, 0, NULL, NULL);
}

void terminateItc(int fd)
{
  close(fd);
}
