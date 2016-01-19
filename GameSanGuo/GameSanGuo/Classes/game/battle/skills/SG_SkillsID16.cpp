//
//  SG_SkillsID16.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#include "SG_SkillsID16.h"

SG_SkillsID16::SG_SkillsID16()
{
    setHeroLayer();
}

SG_SkillsID16::~SG_SkillsID16()
{
    
}

//战意 对敌方未形成阵列的士兵和武将造成伤害时，将伤害值的8%转化为自身的攻击力( 不是伤害是士兵的防御)
int SG_SkillsID16::activateSkills5(SGBattleMap *battleMap, SGAttackList *attackList, float num, ShiBing *sb)
{
    getenemyHero()->showEffectLayer(18, true, sb->getSbindex());
    attackList->drawSkillsEffect(2);
    int a = (num * attackList->getAfftributeNum());
    int ap = num - a;
    return ap;
}
