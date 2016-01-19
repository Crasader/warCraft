//
//  SG_SkillsID12.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#ifndef __GameSanGuo__SG_SkillsID12__
#define __GameSanGuo__SG_SkillsID12__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID12: public SG_SkillsBase {
    
    
public:
    SG_SkillsID12();
    ~SG_SkillsID12();

    virtual void activatePassivitySkills(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1);
    virtual int assistPassivitySkills(int ap, SGAttackList *attackList, float num);;

};

#endif /* defined(__GameSanGuo__SG_SkillsID12__) */
