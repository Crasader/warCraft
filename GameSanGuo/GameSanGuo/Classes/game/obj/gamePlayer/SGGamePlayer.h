//
//  SGGamePlayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-5.
//
//

/****************游戏中的其他玩家数据（简略数据）*****************
 
 optional int32 roleId = 1;//自己ID
 optional int32 friendId = 2;//好友ID
 optional string friendName = 3;//好友昵称
 optional int32 typeId = 4;// 0是好友，1黑名单，2申请列表
 optional bool isOnline = 5;//是否在线
 optional int32 level = 6;//好友等级
 optional int32 time = 7;// 上次离线距离现在的时间
 optional int32 image = 8;//好友头像

 ************************************************************/

#ifndef __GameSanGuo__SGGamePlayer__
#define __GameSanGuo__SGGamePlayer__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class SGGamePlayer : public CCObject
{
public:

    CC_SYNTHESIZE(int, roleID, RoleID);
    CC_SYNTHESIZE(int, friendID, FriendID);
    CC_SYNTHESIZE_RETAIN(CCString*, friendName, FriendName);
    CC_SYNTHESIZE(int, typeID, TypeID);
    CC_SYNTHESIZE(bool , isOnline, IsOnline);
    CC_SYNTHESIZE(int, level, Level);
    CC_SYNTHESIZE(int, time, Time);
    CC_SYNTHESIZE(int , image, Image);
    CC_SYNTHESIZE(int , isGiftPower, IsGiftPower);//今天是否赠送过体力
    SGGamePlayer();
    ~SGGamePlayer();
};



#endif /* defined(__GameSanGuo__SGGamePlayer__) */
