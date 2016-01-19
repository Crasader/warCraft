//
//  SG_SkillsID21.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#ifndef __GameSanGuo__SG_SkillsID21__
#define __GameSanGuo__SG_SkillsID21__

#include <iostream>
#include "SG_BuffBase.h"

class SG_SkillsID21: public SG_SkillsBase {
    
    
public:
    
    SG_SkillsID21();
    ~SG_SkillsID21();
    
    virtual void detectionReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num);
    virtual void chackReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, SGAttackList *attackList1);
    virtual void removeReviveAttack(SGBattleMap *battleMap, SGAttackList *attackList);
private:
    bool isRevive;
    
};

#endif /* defined(__GameSanGuo__SG_SkillsID21__) */
