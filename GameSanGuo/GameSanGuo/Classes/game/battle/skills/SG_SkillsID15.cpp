//
//  SG_SkillsID15.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#include "SG_SkillsID15.h"


SG_SkillsID15::SG_SkillsID15()
{
    setHeroLayer();
}


SG_SkillsID15::~SG_SkillsID15()
{
    
}
//筑城 蓄力阶段，每回合恢复我方城墙42总血量
void SG_SkillsID15::activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    SGHeroLayer *hero = NULL;
    
	//回复我方血量
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        int blood = num;
        getmyHero()->changBloodNum(blood, false, true);
        hero = getmyHero();
    }else{
        int blood = num;
        getenemyHero()->changBloodNum(blood, false, true);
        hero = getenemyHero();
    }
    ShiBing *s = (ShiBing*) attackList->getAttackSbs()->objectAtIndex(0);
	//播放加血量的动画
    hero->showAddApEff(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound(), s->getPosition());
}
