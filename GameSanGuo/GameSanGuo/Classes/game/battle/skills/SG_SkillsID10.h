//
//  SG_SkillsID10.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#ifndef __GameSanGuo__SG_SkillsID10__
#define __GameSanGuo__SG_SkillsID10__

#include <iostream>
#include "SG_SkillsBase.h"



class SG_SkillsID10: public SG_SkillsBase {
    
    
public:
    SG_SkillsID10();
    
    ~SG_SkillsID10();
    
    virtual void activateSkills4(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
};


#endif /* defined(__GameSanGuo__SG_SkillsID10__) */
