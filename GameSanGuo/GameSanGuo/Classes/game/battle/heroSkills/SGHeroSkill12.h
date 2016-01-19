//
//  SGHeroSkill12.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill12__
#define __GameSanGuo__SGHeroSkill12__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill12 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill12();
    ~SGHeroSkill12();
    void activateSkill_20014(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid);
    
};
#endif /* defined(__GameSanGuo__SGHeroSkill12__) */
