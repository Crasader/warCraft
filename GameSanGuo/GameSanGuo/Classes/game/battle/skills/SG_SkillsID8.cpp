//
//  SG_SkillsID8.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#include "SG_SkillsID8.h"


SG_SkillsID8::SG_SkillsID8()
{
    setHeroLayer();
}

SG_SkillsID8::~SG_SkillsID8()
{
    
}
//乾坤阵 攻击时，命中敌方底线后，降低我方主将计冷却1回合，增加对方主将计冷却0回合
bool SG_SkillsID8::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    SGHeroLayer *hero = getmyHero();
    SGHeroLayer *hero_ = getenemyHero();
    if(!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        hero = getenemyHero();
        hero_ = getmyHero();
        
    }
    hero->setSkillsRoundNum(-attackList->getAfftributeNum(), true);//降低自己方回合数
    hero_->setSkillsRoundNum(attackList->getAfftributeNum1(), true);//增加对方回合数
    
    return false;
}
