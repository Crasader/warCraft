//
//  SG_SkillsID14.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#ifndef __GameSanGuo__SG_SkillsID14__
#define __GameSanGuo__SG_SkillsID14__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID14: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID14();
    ~SG_SkillsID14();

    
    virtual void activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1, int row = 0);
    
    void addBuff();
    
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID14__) */
