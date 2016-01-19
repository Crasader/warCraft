//
//  SG_SkillsID35.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#ifndef __GameSanGuo__SG_SkillsID35__
#define __GameSanGuo__SG_SkillsID35__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID35: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID35();
    ~SG_SkillsID35();
    
    virtual void activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};

#endif /* defined(__GameSanGuo__SG_SkillsID35__) */
