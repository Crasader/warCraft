//
//  SG_SkillsID17.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-17.
//
//

#ifndef __GameSanGuo__SG_SkillsID17__
#define __GameSanGuo__SG_SkillsID17__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID17: public SG_SkillsBase {

    
public:
    
    SG_SkillsID17();
    ~SG_SkillsID17();
    
    virtual void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num); 
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID17__) */
