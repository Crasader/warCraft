//
//  SGHeroSkill21.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill21__
#define __GameSanGuo__SGHeroSkill21__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill21 : public SGHeroSkillsBase {
    
    
public:
    int seconds;
    SGHeroSkill21();
    ~SGHeroSkill21();
    void activateSkill_20038(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, float value1, int roleid, int jianid);
    void callback();
    void removeBullet(cocos2d::CCObject *obj, void* tag);
};
#endif /* defined(__GameSanGuo__SGHeroSkill21__) */
