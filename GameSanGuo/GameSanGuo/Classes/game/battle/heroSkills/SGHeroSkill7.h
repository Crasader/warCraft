//
//  SGHeroSkill7.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill7__
#define __GameSanGuo__SGHeroSkill7__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill7 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill7();
    ~SGHeroSkill7();
    void activateSkill_20006(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, CCPoint point, int skillId);
};

#endif /* defined(__GameSanGuo__SGHeroSkill7__) */
