//
//  SGHeroSkill9.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill9.h"

SGHeroSkill9::SGHeroSkill9()
{
    
}

SGHeroSkill9::~SGHeroSkill9()
{
    
}

//发动后，恢复我方血量值10%
void SGHeroSkill9::activateSkill_20008(SGHeroLayer *heroLayer, float rate)
{
    heroLayer->changBloodNum(rate, false, true);
}