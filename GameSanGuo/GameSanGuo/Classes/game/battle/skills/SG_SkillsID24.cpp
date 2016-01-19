//
//  SG_SkillsID24.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#include "SG_SkillsID24.h"


SG_SkillsID24::SG_SkillsID24()
{
    setHeroLayer();
}


SG_SkillsID24::~SG_SkillsID24()
{
    
}
//激活溅射技能,攻击时，对攻击目标左右单位造成当前攻击力5%伤害......更新14.10.15 by angel 這個技能改為傷害攻擊格子的左右各一格
void SG_SkillsID24::activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num,  GameIndex index, GameIndex index1,int damage)
{
    GPCCLOG("SG_SkillsID30::activateSkills8");
    SGBattleMap *enemyBattleMap = NULL;
    SGHeroLayer *hero = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        enemyBattleMap = getenemyHero()->getBattleMap();
        hero = getenemyHero();
    }else{
        enemyBattleMap = getmyHero()->getBattleMap();
        hero = getmyHero();
    }
    
    hero->showEffectLayer(5, false, index);
    	//算出被攻击位置的左右位置
    //int indexi = (index.i + 1 == mapRow) ? (index.i) : (index.i + 1);
    int indexjr = index.j - 1 ;
    int indexjl = index.j + 1 ;
    
    
    int apNum = 0;
    apNum = ceil(damage * num);
    
    if(indexjr>= 0 &&indexjr < mapList)
    {      
        SGGridBase *gridr = enemyBattleMap->myGrids[index.i][indexjr];
        if (gridr) {
            attackGrid(enemyBattleMap, gridr, apNum);
        }

    }
    if(indexjl>= 0 &&indexjl < mapList)
    {
        SGGridBase *gridl = enemyBattleMap->myGrids[index.i][indexjl];
        if (gridl) {
            attackGrid(enemyBattleMap, gridl, apNum);
        }
        
    }
    
}

void SG_SkillsID24::attackGrid(SGBattleMap* map,SGGridBase *sgrid, int num)
{
    if (!sgrid ||
        sgrid->getIndex().i >= mapRow ||
        sgrid->getIndex().j >= mapList ||
        sgrid->getIndex().j < 0||num<= 0) {
        return;
    }
    
    SGSoldier *soldier = NULL;
    switch (sgrid->getStype()) {
        case knormal:
        {
            soldier = map->getShiBingWithGrid(sgrid);
            if (soldier) {
                int def = soldier->getAp();
                if(def>num)
                {
                    map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                }else{
                    map->getHero_Obj()->showBattleLabel(soldier->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                }
                map->removeAttackedSB((ShiBing*)soldier, soldier->getAp());
            }
            
        }
            break;
        case kdefend:
        {
            soldier = map->getShiBingWithGrid(sgrid);
            if (soldier) {
                if(soldier->getDef() > num)
                {
                    soldier->setDef(soldier->getDef() - num);
                    map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                    ShiBing *bing = (ShiBing *) soldier;
                    bing->showDef(soldier->getDef());
                }else{
                    map->getHero_Obj()->showBattleLabel(soldier->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                    map->removeAttackedSB((ShiBing*)soldier, soldier->getAp(), map->getHero_Obj());
                }
            }
        }
            break;
        case kattack:
        {
            SGAttackList * soldier = map->getAttackListByGrid(sgrid);

            if (soldier) {
                int ap = soldier->attackSubAp(num);
                
                map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, soldier->getAttackIndex());
                if (ap > 0) {
                    soldier->changeAP(ap, false, true);
                }else{
                    map->removeAttackList((SGAttackList *)soldier, map->getHero_Obj(), sgrid);
                }
            }
            
        }
            break;
            
        default:
            break;
    }
}


