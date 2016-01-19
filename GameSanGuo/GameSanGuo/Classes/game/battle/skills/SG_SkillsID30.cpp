//
//  SG_SkillsID30.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#include "SG_SkillsID30.h"



SG_SkillsID30::SG_SkillsID30()
{
    setHeroLayer();
}


SG_SkillsID30::~SG_SkillsID30()
{
    
}
//穿刺..命中蓄力单位后对其后方格子造成x%伤害现改为按格子计算放弃蓄力单位这种描述与实现
void SG_SkillsID30::activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage)
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
    
     SGGridBase *grida = enemyBattleMap->myGrids[index.i+1][index.j];
     if(grida)
     {
         hero->showEffectLayer(6, false, gameIndex(index.i + 1, index.j));
         int apNum =ceil( damage * num);
         attackGrid(enemyBattleMap, grida, apNum);

     }
    
  
    
}


void SG_SkillsID30::attackGrid(SGBattleMap* map,SGGridBase *sgrid, int num)
{
    if(num<=0)
        return;
    SGSoldier *soldier = NULL;
    switch (sgrid->getStype()) {
        case knormal:
        {
            soldier = map->getShiBingWithGrid(sgrid);
            if(soldier)
            {
                int def = ((ShiBing*)soldier)->getDef();
                if(def>num)
                {
                    map->getHero_Obj()->showBattleLabel(((ShiBing*)soldier)->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                }else
                {
                    map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                }
                map->removeAttackedSB((ShiBing*)soldier, soldier->getAp(), map->getHero_Obj());
            }
        }
            break;
        case kdefend:
        {
            soldier = map->getShiBingWithGrid(sgrid);
            if(soldier)
            {
                if(soldier->getDef() > num)
                {
                    soldier->setDef(soldier->getDef() - num);
                    ShiBing *bing = (ShiBing *) soldier;
                    map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, bing->getSbindex());
                    bing->showDef(soldier->getDef());
                }else{
                    map->getHero_Obj()->showBattleLabel(((ShiBing*)soldier)->getDef(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, ((ShiBing*)soldier)->getSbindex());
                    map->removeAttackedSB((ShiBing*)soldier, soldier->getAp(), map->getHero_Obj());
                }
            }
        }
            break;
        case kattack:
        {
            soldier = map->getAttackListByGrid(sgrid);
            if(soldier)
            {
                int ap = ((SGAttackList*)soldier)->attackSubAp(num);
                
                map->getHero_Obj()->showBattleLabel(num, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, sgrid->getIndex());
                if (ap > 0) {
                    ((SGAttackList*)soldier)->changeAP(ap, true, true);
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
