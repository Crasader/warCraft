//
//  SG_SkillsID12.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#include "SG_SkillsID12.h"

SG_SkillsID12::SG_SkillsID12()
{
    setHeroLayer();
}


SG_SkillsID12::~SG_SkillsID12()
{
    
}


void SG_SkillsID12::activatePassivitySkills(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    int ap = attackList1->getAp() * num;
    attackList1->changeAP(attackList1->getAp() - ap, true, true);
    attackList->drawSkillsEffect(1);
    
    SGHeroLayer *hero = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        hero = getenemyHero();
    }else{
        hero = getmyHero();
    }
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList1->attackSbs, obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        hero->showEffectLayer(3, true, sb->getSbindex());
    }
}

int SG_SkillsID12::assistPassivitySkills(int ap, SGAttackList *attackList, float num)
{
    int a = ap * num;
    return ap - a;
}