#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "messages.h"
#include "itc.h"

#define NAME "tranServer"

int main()
{ 
  int fd;
  void* buf = malloc(16);
  char* client = (char*)malloc(16);

  if (!initItc(NAME, &fd))
  {
    return 0;
  }
  union itcMsg* msg = receiveData();
  itcPrintMsg(msg);
  if (msg->msgNo == REGISTER_APP_REQ)
  {
	  strcpy(client, msg->registerAppReq.appName);
	  itcFree(msg);
	  union itcMsg* msg = itcAlloc(sizeof(RegisterAppCfmS), REGISTER_APP_CFM);
	  msg->registerAppCfm.result = true;
	  strcpy(msg->registerAppReq.appName, client);
	  sendData(fd, client, msg);
  }

  while(1)
  {
	  fgets((char*)buf, 16, stdin);
	  union itcMsg* msg = itcAlloc(sizeof(DispatchAppS), DISPATCH_APP);
	  strcpy(msg->dispatchApp.message, (char*)buf);
	  sendData(fd, client, msg);
  }
  terminateItc(fd);
}
