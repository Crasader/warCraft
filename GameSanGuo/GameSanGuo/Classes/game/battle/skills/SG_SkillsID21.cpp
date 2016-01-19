//
//  SG_SkillsID21.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#include "SG_SkillsID21.h"



SG_SkillsID21::SG_SkillsID21()
:isRevive(false)
{
    setHeroLayer();
}


SG_SkillsID21::~SG_SkillsID21()
{
    
}
//检测复活技能,除了传进这个攻击队列,设置其他攻击队列的可复活次数,复活的 攻击比例
void SG_SkillsID21::detectionReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    SGBattleMap *enemyBattleMap = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        enemyBattleMap = getmyHero()->getBattleMap();
    }else{
        
        enemyBattleMap = getenemyHero()->getBattleMap();
    }
    
        isRevive = true;

    CCObject *obj = NULL;
    CCARRAY_FOREACH(enemyBattleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        
		//检测在地图上的每一个攻击队列
        if (isRevive) {
            if (attack != attackList) {//除了引发这个技能的攻击队列(武将)
                if (/*attack->getReviveNum() == 0 &&*/
                    false == attack->getIsRevive()) {//并且本身没有复活
                    CCLOG("attack->getAfftributeNum()===%f",attackList->getAfftributeNum());
                    attack->setResurrectionScale(attackList->getAfftributeNum());//设置复活时攻击的比例
                    int a = attack->getReviveNum() + 1;//设置复活和次数
                    attack->setReviveNum(a);//设置复活的次数
                }
            }
        }
    }
}

void SG_SkillsID21::chackReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, SGAttackList *attackList1)
{
    if (this->isRevive) {
        int a = attackList->getReviveNum() + 1;
        attackList->setReviveNum(a);
        attackList->setResurrectionScale(attackList1->getAfftributeNum());
        CCLOG("attackList1->getAfftributeNum()==%f",attackList1->getAfftributeNum());
    }
}

void SG_SkillsID21::removeReviveAttack(SGBattleMap *battleMap, SGAttackList *attackList)
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
        if (//!attack->getIsDeath() &&
            attack != attackList)
        {
            int a = attack->getReviveNum();
            attack->setReviveNum((a > 0) ? (a - 1) : 0);
            if (a == 1) {
                attack->setResurrectionScale(0);
            }
        }
    }
}
