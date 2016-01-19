//
//  SG_SkillsID28.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#include "SG_SkillsID28.h"

SG_SkillsID28::SG_SkillsID28()
{
    setHeroLayer();
}

SG_SkillsID28::~SG_SkillsID28()
{
    
}
//破城 攻击时，对攻击路线上互相连接的敌方防御墙造成伤害。伤害值每次传递降低81%
void SG_SkillsID28::activateSkills12(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1)
{
    bool isme = false;
    SGBattleMap *enemyBattleMap = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        enemyBattleMap = getenemyHero()->getBattleMap();
        isme = true;
    }else{
        enemyBattleMap = getmyHero()->getBattleMap();
    }
    
    int a = attackList->getAp() * num;
    int apNum = attackList->getAp() - a;
    int apNum1 = attackList->getAp() - a;   
    
	// 攻击相连的城墙,
    for (int i = index.j + 1; i < mapList; i++) {
        SGGridBase *grid = enemyBattleMap->myGrids[index.i][i];
        
        if (grid &&
            grid->getStype() == kdefend &&
            apNum > 0) {
            attackGrid(enemyBattleMap, grid, apNum);
            getmyHero()->showEffectLayer(5, false, (isme ? gameIndex(-(index.i + 1), mapRow - 1 - i) : gameIndex(index.i, i)));
            int b = apNum * num;
            apNum = apNum - b;//攻击值每次递减
        }else
        {
            break;
        }
    }
    
    for (int i = index1.j - 1; i >= 0 ; i--) {
        SGGridBase *grid = enemyBattleMap->myGrids[index1.i][i];
        
        if (grid &&
            grid->getStype() == kdefend &&
            apNum1 > 0) {
            getmyHero()->showEffectLayer(5, false, (isme ? gameIndex(-(index.i + 1), mapRow - 1 - i) : gameIndex(index.i, i)));
            attackGrid(enemyBattleMap, grid, apNum1);
            int b = apNum1 * num;
            apNum1 = apNum1 - b;
        }else
        {
            break;
        }
    }
}

void SG_SkillsID28::attackGrid(SGBattleMap* map,SGGridBase *sgrid, int num)
{
    SGSoldier *soldier = NULL;

    soldier = map->getShiBingWithGrid(sgrid);
    if(soldier){
        map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*) soldier)->getSbindex());
        if(soldier->getDef() > num)
        {
            soldier->setDef(soldier->getDef() - num);
            ShiBing *bing = (ShiBing *) soldier;
            bing->showDef(soldier->getDef());
            ((ShiBing*)soldier)->setDefImage();
        }else{
            map->removeAttackedSB((ShiBing*)soldier, soldier->getAp(), map->getHero_Obj());
        }
    }
    
    

}


