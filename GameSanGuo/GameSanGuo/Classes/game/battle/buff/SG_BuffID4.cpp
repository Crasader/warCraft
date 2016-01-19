//
//  SG_BuffID4.cpp
//  GameSanGuo
//
//  Created by zenghui w on 14-5-12.
//
//

#include "SG_BuffID4.h"

SG_BuffID4::SG_BuffID4( SG_SkillsBase*skbase )
{
    setHeroLayer(skbase);
    setRoundNum(0);
    setAfftributeNum(0);
}

SG_BuffID4::~SG_BuffID4()
{
    
}

void SG_BuffID4::activateBuff(SGAttackList *attack)
{
    
    SGHeroLayer *hero = NULL;
    SGBattleMap *enemyBattleMap = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        hero = getmyHero();
        enemyBattleMap = getmyHero()->getBattleMap();
    }else{
        hero = getenemyHero();
        enemyBattleMap = getenemyHero()->getBattleMap();
    }

    CCLOG("attack->getAp()==%d",attack->getAp());
    int ap = attack->getAp() * this->getAfftributeNum();
    
    this->pushBuffData(CCString::createWithFormat("buff: %d, %d", this->getBuffID(), this->getRoundNum()));
    
    //attack->damageNum += ap;
    ap = attack->getAp() - ap;
    if (ap > 0) {
        hero->showBattleLabel(attack->getAp() - ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex());
        attack->changeAP(ap, true, true);
    }else{
        hero->showBattleLabel(attack->getAp(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex());
        attack->setAp(0);
        if (attack->getRoundV() == 0)
        {
            attack->setRoundV(1);
        }
        hero->getBattleMap()->removeAttackList(attack);
		//void SGBattleMap::removeAttackList(SGAttackList *attackList, SGHeroLayer *hero, SGGridBase *grid, bool isMe)
		//hero->getBattleMap()->removeAttackList(attack);
    }
    
}
