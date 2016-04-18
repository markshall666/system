#ifndef LIB_FACTORY_H
#define LIB_FACTORY_H

#include "libCaller.h"

class LibFactory
{
  static LibCaller* instance;
  static char* appName;
  public:
  static LibCaller* getInstance();
  static LibCaller* getInstance(const char* _appName);
};

#endif
