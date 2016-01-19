//
//  SG_SkillsID20.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-18.
//
//

#include "SG_SkillsID20.h"



SG_SkillsID20::SG_SkillsID20()
{
    setHeroLayer();
}

SG_SkillsID20::~SG_SkillsID20()
{
    
}

void SG_SkillsID20::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
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
    CCArray *array = enemyBattleMap->getAttackList();
    
    CCArray *officerArray = CCArray::create();
    CCObject *attackObj = NULL;
    sgAttackSoldierType attackType = kattacknormal;
    CCARRAY_FOREACH(array, attackObj)
    {
        attackType = ((SGAttackList *)attackObj)->getAttackType();
        if (attackType == kattackfour ||  attackType == kattacktwo) {
            officerArray->addObject(attackObj);
        }
    }
    
    if (officerArray->count()) {
        
        int ap = floor(attackList->getAp() * attackList->getAfftributeNum());
        attackList->changeAP(attackList->getAp() - ap, false, true);//自身攻击力降低一个百分比
        //attackList->drawSkillsEffect(0);
        hero->showBattleLabel(ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attackList->getAttackIndex());
        
		//对敌方攻击阵列(有武将的),随机一个攻击阵列,含有武将的杀死
        int randNum = 0;
        int a = attackList->getAfftributeNum2();
        randNum =  a%officerArray->count();
        
        SGAttackList *attack = (SGAttackList*)officerArray->objectAtIndex(randNum);
        
        hero->showEffectLayer(1, true, attack->getAttackIndex());
        hero->showEffectLayer(19, true, attack->getAttackIndex());
		//移除这个随机到的攻击队列
        enemyBattleMap->getHero_Obj()->showBattleLabel(attack->getAp(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attackList->getAttackIndex());
        enemyBattleMap->removeAttackList(attack, enemyBattleMap->getHero_Obj(), enemyBattleMap->myGrids[attack->getAttackIndex().i][attack->getAttackIndex().j]);
       
    }
}
