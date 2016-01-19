//
//  SG_SkillsID28.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#ifndef __GameSanGuo__SG_SkillsID28__
#define __GameSanGuo__SG_SkillsID28__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID28: public SG_SkillsBase {
    
    
public:
    SG_SkillsID28();
    ~SG_SkillsID28();
    
    virtual void activateSkills12(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1);
    void attackGrid(SGBattleMap* map,SGGridBase *sgrid, int num);
};


#endif /* defined(__GameSanGuo__SG_SkillsID28__) */
