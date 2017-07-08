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


int main(int argc, char* argv[])
{
  if (strstr(argv[0], "cmdShell"))
    {
      printf("Could not run command directly from cmdShell!\n");
      return 0;
    }

  if (!initItc(CMD_SHELL, &itcId))
  {
    printf("Could not run communication, existing...\n");
    return 0;
  }

  union itcMsg* msg = itcAlloc(sizeof(CmdCheckReqS), CMD_CHECK_REQ);
  strcpy(msg->cmdCheckReq.cmd, argv[0]);
  if (!sendData(CMD_DAEMON, msg))
  {
    printf("Could not run communication, existing...\n");
    terminateItc(&itcId);
    return 0;
  }

  msg = receiveData();

  uint32_t receiverTid = msg->cmdCheckRsp.tId;
  itcFree(msg);

  if (receiverTid)
  {
    union itcMsg* msg = itcAlloc(sizeof(CmdExecuteReqS), CMD_EXECUTE_REQ);
    strcpy(msg->cmdExecuteReq.cmd, argv[0]);
    for (int i = 1; i < argc; i++)
    {
      strcpy(msg->cmdExecuteReq.arg[i-1], argv[i]);
    }
    msg->cmdExecuteReq.argNo = argc - 1;

    char receiver[8];
    sprintf(receiver, "%x", receiverTid);
    if (!sendData(receiver, msg))
    {
      printf("Could not run command, existing...\n");
      terminateItc(&itcId);
      return 0;
    }

    msg = receiveData();
    printf("%s\n", msg->cmdExecuteRsp.output);
    itcFree(msg);
  }
  else
  {
    printf("No such command receiver, exiting...\n");
  }

  terminateItc(&itcId);
}
