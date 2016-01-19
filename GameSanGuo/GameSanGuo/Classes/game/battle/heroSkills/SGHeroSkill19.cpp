//
//  SGHeroSkill19.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill19.h"

SGHeroSkill19::SGHeroSkill19()
{
    
}

SGHeroSkill19::~SGHeroSkill19()
{
    
}

//增加现在两格蓄力状态阵列攻击力10%
void SGHeroSkill19::activateSkill_20033(SGHeroLayer *heroLayer, float value)
{
    CCArray *attackList = heroLayer->battleMap->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList, obj)
    {
        SGAttackList *attackList = (SGAttackList *)obj;
        if (attackList->getAttackType() == kattacktwo)
        {
            int ap = attackList->getMax_Ap() * value;
            attackList->setSpAddAp(attackList->getSpAddAp() + ap);
            attackList->changeAP(attackList->getAp() + ap, true);
            ShiBing *sb = (ShiBing*) attackList->attackSbs->objectAtIndex(0);
            attackList->showBattleLabel(ap, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
        }
    }
}