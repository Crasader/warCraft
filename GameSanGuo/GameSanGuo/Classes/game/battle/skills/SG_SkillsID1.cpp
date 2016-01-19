//
//  SG_SkillsID1.cpp
//  GameSanGuo
//  
//  Created by wenlong w on 13-1-11.
//
//

#include "SG_SkillsID1.h"



SG_SkillsID1::SG_SkillsID1()
:skillsId(-1)
{
    setHeroLayer();
}

SG_SkillsID1::~SG_SkillsID1(){
    
}
//煽动术 蓄力阶段，每回合增加我方其他蓄力部队5%攻击力
void SG_SkillsID1::activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(battleMap->getAttackList(), obj)
    {
        SGAttackList *attack = (SGAttackList *)obj;
        if (attack != attackList) {
            
            attack->drawSkillsEffect(2);
			
            
            int ap = attack->getMax_Ap() * num;
            attack->setSpAddAp(attack->getSpAddAp() + ap);
            attack->changeAP(attack->getAp() + ap, true);//设置除当attacklist之外的其他攻击队列的的攻击力
						
            ShiBing *sb = (ShiBing*)attack->attackSbs->objectAtIndex(0);

            getmyHero()->showBattleLabel(ap, "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, gameIndex(-sb->getSbindex().i, mapList - 1 - sb->getSbindex().j));
        }
    }
	
}
