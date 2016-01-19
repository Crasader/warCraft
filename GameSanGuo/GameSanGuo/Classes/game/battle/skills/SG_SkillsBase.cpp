//
//  SG_SkillsBase.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-1-11.
//
//

#include "SG_SkillsBase.h"



SG_SkillsBase::SG_SkillsBase()
:m_myHeroLayer(NULL)
,m_enemyHeroLayer(NULL)
{
    
}

SG_SkillsBase::~SG_SkillsBase()
{
    CCLOG("~SG_SkillsBase");
}

void SG_SkillsBase::activateSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}
void SG_SkillsBase::activateSkills(bool isHero, SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}

bool SG_SkillsBase::activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, float damage)
{
    return false;
}

bool SG_SkillsBase::activateSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    return false;
}

void SG_SkillsBase::activateSkills1_1(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index)
{
    
}

bool SG_SkillsBase::activateSkills2(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    return false;
}

void SG_SkillsBase::activateSkills3(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}

void SG_SkillsBase::activateSkills4(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}

int SG_SkillsBase::activateSkills5(SGBattleMap *battleMap, SGAttackList *attackList, float num, ShiBing *sb)
{
    return num;
}

int SG_SkillsBase::activateSkills5_1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    return num;
}

void SG_SkillsBase::activateSkills6(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}

void SG_SkillsBase::activateSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}

bool SG_SkillsBase::assistSkills7(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    return false;
}

void SG_SkillsBase::activateSkills8(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1,int damage)
{
    GPCCLOG("SG_SkillsBase::activateSkills8\n");
}

void SG_SkillsBase::activateSkills9(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    
}
void SG_SkillsBase::activateSkills10(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}

bool SG_SkillsBase::activateSkills11(SGBattleMap *battleMap, SGAttackList *attackList, ShiBing *sb)
{
    return false;
}

void SG_SkillsBase::activateSkills12(SGBattleMap *battleMap, SGAttackList *attackList, float num, GameIndex index, GameIndex index1)
{
    
}

void SG_SkillsBase::detectionReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}

int SG_SkillsBase::assistPassivitySkills(int ap, SGAttackList *attackList, float num)
{
    return ap;
}


void SG_SkillsBase::activatePassivitySkills(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{

}
void SG_SkillsBase::activatePassivitySkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num)
{
    
}


void SG_SkillsBase::activateBuffSkills(SGBattleMap *battleMap, SGAttackList *attackList, GameIndex index, GameIndex index1, int row)
{
    
}

void SG_SkillsBase::activateBuffSkills1(SGBattleMap *battleMap, SGAttackList *attackList, float num, SGAttackList *attackList1)
{
    
}


void SG_SkillsBase::chackReviveSkills(SGBattleMap *battleMap, SGAttackList *attackList, SGAttackList *attackList1)
{
    
}

void SG_SkillsBase::removeReviveAttack(SGBattleMap *battleMap, SGAttackList *attackList)
{
    
}

void SG_SkillsBase::setHeroLayer()
{
    SGHeroLayer* myHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getBattleLayer()->getChildByTag(sg_myHeroLayerTag);
    setmyHero(myHeroLayer);
    
    SGHeroLayer* enmyHeroLayer = (SGHeroLayer*)SGBattleManager::sharedBattle()->getBattleLayer()->getChildByTag(sg_enemyHeroLayerTag);
    setenemyHero(enmyHeroLayer);
}


