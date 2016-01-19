//
//  SGHeroSkill20.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill20.h"

SGHeroSkill20::SGHeroSkill20()
{
    
}

SGHeroSkill20::~SGHeroSkill20()
{
    
}

//发动后，我方在下2个回合内，召唤等待区的兵不费回合数。
void SGHeroSkill20::activateSkill_20034(SGHeroLayer *heroLayer, int round, int buffid)
{
    CCDictionary *buff = CCDictionary::create();
    
    buff->setObject(CCString::createWithFormat("%d",buffid), "buffId");
    buff->setObject(CCString::createWithFormat("%d",round), "round");
    heroLayer->addBuff(buff);
}