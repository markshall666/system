#include "libCaller.h"
#include "libFactory.h"
#include "callbackObj.h"
#include "myCallbackObj.h"
#include <iostream>

using namespace std;

int main()
{  
  const char* name = "kotek";
  LibCaller* caller = LibFactory::getInstance(name);
  CallbackObj* obj = new MyCallbackObj();  
  if (caller->registerObj(obj))
  {
   cout << "Registered successful\n";
  }
  
  caller->init();  

  if (caller->dispatch())
  {
   cout << "Dispatch returned true\n";
  }
  //while(1);

  return 0;
}
