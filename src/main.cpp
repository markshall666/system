#include "libCaller.h"
#include "libFactory.h"
#include "callbackObj.h"
#include "myCallbackObj.h"
#include <iostream>
#include <poll.h>

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
  
  if (caller->init())
  {
	  cout << "Init ok\n";
  }

  struct pollfd fds;
  fds.fd = caller->getEventDescriptor();
  fds.events = POLLIN;
  while(1)
  {
	poll(&fds, 1, -1);
	if (fds.revents == POLLIN)
	{
		if (caller->dispatch())
		{
		  cout << "Dispatch returned true\n";
		}
	}
  }
  return 0;
}
