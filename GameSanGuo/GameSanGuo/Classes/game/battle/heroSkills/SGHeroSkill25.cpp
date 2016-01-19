//
//  SGHeroSkill25.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill25.h"
#include "SGSkillManager.h"

SGHeroSkill25::SGHeroSkill25()
{
    
}

SGHeroSkill25::~SGHeroSkill25()
{
    
}

//发动后，等于点击了一次等待区，补齐了兵种。
void SGHeroSkill25::activateSkill_20058(SGHeroLayer *heroLayer, float value1)
{
    heroLayer->battleMap->m_touchState = eTouchNull;
    if (value1 > 0) {
        heroLayer->roundNum += 1;
        heroLayer->fillUp();
    }else{
        SGSkillManager::shareSkill()->beginAi();
        SGSkillManager::shareSkill()->beginUpTime();
    }
}
