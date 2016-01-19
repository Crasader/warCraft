//
//  SG_SkillsID41.h
//  GameSanGuo
//
//  Created by wenlong w on 13-2-27.
//
//

#ifndef __GameSanGuo__SG_SkillsID41__
#define __GameSanGuo__SG_SkillsID41__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID41: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID41();
    ~SG_SkillsID41();
    
    virtual void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID41__) */
