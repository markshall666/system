/*
 * transactionHandler.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "transactionHandler.h"
#include "marekObject.h"

TransactionHandler::TransactionHandler(DataBaseIf* dataBasePtrIn)
{
  dataBasePtr = dataBasePtrIn;
}

TransactionHandler::~TransactionHandler()
{}

bool TransactionHandler::handleCreate(std::string name, void* attr)
{
  return true;
}

bool TransactionHandler::handleModify(std::string name, void* attr)
{
  return true;
}

bool TransactionHandler::handleDelete(std::string name)
{
  return true;
}
