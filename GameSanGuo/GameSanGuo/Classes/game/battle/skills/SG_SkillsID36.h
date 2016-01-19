//
//  SG_SkillsID36.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#ifndef __GameSanGuo__SG_SkillsID36__
#define __GameSanGuo__SG_SkillsID36__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID36: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID36();
    ~SG_SkillsID36();
    
    virtual void activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
};


#endif /* defined(__GameSanGuo__SG_SkillsID36__) */
