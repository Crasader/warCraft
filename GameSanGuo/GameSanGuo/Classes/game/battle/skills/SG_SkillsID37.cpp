//
//  SG_SkillsID37.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#include "SG_SkillsID37.h"


SG_SkillsID37::SG_SkillsID37()
{
    setHeroLayer();
   ResourceManager::sharedInstance()->bindTexture("animationFile/zd.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
}

SG_SkillsID37::~SG_SkillsID37()
{
    ;
}

//淬毒 攻击时，使被命中的敌方攻击阵列中毒，每回合降低其当前攻击力4%(一个技能buff)
void SG_SkillsID37::activateBuffSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    
    SG_BuffBase *buff = SG_BuffManager::initBuffManager(this, attackList)->getbfBase();
    attackList1->setBuffListObj(buff);
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList1->attackSbs, obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        sb->show_Buff_Effect();//显示毒特效
    }

}