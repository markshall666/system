/*
 * cmdDaemon.cpp
 *
 *  Created on: 08-07-2017
 *      Author: marek
 */
#include "cmdDaemon.h"
#include "itc.h"
#include "trace.h"
#include "messages.h"
#include "unistd.h"
#include "stdlib.h"
#include "signal.h"


void sigHandler(int signo)
{
  terminateItc(&itcId);
  abort();
}


int main(int argc, const char** argv)
{
  TRACE_INIT(argc, argv);
  signal(SIGINT, sigHandler);

  if (!initItc(CMD_DAEMON, &itcId))
  {
    TRACE_ERROR("Could not run communication, existing...");
    return 0;
  }

  while (1)
  {
    union itcMsg* msg = receiveData();
    if (msg->msgNo == CMD_REGISTER_REQ)
    {
      int sender = (int)getSenderTId(msg);
      TRACE_DEBUG("Registering command %s, tId = 0x%x", msg->cmdRegisterReq.cmd, sender);
      std::pair<std::map<std::string, unsigned>::iterator, bool> ret;
      ret = cmdList.insert(std::pair<std::string, unsigned>(msg->cmdRegisterReq.cmd, sender));
      if (ret.second)
      {
	char* dir = get_current_dir_name();
	char* shell = (char*)malloc(strlen(dir) + 10);
	char* link = (char*)malloc(strlen(dir) + strlen(msg->cmdRegisterReq.cmd) + 1);
	strcpy(shell, dir);
	strcat(shell, "/");
	strcat(shell, "cmdShell");
	strcpy(link, dir);
	strcat(link, "/");
	strcat(link, msg->cmdRegisterReq.cmd);
	TRACE_DEBUG("Creating symlink %s %s", shell, link);
	symlink(shell, link);
	free(shell);
	free(link);
      }
      else
      {
        TRACE_ERROR("Command %s already registered", msg->cmdRegisterReq.cmd);
      }
    }
    else if (msg->msgNo == CMD_CHECK_REQ)
    {
      TRACE_DEBUG("Received CMD_CHECK_REQ with %s", msg->cmdCheckReq.cmd);
      std::map<std::string, unsigned>::iterator ret = cmdList.find(msg->cmdCheckReq.cmd);
      if (ret != cmdList.end())
      {
	union itcMsg* msg = itcAlloc(sizeof(CmdCheckRspS), CMD_CHECK_RSP);
	msg->cmdCheckRsp.tId = ret->second;
	sendData(CMD_SHELL, msg);
      }
      else
      {
        union itcMsg* msg = itcAlloc(sizeof(CmdCheckRspS), CMD_CHECK_RSP);
        msg->cmdCheckRsp.tId = 0;
        sendData(CMD_SHELL, msg);
      }
    }
    else
    {
      TRACE_ERROR("Unknown message = 0x%x", msg->msgNo);
    }
    itcFree(msg);
  }
}
