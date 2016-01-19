//
//  SGHeroSkill14.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill14__
#define __GameSanGuo__SGHeroSkill14__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill14 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill14();
    ~SGHeroSkill14();
    void activateSkill_20020(SGHeroLayer *heroLayer, int addValue);
    
};
#endif /* defined(__GameSanGuo__SGHeroSkill14__) */
