//
//  SGHeroSkill6.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill6__
#define __GameSanGuo__SGHeroSkill6__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill6 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill6();
    ~SGHeroSkill6();
    void activateSkill_20005(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, CCPoint point, int roleid, int skillid);
};


#endif /* defined(__GameSanGuo__SGHeroSkill6__) */
