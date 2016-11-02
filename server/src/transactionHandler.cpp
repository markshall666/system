/*
 * transactionHandler.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "transactionHandler.h"
#include "communicationHandler.h"
#include "trace.h"

using namespace std;

TransactionHandler::TransactionHandler(DataBaseIf* dataBasePtrIn):
comPtr(NULL),
txState(IDLE),
ongoingTxId(0)
{
  dataBasePtr = dataBasePtrIn;
  srand(time(NULL));
}

TransactionHandler::~TransactionHandler()
{}

bool TransactionHandler::handleCreate(unsigned objectId, vector<string>& mo)
{
  startTransaction();
  txState = CREATE;
  comPtr->triggerEvent(ongoingTxId, CREATE, objectId, (void*)&mo);
  if (wait())
  {
    comPtr->triggerEvent(ongoingTxId, COMPLETE, objectId, (void*)&mo);
    if (wait())
    {
      comPtr->triggerEvent(ongoingTxId, APPLY, objectId, (void*)&mo);
      if (wait())
      {
        return true;
      }
      else
      {
        txState = IDLE;
        return true;
      }
    }
    else
    {
      comPtr->triggerEvent(ongoingTxId, ABORT, objectId, NULL);
      return false;
    }
  }
  else
  {
    comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)&mo);
    return false;
  }
}

bool TransactionHandler::handleModify(std::string name, void* attr)
{
  return true;
}

bool TransactionHandler::handleDelete(std::string name)
{
  return true;
}

void TransactionHandler::setCommunicationHandlerPtr(CommunicationHandler* inPtr)
{
  comPtr = inPtr;
}

void TransactionHandler::startTransaction()
{
  ongoingTxId = rand() % 100;
  TRACE_DEBUG("Transaction started, txId = %d", ongoingTxId);
}

void TransactionHandler::changeState(transactionState state, void* data)
{
  switch (txState)
  {
    case IDLE:
    {
      switch (state)
      {
	case CREATE:
	{
	  TRACE_DEBUG("state CREATE");
	  //txState = CREATE;

	  //handle create
	  break;
	}
	case MODIFY:
	{
	  txState = MODIFY;
	  //handle modify
	  break;
	}
	case DELETE:
	{
	  txState = DELETE;
	  //handle delete
	  break;
	}
	default:
	{
	  //wrong state
	  break;
	}
      }
      break;
    }
    case CREATE:
    {
      switch (state)
      {
      	case COMPLETE:
      	{
      	  txState = COMPLETE;
      	  TRACE_DEBUG("state COMPLETE");
      	  //handle complete
      	  break;
      	}
      	case ABORT:
      	{
      	  txState = ABORT;
      	  //handle abort
      	  break;
      	}
	default:
	{
	  //wrong state
	  break;
	}
      }
      break;
    }
    case MODIFY:
    {
      switch (state)
      {
	case COMPLETE:
	{
	  txState = COMPLETE;
	  //handle complete
	  break;
	}
	case ABORT:
	{
	  txState = ABORT;
	  //handle abort
	  break;
	}
	default:
	{
	  //wrong state
	  break;
	}
      }
      break;
    }
    case DELETE:
    {
      switch (state)
      {
	case COMPLETE:
	{
	  txState = COMPLETE;
	  //handle complete
	  break;
	}
	case ABORT:
	{
	  txState = ABORT;
	  //handle abort
	  break;
	}
	default:
	{
	  //wrong state
	  break;
	}
      }
      break;
    }
    case COMPLETE:
    {
      switch (state)
      {
	case APPLY:
	{
	  txState = APPLY;
	  TRACE_DEBUG("state APPLY");
	  //handle apply
	  break;
	}
	case ABORT:
	{
	  txState = ABORT;
	  //handle abort
	  break;
	}
	default:
	{
	  //wrong state
	  break;
	}
      }
      break;
    }
    case APPLY:
    {
      switch (state)
      {
	case IDLE:
	{
	  txState = IDLE;
	  TRACE_DEBUG("state IDLE");
	  //handle idle
	  break;
	}
	default:
	{
	  //wrong state
	  break;
	}
      }
      break;
    }
    case ABORT:
    {
      switch (state)
      {
    	case IDLE:
    	{
    	txState = IDLE;
    	  //handle idle
    	  break;
    	}
    	default:
    	{
    	  //wrong state
    	  break;
    	}
      }
      break;
    }
  }
}

bool TransactionHandler::wait()
{
  transactionState tempTxstate = txState;
  for (unsigned i = 0; i < 10000; ++i)
  {
    if (tempTxstate != txState)
    {
      return true;
    }
    usleep(1000);
  }
  return false;
}

void TransactionHandler::setTransactionState(transactionState state)
{
  txState = state;
}
