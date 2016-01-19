//
//  SGHeroSkill17.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-8-27.
//
//

#include "SGHeroSkill17.h"

SGHeroSkill17::SGHeroSkill17()
{
    
}

SGHeroSkill17::~SGHeroSkill17()
{
    
}

//降低总血量值25%，增加所有当前攻击阵列攻击力10%
void SGHeroSkill17::activateSkill_20028(SGHeroLayer *heroLayer,  float value1, float value2)
{
    int ap = heroLayer->heroBloodMax*value1;
    heroLayer->changBloodNum(-ap, false, true);
    //    heroLayer->showMoraleLabel(-ap, "battle/sub_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H);
    
    this->activateSkill_20032(heroLayer, value2);
    if (heroLayer->getBlood() <= 0) {
        SGBattleManager::sharedBattle()->getBattleLayer()->showIsWin();
    }
}