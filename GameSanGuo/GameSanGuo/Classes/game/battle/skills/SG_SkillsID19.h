//
//  SG_SkillsID19.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-18.
//
//

#ifndef __GameSanGuo__SG_SkillsID19__
#define __GameSanGuo__SG_SkillsID19__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID19: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID19();
    ~SG_SkillsID19();
    
    virtual bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID19__) */
