//
//  SG_SkillsID31.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#include "SG_SkillsID31.h"

SG_SkillsID31::SG_SkillsID31()
{
    setHeroLayer();
}

SG_SkillsID31::~SG_SkillsID31()
{
    
}

void SG_SkillsID31::activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    ShiBing *bing = (ShiBing*)attackList->attackSbs->objectAtIndex(0);
    SGGridBase *grid = (SGGridBase*)bing->getPrinceGridArray()->objectAtIndex(0);
    addGridDef(grid, num, false);
    addGridDef(grid, num, true);
    
}

void SG_SkillsID31::addGridDef(SGGridBase * grid, float num, bool right)
{
    
    int rand = right ? 1 : -1;
    
    if (grid->getIndex().j + rand < 0 ||
        grid->getIndex().j + rand >= mapList) {
        return;
    }
    
    
    SGBattleMap *map = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        map = getmyHero()->getBattleMap();
    }else{
        map = getenemyHero()->getBattleMap();
    }
        
     
    if (grid->getIndex().j >= 0 &&
        grid->getIndex().j <= mapList - 1) {
        SGGridBase *grid1 = map->myGrids[grid->getIndex().i][grid->getIndex().j + rand];
        SGGridBase *grid2 = map->myGrids[grid->getIndex().i + 1][grid->getIndex().j + rand];
        SGSoldier *soldier = NULL;
        SGSoldier *soldier1 = NULL;
        if (grid1) {
            switch (grid1->getStype()) {
                case knormal:
                case kdefend:
                {
                    soldier = map->getShiBingWithGrid(grid1);
                    if(soldier)
                    {
                        int def = soldier->getDef() + soldier->getDef() * num;
                        soldier->setDef(def);
                        ((ShiBing*)soldier)->showDef(soldier->getDef());
                        ((ShiBing*)soldier)->showEffect(1);
                    }
                    
                }
                    break;
                case kattack:
                {
                    soldier = map->getAttackListByGrid(grid1);
                    if(soldier)
                    {
                        SGAttackList *attack = (SGAttackList*)soldier;
                        
                        int ap = attack->getAp() - attack->getSpAddAp();
                        attack->setSpAddAp(attack->getSpAddAp() + attack->getAp() * num);
                        attack->changeAP(attack->getSpAddAp() + ap, true);

                        attack->drawSkillsEffect(1);
                    }
                }
                    break;
                    
                default:
                    break;
            }
        }
        
        if (grid2) {
            switch (grid2->getStype()) {
                case knormal:
                case kdefend:
                {
                    soldier1 = map->getShiBingWithGrid(grid2);
                    if(soldier1)
                    {
                        if (soldier != soldier1) {
                            int def = soldier1->getDef() + soldier1->getDef() * num;
                            soldier1->setDef(def);
                            ((ShiBing*)soldier1)->showDef(soldier1->getDef());
                            ((ShiBing*)soldier1)->showEffect(1);
                        }
                    }
                }
                    break;
                case kattack:
                {
                    soldier1 = map->getAttackListByGrid(grid2);
                    if(soldier1)
                    {
                        if (soldier != soldier1) {
                            SGAttackList *attack = (SGAttackList*)soldier1;
                            int ap = attack->getAp() - attack->getSpAddAp();
                            attack->setSpAddAp(attack->getSpAddAp() + attack->getAp() * num);
                            attack->changeAP(attack->getSpAddAp() + ap,true);
    //                        CCObject *obj = NULL;
    //                        CCARRAY_FOREACH(attack->attackSbs, obj)
    //                        {
    //                            ShiBing *sb = ((ShiBing*)obj);
    //                            sb->showEffect(1);
    //                        }
                        
                            attack->drawSkillsEffect(1);
                        }
                    }
                    
                }
                    break;
                    
                default:
                    break;
            }
        }
    }

}



