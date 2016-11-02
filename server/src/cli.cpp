/*
 * cli.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "cli.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "trace.h"

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
  cout << "print <MO name> print MO(s)\n";
  cout << "Enter command:\n";
}

void Cli::readCommand()
{
  string input;
  vector<string> arg;
  string errorMsg;
  getline(cin, input);

  switch (validateAndTokenizeInput(input, arg, errorMsg))
  {
    case GET:
    {
      cout << handleRead(arg);
      break;
    }
    case CREATE:
    {
      cout << handleCreate(arg);
      break;
    }
    case DEL:
    {
      cout << handleDelete(arg);
      break;
    }
    case SET:
    {
      cout << handleSet(arg);
      break;
    }
    case PRINT:
    {
      cout << handlePrint(arg);
      break;
    }
    case UNKNOWN:
    {
      if (errorMsg.empty())
      {
        cout << "Unrecognized command, type again\n";
      }
      else
      {
        cout << errorMsg.c_str() << "\n";
      }
      printMenu();
      break;
    }
  }
}

string Cli::handleRead(vector<string>& attr)
{
  vector<string> result = dataBasePtr->getMO(attr);
  if (!result.empty())
  {
    return attr[1] + " " + result[0] + " = " + result[1] + "\n";
  }
  else
  {
    return "no such MO\n";
  }
}

string Cli::handleCreate(vector<string>& attr)
{
  //here should be transactionHandler call
  string errorStr;
  if (validateMO(attr, errorStr))
  {
    handleDefaults(attr);
    unsigned objectId = dataBasePtr->getMaxId();
    if (transactionHandlerPtr->handleCreate(objectId, attr))
    {
      dataBasePtr->addMO(attr, objectId + 1);
      return "ok\n";
    }
    else
    {
      return "cannot create MO\n";
    }
  }
  else
  {
    return errorStr;
  }
}

string Cli::handleDelete(vector<string>& attr)
{
  vector<string> result = dataBasePtr->printMO(attr);
  if (!result.empty())
  {
    if (dataBasePtr->deleteMO(attr))
    {
      return "ok\n";
    }
    else
    {
      return "cannot delete MO\n";
    }
  }
  else
  {
    return "no such MO\n";
  }
}

string Cli::handleSet(vector<string>& attr)
{
  if (dataBasePtr->modifyMO(attr))
  {
    return "ok\n";
  }
  else
  {
    return "cannot set MO\n";
  }
}

string Cli::handlePrint(vector<string>& attr)
{
  vector<string> result = dataBasePtr->printMO(attr);
  stringstream ss;
  for (unsigned i = 0; i < result.size(); ++i)
  {
    ss << result[i] << "\n";
  }
  return ss.str();
}

Cli::operation Cli::validateAndTokenizeInput(string& in, vector<string>& out, string& errorMsg)
{
  operation op = UNKNOWN;
  string buf;
  stringstream ss(in);
  while (ss >> buf) out.push_back(buf);

  if (out.size() >= 2)
  {
    if (out[0].find("get") != string::npos)
    {
      op = GET;
    }
    else if (out[0].find("create") != string::npos)
    {
      op = CREATE;
    }
    else if (out[0].find("delete") != string::npos)
    {
      op = DEL;
    }
    else if (out[0].find("set") != string::npos)
    {
      op = SET;
    }
    else if (out[0].find("print") != string::npos)
    {
      op = PRINT;
    }
    else
    {
      op = UNKNOWN;
    }
  }
  else if (out.size() > 0 && out[0].find("print") != string::npos)
  {
    op = PRINT;
  }
  else
  {
    errorMsg = "Too few arguments";
  }

  if (op == SET && out.size() != 4)
  {
    errorMsg = "Too few arguments";
    op = UNKNOWN;
  }

  return op;
}

bool Cli::validateMO(vector<string>& in, string& errorStr)
{
  //temp hardcoded checking MO type
  if (in[1].find("kotek=") == string::npos)
  {
    errorStr = "Wrong MO type!\n";
    return false;
  }
  vector<string> result = dataBasePtr->printMO(in);
  if (result.empty())
  {
    return true;
  }
  else
  {
    errorStr = "MO " + in[1] + " already exist!\n";
    return false;
  }
}

void Cli::handleDefaults(vector<string>& in)
{
  if (in.size() <= 2)
  {
    //temp hardcoded values
    in.push_back("val");
    in.push_back("0");
  }
}
