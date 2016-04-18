#ifndef CALLBACK_OBJ_H
#define CALLBACK_OBJ_H

class CallbackObj
{
  public:
  virtual ~CallbackObj()
  {
  }

  virtual bool onCall(const char* text) = 0;
};

#endif
