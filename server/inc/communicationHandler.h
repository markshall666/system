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
#include <map>
#include "transactionHandler.h"

#define NAME "configServer"

class CommunicationHandler
{
  public:
  CommunicationHandler(std::map<pthread_t, std::string>* mapPtr, TransactionHandler* tranHandler);
  virtual ~CommunicationHandler();

  void init();
  bool sendEvent(std::string);

  private:
  static void* mainLoop(void* arg);
  void handleRegisterAppReq(union itcMsg* msg);
  void handleUpdateMoReq(union itcMsg* msg);

  std::map<pthread_t, std::string>* appMapPtr;
  TransactionHandler* tranHandlerPtr;
};

#endif /* COMMUNICATIONHANDLER_H_ */
