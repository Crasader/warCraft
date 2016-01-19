//
//  SG_SkillsID7.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#include "SG_SkillsID7.h"


SG_SkillsID7::SG_SkillsID7()
{
    
}

SG_SkillsID7::~SG_SkillsID7(){
    
    
}
//藐视 攻击时，若敌方阵列攻击力、未蓄力士兵及武将防御值，小于自身当前攻击力25%。则在杀死该单位后，不会损失自身攻击力。该技能对防御墙无效
bool SG_SkillsID7::activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    bool isWu = false;
    int a = num / attackList->getAfftributeNum();
    if (attackList->getAp() > a) {
        attackList->changeAP(attackList->getAp() + num);
        isWu = true;
    }
    
    return isWu;
}

