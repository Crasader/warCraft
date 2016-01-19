//
//  SG_SkillsID38.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-24.
//
//

#include "SG_SkillsID38.h"

SG_SkillsID38::SG_SkillsID38()
{
}

SG_SkillsID38::~SG_SkillsID38()
{
    
}

void SG_SkillsID38::activateSkills10(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    attackList->drawSkillsEffect(2);
    int rand = num;
    int numa = std::abs(rand);
    int ap = attackList->getAp() * attackList->getAfftributeNum() * numa;
    attackList->setSpAddAp(attackList->getSpAddAp() + ap);
    attackList->changeAP(attackList->getAp() + ap, true);
    
}

