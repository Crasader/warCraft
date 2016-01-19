//
//  SG_SkillsID29.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#ifndef __GameSanGuo__SG_SkillsID29__
#define __GameSanGuo__SG_SkillsID29__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID29: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID29();
    ~SG_SkillsID29();
    
    virtual void activatePassivitySkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num);

    
};



#endif /* defined(__GameSanGuo__SG_SkillsID29__) */
