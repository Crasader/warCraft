//
//  SG_SkillsID40.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-24.
//
//

#include "SG_SkillsID40.h"

SG_SkillsID40::SG_SkillsID40()
{
    
}

SG_SkillsID40::~SG_SkillsID40()
{
    
}

//无调用
int SG_SkillsID40::activateSkills5_1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (attackList->getAp() > num * attackList->getAfftributeNum()) {
        return 0;
    }
    
    return num;
}