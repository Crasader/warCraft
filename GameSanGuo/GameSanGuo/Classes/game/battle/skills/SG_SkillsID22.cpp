//
//  SG_SkillsID22.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#include "SG_SkillsID22.h"


SG_SkillsID22::SG_SkillsID22()
{
    
}


SG_SkillsID22::~SG_SkillsID22()
{
    
}

void SG_SkillsID22::activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    attackList->changeAP(attackList->getMax_Ap());
}


