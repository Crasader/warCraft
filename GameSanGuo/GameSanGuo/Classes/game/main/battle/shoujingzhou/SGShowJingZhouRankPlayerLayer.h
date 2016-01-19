//
//  SGShowJingZhouRankPlayerLayer.h
//  GameSanGuo
//
//  Created by kanata on 13-11-18.
//
//

#ifndef __GameSanGuo__SGShowJingZhouRankPlayerLayer__
#define __GameSanGuo__SGShowJingZhouRankPlayerLayer__

#include "cocos2d.h"

class SGShowJingZhouRankPlayerLayer : public cocos2d::CCObject
{
public:
    SGShowJingZhouRankPlayerLayer();
    ~SGShowJingZhouRankPlayerLayer();
    
public:
    static SGShowJingZhouRankPlayerLayer* create(int id, const char *name, int rank, int wav, int damage,int head, int gold);
    virtual bool initWithInfo(int id, const char *name, int rank, int wav, int damage,int head, int gold);
    
private:
    CC_SYNTHESIZE_READONLY(int, _playerId, PlayerId);
    CC_SYNTHESIZE_READONLY(cocos2d::CCString*, _name, Name);
    CC_SYNTHESIZE_READONLY(int, _headId, HeadId);
    CC_SYNTHESIZE_READONLY(int, _rank, Rank);
    CC_SYNTHESIZE_READONLY(int, _wave, Wave);
    CC_SYNTHESIZE_READONLY(int, _damage, Damage);
	CC_SYNTHESIZE_READONLY(int, _goldNum, GoldNum);//能领取元宝的数量
	
};









#endif /* defined(__GameSanGuo__SGShowJingZhouRankPlayerLayer__) */
