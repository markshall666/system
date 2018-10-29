#ifndef LIB_CALLER_H
#define LIB_CALLER_H

#include "callbackObj.h"
#include "iostream"
#include <map>

#define CONFIG_SERVER "CONFIG_SERVER_SERVICE"

class LibCaller
{
  std::map<int, CallbackObj*> objMap;
  int mapIter;
  char* appName;
  unsigned long int itcId;
  public:
  LibCaller(const char* _appName);
  virtual ~LibCaller();

  bool dispatch();
  bool registerObj(CallbackObj* _obj);
  bool init();
  void terminate();
  int getEventDescriptor();
};

#endif
