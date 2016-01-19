//
//  SGHeroSkill9.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill9__
#define __GameSanGuo__SGHeroSkill9__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill9 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill9();
    ~SGHeroSkill9();
    void activateSkill_20008(SGHeroLayer *heroLayer, float rate);
};
#endif /* defined(__GameSanGuo__SGHeroSkill9__) */
