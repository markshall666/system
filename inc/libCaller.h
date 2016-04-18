#ifndef LIB_CALLER_H
#define LIB_CALLER_H

#include "callbackObj.h"
#include "iostream"
#include <map>

class LibCaller
{
  std::map<int, CallbackObj*> objMap;
  int mapIter;
  char* appName;
  public:
  LibCaller(const char* _appName);

  virtual ~LibCaller()
  {
  }
  bool dispatch();
  bool registerObj(CallbackObj* _obj);
  void init();
  int getEventDescriptor();
};

#endif
