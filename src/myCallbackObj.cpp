#include "myCallbackObj.h"
#include <iostream>
#include <stdio.h> //delete?

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
   if (text[0] == '\n')
   {
	   return false;
   }
   return true;
}
