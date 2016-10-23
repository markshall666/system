/*
 * messages.h
 *
 *  Created on: 16-06-2016
 *      Author: marek
 */

#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <stdint.h>

#define REGISTER_APP_REQ	0x66600001
typedef struct RegisterAppReqS
{
  uint32_t msgNo;
  char appName[16];
} RegisterAppReq;

#define REGISTER_APP_CFM	0x66600002
typedef struct RegisterAppCfmS
{
  uint32_t msgNo;
  char appName[16];
  bool result;
} RegisterAppCfm;

#define UPDATE_MO_REQ	        0x66600003
typedef struct UpdateMoReqS
{
  uint32_t msgNo;
  char moName[16];
  char update[16];
} UpdateMoReq;

#define UPDATE_MO_CFM	        0x66600004
typedef struct UpdateMoCfmS
{
  uint32_t msgNo;
  char moName[16];
} UpdateMoCfm;

#define DISPATCH_APP 		0x66600666
typedef struct DispatchAppS
{
  uint32_t msgNo;
  char message[16];
} DispatchApp;

union itcMsg
{
  uint32_t msgNo;
  struct RegisterAppReqS registerAppReq;
  struct RegisterAppCfmS registerAppCfm;
  struct UpdateMoReqS updateMoReq;
  struct UpdateMoReqS updateMoCfm;
  struct DispatchAppS dispatchApp;
};


#endif /* MESSAGES_H_ */
