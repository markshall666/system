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
typedef void (*cmdFunc)(std::string&);

class CmdAppClient
{
  public:
  CmdAppClient();
  virtual ~CmdAppClient();

  bool registerCmd(std::string cmd, void (*handler)(std::string&));
  bool start();

  enum Choice
  {
    INIT,
    REGISTER,
    LISTEN,
    IDLE,
    TERMINATE
  };

  private:
  static pthread_t tId;
  static int itcId;
  static std::vector<std::pair<std::string, cmdFunc> > cmdList;
  static bool loop;
  static std::vector<Choice> state;

  static void* mainLoop(void* arg);
};



#endif /* CMDAPPCLIENT_H_ */
