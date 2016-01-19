//
//  SG_SkillsID37.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#ifndef __GameSanGuo__SG_SkillsID37__
#define __GameSanGuo__SG_SkillsID37__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID37: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID37();
    ~SG_SkillsID37();
    
    virtual void activateBuffSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
};

#endif /* defined(__GameSanGuo__SG_SkillsID37__) */
