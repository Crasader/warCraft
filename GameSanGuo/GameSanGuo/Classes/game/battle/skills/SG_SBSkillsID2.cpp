//
//  SG_SBSkillsID2.cpp
//  GameSanGuo
//
//  Created by wwl on 14-2-25.
//
//

#include "SG_SBSkillsID2.h"

SG_SBSkillsID2::SG_SBSkillsID2()
{
    
}


SG_SBSkillsID2::~SG_SBSkillsID2()
{
    
}

void SG_SBSkillsID2::activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    //if (attackList1->getAttackType() != kattacknormal)
    {
        attackList1->changeRound(attackList->getAfftributeNum1(), false);
    }
    
    SG_BuffBase *buff = SG_BuffManager::initBuffManager(this, attackList)->getbfBase();
    attackList1->setBuffListObj(buff);
    CCObject *obj = NULL;
    CCARRAY_FOREACH(attackList1->attackSbs, obj)
    {
        ShiBing *sb = (ShiBing*)obj;
        sb->show_Buff_Effect();
    }
    
}
