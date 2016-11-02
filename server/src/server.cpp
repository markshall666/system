#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"
#include "messages.h"
#include "itc.h"
#include "communicationHandler.h"
#include "transactionHandler.h"
#include "trace.h"
#include "databaseIf.h"
#include "database.h"
#include "cli.h"

int trace_enable;

int main(int argc, const char** argv)
{
  TRACE_INIT(argc, argv);
  DataBaseIf* dataBasePtr = new DataBase();
  TransactionHandler* tranPtr = new TransactionHandler(dataBasePtr);
  std::map<pthread_t, std::string> appMap;
  CommunicationHandler* comHandler = new CommunicationHandler(&appMap, tranPtr);
  tranPtr->setCommunicationHandlerPtr(comHandler);
  comHandler->init();
  Cli* cliPtr = new Cli(tranPtr, dataBasePtr);

  /*while(appMap.empty());
  for (std::map<pthread_t, std::string>::iterator it = appMap.begin(); it != appMap.end(); ++it)
  {
    TRACE_DEBUG("App name = %s, tid = 0x%x", it->second.c_str(), (unsigned int)it->first);
  }*/

  cliPtr->start();
  /*while(1)
  {
	  memset(buf, 0, 16);
	  fgets((char*)buf, 16, stdin);
	  union itcMsg* msg = itcAlloc(sizeof(DispatchAppS), DISPATCH_APP);
	  strcpy(msg->dispatchApp.message, (char*)buf);
	  sendData(client, msg);
  }*/
  //terminateItc(&fd);
}
