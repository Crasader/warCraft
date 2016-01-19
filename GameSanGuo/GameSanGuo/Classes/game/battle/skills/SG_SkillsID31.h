//
//  SG_SkillsID31.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#ifndef __GameSanGuo__SG_SkillsID31__
#define __GameSanGuo__SG_SkillsID31__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID31: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID31();
    ~SG_SkillsID31();
    
    virtual void activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
    void addGridDef(SGGridBase * grid, float num, bool right);

    
};




#endif /* defined(__GameSanGuo__SG_SkillsID31__) */
