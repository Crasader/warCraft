//
//  SG_SkillsID18.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-18.
//
//

#include "SG_SkillsID18.h"


SG_SkillsID18::SG_SkillsID18()
{
    setHeroLayer();
}


SG_SkillsID18::~SG_SkillsID18()
{
    
}

bool SG_SkillsID18::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
     if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
     {
         getmyHero()->setSkillsRoundNum(-num, true);
     }else
     {
         getenemyHero()->setSkillsRoundNum(-num, true);
     }

    
    return false;
}

