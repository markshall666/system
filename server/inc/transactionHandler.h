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

  bool handleCreate(unsigned, std::vector<std::string>&);
  bool handleModify(std::string name, void* attr);
  bool handleDelete(std::string name);

  void setTransactionState(transactionState);

  private:
  void startTransaction();
  void changeState(transactionState, void*);
  bool wait();

  DataBaseIf* dataBasePtr;
  CommunicationHandler* comPtr;
  transactionState txState;
  uint8_t ongoingTxId;
};

#endif /* TRANSACTIONHANDLER_H_ */
