//
//  SG_BuffID1.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-17.
//
//

#include "SG_BuffID1.h"

SG_BuffID1::SG_BuffID1( SG_SkillsBase*skbase )
:sbEff(NULL)
{
    setHeroLayer(skbase);
    setRoundNum(0);
    setAfftributeNum(0);
}

SG_BuffID1::~SG_BuffID1()
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("animationFile/dmdy.plist");
}
//干掉网格上所有的散兵(?)
void SG_BuffID1::activateBuff(GameIndex index)
{
    SGBattleMap *enemyBattleMap = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        enemyBattleMap = getmyHero()->getBattleMap();
    }else{
        enemyBattleMap = getenemyHero()->getBattleMap();
    }
    
    this->pushBuffData(CCString::createWithFormat("buff: %d, %d", this->getBuffID(), this->getRoundNum()));
    
    SGGridBase *grid = enemyBattleMap->myGrids[index.i][index.j];
    if (grid &&
        grid->getStype() == knormal) {
        ShiBing *sb = enemyBattleMap->getShiBingWithGrid(grid);
        if(sb)
        {
            enemyBattleMap->getHero_Obj()->showBattleLabel(sb->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sb->getSbindex());
            enemyBattleMap->removeAttackedSB(sb, 0, enemyBattleMap->getHero_Obj());
        }
    }
    
    int rand = this->getRoundNum() - 1;
    this->setRoundNum(rand);
    
    CCLOG("删除格子上的所有散兵。。");
}


