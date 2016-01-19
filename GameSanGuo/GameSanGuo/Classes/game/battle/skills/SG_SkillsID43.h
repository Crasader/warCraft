//
//  SG_SkillsID43.h
//  GameSanGuo
//
//  Created by wwl on 14-2-20.
//
//

#ifndef __GameSanGuo__SG_SkillsID43__
#define __GameSanGuo__SG_SkillsID43__

#include <iostream>
#include "SG_SkillsBase.h"

class SG_SkillsID43 : public SG_SkillsBase{
    
public:
    SG_SkillsID43();
    ~SG_SkillsID43();
    
    virtual void activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num);
    
};


#endif /* defined(__GameSanGuo__SG_SkillsID43__) */
