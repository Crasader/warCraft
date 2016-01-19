//
//  SG_SkillsID14.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-16.
//
//

#include "SG_SkillsID14.h"
#include "ResourceManager.h"

SG_SkillsID14::SG_SkillsID14()
{
    setHeroLayer();
}


SG_SkillsID14::~SG_SkillsID14()
{
    CCLOG("~SG_SkillsID14");
}

// 毒龙斩 攻击时，在攻击线路上喷洒毒液，维持1回合，所有接触到毒液的未蓄力士兵和武将都会因为受到伤害而死亡
void SG_SkillsID14::activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1, int row)
{
    //row = 0;
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
    if (attackList->getAttackType() == kattackfour) {
        rand = 1;
    }
    ResourceManager::sharedInstance()->bindTexture("animationFile/dmdy.plist", RES_TYPE_LAYER_UI, sg_battleLayer);

	//在对应的位置播放毒液动画
    for (int i = 0; i <= row; i++) {
        for (int j = 0; j <= rand; j++) {
            SGGridBase *grid = heroLayer->battleMap->m_seveBuffGrid[index.i + i][index.j - j];
            SG_BuffBase *buff = SG_BuffManager::initBuffManager(this, attackList)->getbfBase();
            bool isExist = grid->setBuffListObj(buff);
            if (isExist) {
                CCSpriterX *fermEffect = CCSpriterX::create("animationFile/dmdy.scml", true, true);
                fermEffect->setanimaID(0);
                fermEffect->setisloop(true);
                fermEffect->setPosition(GameConfig::getGridPoint(index.i + i, index.j - j,isMy));
                int tag = buff->getEffectType() * BUFF_EFFECT_TAG;
                heroLayer->addChild(fermEffect, 0, tag);
                fermEffect->play();
                
            }
        }
       
    }
    
    
}

