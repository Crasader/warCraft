//
//  SGRankAllPlayer.h
//  GameSanGuo
//
//  Created by kanata on 14-2-11.
//
//

#ifndef __GameSanGuo__SGRankAllPlayer__
#define __GameSanGuo__SGRankAllPlayer__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class SGRankAllPlayer : public CCObject
{
public:
    SGRankAllPlayer();
    ~SGRankAllPlayer();
    
public:
    static SGRankAllPlayer* create(const char *name,const char *servername,int headid,int rank,int playerId,int ranktype, int servernum);
    virtual bool initWithInfo(const char *name,const char *servername,int headid,int rank,int playerId,int ranktype, int servernum);
    
public:
   
    CC_SYNTHESIZE_RETAIN(CCString*, _name, Name);//玩家名字
    CC_SYNTHESIZE_RETAIN(CCString*, _servername, ServerName);//服务器名字
    CC_SYNTHESIZE(int, _servernum, ServerNum);    //服务器数字
    CC_SYNTHESIZE(int, _playerId, playerId);   //玩家ID
    CC_SYNTHESIZE(int, _headId, HeadId);      //玩家头像ID
    CC_SYNTHESIZE(int, _rank, Rank);         //排名
    CC_SYNTHESIZE(int, _ranktype, RankType);     //排名类型
    CC_SYNTHESIZE(int, _isin, IsIn);     //是否进榜
private:
    
};

#endif /* defined(__GameSanGuo__SGRankAllPlayer__) */
