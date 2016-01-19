//
//  SG_SkillsID19.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-18.
//
//

#include "SG_SkillsID19.h"

SG_SkillsID19::SG_SkillsID19()
{
    setHeroLayer();
}

SG_SkillsID19::~SG_SkillsID19()
{
    
}


bool SG_SkillsID19::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
     if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
     {
         getmyHero()->setSkillsRoundNum(-num, true);
         
         getenemyHero()->setSkillsRoundNum(attackList->getAfftributeNum1(), true);
     }else{
         getenemyHero()->setSkillsRoundNum(-num, true);
         
         getmyHero()->setSkillsRoundNum(attackList->getAfftributeNum1(), true);
     }
    
    
    return false;
}
