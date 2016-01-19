//
//  SG_SkillsID18.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-18.
//
//

#ifndef __GameSanGuo__SG_SkillsID18__
#define __GameSanGuo__SG_SkillsID18__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID18: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID18();
    ~SG_SkillsID18();
    
    virtual bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID18__) */
