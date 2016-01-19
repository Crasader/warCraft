//
//  SGShouJingZhouProcessData.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-14.
//
//

#include "SGShouJingZhouProcessData.h"


USING_NS_CC;

SGShouJingZhouProcessData::SGShouJingZhouProcessData()
{
    _level=-10;
    _gold=-10;
    _junhun=-10;
    
    
    
    
}
SGShouJingZhouProcessData::~SGShouJingZhouProcessData()
{
  
}

SGShouJingZhouProcessData* SGShouJingZhouProcessData::create(int level,int gold,int junhun)
{
    SGShouJingZhouProcessData *player = new SGShouJingZhouProcessData();
    if (player&&player->initWithInfo(level,gold,junhun)) {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return NULL;
}

bool SGShouJingZhouProcessData::initWithInfo(int level,int gold,int junhun)
{
    
    _level=level;
    _gold=gold;
    _junhun=junhun;
    
    
    
    
    return true;
}