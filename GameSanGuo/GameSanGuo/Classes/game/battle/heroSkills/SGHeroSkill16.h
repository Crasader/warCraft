//
//  SGHeroSkill16.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill16__
#define __GameSanGuo__SGHeroSkill16__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill16 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill16();
    ~SGHeroSkill16();
    void activateSkill_20024(SGHeroLayer *heroLayer, int value);
};
#endif /* defined(__GameSanGuo__SGHeroSkill16__) */
