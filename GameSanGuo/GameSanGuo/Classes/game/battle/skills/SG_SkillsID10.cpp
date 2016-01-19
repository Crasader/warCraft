//
//  SG_SkillsID10.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#include "SG_SkillsID10.h"



SG_SkillsID10::SG_SkillsID10()
{
    
}

SG_SkillsID10::~SG_SkillsID10(){
    
}

void SG_SkillsID10::activateSkills4(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    attackList->drawSkillsEffect(2);
    int ap = attackList->getMax_Ap() * attackList->getAfftributeNum();
    int ap1 = (ap * num);
    attackList->setSpAddAp(attackList->getSpAddAp() + ap1);
    attackList->changeAP(attackList->getAp() + ap1, true);
}