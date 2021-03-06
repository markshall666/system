#include "libCaller.h"
#include "libFactory.h"
#include "callbackObj.h"
#include "myCallbackObj.h"
#include <iostream>
#include <poll.h>
#include "trace.h"

using namespace std;

int trace_enable;

int main(int argc, const char** argv)
{
  TRACE_INIT(argc, argv);
  const char* name = "kotek";
  LibCaller* caller = LibFactory::getInstance(name);
  CallbackObj* obj = new MyCallbackObj();
  if (caller->registerObj(obj))
  {
   cout << "Registered successful\n";
  }
  else
  {
    LibFactory::releaseInstance();
    return 0;
  }

  if (caller->init())
  {
	  cout << "Init ok\n";
  }
  else
  {
	  LibFactory::releaseInstance();
	  return 0;
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
		else
		{
		  cout << "exit application\n";
		  LibFactory::releaseInstance();
		  break;
		}
	}
  }
  return 0;
}
