//
//  SGHeroSkill5.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill5.h"


SGHeroSkill5::SGHeroSkill5()
{
    
}

SGHeroSkill5::~SGHeroSkill5()
{
    
}

void SGHeroSkill5::activateSkill_20004(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float valule2,int heroId)
{
    heroLayer->changRoundNum(value1, true);
}
