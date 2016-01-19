//
//  SG_SkillsID41.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-2-27.
//
//

#include "SG_SkillsID41.h"

SG_SkillsID41::SG_SkillsID41()
{
    setHeroLayer();
}

SG_SkillsID41::~SG_SkillsID41()
{
    
}


void SG_SkillsID41::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
    SGBattleMap *enemyBattleMap = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        enemyBattleMap = getenemyHero()->getBattleMap();
    }else{
        enemyBattleMap = getmyHero()->getBattleMap();
    }
    
    CCObject *obj = NULL;
    CCArray *array = CCArray::create();
    CCARRAY_FOREACH(enemyBattleMap->getAttackList(), obj)
    {

        SGAttackList *attack = (SGAttackList *)obj;
        if ( attackList->getAttackType() == kattacktwo) {
            if (attack->getAttackType() == kattacktwo) {
                array->addObject(attack);
            }
        }else if(attackList->getAttackType() == kattackfour)
        {
            if (attack->getAttackType() == kattacktwo ||
                attack->getAttackType() == kattackfour) {
                array->addObject(attack);
            }
        }
        
    }
    
    if (array->count()) {
        int randNum = 0;
        int a = attackList->getAfftributeNum2();
        randNum =  a%array->count();
        
        SGAttackList *attack = (SGAttackList*)array->objectAtIndex(randNum);
        enemyBattleMap->removeAttackList(attack);
    }
}
