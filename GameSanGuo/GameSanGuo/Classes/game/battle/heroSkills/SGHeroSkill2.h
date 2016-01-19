//
//  SGHeroSkill2.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill2__
#define __GameSanGuo__SGHeroSkill2__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill2 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill2();
    ~SGHeroSkill2();
    float activateSkill_20001(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid);
    void removeBullet(cocos2d::CCObject *obj, void* tag);
};

#endif /* defined(__GameSanGuo__SGHeroSkill2__) */
