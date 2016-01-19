//
//  SGHeroSkill24.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill24.h"

SGHeroSkill24::SGHeroSkill24()
{
    
}

SGHeroSkill24::~SGHeroSkill24()
{
    
}

//增加连锁部队攻击力2%（永久起效）
void SGHeroSkill24::activateSkill_20052(SGHeroLayer *heroLayer, float value1,int buffId)
{
    
    CCDictionary *buff = CCDictionary::create();
    buff->setObject(CCString::createWithFormat("%d",buffId), "buffId");
    buff->setObject(CCString::createWithFormat("%f",value1), "value1");
    heroLayer->getBattleMap()->addHeroBuff(buff);
    
    heroLayer->getBattleMap()->activateHeroBuff(false);
    
}
