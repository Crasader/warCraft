//
//  SGStoryReward.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-7.
//
//

#ifndef __GameSanGuo__SGStoryReward__
#define __GameSanGuo__SGStoryReward__

#include "cocos2d.h"

USING_NS_CC;

class SGStoryReward : public CCObject
{
    CC_SYNTHESIZE_READONLY(int, _rating, Rating);
    CC_SYNTHESIZE_READONLY(int, _exp, Exp);       // base exp，如果升级了，这也是升级之前的当时exp信息。
    CC_SYNTHESIZE_READONLY(int, _coin, Coin);
    CC_SYNTHESIZE_READONLY(int, _jexp, Jexp);
	CC_SYNTHESIZE_READONLY(int, _soul, Soul);//守荆州军魂
	CC_SYNTHESIZE_READONLY(int, _curHp, CurHp);//守荆州当前血量
	CC_SYNTHESIZE_READONLY(int, _totalHp, TotalHp);//守荆州玩家总血量

    CC_SYNTHESIZE_READONLY(CCArray *, _items, Items);
	
    
    // PVP 新增字段
    CC_SYNTHESIZE(int, _gold, Gold);
    CC_SYNTHESIZE(int, _liandan, LianDan);
    CC_SYNTHESIZE(float, _hprate, HpRate);
    CC_SYNTHESIZE(int, _extExp, ExtExp); // extend exp，增加的exp数值。
    CC_SYNTHESIZE(int, _beforelevel, BeforeLevel);//升级前的等级
    
public:
    SGStoryReward(void);
    ~SGStoryReward(void);
    
    static SGStoryReward* create(int rate, int exp, int coin, int jun, int soul, int curHp, int totalHp, CCArray *data);
    void initWithInfo(int rate, int exp, int coin, int jun, int soul, int curHp, int totalHp, CCArray *data);
};

#endif /* defined(__GameSanGuo__SGStoryReward__) */
