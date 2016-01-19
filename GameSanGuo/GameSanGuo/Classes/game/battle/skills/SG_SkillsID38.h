//
//  SG_SkillsID38.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-24.
//
//

#ifndef __GameSanGuo__SG_SkillsID38__
#define __GameSanGuo__SG_SkillsID38__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID38: public SG_SkillsBase {
    
    
public:
    SG_SkillsID38();
    ~SG_SkillsID38();
    virtual void activateSkills10(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};

#endif /* defined(__GameSanGuo__SG_SkillsID38__) */
