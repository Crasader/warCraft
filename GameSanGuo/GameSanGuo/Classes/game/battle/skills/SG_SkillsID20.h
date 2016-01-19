//
//  SG_SkillsID20.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-18.
//
//

#ifndef __GameSanGuo__SG_SkillsID20__
#define __GameSanGuo__SG_SkillsID20__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID20: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID20();
    ~SG_SkillsID20();
    
    virtual void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID20__) */
