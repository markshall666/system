#ifndef LIB_CALLER_H
#define LIB_CALLER_H

#include "callbackObj.h"
#include "iostream"
#include <map>

#define TRAN_SERVER "tranServer"

class LibCaller
{
  std::map<int, CallbackObj*> objMap;
  int mapIter;
  char* appName;
  int itcId;
  public:
  LibCaller(const char* _appName);

  virtual ~LibCaller()
  {
  }
  bool dispatch();
  bool registerObj(CallbackObj* _obj);
  bool init();
  int getEventDescriptor();
};

#endif
