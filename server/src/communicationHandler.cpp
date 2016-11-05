/*
 * communicationHandler.cpp
 *
 *  Created on: 23-10-2016
 *      Author: marek
 */

#include "communicationHandler.h"
#include "pthread.h"
#include "messages.h"
#include "itc.h"
#include <stdio.h>
#include <stdlib.h>
#include "trace.h"
#include "poll.h"

bool CommunicationHandler::newEvent(false);

CommunicationHandler::CommunicationHandler(std::map<pthread_t, std::string>* mapPtr, TransactionHandler* tranHandler)
{
  appMapPtr = mapPtr;
  tranHandlerPtr = tranHandler;
  //newEvent = false;
}

CommunicationHandler::~CommunicationHandler()
{}

void CommunicationHandler::init()
{
  pthread_t tId;
  pthread_create(&tId, NULL, mainLoop, (void*)this);
  TRACE_DEBUG("Starting communication thread, tId = 0x%x", (unsigned int)tId);
}

void* CommunicationHandler::mainLoop(void* ptr)
{
  CommunicationHandler* handlerPtr = (CommunicationHandler*)ptr;
  int fd;
  //void* buf = malloc(16);

  if (!initItc(NAME, &fd))
  {
    return NULL;
  }

  struct pollfd fds;
  fds.fd = fd;
  fds.events = POLLIN;

  while (true)
  {
    poll(&fds, 1, 1);
    if (fds.revents == POLLIN)
    {
      union itcMsg* msg = receiveData();
      TRACE_MSG(msg);
      switch (msg->msgNo)
      {
	case REGISTER_APP_REQ:
	{
	  handlerPtr->handleRegisterAppReq(msg);
	  break;
	}
	case UPDATE_MO_REQ:
	{
	  handlerPtr->handleUpdateMoReq(msg);
	  break;
	}
	case CREATE_MO_CFM:
        {
          handlerPtr->handleCreateMoCfm(msg);
          break;
        }
	case CREATE_MO_REJ:
        {
          handlerPtr->handleCreateMoRej(msg);
          break;
        }
	case COMPLETED_MO_CFM:
        {
          handlerPtr->handleCompletedMoCfm(msg);
          break;
        }
	case COMPLETED_MO_REJ:
        {
          handlerPtr->handleCompletedMoRej(msg);
          break;
        }
	case APPLY_MO_CFM:
        {
          handlerPtr->handleApplyMoCfm(msg);
          break;
        }
	default:
	{
	  TRACE_ERROR("Received unknown message, msgNo = 0x%x", msg->msgNo);
	}
      }
    }
    else if (newEvent)
    {
      handlerPtr->handleEvent();
    }
  }
  return NULL;
}

void CommunicationHandler::handleRegisterAppReq(union itcMsg* msgIn)
{
  TRACE_ENTER;
  char* client = (char*)malloc(16);
  strcpy(client, msgIn->registerAppReq.appName);
  appMapPtr->insert(std::pair<pthread_t, std::string>(getSenderTId(msgIn),client));
  itcFree(msgIn);
  union itcMsg* msg = itcAlloc(sizeof(RegisterAppCfmS), REGISTER_APP_CFM);
  msg->registerAppCfm.result = true;
  strcpy(msg->registerAppReq.appName, client);
  sendData(client, msg);
  free(client);
}

void  CommunicationHandler::handleUpdateMoReq(union itcMsg* msgIn)
{
  TRACE_ENTER;
  //here database write will be called
  std::map<pthread_t, std::string>::iterator it = appMapPtr->find(getSenderTId(msgIn));
  itcFree(msgIn);
  union itcMsg* msg = itcAlloc(sizeof(UpdateMoCfmS), UPDATE_MO_CFM);
  sendData(it->second.c_str(), msg);
}

void CommunicationHandler::handleCreateMoCfm(union itcMsg* msgIn)
{
  TRACE_ENTER;
  itcFree(msgIn);
  tranHandlerPtr->requestChangeState(TransactionHandler::COMPLETE);
}

void CommunicationHandler::handleCreateMoRej(union itcMsg* msgIn)
{
  TRACE_ENTER;
  tranHandlerPtr->requestChangeState(TransactionHandler::ABORT);
  itcFree(msgIn);
}

void CommunicationHandler::handleCompletedMoCfm(union itcMsg* msgIn)
{
  TRACE_ENTER;
  itcFree(msgIn);
  tranHandlerPtr->requestChangeState(TransactionHandler::APPLY);
}

void CommunicationHandler::handleCompletedMoRej(union itcMsg* msgIn)
{
  TRACE_ENTER;
  tranHandlerPtr->setErrorStr(msgIn->completedMoRej.error);
  tranHandlerPtr->requestChangeState(TransactionHandler::ABORT);
  itcFree(msgIn);
}

void CommunicationHandler::handleApplyMoCfm(union itcMsg* msgIn)
{
  TRACE_ENTER;
  itcFree(msgIn);
  tranHandlerPtr->requestChangeState(TransactionHandler::IDLE);
}

void CommunicationHandler::handleEvent()
{
  std::vector<std::string>* data = (std::vector<std::string>*)txData;

  switch (txState)
  {
    case TransactionHandler::CREATE:
    {
      union itcMsg* msg = itcAlloc(sizeof(CreateMoReqS), CREATE_MO_REQ);
      msg->createMoReq.txId = txId;
      msg->createMoReq.objectId = objectId;
      strcpy(msg->createMoReq.moName, data->at(1).c_str());
      strcpy(msg->createMoReq.attr0name, data->at(2).c_str());
      strcpy(msg->createMoReq.attr0value, data->at(3).c_str());
      sendData("kotek", msg);
      break;
    }
    case TransactionHandler::COMPLETE:
    {
      union itcMsg* msg = itcAlloc(sizeof(CompletedMoReqS), COMPLETED_MO_REQ);
      msg->completedMoReq.txId = txId;
      msg->completedMoReq.objectId = objectId;
      strcpy(msg->completedMoReq.moName, data->at(1).c_str());
      sendData("kotek", msg);
      break;
    }
    case TransactionHandler::APPLY:
    {
      union itcMsg* msg = itcAlloc(sizeof(ApplyMoReqS), APPLY_MO_REQ);
      msg->applyMoReq.txId = txId;
      msg->applyMoReq.objectId = objectId;
      strcpy(msg->applyMoReq.moName, data->at(1).c_str());
      sendData("kotek", msg);
      break;
    }
    default:
    {
      TRACE_ERROR("state not handled yet");
    }
  }
  newEvent = false;
}

void CommunicationHandler::triggerEvent(uint8_t inTxId, TransactionHandler::transactionState state, unsigned obId, void* data)
{
  txId = inTxId;
  txState = state;
  objectId = obId;
  txData = data;
  newEvent = true;
}
