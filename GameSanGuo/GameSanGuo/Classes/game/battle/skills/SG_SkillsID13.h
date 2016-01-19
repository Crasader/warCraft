//
//  SG_SkillsID13.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#ifndef __GameSanGuo__SG_SkillsID13__
#define __GameSanGuo__SG_SkillsID13__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID13: public SG_SkillsBase {
    
    
public:
    SG_SkillsID13();
    ~SG_SkillsID13();
    
    virtual void activateSkills6(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    virtual bool activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, float damage);
    
private:
    bool isAddAp;
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID13__) */
