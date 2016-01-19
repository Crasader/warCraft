//
//  SG_SkillsID42.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-2-27.
//
//

#include "SG_SkillsID42.h"


SG_SkillsID42::SG_SkillsID42()
{
    setHeroLayer();
}


SG_SkillsID42::~SG_SkillsID42()
{
    
}

//电击 蓄力完成后，对敌方所有阵列造成101点雷击伤害 // 灼烧,蓄力完成后，对敌方所有阵列造成50点火焰伤害，并使这些阵列延长1个蓄力回合
void SG_SkillsID42::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    SGHeroLayer *hero = getenemyHero();
    if (!SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound()) {
        hero = getmyHero();
    }
    
    if (attackList->getAfftributeNum1() > 0)
    {
        //蓄力完成后，对敌方所有阵列造成50点火焰伤害，并使这些阵列延长1个蓄力回合,针对于"灼烧"的技能
        attackList->drawSkillsEffect(9);
    }

    //attackList->drawSkillsEffect(9);
    
    CCArray *arr = CCArray::create();
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(hero->battleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList*)obj;
        if (attackList->getSkillsId() == SKILLS_ID3) {//不同的特效
            hero->showEffectLayer(20, true, attack->getAttackIndex());
        }else{
            hero->showEffectLayer(21, true, attack->getAttackIndex());
        }
        //减去相应的攻击数值
        int ap = attack->attackSubAp(attackList->getAfftributeNum());
        hero->showBattleLabel(attackList->getAfftributeNum(), "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, attack->getAttackIndex());
        if (ap > 0) {//攻击阵列没有死掉
            attack->changeAP(ap, true, true);//设置新的攻击数值
            if (attackList->getAfftributeNum1() > 0) {//蓄力完成后，对敌方所有阵列造成50点火焰伤害，并使这些阵列延长1个蓄力回合,针对于"灼烧"的技能
                attack->changeRound(attackList->getAfftributeNum1());//延长其续力回合
            }
        }else{//打死,放入待删除的数组中
            arr->addObject(attack);
        }
    }
    obj = NULL;
	//清除已经死掉地攻击阵列
    CCARRAY_FOREACH(arr, obj)
    {
        SGAttackList *attack = (SGAttackList*)obj;
        hero->battleMap->removeAttackList(attack);
    }
    
    CC_SAFE_DELETE(arr);
    arr = NULL;
}



