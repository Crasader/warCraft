//
//  SGHeroSkill12.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill12.h"

SGHeroSkill12::SGHeroSkill12()
{
    
}

SGHeroSkill12::~SGHeroSkill12()
{
    
}

//

//降低总血量值25%，增加所有攻击力（使用后包括现在攻击阵列的，和以后形成攻击阵列的）5%
void SGHeroSkill12::activateSkill_20014(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid)
{
    CCDictionary *buff = CCDictionary::create();
    
    buff->setObject(CCString::createWithFormat("%d",buffid), "buffId");
    buff->setObject(CCString::createWithFormat("%d",round), "round");
    float num = value1;
    buff->setObject(CCString::createWithFormat("%f",num), "value1");
    buff->setObject(CCString::createWithFormat("%f",value2), "value2");
    
    heroLayer->battleMap->addHeroBuff(buff);
    
    heroLayer->battleMap->activateHeroBuff(false);
    heroLayer->activateBuff();
}