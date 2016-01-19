//
//  SG_SBSkillsID1.h
//  GameSanGuo
//
//  Created by wwl on 14-2-25.
//
//

#ifndef __GameSanGuo__SG_SBSkillsID1__
#define __GameSanGuo__SG_SBSkillsID1__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SBSkillsID1 : public SG_SkillsBase
{
public:
    SG_SBSkillsID1();
    ~SG_SBSkillsID1();
    virtual void activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index);  //近战小兵
    virtual bool activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num);
};

#endif /* defined(__GameSanGuo__SG_SBSkillsID1__) */
