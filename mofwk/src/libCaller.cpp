#include "libCaller.h"
#include "callbackObj.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "messages.h"
#include "itc.h"
#include "trace.h"

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

  itcId = initItc(appName);
  if (!itcId)
  {
    return false;
  }

  union itcMsg* msg = itcAlloc(sizeof(RegisterAppReqS), REGISTER_APP_REQ);
  strcpy(msg->registerAppReq.appName, appName);
  TRACE_MSG(msg);
  if (!sendData(TRAN_SERVER, msg))
  {
    return false;
  }

  msg = receiveData();
  TRACE_MSG(msg);

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
	return getFileDescriptor(itcId);
}

bool LibCaller::dispatch()
{
  bool result;
  union itcMsg* msg = receiveData();
  TRACE_MSG(msg);
  switch (msg->msgNo)
  {
    case CREATE_MO_REQ:
    {
      result = objMap[1]->onCall(msg->createMoReq.moName);
      itcFree(msg);
      msg = itcAlloc(sizeof(CreateMoCfmS), CREATE_MO_CFM);
      sendData(TRAN_SERVER, msg);
      break;
    }
    case COMPLETED_MO_REQ:
    {
      result = objMap[1]->onCall(msg->completedMoReq.moName);
      itcFree(msg);
      msg = itcAlloc(sizeof(CompletedMoCfmS), COMPLETED_MO_CFM);
      //msg = itcAlloc(sizeof(CompletedMoRejS), COMPLETED_MO_REJ);
      //strcpy(msg->completedMoRej.error, "dupa");
      sendData(TRAN_SERVER, msg);
      break;
    }
    case APPLY_MO_REQ:
    {
      result = objMap[1]->onCall(msg->applyMoReq.moName);
      itcFree(msg);
      msg = itcAlloc(sizeof(ApplyMoCfmS), APPLY_MO_CFM);
      sendData(TRAN_SERVER, msg);
      break;
    }
    default:
    {
      TRACE_ERROR("Unknown message, msgNo = 0x%x", msg->msgNo);
    }
  }

  return result;
}
