//
//  SG_SBSkillsID2.h
//  GameSanGuo
//
//  Created by wwl on 14-2-25.
//
//

#ifndef __GameSanGuo__SG_SBSkillsID2__
#define __GameSanGuo__SG_SBSkillsID2__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SBSkillsID2 : public SG_SkillsBase
{
public:
    SG_SBSkillsID2();
    ~SG_SBSkillsID2();
    virtual void activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
};


#endif /* defined(__GameSanGuo__SG_SBSkillsID2__) */
