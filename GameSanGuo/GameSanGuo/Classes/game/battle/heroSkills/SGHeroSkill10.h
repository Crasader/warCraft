//
//  SGHeroSkill10.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill10__
#define __GameSanGuo__SGHeroSkill10__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill10 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill10();
    ~SGHeroSkill10();
    void activateSkill_20010(SGHeroLayer *heroLayer,  float rate);

};
#endif /* defined(__GameSanGuo__SGHeroSkill10__) */
