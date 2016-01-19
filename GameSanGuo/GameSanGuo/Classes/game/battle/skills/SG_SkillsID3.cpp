//
//  SG_SkillsID3.cpp
//  GameSanGuo
//  
//  Created by wenlong w on 13-1-11.
//
//

#include "SG_SkillsID3.h"


SG_SkillsID3::SG_SkillsID3()
:skillsId(-1)
{
    setHeroLayer();
}


SG_SkillsID3::~SG_SkillsID3(){

}

void SG_SkillsID3::activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index)
{
    GPCCLOG("SG_SBSkillsID3::activateSkills1_1");
    if (num == 0) return;
    SGHeroLayer *hero = NULL;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        hero = getenemyHero();
    }else{
        hero = getmyHero();
    }
    hero->showBattleLabel((num * attackList->getAfftributeNum()), "sanguobigpic/add_hp_label.png", SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, index);
    
    attackList->setAp(attackList->getAp() + (num * attackList->getAfftributeNum()));
    attackList->changeAP(attackList->getAp());
    
}

