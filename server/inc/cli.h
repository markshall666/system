/*
 * cli.h
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#ifndef CLI_H_
#define CLI_H_

#include "transactionHandler.h"
#include "databaseIf.h"

class Cli
{
  public:
  Cli(TransactionHandler* tranPtr, DataBaseIf* dataPtr);
  virtual ~Cli();

  void start();

  private:
  void mainLoop();
  void printMenu();
  void readCommand();
  std::string handleRead(std::string);
  std::string handleCreate(std::string);

  TransactionHandler* transactionHandlerPtr;
  DataBaseIf* dataBasePtr;
};



#endif /* CLI_H_ */
