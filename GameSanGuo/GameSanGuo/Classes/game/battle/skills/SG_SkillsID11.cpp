//
//  SG_SkillsID11.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#include "SG_SkillsID11.h"


SG_SkillsID11::SG_SkillsID11()
{
    setHeroLayer();
}

SG_SkillsID11::~SG_SkillsID11()
{
    
}
////隐忍 汲魂术 反戈一击 蓄力阶段，受到的所有伤害，将在自身发动攻击时以16%转化为攻击力（如果自己没被打死的话）
void SG_SkillsID11::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    attackList->drawSkillsEffect(11);
    CCLOG("attackList->damageNum==%d",attackList->damageNum);
    int damage = floor(attackList->damageNum * attackList->getAfftributeNum());//将自身的受到伤害的百分比 做为攻击力
    attackList->changeAP(attackList->getAp() + damage);//加上攻击力
    
}

