//
//  SG_SkillsID1.h
//  GameSanGuo
//  SkillsID SKILLS_ID1
//  Created by wenlong w on 13-1-11.
//
//

#ifndef __GameSanGuo__SG_SkillsID1__
#define __GameSanGuo__SG_SkillsID1__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID1: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID1();
    ~SG_SkillsID1();

    virtual void activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);

public:

    int skillsId;
    

};

#endif /* defined(__GameSanGuo__SG_SkillsID1__) */
