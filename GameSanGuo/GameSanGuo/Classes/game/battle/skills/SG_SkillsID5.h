//
//  SG_SkillsID5.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#ifndef __GameSanGuo__SG_SkillsID5__
#define __GameSanGuo__SG_SkillsID5__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID5: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID5();
    ~SG_SkillsID5();
    
    virtual bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID5__) */
