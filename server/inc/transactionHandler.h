/*
 * transactionhandler.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef TRANSACTIONHANDLER_H_
#define TRANSACTIONHANDLER_H_

#include "databaseIf.h"

class TransactionHandler
{
  public:
  TransactionHandler(DataBaseIf* dataBasePtrIn);
  virtual ~TransactionHandler();

  bool handleCreate(std::string name, void* attr);
  bool handleModify(std::string name, void* attr);
  bool handleDelete(std::string name);

  private:
  DataBaseIf* dataBasePtr;
};

#endif /* TRANSACTIONHANDLER_H_ */
