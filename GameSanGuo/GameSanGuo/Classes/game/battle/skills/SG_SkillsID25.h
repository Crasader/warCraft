//
//  SG_SkillsID25.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#ifndef __GameSanGuo__SG_SkillsID25__
#define __GameSanGuo__SG_SkillsID25__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID25: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID25();
    ~SG_SkillsID25();
    
    virtual void activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1);
};


#endif /* defined(__GameSanGuo__SG_SkillsID25__) */
