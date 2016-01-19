//
//  SGHeroSkill5.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill5__
#define __GameSanGuo__SGHeroSkill5__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill5 : public SGHeroSkillsBase {
    
    
public:
    
    SGHeroSkill5();
    ~SGHeroSkill5();
    void activateSkill_20004(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float valule2,int heroId);
};

#endif /* defined(__GameSanGuo__SGHeroSkill5__) */
