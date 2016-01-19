//
//  SGHeroSkill13.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#ifndef __GameSanGuo__SGHeroSkill13__
#define __GameSanGuo__SGHeroSkill13__

#include <iostream>
#include "SGHeroSkillsBase.h"

class SGHeroSkill13 : public SGHeroSkillsBase {
    
    
public:
    SGHeroSkill13();
    ~SGHeroSkill13();
    void activateSkill_20018(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, GameIndex index, cocos2d::CCArray *array, float value2, int num);
    void activateSkill_20018(SGHeroLayer *heroLayer, SGHeroLayer *heroLayer1, GameIndex index, cocos2d::CCArray *array, float value2, int num, bool isend);

    
    void baoZa(CCObject *obj, ShiBing *sb);
};
#endif /* defined(__GameSanGuo__SGHeroSkill13__) */
