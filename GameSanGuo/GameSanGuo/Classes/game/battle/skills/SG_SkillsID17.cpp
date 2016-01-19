//
//  SG_SkillsID17.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-17.
//
//

#include "SG_SkillsID17.h"

SG_SkillsID17::SG_SkillsID17()
{
    setHeroLayer();
}

SG_SkillsID17::~SG_SkillsID17(){
    
}


void SG_SkillsID17::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
    SGBattleMap *enemyBattleMap = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        enemyBattleMap = getenemyHero()->getBattleMap();
    }else{
        enemyBattleMap = getmyHero()->getBattleMap();
    }
    
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(enemyBattleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        if (attack->getRoundV() == attackList->getAfftributeNum()) {
            int ap = attack->getMax_Ap() * num;
            ap = attack->attackSubAp(ap);
            if (ap > 0) {
                attack->drawSkillsEffect(0);
                attack->changeAP(ap, true);
                
            }else{
                enemyBattleMap->removeAttackList(attack);
            }
        }
    }
}

