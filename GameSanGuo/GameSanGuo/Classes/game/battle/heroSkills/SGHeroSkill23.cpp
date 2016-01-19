//
//  SGHeroSkill23.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill23.h"

SGHeroSkill23::SGHeroSkill23()
{
    
}

SGHeroSkill23::~SGHeroSkill23()
{
    
}

//使对方主将计发动增加2回合。
void SGHeroSkill23::activateSkill_20048(SGHeroLayer *heroLayer, float value)
{
    heroLayer->setSkillsRoundNum(value, true);
}