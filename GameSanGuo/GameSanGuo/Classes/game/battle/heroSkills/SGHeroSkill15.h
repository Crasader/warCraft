//
//  SGHeroSkill15.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill15__
#define __GameSanGuo__SGHeroSkill15__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill15 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill15();
    ~SGHeroSkill15();
    float activateSkill_20022(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid);
    void removeBullet(cocos2d::CCObject *obj, void* tag);
};
#endif /* defined(__GameSanGuo__SGHeroSkill15__) */
