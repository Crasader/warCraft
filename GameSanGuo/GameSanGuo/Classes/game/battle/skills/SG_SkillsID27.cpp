//
//  SG_SkillsID27.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#include "SG_SkillsID27.h"



SG_SkillsID27::SG_SkillsID27()
{
    setHeroLayer();
}

SG_SkillsID27::~SG_SkillsID27()
{
    
}


bool SG_SkillsID27::activateSkills11(SGBattleMap *battleMap, SGAttackList *attackList,  ShiBing *sb)
{
    if (attackList->getAp() > sb->getDef())
    {
        int ap = attackList->getAfftributeNum() * sb->getDef();
        
        attackList->changeAP(attackList->getAp() + ap);
        sb->setDef(0);
        return true;
    }
    return false;
    
}

