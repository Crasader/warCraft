//
//  SG_SkillsID35.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#include "SG_SkillsID35.h"

SG_SkillsID35::SG_SkillsID35()
{
    setHeroLayer();
}

SG_SkillsID35::~SG_SkillsID35()
{
    
}
//疗伤 蓄力阶段，每回合为我方所有受伤蓄力阵列恢复其最大攻击力的6%
void SG_SkillsID35::activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    SGHeroLayer *hero = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        hero = getenemyHero();
    }else{
        hero = getmyHero();
    }
    CCObject *obj = NULL;
    CCARRAY_FOREACH(battleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
//        if (attack != attackList) {
            if (attack->damageNum > 0 &&
                attack->getAp() < attack->getMax_Ap()) {
                CCObject *sbobj = NULL;
                CCARRAY_FOREACH(attack->attackSbs, sbobj)
                {
                    ShiBing *sb = (ShiBing*)sbobj;
                    sb->showEffect(0);
                }
                
                int ap = attack->damageNum * num;
                attack->setSpAddAp(attack->getSpAddAp() + ap);
                attack->changeAP(attack->getAp() + ap, true);
                hero->showBattleLabel(ap, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex());
                
//            }
        }
    }
    
    attackList->drawSkillsEffect(8);
    
}
