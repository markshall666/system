/*
 * cmdShell.cpp
 *
 *  Created on: 08-07-2017
 *      Author: marek
 */

#include "stdio.h"
#include <string.h>
#include "cmdShell.h"
#include "itc.h"
#include "messages.h"
#include "trace.h"


int main(int argc, char* argv[])
{
  TRACE_INIT(argc, argv);
  if (strstr(argv[0], "cmdShell"))
    {
      TRACE_ERROR("Could not run command directly from cmdShell!");
      return 0;
    }

  if (!initItc(CMD_SHELL, &itcId))
  {
      TRACE_ERROR("Could not run communication, existing...");
    return 0;
  }

  union itcMsg* msg = itcAlloc(sizeof(CmdCheckReqS), CMD_CHECK_REQ);
  strcpy(msg->cmdCheckReq.cmd, argv[0] + 2);
  if (!sendData(CMD_DAEMON, msg))
  {
    TRACE_ERROR("Could not run communication, existing...");
    terminateItc(&itcId);
    return 0;
  }

  msg = receiveData();

  uint32_t receiverTid = msg->cmdCheckRsp.tId;
  itcFree(msg);
  msg = NULL;

  if (receiverTid)
  {
    msg = itcAlloc(sizeof(CmdExecuteReqS) + argc*sizeof(char*) + argc*sizeof(CmdExecuteReqS::cmd), CMD_EXECUTE_REQ);
    strcpy(msg->cmdExecuteReq.cmd, argv[0] + 2);
    for (int i = 0; i < argc; i++)
    {
      msg->cmdExecuteReq.argv[i] = (char*)msg->cmdExecuteReq.argv + argc * sizeof(char*) + (i * sizeof(CmdExecuteReqS::cmd));
      strcpy(msg->cmdExecuteReq.argv[i], argv[i]);
    }

    msg->cmdExecuteReq.argc = argc;
    char receiver[8];
    sprintf(receiver, "%x", receiverTid);
    if (!sendData(receiver, msg))
    {
      TRACE_ERROR("Could not run command, existing...");
      terminateItc(&itcId);
      return 0;
    }

    msg = receiveData();
    printf("%s\n", msg->cmdExecuteRsp.output);
    itcFree(msg);
  }
  else
  {
    TRACE_ERROR("No such command receiver, exiting...");
  }

  terminateItc(&itcId);
}
