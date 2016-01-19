//
//  SG_SkillsID22.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#ifndef __GameSanGuo__SG_SkillsID22__
#define __GameSanGuo__SG_SkillsID22__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID22: public SG_SkillsBase {
    
    
public:
    
    
    SG_SkillsID22();
    ~SG_SkillsID22();

    virtual void activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num);

};

#endif /* defined(__GameSanGuo__SG_SkillsID22__) */
