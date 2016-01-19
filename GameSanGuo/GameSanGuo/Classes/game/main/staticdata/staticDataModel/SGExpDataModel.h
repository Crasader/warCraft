//
//  SGExpDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-7.
//
//

#ifndef __GameSanGuo__SGExpDataModel__
#define __GameSanGuo__SGExpDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;
class SGExpDataModel : public CCObject
{
    //经验值
    CC_SYNTHESIZE(int, expValue, ExpValue);
    //beanId
    CC_SYNTHESIZE_RETAIN(CCString*, expId, ExpId);
    //经验等级
    CC_SYNTHESIZE(int, expLevel, ExpLevel);
    //升级所需金钱
    CC_SYNTHESIZE(int, expReGold, ExpReGold);
    //升级所需全部经验
    CC_SYNTHESIZE(int, expAll, expAll);
    //升级装备所需铜钱
    CC_SYNTHESIZE(int, equipCost, EquipCost);
public:
    SGExpDataModel();
    
    ~SGExpDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGExpDataModel__) */
