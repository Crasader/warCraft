//
//  SG_BuffID2.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#include "SG_BuffID2.h"


SG_BuffID2::SG_BuffID2( SG_SkillsBase*skbase )
{
    setHeroLayer(skbase);
    setRoundNum(0);
    setAfftributeNum(0);
}

SG_BuffID2::~SG_BuffID2()
{
    
}

void SG_BuffID2::activateBuff(GameIndex index)
{
    SGBattleMap *enemyBattleMap = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        enemyBattleMap = getmyHero()->getBattleMap();
    }else{
        enemyBattleMap = getenemyHero()->getBattleMap();
    }
    
    SGGridBase *grid = enemyBattleMap->myGrids[index.i][index.j];
    if (grid &&
        grid->getStype() == kattack) {
        SGAttackList* attact = enemyBattleMap->getAttackListByGrid(grid);
        if (attact&&!attact->IsRoundBuff) {
            attact->changeRound(this->getAfftributeNum(), true);
            attact->IsRoundBuff = true;
        }
    }
    
    int rand = this->getRoundNum() - 1;
    this->setRoundNum(rand);
    this->pushBuffData(CCString::createWithFormat("buff: %d, %d", this->getBuffID(), this->getRoundNum()));
}