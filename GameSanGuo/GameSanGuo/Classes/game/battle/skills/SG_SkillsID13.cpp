//
//  SG_SkillsID13.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#include "SG_SkillsID13.h"



SG_SkillsID13::SG_SkillsID13()
:isAddAp(false)
{
    
}


SG_SkillsID13::~SG_SkillsID13(){
    
}
//挑战 自强术 攻击时，如果遇到比自己攻击力强的敌方蓄力阵列，则针对这些阵列的自身攻击力提升0.18倍
void SG_SkillsID13::activateSkills6(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    if (!isAddAp) {
        isAddAp = true;
        CCLOG("ap===%d",attackList->getAp());
        attackList->changeAP(attackList->getAp() * num);
        //MMDEBUG: FIXME 安卓shiyao需要找到原图！
//        attackList->drawSkillsEffect(10);
        attackList->drawSkillsEffect(12);
    }
}

bool SG_SkillsID13::activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, float damage)
{
    bool isWuShun = false;
    if (isAddAp) {
        isAddAp = false;
        CCLOG("num===%f",attackList->getAfftributeNum());
        CCLOG("ap==%d",attackList->getAp());
        attackList->changeAP(attackList->getAp() / attackList->getAfftributeNum());
        isWuShun = true;
    }

    return isWuShun;
}

