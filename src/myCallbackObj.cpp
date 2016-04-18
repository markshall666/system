#include "myCallbackObj.h"
#include <iostream>

using namespace std;

MyCallbackObj::MyCallbackObj()
{
}
MyCallbackObj::~MyCallbackObj()
{
}
  
bool MyCallbackObj::onCall(const char* text)
{
   cout << "I am callback, " << text << "\n";
   return true;
}
