/*
 * cmdAppClient.h
 *
 *  Created on: 08-07-2017
 *      Author: marek
 */

#ifndef CMDAPPCLIENT_H_
#define CMDAPPCLIENT_H_

#include "pthread.h"
#include <vector>
#include <string>

#define CMD_DAEMON "icmdDaemon"
#define CMD_SHELL "icmdShell"
typedef char* (*cmdFunc)(void);

class CmdAppClient
{
  public:
  CmdAppClient();
  virtual ~CmdAppClient();

  bool registerCmd(std::string cmd, char* (*handler)(void));
  bool start();

  enum Choice
  {
    REGISTER,
    LISTEN,
    IDLE
  };

  private:
  pthread_t tId;
  int itcId;
  static std::vector<std::pair<std::string, cmdFunc> > cmdList;
  static bool loop;
  static Choice choice;

  static void* mainLoop(void* arg);
};



#endif /* CMDAPPCLIENT_H_ */
