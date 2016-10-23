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

CommunicationHandler::CommunicationHandler(std::map<pthread_t, std::string>* mapPtr, TransactionHandler* tranHandler)
{
  appMapPtr = mapPtr;
  tranHandlerPtr = tranHandler;
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

  while (true)
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
      default:
      {
	TRACE_ERROR("Received unknown message, msgNo = 0x%x", msg->msgNo);
      }
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
