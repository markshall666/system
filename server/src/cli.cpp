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
#include "moTypes.h"

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

  switch (tokenizeAndCheckOperation(input, arg, errorMsg))
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
  /*vector<string> result = dataBasePtr->getMO(attr);
  if (!result.empty())
  {
    return attr[1] + " " + result[0] + " = " + result[1] + "\n";
  }
  else
  {
    return "no such MO\n";
  }*/
  return "ok\n";
}

string Cli::handleCreate(vector<string>& attr)
{
  string errorStr;
  bool result = true;
  int type = validateAndCheckType(attr, errorStr);
  if (type != -1)
  {
    unsigned objectId = dataBasePtr->getMaxId();
    if (transactionHandlerPtr->handleCreate(objectId+1, attr, errorStr))
    {
      vector<string> attrs, vals;
      if (attr.size() == 4)
      {
	attrs.push_back(attr[2]);
	vals.push_back(attr[3]);
	if (!dataBasePtr->addMO(objectId+1, type, attr[1], attrs, vals))
	{
	  result = false;
	}
      }
    }
    else
    {
      result = false;
    }
  }
  else
  {
    errorStr = "Wrong MO type!\n";
    result = false;
  }

  if (result)
  {
    return "ok\n";
  }
  else
  {
    return "cannot create MO: " + errorStr + "\n";
  }
}

string Cli::handleDelete(vector<string>& attr)
{
  vector<string> result;
  dataBasePtr->printMO(attr[1], result);
  if (!result.empty())
  {
    unsigned objectId = dataBasePtr->getObjectId(0x77700001, attr[1]);
    string errorStr;
    if (transactionHandlerPtr->handleDelete(objectId, attr, errorStr))
    {
      dataBasePtr->deleteMO(objectId, 0x77700001, attr[1]);
      return "ok\n";
    }
    else
    {
      return "cannot delete MO: " + errorStr + "\n";
    }
  }
  else
  {
    return "no such MO\n";
  }
}

string Cli::handleSet(vector<string>& attr)
{
  /*vector<string> result = dataBasePtr->printMO(attr);
  if (!result.empty())
  {
    unsigned objectId = dataBasePtr->getObjectId(attr[1]);
    string errorStr;
    if (transactionHandlerPtr->handleModify(objectId, attr, errorStr))
    {
      dataBasePtr->modifyMO(attr);
      return "ok\n";
    }
    else
    {
      return "cannot set MO: " + errorStr + "\n";
    }
  }
  else
  {
    return "no such MO\n";
  }*/
  vector<string> a, b;
  a.push_back("attr1");
  a.push_back("attr2");
  b.push_back("777");
  b.push_back("888");
  string name("miau");
  dataBasePtr->modifyMO(1, 2003828737, name, a, b);
  return "ok\n";
}

string Cli::handlePrint(vector<string>& attr)
{
  /*vector<string> result = dataBasePtr->printMO(attr);
  stringstream ss;
  for (unsigned i = 0; i < result.size(); ++i)
  {
    ss << result[i] << "\n";
  }
  return ss.str();*/
  return "ok\n";
}

Cli::operation Cli::tokenizeAndCheckOperation(string& in, vector<string>& out, string& errorMsg)
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

  if (op == SET && out.size() < 4)
  {
    errorMsg = "Too few arguments";
    op = UNKNOWN;
  }

  return op;
}

int Cli::validateAndCheckType(vector<string>& in, string& errorStr)
{
  size_t equal = in[1].find("=");
  if (equal != string::npos)
  {
    return getMoTypeByName(in[1].substr(0, equal).c_str());
  }
  else
  {
    errorStr = "MO should be named with <type>=<name>\n";
    return -1;
  }
  /*vector<string> result = dataBasePtr->printMO(in);
  if (result.empty())
  {
    return true;
  }
  else
  {
    errorStr = "MO " + in[1] + " already exist!\n";
    return false;
  }*/
}

void Cli::handleDefaults(vector<string>& in)
{
  if (in.size() <= 2)
  {
    //temp hardcoded values
    in.push_back("attr1");
    in.push_back("0");
  }
}
