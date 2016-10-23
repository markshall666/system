/*
 * communicationHandler.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef COMMUNICATIONHANDLER_H_
#define COMMUNICATIONHANDLER_H_

#include "pthread.h"
#include <string>

#define NAME "configServer"

class CommunicationHandler
{
  public:
  CommunicationHandler();
  virtual ~CommunicationHandler();

  void init();
  bool sendEvent(std::string);

  private:
  static void* mainLoop(void* arg);
};

#endif /* COMMUNICATIONHANDLER_H_ */
