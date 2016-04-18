#ifndef MY_CALLBACK_OBJ_H
#define MY_CALLBACK_OBJ_H

#include "callbackObj.h"

class MyCallbackObj: public CallbackObj
{
  public:
  MyCallbackObj();
  ~MyCallbackObj();
  
  bool onCall(const char* text);
};

#endif
