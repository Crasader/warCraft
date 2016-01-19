//
//  SGSoldierSkillDataModel.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-17.
//
//

#ifndef __GameSanGuo__SGSoldierSkillDataModel__
#define __GameSanGuo__SGSoldierSkillDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 士兵表 */

class SGSoldierSkillDataModel :public CCObject
{
    
     CC_SYNTHESIZE(int, m_skillid, SkillID);
    CC_SYNTHESIZE_RETAIN(CCString*, soldierBeanId, SoldierBeanId);
     CC_SYNTHESIZE_RETAIN(CCString*, skillname, SkillName);
    
    CC_SYNTHESIZE(int, m_skilllevel, SkillLevel);
    CC_SYNTHESIZE_RETAIN(CCString*, skillinfo, SkillInfo);
    CC_SYNTHESIZE(int, m_nextid, NextId);
    
public:
    SGSoldierSkillDataModel();
    
    ~SGSoldierSkillDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGSoldierSkillDataModel__) */
