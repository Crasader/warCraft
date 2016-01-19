//
//  SGHeroSkill28.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill28__
#define __GameSanGuo__SGHeroSkill28__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill28 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill28();
    ~SGHeroSkill28();
    void activateSkill_20065(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, float value2, int roleid);
    void removeBullet(cocos2d::CCObject *obj, void* tag);
};
#endif /* defined(__GameSanGuo__SGHeroSkill28__) */
