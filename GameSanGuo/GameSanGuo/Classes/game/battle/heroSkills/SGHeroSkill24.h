//
//  SGHeroSkill24.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill24__
#define __GameSanGuo__SGHeroSkill24__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill24 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill24();
    ~SGHeroSkill24();
    void activateSkill_20052(SGHeroLayer *heroLayer, float value1,int buffId);
};
#endif /* defined(__GameSanGuo__SGHeroSkill24__) */
