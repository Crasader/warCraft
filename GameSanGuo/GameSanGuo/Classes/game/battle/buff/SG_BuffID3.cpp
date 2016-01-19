//
//  SG_BuffID3.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-3-1.
//
//

#include "SG_BuffID3.h"

SG_BuffID3::SG_BuffID3( SG_SkillsBase*skbase )
{
    setHeroLayer(skbase);
    setRoundNum(0);
    setAfftributeNum(0);
}

SG_BuffID3::~SG_BuffID3()
{
    
}

void SG_BuffID3::activateBuff(SGAttackList *attack)
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
    int ap = this->getAfftributeNum();
    
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
