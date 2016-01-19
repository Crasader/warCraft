//
//  SGHeroSkill8.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill8__
#define __GameSanGuo__SGHeroSkill8__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill8 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill8();
    ~SGHeroSkill8();
    void activateSkill_20007(SGHeroLayer *heroLayer, float value1, int value2, int buffid);
};
#endif /* defined(__GameSanGuo__SGHeroSkill8__) */
