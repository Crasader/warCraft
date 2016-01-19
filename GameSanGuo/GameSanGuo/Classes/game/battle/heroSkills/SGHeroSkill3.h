//
//  SGHeroSkill3.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill3__
#define __GameSanGuo__SGHeroSkill3__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill3 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill3();
    ~SGHeroSkill3();
    void activateSkill_20002(SGHeroLayer *heroLayer);
};

#endif /* defined(__GameSanGuo__SGHeroSkill3__) */
