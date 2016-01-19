//
//  SG_SkillsID34.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#ifndef __GameSanGuo__SG_SkillsID34__
#define __GameSanGuo__SG_SkillsID34__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID34: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID34();
    ~SG_SkillsID34();
    
    virtual void activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index);
};


#endif /* defined(__GameSanGuo__SG_SkillsID34__) */
