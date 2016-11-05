/*
 * transactionHandler.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include "transactionHandler.h"
#include "communicationHandler.h"
#include "trace.h"

using namespace std;

TransactionHandler::TransactionHandler(DataBaseIf* dataBasePtrIn):
comPtr(NULL),
txState(IDLE),
requestedTxState(IDLE),
ongoingTxId(0),
txData(NULL),
objectId(0),
txResult(false)
{
  dataBasePtr = dataBasePtrIn;
  srand(time(NULL));

  pthread_t tId;
  pthread_create(&tId, NULL, startLoop, (void*)this);
  TRACE_DEBUG("Starting transaction thread, tId = 0x%x", (unsigned int)tId);
}

TransactionHandler::~TransactionHandler()
{}

void* TransactionHandler::startLoop(void* ptr)
{
  TransactionHandler* objectPtr = (TransactionHandler*)ptr;
  objectPtr->mainLoop();
  return NULL;
}

void TransactionHandler::mainLoop()
{
  while(1)
  {
    while(txState == requestedTxState);
    switch (requestedTxState)
    {
      case CREATE:
      {
        startTransaction();
	txState = CREATE;
	TRACE_DEBUG("TxState = CREATE");
	comPtr->triggerEvent(ongoingTxId, CREATE, objectId, (void*)txData);
	if (waitTimeOut())
        {
          switch (requestedTxState)
          {
            case COMPLETE:
            {
              txState = COMPLETE;
              TRACE_DEBUG("TxState = COMPLETE");
              comPtr->triggerEvent(ongoingTxId, COMPLETE, objectId, (void*)txData);
              if (waitTimeOut())
              {
                switch (requestedTxState)
                {
                  case APPLY:
                  {
                    txState = APPLY;
                    TRACE_DEBUG("TxState = APPLY");
                    comPtr->triggerEvent(ongoingTxId, APPLY, objectId, (void*)txData);
                    if (waitTimeOut())
                    {
                      switch (requestedTxState)
                      {
                	case IDLE:
                        {
                          txResult = true;
                          break;
                        }
                	default:
                        {
                          TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
                          txResult = true;
                        }
                      }
                    }
                    else
                    {
                      TRACE_ERROR("Transaction timeout in state %s", getTxStateString(txState));
                    }
                    break;
                  }
                  case ABORT:
                  {
                    txState = ABORT;
                    TRACE_DEBUG("TxState = ABORT");
                    comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
                    break;
                  }
                  default:
                  {
                    TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
                    break;
                  }
                }
              }
              else
              {
                TRACE_ERROR("Transaction timeout in state %s, aborting", getTxStateString(txState));
                txState = ABORT;
                comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
              }
              break;
            }
            case ABORT:
            {
              txState = ABORT;
              TRACE_DEBUG("TxState = ABORT");
              comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
              break;
            }
            default:
            {
              TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
              break;
            }
          }
        }
	else
        {
          TRACE_ERROR("Transaction timeout in state %s, aborting", getTxStateString(txState));
          txState = ABORT;
          comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
        }
        txState = IDLE;
        requestedTxState = IDLE;
        ongoingTxId = 0;
        TRACE_DEBUG("TxState = IDLE");
        break;
      }
      case MODIFY:
      {
        startTransaction();
      	txState = MODIFY;
      	TRACE_DEBUG("TxState = MODIFY");
      	comPtr->triggerEvent(ongoingTxId, MODIFY, objectId, (void*)txData);
      	if (waitTimeOut())
        {
	  switch (requestedTxState)
	  {
	    case COMPLETE:
	    {
	      txState = COMPLETE;
	      TRACE_DEBUG("TxState = COMPLETE");
	      comPtr->triggerEvent(ongoingTxId, COMPLETE, objectId, (void*)txData);
	      if (waitTimeOut())
	      {
		switch (requestedTxState)
		{
		  case APPLY:
		  {
		    txState = APPLY;
		    TRACE_DEBUG("TxState = APPLY");
		    comPtr->triggerEvent(ongoingTxId, APPLY, objectId, (void*)txData);
		    if (waitTimeOut())
		    {
		      switch (requestedTxState)
		      {
		        case IDLE:
			{
			  txResult = true;
			  break;
			}
		        default:
			{
			  TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
			  txResult = true;
			}
		      }
		    }
		    else
		    {
		      TRACE_ERROR("Transaction timeout in state %s", getTxStateString(txState));
		    }
		    break;
		  }
		  case ABORT:
		  {
		    txState = ABORT;
		    TRACE_DEBUG("TxState = ABORT");
		    comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
		    break;
		  }
		  default:
		  {
		    TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
		    break;
		  }
		}
	      }
	      else
	      {
		TRACE_ERROR("Transaction timeout in state %s, aborting", getTxStateString(txState));
		txState = ABORT;
		comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
	      }
	      break;
	    }
	    case ABORT:
	    {
	      txState = ABORT;
	      TRACE_DEBUG("TxState = ABORT");
	      comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
	      break;
	    }
	    default:
	    {
	      TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
	      break;
	    }
	  }
	}
      	else
        {
	  TRACE_ERROR("Transaction timeout in state %s, aborting", getTxStateString(txState));
	  txState = ABORT;
	  comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
	}
	txState = IDLE;
	requestedTxState = IDLE;
	ongoingTxId = 0;
	TRACE_DEBUG("TxState = IDLE");
	break;
      }
      case DELETE:
      {
        startTransaction();
      	txState = DELETE;
      	TRACE_DEBUG("TxState = DELETE");
      	comPtr->triggerEvent(ongoingTxId, DELETE, objectId, (void*)txData);
      	if (waitTimeOut())
	{
	  switch (requestedTxState)
	  {
	    case COMPLETE:
	    {
	      txState = COMPLETE;
	      TRACE_DEBUG("TxState = COMPLETE");
	      comPtr->triggerEvent(ongoingTxId, COMPLETE, objectId, (void*)txData);
	      if (waitTimeOut())
	      {
		switch (requestedTxState)
		{
		  case APPLY:
		  {
		    txState = APPLY;
		    TRACE_DEBUG("TxState = APPLY");
		    comPtr->triggerEvent(ongoingTxId, APPLY, objectId, (void*)txData);
		    if (waitTimeOut())
		    {
		      switch (requestedTxState)
		      {
		        case IDLE:
			{
			  txResult = true;
			  break;
			}
		        default:
			{
			  TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
			  txResult = true;
			}
		      }
		    }
		    else
		    {
		      TRACE_ERROR("Transaction timeout in state %s", getTxStateString(txState));
		    }
		    break;
		  }
		  case ABORT:
		  {
		    txState = ABORT;
		    TRACE_DEBUG("TxState = ABORT");
		    comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
		    break;
		  }
		  default:
		  {
		    TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
		    break;
		  }
		}
	      }
	      else
	      {
		TRACE_ERROR("Transaction timeout in state %s, aborting", getTxStateString(txState));
		txState = ABORT;
		comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
	      }
	      break;
	    }
	    case ABORT:
	    {
	      txState = ABORT;
	      TRACE_DEBUG("TxState = ABORT");
	      comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
	      break;
	    }
	    default:
	    {
	      TRACE_ERROR("Unsupported state: %s", getTxStateString(requestedTxState));
	      break;
	    }
	  }
	}
      	else
	{
	  TRACE_ERROR("Transaction timeout in state %s, aborting", getTxStateString(txState));
	  txState = ABORT;
	  comPtr->triggerEvent(ongoingTxId, ABORT, objectId, (void*)txData);
	}
	txState = IDLE;
	requestedTxState = IDLE;
	ongoingTxId = 0;
	TRACE_DEBUG("TxState = IDLE");
	break;
      }
      default:
      {
	TRACE_ERROR("Unsupported yet state: %s", getTxStateString(requestedTxState));
      }
    }
  }
}

bool TransactionHandler::handleCreate(unsigned inObjectId, vector<string>& mo, string& inErrorStr)
{
  objectId = inObjectId;
  txData = &mo;
  requestedTxState = CREATE;
  if (waitTransactionEnd())
  {
    inErrorStr = errorStr;
    return txResult;
  }
  else
  {
    errorStr = "Transaction timed out\n";
    return false;
  }
}

bool TransactionHandler::handleModify(unsigned inObjectId, vector<string>& mo, string& inErrorStr)
{
  objectId = inObjectId;
  txData = &mo;
  requestedTxState = MODIFY;
  if (waitTransactionEnd())
  {
    inErrorStr = errorStr;
    return txResult;
  }
  else
  {
    errorStr = "Transaction timed out\n";
    return false;
  }
}

bool TransactionHandler::handleDelete(unsigned inObjectId, vector<string>& mo, string& inErrorStr)
{
  objectId = inObjectId;
  txData = &mo;
  requestedTxState = DELETE;
  if (waitTransactionEnd())
  {
    inErrorStr = errorStr;
    return txResult;
  }
  else
  {
    errorStr = "Transaction timed out\n";
    return false;
  }
}

void TransactionHandler::setCommunicationHandlerPtr(CommunicationHandler* inPtr)
{
  comPtr = inPtr;
}

void TransactionHandler::startTransaction()
{
  ongoingTxId = rand() % 100;
  txResult = false;
  TRACE_DEBUG("Transaction started, txId = %d", ongoingTxId);
}

bool TransactionHandler::waitTimeOut()
{
  for (unsigned i = 0; i < 10000; ++i)
  {
    if (requestedTxState != txState)
    {
      return true;
    }
    usleep(1000);
  }
  return false;
}

bool TransactionHandler::waitTransactionEnd()
{
  unsigned i = 0;
  while (i < 30000)
  {
    if (txState != IDLE) break;
    usleep(1000);
    ++i;
  }
  while (i < 30000)
  {
    if (txState == IDLE) return true;
    usleep(1000);
    ++i;
  }
  return false;
}

void TransactionHandler::requestChangeState(transactionState state)
{
  requestedTxState = state;
}

void TransactionHandler::setErrorStr(const char* error)
{
  errorStr.clear();
  errorStr = error;
}

const char* TransactionHandler::getTxStateString(transactionState state)
{
  switch (state)
  {
    case IDLE: return "IDLE";
    case CREATE: return "CREATE";
    case MODIFY: return "MODIFY";
    case DELETE: return "DELETE";
    case ABORT: return "ABORT";
    case COMPLETE: return "COMPLETE";
    case APPLY: return "APPLY";
  }
  return "UNKNOWN";
}
