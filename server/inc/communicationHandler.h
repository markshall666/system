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
  void triggerEvent(uint8_t, TransactionHandler::transactionState, unsigned, void*);

  private:
  static void* mainLoop(void* arg);
  void handleRegisterAppReq(union itcMsg* msg);
  void handleUpdateMoReq(union itcMsg* msg);
  void handleCreateMoCfm(union itcMsg* msg);
  void handleCreateMoRej(union itcMsg* msg);
  void handleCompletedMoCfm(union itcMsg* msg);
  void handleCompletedMoRej(union itcMsg* msg);
  void handleApplyMoCfm(union itcMsg* msg);
  void handleEvent();

  std::map<pthread_t, std::string>* appMapPtr;
  TransactionHandler* tranHandlerPtr;
  static bool newEvent;
  uint8_t txId;
  TransactionHandler::transactionState txState;
  unsigned objectId;
  void* txData;

};

#endif /* COMMUNICATIONHANDLER_H_ */
