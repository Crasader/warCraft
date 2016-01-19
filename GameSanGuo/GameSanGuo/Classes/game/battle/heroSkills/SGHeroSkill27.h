//
//  SGHeroSkill27.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill27__
#define __GameSanGuo__SGHeroSkill27__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill27 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill27();
    ~SGHeroSkill27();
    void activateSkill_20063(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, int roleid, int buffid, int round, float value1, float value2, float value3);
};
#endif /* defined(__GameSanGuo__SGHeroSkill27__) */
