//
//  SG_SkillsID7.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#ifndef __GameSanGuo__SG_SkillsID7__
#define __GameSanGuo__SG_SkillsID7__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID7: public SG_SkillsBase {
    
    
public:
    SG_SkillsID7();
    ~SG_SkillsID7();
    
    virtual bool activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};

#endif /* defined(__GameSanGuo__SG_SkillsID7__) */
