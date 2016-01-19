//
//  SG_SkillsID6.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-14.
//
//

#ifndef __GameSanGuo__SG_SkillsID6__
#define __GameSanGuo__SG_SkillsID6__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID6: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID6();
    ~SG_SkillsID6();
    
    virtual void activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    void removePlist(CCObject *obj, CCString *str);
    
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID6__) */
