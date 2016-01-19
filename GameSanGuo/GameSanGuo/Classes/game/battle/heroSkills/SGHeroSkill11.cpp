//
//  SGHeroSkill11.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill11.h"
#include "SGSkillManager.h"

SGHeroSkill11::SGHeroSkill11()
{
    
}

SGHeroSkill11::~SGHeroSkill11()
{
    
}
//  要武将位置和和伤害  改

// 同时降低敌方一名四格武将和己方一名四格武将，当前攻击力的50%
//（优先蓄力武将，其次散兵武将，对其造成他攻击力50%的伤害。）
void SGHeroSkill11::activateSkill_20012(SGHeroLayer *heroLayer,SGHeroLayer *heroLayer1, float value1, int s_id, CCPoint pos)
{
    float value = value1;
    CCLOG("value1:%f",value1);
    CCLOG("value:%f",value);
    
    checkTheHero(heroLayer, value1, s_id, pos);
}

void SGHeroSkill11::checkTheHero(SGHeroLayer *hero, float value1, int s_id, CCPoint pos)
{
    if (pos.x < 0 || pos.y < 0) {
        return;
    }
    CCLOG("value1::::%f",value1);
    
    int i = pos.y;
    int j = pos.x;
    SGGridBase *grid = hero->battleMap->myGrids[i][j];
    if (grid)
    {
        if (grid->getStype() == kattack) {
            SGAttackList* attack = hero->battleMap->getAttackListByGrid(grid);
            if(attack)
            {
                attack->drawSkillsEffect(0);
                int ap = attack->getMax_Ap() * value1;
                CCLOG("nowap:%d",attack->getAp());
                CCLOG("ap:%d",ap);
                int a = attack->getAp();
                attack->setAp(attack->getAp() - ap);
                if ((grid->getStype() == kattackfour || grid->getStype() == kattacktwo) && ap > 0)
                {
                    attack->playEffectWithType(kAttackEffect);
                }
                
                CCLOG("afterap:%d",attack->getAp());
                
                
                if (attack->getAp()<=0)
                {
                    attack->heroSkillSubAp(a);
                    hero->battleMap->removeAttackList(attack);
                }else{
                    attack->heroSkillSubAp(ap);
                    attack->changeAP(attack->getAp(), true, true);
                }
            }
        }else if (grid->getStype() == knormal)
        {
            ShiBing *bing = hero->battleMap->getShiBingWithGrid(grid);
            if(bing)
                hero->battleMap->removeAttackedSB(bing, 0);
        }
        
    }else{
        CCLOG("哎呀 妈呀  随机位置竟然是空的 行==%d，  列==%d", i, j);
    }
    
    return;
    sgShiBingType sbType;
    sgAttackSoldierType type;
    if (s_id == skill_id_20012)
    {
        sbType = ksbfour;
        type = kattackfour;
    }else{
        sbType = ksbtwo;
        type = kattacktwo;
    }
    
    bool isFind = false;
    for (int i = 0; i < mapRow; i++)
    {
        for (int j = 0; j < mapList; j++)
        {
            SGGridBase *grid = hero->battleMap->myGrids[i][j];
            if (grid && grid->getStype() == kattack)
            {
                SGAttackList* attack = hero->battleMap->getAttackListByGrid(grid);
                if (attack&&attack->getAttackType() == type)
                {
                    attack->drawSkillsEffect(0);
                    int ap = attack->getMax_Ap() * value1;
                    CCLOG("nowap:%d",attack->getAp());
                    CCLOG("ap:%d",ap);
                    int a = attack->getAp();
                    attack->setAp(attack->getAp() - ap);
					
					if (ap > 0)
					{
						attack->playEffectWithType(kAttackEffect);
					}
					
                    CCLOG("afterap:%d",attack->getAp());
                    
                    
                    hero->showBattleLabel( ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, grid->getIndex());
                    
                    if (attack->getAp()<=0)
                    {
                        attack->heroSkillSubAp(a);
                        hero->battleMap->removeAttackList(attack);
                    }else{
                        attack->heroSkillSubAp(ap);
                        attack->changeAP(attack->getAp(), true, true);
                    }
                    
                    isFind  = true;
                    break;
                }
                
            }
        }
        if (isFind)
        {
            break;
        }
    }
    
    if (!isFind)
    {
        for (int i = 0; i < mapList; i++)
        {
            for (int j = 0; j < mapRow; j++)
            {
                SGGridBase *grid = hero->battleMap->myGrids[j][i];
                if (grid && grid->getStype() == knormal && grid->getSbType() == sbType)
                {
                    isFind = true;
                    ShiBing *bing = hero->battleMap->getShiBingWithGrid(grid);
                    if(bing)
                        hero->battleMap->removeAttackedSB(bing, 0);
                    break;
                    //                    if (bing->getSbType() == ksbfour)
                    //                    {
                    //                        hero->battleMap->removeAttackedSB(bing, 0);
                    //                    }
                }
            }
            if (isFind)
            {
                break;
            }
        }
    }
    
}