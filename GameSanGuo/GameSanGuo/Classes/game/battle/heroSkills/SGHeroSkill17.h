//
//  SGHeroSkill17.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill17__
#define __GameSanGuo__SGHeroSkill17__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill17 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill17();
    ~SGHeroSkill17();
    void activateSkill_20028(SGHeroLayer *heroLayer,  float value1, float value2);
};
#endif /* defined(__GameSanGuo__SGHeroSkill17__) */
