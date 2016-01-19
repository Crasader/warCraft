//
//  SG_SkillsID11.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#ifndef __GameSanGuo__SG_SkillsID11__
#define __GameSanGuo__SG_SkillsID11__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID11: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID11();
    ~SG_SkillsID11();
    
    virtual void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};

#endif /* defined(__GameSanGuo__SG_SkillsID11__) */
