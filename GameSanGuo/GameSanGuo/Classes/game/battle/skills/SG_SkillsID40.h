//
//  SG_SkillsID40.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-24.
//
//

#ifndef __GameSanGuo__SG_SkillsID40__
#define __GameSanGuo__SG_SkillsID40__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID40: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID40();
    ~SG_SkillsID40();
    
    virtual int activateSkills5_1(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};


#endif /* defined(__GameSanGuo__SG_SkillsID40__) */
