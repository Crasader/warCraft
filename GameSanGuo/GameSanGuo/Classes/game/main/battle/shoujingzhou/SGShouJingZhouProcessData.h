//
//  SGShouJingZhouProcessData.h
//  GameSanGuo
//
//  Created by kanata on 14-2-14.
//
//

#ifndef __GameSanGuo__SGShouJingZhouProcessData__
#define __GameSanGuo__SGShouJingZhouProcessData__

#include <iostream>
#include "cocos2d.h"

class SGShouJingZhouProcessData : public cocos2d::CCObject
{
public:
    SGShouJingZhouProcessData();
    ~SGShouJingZhouProcessData();
    
public:
    static SGShouJingZhouProcessData* create(int level,int gold,int junhun);
    virtual bool initWithInfo(int level,int gold,int junhun);
    
public:
    
 
    CC_SYNTHESIZE(int, _level, level);   //波数
    CC_SYNTHESIZE(int, _gold, gold);      //钱
    CC_SYNTHESIZE(int, _junhun, junhun);  //军魂
 
    
private:
    
};

#endif /* defined(__GameSanGuo__SGShouJingZhouProcessData__) */
