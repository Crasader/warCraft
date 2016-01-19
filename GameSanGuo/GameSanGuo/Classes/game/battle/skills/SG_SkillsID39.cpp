//
//  SG_SkillsID39.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-24.
//
//

#include "SG_SkillsID39.h"


SG_SkillsID39::SG_SkillsID39()
{
    setHeroLayer();
}

SG_SkillsID39::~SG_SkillsID39()
{
    
}

bool SG_SkillsID39::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    getmyHero()->setSkillsRoundNum(-attackList->getAfftributeNum(), true);
    
    return false;
}

