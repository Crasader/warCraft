//
//  SG_SkillsID29.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#include "SG_SkillsID29.h"


SG_SkillsID29::SG_SkillsID29()
{
    
}

SG_SkillsID29::~SG_SkillsID29()
{
    
}

//固守 蓄力阶段，无视低于自身当前攻击力15%的伤害
void SG_SkillsID29::activatePassivitySkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    int a = attackList->getAp() * attackList->getAfftributeNum();
    if (a > num) {
        attackList->drawSkillsEffect(1);
        attackList->drawSkillsEffect(13);
        attackList->setAp(attackList->getAp() + num);
    }
}

