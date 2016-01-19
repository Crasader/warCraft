//
//  SG_SkillsID9.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#include "SG_SkillsID9.h"


SG_SkillsID9::SG_SkillsID9()
{
    setHeroLayer();
}

SG_SkillsID9::~SG_SkillsID9()
{
    
}
//吸血 攻击时，对地方城墙造成的伤害，将有10%转化为我方城墙血量  //嗜血术  嗜血狂攻
bool SG_SkillsID9::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    int blood = attackList->getAp() * num;//伤害的百分比

    SGHeroLayer *hero = NULL;
	//伤害的血量加入到已方的城墙血量上
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        getmyHero()->changBloodNum(blood, true, true);
        hero = getenemyHero();
    }else{
        getenemyHero()->changBloodNum(blood, true, true);
        hero = getmyHero();
    }
    
    hero->showAddApEff(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound(), CCPointZero);
    
    return false;
}
