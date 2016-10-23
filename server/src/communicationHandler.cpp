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

CommunicationHandler::CommunicationHandler()
{
}

CommunicationHandler::~CommunicationHandler()
{}

void CommunicationHandler::init()
{
  pthread_t tId;
  pthread_create(&tId, NULL, mainLoop, NULL);
  TRACE_DEBUG("Starting communication thread, tId = 0x%x", (unsigned int)tId);
}

void* CommunicationHandler::mainLoop(void* arg)
{
  int fd;
  void* buf = malloc(16);
  char* client = (char*)malloc(16);

  if (!initItc(NAME, &fd))
  {
    return NULL;
  }

  while (true)
  {
    union itcMsg* msg = receiveData();
    TRACE_MSG(msg);
    if (msg->msgNo == REGISTER_APP_REQ)
    {
      strcpy(client, msg->registerAppReq.appName);
      itcFree(msg);
      union itcMsg* msg = itcAlloc(sizeof(RegisterAppCfmS), REGISTER_APP_CFM);
      msg->registerAppCfm.result = true;
      strcpy(msg->registerAppReq.appName, client);
      sendData(client, msg);
    }
  }
  return NULL;
}
