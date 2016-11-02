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

#define CREATE_MO_REQ	        0x66600005
typedef struct CreateMoReqS
{
  uint32_t msgNo;
  uint8_t txId;
  uint32_t objectId;
  char moName[16];
  uint8_t noAttr;
  char attr0name[16];
  char attr0value[16];
} CreateMoReq;

#define CREATE_MO_CFM	        0x66600006
typedef struct CreateMoCfmS
{
  uint32_t msgNo;
  uint8_t txId;
  uint32_t objectId;
  char moName[16];
} CreateMoCfm;

#define CREATE_MO_REJ	        0x66600007
typedef struct CreateMoRejS
{
  uint32_t msgNo;
  uint8_t txId;
  uint32_t objectId;
  char moName[16];
  uint8_t errorCode;
} CreateMoRej;

#define COMPLETED_MO_REQ        0x66600008
typedef struct CompletedMoReqS
{
  uint32_t msgNo;
  uint8_t txId;
  uint32_t objectId;
  char moName[16];
} CompletedMoReq;

#define COMPLETED_MO_CFM        0x66600009
typedef struct CompletedMoCfmS
{
  uint32_t msgNo;
  uint8_t txId;
  uint32_t objectId;
  char moName[16];
} CompletedMoCfm;

#define COMPLETED_MO_REJ        0x6660000A
typedef struct CompletedMoRejS
{
  uint32_t msgNo;
  uint8_t txId;
  uint32_t objectId;
  char moName[16];
  char error[64];
} CompletedMoRej;

#define APPLY_MO_REQ            0x6660000B
typedef struct ApplyMoReqS
{
  uint32_t msgNo;
  uint8_t txId;
  uint32_t objectId;
  char moName[16];
} ApplyMoReq;

#define APPLY_MO_CFM            0x6660000C
typedef struct ApplyMoCfmS
{
  uint32_t msgNo;
  uint8_t txId;
  uint32_t objectId;
  char moName[16];
} ApplyMoCfm;

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
  struct CreateMoReqS createMoReq;
  struct CreateMoCfmS createMoCfm;
  struct CreateMoRejS createMoRej;
  struct CompletedMoReqS completedMoReq;
  struct CompletedMoCfmS completedMoCfm;
  struct CompletedMoRejS completedMoRej;
  struct ApplyMoReqS applyMoReq;
  struct ApplyMoCfmS applyMoCfm;
};


#endif /* MESSAGES_H_ */
