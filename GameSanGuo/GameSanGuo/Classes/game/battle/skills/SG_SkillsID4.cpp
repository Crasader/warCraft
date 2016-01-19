//
//  SG_SkillsID4.cpp
//  GameSanGuo
//  
//  Created by wenlong w on 13-1-14.
//
//

#include "SG_SkillsID4.h"


SG_SkillsID4::SG_SkillsID4()
{
    setHeroLayer();
}

SG_SkillsID4::~SG_SkillsID4(){
    
    
}
//威慑 蓄力阶段，每回合降低敌方当前最高攻击力阵列2%攻击力
void SG_SkillsID4::activateSkills(bool isHero,SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    CCObject *obj = NULL;
    SGBattleMap *enemyBattleMap = NULL;
    
    bool a = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();

    if (isHero && a) {
        enemyBattleMap = getenemyHero()->getBattleMap();
    }else{
        enemyBattleMap = getmyHero()->getBattleMap();
    }
    
    
//    if (!isHero) {
//        enemyBattleMap = getmyHero()->getBattleMap();
//    }else{
//        enemyBattleMap = getenemyHero()->getBattleMap();
//    }
    
    int ap = 0;
    CCArray *array = CCArray::create();
    CCARRAY_FOREACH(enemyBattleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        if (ap == 0) {
            ap = attack->getAp();
        }
        if (ap <= attack->getAp()) {
            if (ap == attack->getAp()) {
                array->addObject(attack);
            }else{
                array->removeAllObjects();
                ap = attack->getAp();
                array->addObject(attack);
            }
            
        }
    }
    
    CCObject *attackobj = NULL;
    int subAp = 0;
    CCARRAY_FOREACH(array, attackobj)
    {
        SGAttackList *attack = (SGAttackList *)attackobj;
        subAp = attack->getAp() * num;
        int ap = attack->attackSubAp(subAp);
         enemyBattleMap->getHero_Obj()->showBattleLabel(subAp, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex());
        attack->drawSkillsEffect(0);
        if (ap > 0) {
            attack->changeAP(ap, true, true);
        }else{
            enemyBattleMap->removeAttackList(attack);
        }
    }
}


