#include "libCaller.h"
#include "callbackObj.h"
#include <iostream>
#include "itc.h"



using namespace std;

LibCaller::LibCaller(const char* _appName)
  :mapIter(0)
{
  appName = (char*)_appName;
}

bool LibCaller::dispatch()
{
  const char* message = "Something";
  return objMap[1]->onCall(message);
}

bool LibCaller::registerObj(CallbackObj* obj)
{
  objMap.insert(pair<int,  CallbackObj*>(++mapIter, obj));
  cout << "Object registered with index: " << mapIter << "\n";
  return true;
}

bool LibCaller::init()
{
  cout << "Init called\n";
  if (!initItc(appName, &fd))
  {
	  return false;
  }
  return true;
}
