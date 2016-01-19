//
//  SG_SkillsID23.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#include "SG_SkillsID23.h"


SG_SkillsID23::SG_SkillsID23()
{
    
}

SG_SkillsID23::~SG_SkillsID23()
{
    
}


void SG_SkillsID23::activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    CCLOG("ap===%f",attackList->getMax_Ap() * num);
    attackList->changeAP(attackList->getMax_Ap() * num);
    attackList->drawSkillsEffect(1);
}

bool SG_SkillsID23::assistSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList) {
        if (attackList->damageNum == 0) {
            CCLOG("ap====%d",attackList->getMax_Ap());
            attackList->changeAP(attackList->getMax_Ap());
        }
        
    }
    return true;
}

