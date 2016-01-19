//
//  SG_SkillsID42.h
//  GameSanGuo
//
//  Created by wenlong w on 13-2-27.
//
//

#ifndef __GameSanGuo__SG_SkillsID42__
#define __GameSanGuo__SG_SkillsID42__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID42: public SG_SkillsBase{
    
public:
    SG_SkillsID42();
    ~SG_SkillsID42();
    
    virtual void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};


#endif /* defined(__GameSanGuo__SG_SkillsID42__) */
