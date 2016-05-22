#include "libCaller.h"
#include "callbackObj.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "itc.h"



using namespace std;

LibCaller::LibCaller(const char* _appName)
  :mapIter(0),fd(0)
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
  if (!initItc(appName, &fd))
  {
	  return false;
  }
  int numBytes = strlen(appName);
  sendData(fd, "tranServer", appName, numBytes);

  void* buf = malloc(8);
  numBytes = receiveData(fd, buf);
  printf("received %d bytes %s\n", numBytes, (char*)buf);

  if (!strcmp((char*)buf, "initOk"))
  {
	  return true;
  }
  else
  {
	  return false;
  }
}

int LibCaller::getEventDescriptor()
{
	return fd;
}

bool LibCaller::dispatch()
{
	void* buf = malloc(8);
    int numBytes = receiveData(fd, buf);
	const char* message = (const char*)buf;
    return objMap[1]->onCall(message);
}
