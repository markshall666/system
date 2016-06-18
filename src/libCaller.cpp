#include "libCaller.h"
#include "callbackObj.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "itc.h"
#include "messages.h"



using namespace std;

LibCaller::LibCaller(const char* _appName)
  :mapIter(0),itcId(0)
{
  appName = (char*)_appName;
}

bool LibCaller::registerObj(CallbackObj* obj)
{
  objMap.insert(pair<int,  CallbackObj*>(++mapIter, obj));
  printf("Object registered with index: %d\n", mapIter);
  return true;
}

bool LibCaller::init()
{
  printf("Init called\n");
  bool result = false;

  if (!initItc(appName, &itcId))
  {
	  return false;
  }

  union itcMsg* msg = itcAlloc(sizeof(RegisterAppReqS), REGISTER_APP_REQ);
  msg->msgNo = REGISTER_APP_REQ;
  strcpy(msg->registerAppReq.appName, appName);
  itcPrintMsg(msg);
  sendData(itcId, TRAN_SERVER, msg);

  msg = receiveData(itcId);
  itcPrintMsg(msg);

  if (msg->registerAppCfm.result)
  {
	  result = true;
  }
  itcFree(msg);

  return result;
}

int LibCaller::getEventDescriptor()
{
	return itcId;
}

bool LibCaller::dispatch()
{
    union itcMsg* msg = receiveData(itcId);
    itcPrintMsg(msg);
    objMap[1]->onCall(msg->dispatchApp.message);
    itcFree(msg);
    return true;
}
