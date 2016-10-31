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
#include <vector>

class Cli
{
  public:
  Cli(TransactionHandler* tranPtr, DataBaseIf* dataPtr);
  virtual ~Cli();

  enum operation{
    GET,
    CREATE,
    DEL,
    SET,
    UNKNOWN,
    PRINT};

  void start();

  private:
  void mainLoop();
  void printMenu();
  void readCommand();
  std::string handleRead(std::vector<std::string>&);
  std::string handleCreate(std::vector<std::string>&);
  std::string handleDelete(std::vector<std::string>&);
  std::string handleSet(std::vector<std::string>&);
  std::string handlePrint(std::vector<std::string>&);
  operation validateAndTokenizeInput(std::string&, std::vector<std::string>&, std::string&);
  bool validateMO(std::vector<std::string>&, std::string&);

  TransactionHandler* transactionHandlerPtr;
  DataBaseIf* dataBasePtr;

};


#endif /* CLI_H_ */
