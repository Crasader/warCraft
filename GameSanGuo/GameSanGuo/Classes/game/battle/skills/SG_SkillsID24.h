//
//  SG_SkillsID24.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#ifndef __GameSanGuo__SG_SkillsID24__
#define __GameSanGuo__SG_SkillsID24__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID24: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID24();
    ~SG_SkillsID24();

    virtual void activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage);

    
    void attackGrid(SGBattleMap* map,SGGridBase* sgrid, int num);
    
    
};


#endif /* defined(__GameSanGuo__SG_SkillsID24__) */
