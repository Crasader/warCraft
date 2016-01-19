//
//  GameSanGuo
//
//2014-11-01
//

#ifndef __GameSanGuo__SGBaseCombatValueDataModel__
#define __GameSanGuo__SGBaseCombatValueDataModel__

#include "cocos2d.h"
USING_NS_CC;

/*  vip表 */

class SGBaseCombatValueDataModel : public CCObject
{
    //type=6 officer;  type=7 equip
    CC_SYNTHESIZE(int, _type, Type);
    
    //starLev 星级
    CC_SYNTHESIZE(int, _starLev, StarLev);
    
    //小转身次数
    CC_SYNTHESIZE(int, _advNum, AdvNum);
    
    //战力
    CC_SYNTHESIZE(int, _combatValue, CombatValue);
    
    //等级成长系数
    CC_SYNTHESIZE(float, _levelRate, LevelRate);
    
public:
    SGBaseCombatValueDataModel():_type(0) , _starLev(0) , _advNum(0) ,_combatValue(0) , _levelRate(0.0)
    {
        
    }
    
    ~SGBaseCombatValueDataModel(){}
    
};

#endif
