//
//  SGStoryReward.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-7.
//
//

#include "SGStoryReward.h"

SGStoryReward::SGStoryReward(void)
{
    _exp = 0;
    _extExp = 0;
    _rating = 0;
    _coin = 0;
    _jexp = 0;
    _soul = 0;
	_curHp = 0;
	_totalHp = 0;
	
    _gold = 0;
    _hprate = 0;
    _liandan = -1;
    _beforelevel = 0;
    
    _items = NULL;
}

SGStoryReward::~SGStoryReward(void)
{
    CC_SAFE_DELETE(_items);
}

SGStoryReward* SGStoryReward::create(int rate, int exp, int coin, int jun, int soul, int curHp, int totalHp, CCArray *data)
{
    SGStoryReward *obj = new SGStoryReward();
    if(obj) {
        obj->initWithInfo(rate, exp, coin, jun, soul, curHp, totalHp, data);
        obj->autorelease();
        return obj;
    }
    CC_SAFE_DELETE(obj);
    return NULL;
}

void SGStoryReward::initWithInfo(int rate, int exp, int coin, int jun, int soul, int curHp, int totalHp, CCArray *data)
{
    _exp = exp;
    _rating = rate;
    _coin = coin;
    _jexp = jun;
	_soul = soul;
    _curHp = curHp;
	_totalHp = totalHp;
	
	
    _items = data;
    _items->retain();
}