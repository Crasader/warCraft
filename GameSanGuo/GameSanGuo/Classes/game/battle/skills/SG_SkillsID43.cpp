//
//  SG_SkillsID43.cpp
//  GameSanGuo
//
//  Created by wwl on 14-2-20.
//
//

#include "SG_SkillsID43.h"

SG_SkillsID43::SG_SkillsID43()
{
    setHeroLayer();
}

SG_SkillsID43::~SG_SkillsID43()
{
    
}
//火计 蓄力阶段，每回合对敌方的所有攻击阵列造成25点火焰伤害
void SG_SkillsID43::activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    bool a = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    SGHeroLayer *hero = getenemyHero();
    if (isHero && a) {
        
    }else{
        hero = getmyHero();
    }
    
    CCArray *arr = CCArray::create();
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(hero->battleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList*)obj;
        hero->showEffectLayer(5, true, attack->getAttackIndex());
        hero->showBattleLabel(attackList->getAfftributeNum(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex());
        
        int ap = attack->getAp() - attackList->getAfftributeNum();
        if (ap > 0)
        {//没打死,改变攻击力
            attack->changeAP(ap, true, true);
            if (attackList->getAfftributeNum1() > 0)
            {
                attack->changeRound(attackList->getAfftributeNum1());
            }
        }
        else
        {//死掉的加入到待删除的数组中
            arr->addObject(attack);
        }
    }
    obj = NULL;
	//删除已经被打死的攻击阵列
    CCARRAY_FOREACH(arr, obj)
    {
        SGAttackList *attack = (SGAttackList*)obj;
        hero->battleMap->removeAttackList(attack);
    }
    
    CC_SAFE_DELETE(arr);
    arr = NULL;

    
    
}
