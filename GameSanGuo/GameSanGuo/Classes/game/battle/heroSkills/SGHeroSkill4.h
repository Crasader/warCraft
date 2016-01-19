//
//  SGHeroSkill4.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill4__
#define __GameSanGuo__SGHeroSkill4__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill4 : public SGHeroSkillsBase {
    
    
public:
    
    SGHeroSkill4();
    ~SGHeroSkill4();
    void activateSkill_20003(SGHeroLayer *heroLayer, int value1, int value2, int roleid);
    
};


#endif /* defined(__GameSanGuo__SGHeroSkill4__) */
