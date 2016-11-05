/*
 * transactionhandler.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef TRANSACTIONHANDLER_H_
#define TRANSACTIONHANDLER_H_

#include "databaseIf.h"
#include <stdint.h>

class CommunicationHandler;

class TransactionHandler
{
  public:
  TransactionHandler(DataBaseIf* dataBasePtrIn);
  virtual ~TransactionHandler();

  enum transactionState {
    IDLE,
    CREATE,
    MODIFY,
    DELETE,
    ABORT,
    COMPLETE,
    APPLY
  };
  void setCommunicationHandlerPtr(CommunicationHandler*);

  bool handleCreate(unsigned, std::vector<std::string>&, std::string&);
  bool handleModify(unsigned, std::vector<std::string>&, std::string&);
  bool handleDelete(unsigned, std::vector<std::string>&, std::string&);

  void requestChangeState(transactionState);
  void setErrorStr(const char*);

  private:
  static void* startLoop(void* arg);
  void mainLoop();
  void startTransaction();
  bool waitTimeOut();
  bool waitTransactionEnd();
  const char* getTxStateString(transactionState);

  DataBaseIf* dataBasePtr;
  CommunicationHandler* comPtr;
  transactionState txState;
  transactionState requestedTxState;
  uint8_t ongoingTxId;
  std::vector<std::string>* txData;
  unsigned objectId;
  bool txResult;
  std::string errorStr;
};

#endif /* TRANSACTIONHANDLER_H_ */
