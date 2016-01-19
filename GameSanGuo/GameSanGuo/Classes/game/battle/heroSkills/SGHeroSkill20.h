//
//  SGHeroSkill20.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill20__
#define __GameSanGuo__SGHeroSkill20__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill20 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill20();
    ~SGHeroSkill20();
    void activateSkill_20034(SGHeroLayer *heroLayer, int round, int buffid);
};
#endif /* defined(__GameSanGuo__SGHeroSkill20__) */
