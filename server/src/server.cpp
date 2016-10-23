#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "messages.h"
#include "itc.h"
#include "communicationHandler.h"
#include "trace.h"


int main(int argc, const char** argv)
{
  TRACE_INIT(argc, argv);
  CommunicationHandler* comHandler = new CommunicationHandler();
  comHandler->init();
  while(1);

  /*while(1)
  {
	  memset(buf, 0, 16);
	  fgets((char*)buf, 16, stdin);
	  union itcMsg* msg = itcAlloc(sizeof(DispatchAppS), DISPATCH_APP);
	  strcpy(msg->dispatchApp.message, (char*)buf);
	  sendData(client, msg);
  }
  terminateItc(&fd);*/
}
