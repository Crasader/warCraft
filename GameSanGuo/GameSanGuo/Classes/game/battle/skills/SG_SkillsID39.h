//
//  SG_SkillsID39.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-24.
//
//

#ifndef __GameSanGuo__SG_SkillsID39__
#define __GameSanGuo__SG_SkillsID39__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID39: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID39();
    ~SG_SkillsID39();

    virtual bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);

};

#endif /* defined(__GameSanGuo__SG_SkillsID39__) */
