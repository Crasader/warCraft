//
//  SG_SkillsID4.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#ifndef __GameSanGuo__SG_SkillsID4__
#define __GameSanGuo__SG_SkillsID4__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID4: public SG_SkillsBase{
    
public:
    SG_SkillsID4();
    ~SG_SkillsID4();
    
    
    virtual void activateSkills(bool isHero,SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID4__) */
