/*
 * cmdAppClient.cpp
 *
 *  Created on: 08-07-2017
 *      Author: marek
 */

#include "cmdAppClient.h"
#include "itc.h"
#include "stdio.h"
#include "messages.h"
#include <string.h>
#include "unistd.h"

bool CmdAppClient::loop(true);
CmdAppClient::Choice CmdAppClient::choice(IDLE);

CmdAppClient::CmdAppClient()
{
  pthread_create(&tId, NULL, mainLoop, NULL);
  char tIdChar[8];
  sprintf(tIdChar, "%x", (unsigned)tId);
  if (!initItc(tIdChar, &itcId))
  {
    printf("Could not run communication!\n");
    loop = false;
  }
}

CmdAppClient::~CmdAppClient()
{
  terminateItc(&itcId);
  loop = false;
}

bool CmdAppClient::registerCmd(std::string cmd, char* (*handler)(void))
{
  cmdList.push_back(std::pair<std::string, cmdFunc>(cmd, handler));
  choice = CmdAppClient::REGISTER;

  return true;
}

bool CmdAppClient::start()
{
  choice = CmdAppClient::LISTEN;
  return true;
}

void* CmdAppClient::mainLoop(void* arg)
{
  while(loop)
  {
    switch (choice)
    {
      case REGISTER:
      {
	union itcMsg* msg = itcAlloc(sizeof(CmdRegisterReqS), CMD_REGISTER_REQ);
	strcpy(msg->cmdRegisterReq.cmd, cmdList.back().first.c_str());
	if (!sendData(CMD_DAEMON, msg))
	{
	  printf("Could not register command\n");
	}
	choice = CmdAppClient::IDLE;
	break;
      }
      case LISTEN:
      {
        union itcMsg* msg = receiveData();
        for (std::vector<std::pair<std::string, cmdFunc> >::iterator it = cmdList.begin(); it != cmdList.end(); ++it)
        {
          if (strstr(it->first.c_str(), msg->cmdExecuteReq.cmd))
          {
            itcFree(msg);
            msg = itcAlloc(sizeof(CmdExecuteRspS), CMD_EXECUTE_RSP);
            strcpy(msg->cmdExecuteRsp.output, it->second());
    	    if (!sendData(CMD_SHELL, msg))
    	    {
    	      printf("Could not send command output\n");
    	    }
          }
        }
        choice = CmdAppClient::IDLE;
	break;
      }
      case IDLE:
      {
        sleep(1);
      }
    }
  }
  return NULL;
}
