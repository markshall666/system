#include "libFactory.h"
#include "libCaller.h"
#include <iostream>
#include <cstring>

using namespace std;

LibCaller* LibFactory::instance(NULL);
char* LibFactory::appName(NULL);

LibCaller* LibFactory::getInstance()
{
  if (!instance)
  {
    cout << "LibCaller cannot be created without application name!\n";
  }
  return instance;
}

LibCaller* LibFactory::getInstance(const char* _appName)
{
  if (!instance)
  {
    instance = new LibCaller(_appName);
    appName = (char*)_appName;
    cout << "LibCaller for " << _appName << " created\n";
  }
  else if (strcmp(appName, _appName) != 0)
  {
    cout << "Cannot create second caller for same application!\n";
    return NULL;
  }
  return instance;
}

void LibFactory::releaseInstance()
{
	delete instance;
	instance = NULL;
}
