//
//  SGChatMessage.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-12.
//
//


#include "SGChatMessage.h"

SGChatMessage::SGChatMessage()
: chatShowType(0)
, chatShowSSid(0)
, chatShowStarLevel(0)
{
    channelId = 0;
    content = NULL;
    roleId = 0;
    roleNickName = NULL;
    image = 0;
    timeStr = NULL;
    vipLevel=0;
    combateValue=0;
    militaryRank=0;
    soliderID=0;
    militaryName=NULL;
    isChatShowHaHa = false;
    chatShowName = NULL;
}

SGChatMessage::~SGChatMessage()
{
    CC_SAFE_RELEASE(content);
    CC_SAFE_RELEASE(roleNickName);
    CC_SAFE_RELEASE(timeStr);
    CC_SAFE_DELETE(militaryName);
    CC_SAFE_DELETE(chatShowName);
}