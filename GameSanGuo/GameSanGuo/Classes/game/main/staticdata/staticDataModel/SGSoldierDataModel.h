//
//  SGSoldierDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-22.
//
//

#ifndef __GameSanGuo__SGSoldierDataModel__
#define __GameSanGuo__SGSoldierDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 士兵表 */

class SGSoldierDataModel :public CCObject
{
    CC_SYNTHESIZE_RETAIN(CCString*, soldierBeanId, SoldierBeanId);
    
    CC_SYNTHESIZE(int, soldierId, SoldierId);
    
    //小兵阵营 蜀1魏2吴3
    CC_SYNTHESIZE(int, soldierRace, SoldierRace);
    
    //小兵名称
    CC_SYNTHESIZE_RETAIN(CCString*, soldierName, SoldierName);
    
    //基础攻击力
    CC_SYNTHESIZE(float, soldierBaseAtk, SoldierBaseAtk);
    
    //基础防御力
    CC_SYNTHESIZE(float, soldierBaseDef, SoldierBaseDef);
    
    //墙防御力
    CC_SYNTHESIZE(int, soldierWallDef, SoldierWallDef);
    
    //回合数
    CC_SYNTHESIZE(int, soldierRound, SoldierRound);
    
    //初始血量
    CC_SYNTHESIZE(int, soldierMorale, SoldierMorale);
    
    //经验类型
    CC_SYNTHESIZE_RETAIN(CCString*, soldierExpType, SoldierExpType);
    
    CC_SYNTHESIZE(int, soldierNum, SoldierNum);
    CC_SYNTHESIZE(int, soldierAtkMode, SoldierAtkMode);
    CC_SYNTHESIZE(int, soldierAtkMode2, SoldierAtkMode2);
    CC_SYNTHESIZE(int, soldierHead, SoldierHead);
    CC_SYNTHESIZE(int, soldierFileId, SoldierFileId);
    CC_SYNTHESIZE(int, soldierPair, SoldierPair);
    CC_SYNTHESIZE(int, soldierLastHit, SoldierLastHit);
    CC_SYNTHESIZE(float, m_atkrate, SoldierAtkRate);
    CC_SYNTHESIZE(float, m_morrate, SoldierMorRate);
    CC_SYNTHESIZE(int, m_maxlvl, SoldierMaxLevel);
    CC_SYNTHESIZE(int, m_starlvl, SoldierStarLevel);
    CC_SYNTHESIZE(float, m_growvalue, SoldierGrowValue);
    CC_SYNTHESIZE(int, m_attackEffect, AttackEffect);
    CC_SYNTHESIZE(int, m_deathEffect, DeathEffect);
    CC_SYNTHESIZE(int, m_skill, Skill);
    CC_SYNTHESIZE(int, m_growskill, GrowSkill);
    CC_SYNTHESIZE(int, m_talentskill, TalentSkill);
public:
    SGSoldierDataModel();
    
    ~SGSoldierDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGSoldierDataModel__) */
