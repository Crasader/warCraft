//
//  SG_SkillsID9.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#ifndef __GameSanGuo__SG_SkillsID9__
#define __GameSanGuo__SG_SkillsID9__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID9: public SG_SkillsBase {
    
    
public:
    SG_SkillsID9();
    ~SG_SkillsID9();
    
    virtual bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};

#endif /* defined(__GameSanGuo__SG_SkillsID9__) */
