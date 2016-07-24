#include "libCaller.h"
#include "callbackObj.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "messages.h"
#include "itc.h"

using namespace std;

LibCaller::LibCaller(const char* _appName)
  :mapIter(0),itcId(0)
{
  appName = (char*)_appName;
}

LibCaller::~LibCaller()
{
	terminate();
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
  strcpy(msg->registerAppReq.appName, appName);
  itcPrintMsg(msg);
  if (!sendData(TRAN_SERVER, msg))
  {
	  return false;
  }

  msg = receiveData();
  itcPrintMsg(msg);

  if (msg->registerAppCfm.result)
  {
	  result = true;
  }
  itcFree(msg);

  return result;
}

void LibCaller::terminate()
{
  if (itcId)
  {
    terminateItc(itcId);
  }
  for (std::map<int, CallbackObj*>::const_iterator it = objMap.begin(); it != objMap.end(); it++)
  {
    delete it->second;
  }
  objMap.clear();
}

int LibCaller::getEventDescriptor()
{
	return itcId;
}

bool LibCaller::dispatch()
{
    union itcMsg* msg = receiveData();
    itcPrintMsg(msg);
    bool result = objMap[1]->onCall(msg->dispatchApp.message);
    itcFree(msg);
    return result;
}
