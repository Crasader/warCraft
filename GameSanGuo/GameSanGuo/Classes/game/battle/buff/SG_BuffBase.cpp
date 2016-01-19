//
//  SG_BuffBase.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-17.
//
//

#include "SG_BuffBase.h"
#include "SGAIManager.h"


SG_BuffBase::SG_BuffBase()
:m_myHeroLayer(NULL)
,m_enemyHeroLayer(NULL)
,m_buffType(0)
,m_roundNum(0)
,m_attributeNum(0)
,effectType(0)
,m_buffID(0)
{
    
}

SG_BuffBase::~SG_BuffBase()
{
    CCLOG("~SG_BuffBase()");
}
//向服务器推送buff信息
void SG_BuffBase::pushBuffData(cocos2d::CCString *strdata)
{
    bool isRound = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (isRound ||
        SGAIManager::shareManager()->isPVE) {
        SGBattleManager::sharedBattle()->fightLogRequest(strdata->getCString());
    }
}
//
void SG_BuffBase::setHeroLayer(SG_SkillsBase *skbase)
{
    SGHeroLayer* myHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getBattleLayer()->getChildByTag(sg_myHeroLayerTag);
    setmyHero(myHeroLayer);
    
    SGHeroLayer* enmyHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getBattleLayer()->getChildByTag(sg_enemyHeroLayerTag);
    setenemyHero(enmyHeroLayer);
//    setmyHero(skbase->getmyHero());
//    setenemyHero(skbase->getenemyHero());
}
//在某个index上激活buff
void SG_BuffBase::activateBuff(GameIndex index)
{
    
}
//在某个攻击阵列上激活buff
void SG_BuffBase::activateBuff(SGAttackList *attack)
{
    
}




