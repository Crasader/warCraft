//
//  SG_BuffBase.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-17.
//
//

#ifndef __GameSanGuo__SG_BuffBase__
#define __GameSanGuo__SG_BuffBase__

#include <iostream>
#include "cocos2d.h"
#include "SGHeroLayer.h"
#include "SG_SkillsBase.h"
//#include "SGAttackList.h"

using namespace cocos2d;


#define    BUFF_EFFECT_TAG   133


class SGHeroLayer;
//class SGAttackList;

class SG_BuffBase: public CCNode {
    
    
public:
    
    SG_BuffBase();
    ~SG_BuffBase();


    void setHeroLayer(SG_SkillsBase* skbase);
    
    virtual void activateBuff(GameIndex index);
    virtual void activateBuff(SGAttackList *attack);
    void pushBuffData(CCString *strdata);

public:
    CC_SYNTHESIZE(SGHeroLayer*, m_myHeroLayer, myHero);
    CC_SYNTHESIZE(SGHeroLayer*, m_enemyHeroLayer, enemyHero);
    int m_buffType;
    CC_SYNTHESIZE(int, m_roundNum, RoundNum);
    CC_SYNTHESIZE(float, m_attributeNum, AfftributeNum);
    CC_SYNTHESIZE(int, effectType, EffectType);
    CC_SYNTHESIZE(int, m_buffID, BuffID);

};

#endif /* defined(__GameSanGuo__SG_BuffBase__) */
