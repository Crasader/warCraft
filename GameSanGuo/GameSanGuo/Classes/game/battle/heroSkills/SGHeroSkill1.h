//
//  SGHeroSkill1.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill1__
#define __GameSanGuo__SGHeroSkill1__

#include <iostream>
#include "SGHeroSkillsBase.h"


class SGHeroSkill1 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill1();
    ~SGHeroSkill1();
    void activateSkill_20000(SGHeroLayer *heroLayer, float value1, float value2, int round, int buffid);

    
};

#endif /* defined(__GameSanGuo__SGHeroSkill1__) */
