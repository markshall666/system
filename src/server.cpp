#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "itc.h"

#define NAME "tranServer"

int main()
{ 
  int fd;
  void* buf = malloc(8);
  if (!initItc(NAME, &fd))
  {
    return 0;
  }
  int numBytes = receiveData(fd, buf);
  printf("received init from %s\n", (const char*)buf);
  char* client = (char*)malloc(numBytes);
  strcpy(client,(const char*)buf);
  const char* message = "initOk";
  sendData(fd, client, (void*)message, 6);

  while(1)
  {
	  fgets((char*)buf, 8, stdin);
	  sendData(fd, client, buf, 8);
  }
  terminateItc(fd);
}
