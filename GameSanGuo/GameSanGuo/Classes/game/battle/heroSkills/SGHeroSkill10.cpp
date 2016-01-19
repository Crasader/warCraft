//
//  SGHeroSkill10.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill10.h"

SGHeroSkill10::SGHeroSkill10()
{
    
}

SGHeroSkill10::~SGHeroSkill10()
{
    
}
//增加连锁和融合部队攻击力10%
void SGHeroSkill10::activateSkill_20010(SGHeroLayer *heroLayer,  float rate)
{
    CCArray *attackList = heroLayer->getBattleMap()->getAttackList();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList, obj)
    {
        SGAttackList *attackList = (SGAttackList*)obj;
        if (attackList->getLRType() == lr_l || attackList->getLRType() == lr_r || attackList->getLRType() == lr_lr)
        {
            int ap = attackList->getMax_Ap() * rate;
            attackList->setSpAddAp(attackList->getSpAddAp() + ap);
            attackList->changeAP(attackList->getAp() + ap);
            attackList->drawSkillsEffect(2);
        }
    }
}
