//
//  SG_SkillsID3.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-11.
//
//

#ifndef __GameSanGuo__SG_SkillsID3__
#define __GameSanGuo__SG_SkillsID3__

#include <iostream>
#include "SG_SkillsBase.h"


class SG_SkillsID3: public SG_SkillsBase {
    
    
public:
    SG_SkillsID3();
    ~SG_SkillsID3();
    
    virtual void activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index);

public:


    int skillsId;







};



#endif /* defined(__GameSanGuo__SG_SkillsID3__) */
