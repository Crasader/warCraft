//
//  SG_SkillsID32.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-23.
//
//

#ifndef __GameSanGuo__SG_SkillsID32__
#define __GameSanGuo__SG_SkillsID32__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID32: public SG_SkillsBase {
    
    
public:

    SG_SkillsID32();
    ~SG_SkillsID32();
    
    virtual void activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    
};


#endif /* defined(__GameSanGuo__SG_SkillsID32__) */
