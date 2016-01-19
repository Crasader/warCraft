//
//  SG_SkillsID8.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#ifndef __GameSanGuo__SG_SkillsID8__
#define __GameSanGuo__SG_SkillsID8__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID8: public SG_SkillsBase {
    
    
public:
    SG_SkillsID8();
    ~SG_SkillsID8();
    
    virtual bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};

#endif /* defined(__GameSanGuo__SG_SkillsID8__) */
