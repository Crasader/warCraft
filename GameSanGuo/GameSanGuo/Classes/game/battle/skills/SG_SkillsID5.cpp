//
//  SG_SkillsID5.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#include "SG_SkillsID5.h"


SG_SkillsID5::SG_SkillsID5()
{
    setHeroLayer();
}

SG_SkillsID5::~SG_SkillsID5()
{

}

bool SG_SkillsID5::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    int a = attackList->getAfftributeNum2();
    int b = attackList->getAfftributeNum() * 100;
    if (a <= b) {
        return true;
    }
    
    return false;
}