//
//  SG_SkillsID30.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#ifndef __GameSanGuo__SG_SkillsID30__
#define __GameSanGuo__SG_SkillsID30__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID30: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID30();
    ~SG_SkillsID30();
    
    virtual void activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage);
    void attackGrid(SGBattleMap* map,SGGridBase *sgrid, int num);
    
};


#endif /* defined(__GameSanGuo__SG_SkillsID30__) */
