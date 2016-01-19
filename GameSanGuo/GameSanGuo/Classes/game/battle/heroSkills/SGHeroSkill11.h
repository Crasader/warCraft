//
//  SGHeroSkill11.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill11__
#define __GameSanGuo__SGHeroSkill11__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill11 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill11();
    ~SGHeroSkill11();
    void activateSkill_20012(SGHeroLayer *heroLayer,SGHeroLayer *heroLayer1, float value1, int s_id, CCPoint pos);
    void checkTheHero(SGHeroLayer *hero, float value1, int s_id, CCPoint pos);
};
#endif /* defined(__GameSanGuo__SGHeroSkill11__) */
