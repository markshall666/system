/*
 * daemon.h
 *
 *  Created on: 08-07-2017
 *      Author: marek
 */

#ifndef CMDDAEMON_H_
#define CMDDAEMON_H_

#include <map>
#include <string>

#define CMD_DAEMON "icmdDaemon"
#define CMD_SHELL "icmdShell"

int itcId;

std::map<std::string, unsigned> cmdList;

#endif /* CMDDAEMON_H_ */
