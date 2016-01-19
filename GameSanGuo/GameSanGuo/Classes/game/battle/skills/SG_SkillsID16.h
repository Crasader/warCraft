//
//  SG_SkillsID16.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#ifndef __GameSanGuo__SG_SkillsID16__
#define __GameSanGuo__SG_SkillsID16__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID16: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID16();
    ~SG_SkillsID16();
    
    
    virtual int activateSkills5(SGBattleMap *battleMap, SGAttackList *attackList, float num, ShiBing *sb);
};

#endif /* defined(__GameSanGuo__SG_SkillsID16__) */
