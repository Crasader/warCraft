//
//  SGRankPlayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-8.
//
//

#ifndef __GameSanGuo__SGRankPlayer__
#define __GameSanGuo__SGRankPlayer__

#include "cocos2d.h"

/* 战斗玩家信息 -> PvpFight.proto */

class SGRankPlayer : public cocos2d::CCObject
{
public:
    SGRankPlayer(void);
    ~SGRankPlayer(void);
    
public:
    static SGRankPlayer* create(const char *name, int hid, int rank, int levl, int ftime, float rwin,int serverid,const char* servername,int roleid=0,int nation =0);
    virtual bool initWithInfo(const char *name, int hid, int rank, int levl, int ftime, float rwin,int serverid,const char* servername,int roleid=0,int nation=0);
    
public:
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, _name, Name);//名字
    CC_SYNTHESIZE_READONLY(int, _headId, HeadId);//头像ID
    CC_SYNTHESIZE_READONLY(int, _rank, Rank);//天梯排名
    CC_SYNTHESIZE_READONLY(int, _level, Level);//天梯等级
    CC_SYNTHESIZE_READONLY(int, _bcount, Bcount);//参战次数
    CC_SYNTHESIZE_READONLY(float, _winRate, WinRate);
    CC_SYNTHESIZE_READONLY(int, _serverid, ServerId);//服务器ID
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, _servername, ServerName);//服务器名字
    CC_SYNTHESIZE_READONLY(int, _roleid, getRoleId);//角色ID
    CC_SYNTHESIZE_READONLY(int, _nation, Nation);//国家
};

#endif /* defined(__GameSanGuo__SGRankPlayer__) */
