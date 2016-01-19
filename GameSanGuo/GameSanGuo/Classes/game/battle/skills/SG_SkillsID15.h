//
//  SG_SkillsID15.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#ifndef __GameSanGuo__SG_SkillsID15__
#define __GameSanGuo__SG_SkillsID15__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID15: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID15();
    ~SG_SkillsID15();
  
    virtual void activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID15__) */
