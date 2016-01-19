//
//  GameSanGuo
//
//2014-11-01
//

#ifndef __GameSanGuo__SGSkillCombatValueDataModel__
#define __GameSanGuo__SGSkillCombatValueDataModel__

#include "cocos2d.h"
USING_NS_CC;


class SGSkillCombatValueDataModel : public CCObject
{
    //type=1 主将技;  type=2 被动技
    CC_SYNTHESIZE(int, _type, Type);
    
    //grid 格子
    CC_SYNTHESIZE(int, _grid, Grid);
    
    //level 技能等级
    CC_SYNTHESIZE(int, _skillLev, SkillLev);
    
    //技能珍惜度 对应的战力 共有4阶段 概念对应武将技能表中 levelType字段。
    CC_SYNTHESIZE(int, _weightA, WeightA);
    CC_SYNTHESIZE(int, _weightB, WeightB);
    CC_SYNTHESIZE(int, _weightC, WeightC);
    CC_SYNTHESIZE(int, _weightD, WeightD);
    
    
public:
    SGSkillCombatValueDataModel():_type(0) , _grid(0) , _skillLev(0) ,_weightA(0) , _weightB(0),_weightC(0),_weightD(0)
    {
        
    }
    
    ~SGSkillCombatValueDataModel(){}
    
};

#endif
