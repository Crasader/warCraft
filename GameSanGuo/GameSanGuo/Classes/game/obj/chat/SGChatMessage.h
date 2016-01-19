//
//  SGChatMessage.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-12.
//
//

#ifndef __GameSanGuo__SGChatMessage__
#define __GameSanGuo__SGChatMessage__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/*
 message ChatProto {
 optional int32 chId = 1;
 optional string content = 2;
 optional int32 roleId = 3;
 optional int32 image = 4;
 optional string roleNickName = 5;
 optional string timeStr = 6;
 }
 */

class SGChatMessage : public CCObject
{
   
private:
    //频道ID 1：公共频道 2：家族频道 3：私聊
    CC_SYNTHESIZE(int , channelId, ChannelId);
    //聊天内容
    CC_SYNTHESIZE_RETAIN(CCString*, content, Content);
    CC_SYNTHESIZE(int, roleId, RoleId);
    CC_SYNTHESIZE_RETAIN(CCString*, roleNickName, RoleNickName);
    //角色头像框
    CC_SYNTHESIZE(int, image, Image);
    //发言时间
    CC_SYNTHESIZE_RETAIN(CCString*, timeStr, TimeStr);
    CC_SYNTHESIZE(int, vipLevel,VipLevel );//vip等级
    CC_SYNTHESIZE(int , combateValue, CombateValue);//战斗力
    CC_SYNTHESIZE(int,militaryRank, MilitaryRank);//军衔等级
    CC_SYNTHESIZE(int, soliderID, SoliderID);//主将ID
    CC_SYNTHESIZE_RETAIN(CCString *, militaryName, MilitaryName);//军衔名称
    
    //聊天展示相关
    CC_SYNTHESIZE(bool, isChatShowHaHa, IsChatShowHaHa);//是否聊天展示
    CC_SYNTHESIZE_RETAIN(CCString *, chatShowName, ChatShowyName);//展示名称
    CC_SYNTHESIZE(int, chatShowType, ChatShowType);//展示类型
    CC_SYNTHESIZE(int, chatShowSSid, ChatShowSSid);//展示SSid
    CC_SYNTHESIZE(int, chatShowStarLevel, ChatShowStarLevel);//展示SSid
public:
    SGChatMessage();
    ~SGChatMessage();
    
};

#endif /* defined(__GameSanGuo__SGChatMessage__) */
