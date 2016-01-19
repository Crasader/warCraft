//
//  SGHeroSkill18.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill18__
#define __GameSanGuo__SGHeroSkill18__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill18 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill18();
    ~SGHeroSkill18();
    void activateSkill_20030(SGHeroLayer *heroLayer, int skillsId);
};

#endif /* defined(__GameSanGuo__SGHeroSkill18__) */
