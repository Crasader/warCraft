//
//  SGProtocolToMsg.h
//  typhoonwar
//
//  Created by ding huidong on 12-11-2.
//
//

#ifndef typhoonwar_SGProtocolToMsg_h
#define typhoonwar_SGProtocolToMsg_h

#include "SGMsgId.h"
#include "SGPacketHeader.h"
#include <google/protobuf/message_lite.h>
#include "SGSocketClient.h"

#define USING_GOOGLE_PROTOBUF  using namespace google::protobuf; using namespace com::typhoon::fightingGame::protocol;

//网络请求通知中心前缀字符串
#define NETCMD_NC_STRING ("NC_NETCMD_")

//转换命令为字符串
//#define NETCMD_TO_NC_STRING(_CMD_) ({char tmpChar[32]={0}; sprintf(tmpChar, "%s%d", NETCMD_NC_STRING, _CMD_)})

//将网络请求命令号，转化为通知中心字符串
#define ON_NETWORK_EVENT_REG(_CMD_,_SELECTOR_) ({ \
char tmpChar[32]={0}; sprintf(tmpChar, "%s%d", NETCMD_NC_STRING, _CMD_);\
DDNC_ADD_OBSERVER( tmpChar, this, _SELECTOR_ ); })

//取消注册网络
#define ON_NETWORK_EVENT_UNREG(_CMD_) ({ \
char tmpChar[32]={0}; sprintf(tmpChar, "%s%d", NETCMD_NC_STRING, _CMD_);\
DDNC_REMOVE_OBSERVER(this, tmpChar); })

//将包数据转换为msg
google::protobuf::MessageLite* parseMessage(unsigned int cmd, SGPacketHeader* pack);

//登陆
//void sendLoginMsg(const char *name, const char *password);

//匹配完家
void sendMatchPlayer();
#endif
