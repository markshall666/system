/*
 * cli.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "cli.h"
#include <iostream>

using namespace std;

Cli::Cli(TransactionHandler* tranPtr, DataBaseIf* dataPtr)
{
  transactionHandlerPtr = tranPtr;
  dataBasePtr = dataPtr;
}

Cli::~Cli()
{}

void Cli::start()
{
  mainLoop();
}

void Cli::mainLoop()
{
  cout << "Welcome in Marek's transaction server, enter command\n";
  printMenu();
  while(1)
  {
    readCommand();
  }
}

void Cli::printMenu()
{
  cout << "get <MO name> read MO data\n";
  cout << "create <MO name> create MO\n";
  cout << "delete <MO name> delete MO\n";
  cout << "set <MO name> <attribute> <value> set attribute in MO\n";
  cout << "Enter command:\n";
}

void Cli::readCommand()
{
  string input;
  getline(cin, input);
  if (input.find("get") != string::npos)
  {
    cout << handleRead(input);
  }
  else if (input.find("create") != string::npos)
  {
    cout << handleCreate(input);
  }
  else if (input.find("delete") != string::npos)
  {
    cout << handleDelete(input);
  }
  else if (input.find("set") != string::npos)
  {
    cout << handleSet(input);
  }
  else
  {
    cout << "Unrecognized command, type again\n";
    printMenu();
  }
}

string Cli::handleRead(string command)
{
  void* mo = dataBasePtr->getMO(command.substr(4));
  if (mo)
  {
    //temp hack
    string result = *(string*)mo + "\n";
    delete (string*)mo;
    return result;
  }
  else
  {
    return "no such MO\n";
  }
}

string Cli::handleCreate(string command)
{
  string mo = command.substr(7);
  //here should be transactionHandler call
  if (dataBasePtr->addMO(mo, NULL))
  {
    return "ok\n";
  }
  else
  {
    return "cannot create MO\n";
  }
}

string Cli::handleDelete(string command)
{
  if (dataBasePtr->deleteMO(command.substr(7)))
  {
    return "ok\n";
  }
  else
  {
    return "cannot delete MO\n";
  }
}

string Cli::handleSet(string command)
{
  if (dataBasePtr->modifyMO(command.substr(4), NULL))
  {
    return "ok\n";
  }
  else
  {
    return "cannot set MO\n";
  }
}
