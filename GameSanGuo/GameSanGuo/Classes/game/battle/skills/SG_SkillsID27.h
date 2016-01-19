//
//  SG_SkillsID27.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#ifndef __GameSanGuo__SG_SkillsID27__
#define __GameSanGuo__SG_SkillsID27__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID27: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID27();
    ~SG_SkillsID27();
    
    virtual bool activateSkills11(SGBattleMap *battleMap, SGAttackList *attackList,  ShiBing *sb);
};

#endif /* defined(__GameSanGuo__SG_SkillsID27__) */
