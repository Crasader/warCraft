//
//  SG_SkillsID25.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-21.
//
//

#include "SG_SkillsID25.h"
#include "ResourceManager.h"

SG_SkillsID25::SG_SkillsID25()
{
    setHeroLayer();
}


SG_SkillsID25::~SG_SkillsID25()
{
    ;
}

void SG_SkillsID25::activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1)
{
    
    SGHeroLayer* heroLayer = NULL;
    bool isMy = false;
    if(SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound())
    {
        heroLayer = getenemyHero();
        isMy = false;
    }else{
        heroLayer = getmyHero();
        isMy = true;
    }
    
    int rand = 0;
    if (attackList->getAttackType() == kattacktwo) {
        rand = 1;
    }else{
        rand = 2;
    }
    
    SGGridBase *grid = heroLayer->battleMap->m_seveBuffGrid[index.i][index.j];
    SG_BuffBase *buff = SG_BuffManager::initBuffManager(this, attackList)->getbfBase();

    bool isExist = grid->setBuffListObj(buff);

    if (rand == 2) {
        SGGridBase *grid1 = heroLayer->battleMap->m_seveBuffGrid[index1.i][index1.j];
        SG_BuffBase *buff1 = SG_BuffManager::initBuffManager(this, attackList)->getbfBase();
        grid1->setBuffListObj(buff1);
    }
    
    
    
    if (isExist) {
		ResourceManager::sharedInstance()->bindTexture("animationFile/dmdy.plist", RES_TYPE_BATTLE_IMAGE, sg_battleLayer);

        int tag = buff->getEffectType() * BUFF_EFFECT_TAG;
        for (int i = 0; i < rand; i++) {
            CCSpriterX *fermEffect = CCSpriterX::create("animationFile/dmdy.scml", true, true);
            fermEffect->setanimaID(0);
            fermEffect->setisloop(true);
            fermEffect->setPosition(GameConfig::getGridPoint(index.i, index.j - i,isMy));
            
            heroLayer->addChild(fermEffect, 0, tag);
            fermEffect->play();
            
            
        }
    }
    
}

