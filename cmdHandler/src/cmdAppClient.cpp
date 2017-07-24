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
#include "trace.h"

pthread_t CmdAppClient::tId(0);
int CmdAppClient::itcId(0);
bool CmdAppClient::loop(true);
std::vector<CmdAppClient::Choice> CmdAppClient::state;
std::vector<std::pair<std::string, cmdFunc> > CmdAppClient::cmdList;

CmdAppClient::CmdAppClient()
{
  state.push_back(CmdAppClient::INIT);
  pthread_create(&tId, NULL, mainLoop, NULL);
}

CmdAppClient::~CmdAppClient()
{
  state.push_back(CmdAppClient::TERMINATE);
  state.erase(state.begin());
  int temp;
  initItc("temp", &temp);
  union itcMsg* msg = itcAlloc(sizeof(CmdExecuteReqS), CMD_EXECUTE_REQ);
  memset(msg->cmdExecuteReq.cmd, 1, 16);
  char tIdChar[8];
  sprintf(tIdChar, "%x", (unsigned)tId);
  sendData(tIdChar, msg);
  pthread_join(tId, NULL);
  terminateItc(&temp);
}

bool CmdAppClient::registerCmd(std::string cmd, void (*handler)(std::string&, int, char**))
{
  TRACE_DEBUG("register cmd %s", cmd.c_str());
  cmdList.push_back(std::pair<std::string, cmdFunc>(cmd, handler));
  state.push_back(CmdAppClient::REGISTER);
  return true;
}

bool CmdAppClient::start()
{
  state.push_back(CmdAppClient::LISTEN);
  return true;
}

void* CmdAppClient::mainLoop(void* arg)
{
  while(loop)
  {
    switch (state.front())
    {
      case INIT:
      {
	TRACE_DEBUG("state init");
	char tIdChar[8];
	sprintf(tIdChar, "%x", (unsigned)tId);
	if (!initItc(tIdChar, &itcId))
	{
	  printf("Could not run communication!\n");
	  loop = false;
	}
	state.push_back(CmdAppClient::IDLE);
	state.erase(state.begin());
	break;
      }
      case REGISTER:
      {
	TRACE_DEBUG("state register");
	union itcMsg* msg = itcAlloc(sizeof(CmdRegisterReqS), CMD_REGISTER_REQ);
	strcpy(msg->cmdRegisterReq.cmd, cmdList.back().first.c_str());
	if (!sendData(CMD_DAEMON, msg))
	{
	  printf("Could not register command\n");
	}
	state.push_back(CmdAppClient::IDLE);
	state.erase(state.begin());
	break;
      }
      case LISTEN:
      {
	TRACE_DEBUG("state listen");
        union itcMsg* msg = receiveData();
        TRACE_DEBUG("Received command %s", msg->cmdExecuteReq.cmd);
        for (std::vector<std::pair<std::string, cmdFunc> >::iterator it = cmdList.begin(); it != cmdList.end(); ++it)
        {
          if (strstr(it->first.c_str(), msg->cmdExecuteReq.cmd))
          {
            std::string buf;
            for (unsigned i =0; i < msg->cmdExecuteReq.argc; i++)
            {
              msg->cmdExecuteReq.argv[i] = (char*)msg->cmdExecuteReq.argv +  msg->cmdExecuteReq.argc * sizeof(char*)
                                           + (i * sizeof(CmdExecuteReqS::cmd));
            }

            it->second(buf, msg->cmdExecuteReq.argc, msg->cmdExecuteReq.argv);
            itcFree(msg);
            msg = itcAlloc(sizeof(CmdExecuteRspS) + buf.size(), CMD_EXECUTE_RSP);
            strcpy(msg->cmdExecuteRsp.output, buf.c_str());
    	    if (!sendData(CMD_SHELL, msg))
    	    {
    	      printf("Could not send command output\n");
    	    }
          }
        }
	break;
      }
      case IDLE:
      {
	TRACE_DEBUG("state idle");
        sleep(1);
        state.push_back(CmdAppClient::IDLE);
        state.erase(state.begin());
        break;
      }
      case TERMINATE:
      {
	TRACE_DEBUG("state terminate");
	terminateItc(&itcId);
	state.clear();
	loop = false;
      }
    }
  }
  return NULL;
}
