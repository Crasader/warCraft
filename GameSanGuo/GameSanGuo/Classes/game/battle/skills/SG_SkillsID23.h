//
//  SG_SkillsID23.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#ifndef __GameSanGuo__SG_SkillsID23__
#define __GameSanGuo__SG_SkillsID23__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID23: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID23();
    ~SG_SkillsID23();
    
    virtual void activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    virtual bool assistSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};

#endif /* defined(__GameSanGuo__SG_SkillsID23__) */
