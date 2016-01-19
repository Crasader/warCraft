//
//  SGOfficerDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-21.
//
//

#ifndef __GameSanGuo__SGOfficerDataModel__
#define __GameSanGuo__SGOfficerDataModel__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

/*  武将表  */

class SGOfficerDataModel :public CCObject
{
    CC_SYNTHESIZE(int, officerId, OfficerId);
    CC_SYNTHESIZE(int, officerGoverning, OfficerGoverning);
    CC_SYNTHESIZE(float, officerBaseSpeed, OfficerBaseSpeed);//
    CC_SYNTHESIZE(int, officerMaxLevel, OfficerMaxLevel);
    CC_SYNTHESIZE(int, officerType, OfficerType);
    CC_SYNTHESIZE(float, officerBaseDef, OfficerBaseDef);//小
    CC_SYNTHESIZE(int, officerRace, OfficerRace);
    CC_SYNTHESIZE(int, officerRound, OfficerRound);
    CC_SYNTHESIZE_RETAIN(CCString*, officerName, OfficerName);
    CC_SYNTHESIZE(float, officerBaseAtk, OfficerBaseAtk);//
    CC_SYNTHESIZE_RETAIN(CCString*, officerGeneralsSkill, OfficerGeneralsSkill);
    CC_SYNTHESIZE(int, officerCurrStarLevel, OfficerCurrStarLevel);
    CC_SYNTHESIZE_RETAIN(CCString*, officerLordSkill, OfficerLordSkill);
    CC_SYNTHESIZE(float, officermorale, OfficerMorale);//
    CC_SYNTHESIZE(int, officerNum, OfficerNum);
    //经验类型
    CC_SYNTHESIZE_RETAIN(CCString*, officerExpType, OfficerExpType);
    CC_SYNTHESIZE(float, m_officerAtkRate, OfficerAtkRate);//
    CC_SYNTHESIZE(float, m_officerMorRate, OfficerMorRate);//
    CC_SYNTHESIZE(float, m_officergrowvalue, OfficerGrowValue);//
    CC_SYNTHESIZE(int, officerAtkMode, OfficerAtkMode);
    CC_SYNTHESIZE(int, officerAtkMode2, OfficerAtkMode2);
    CC_SYNTHESIZE(int, officerHead, OfficerHead);
    CC_SYNTHESIZE(int, officerFileId, OfficerFileId);
    CC_SYNTHESIZE(int, officerPair, OfficerPair);
    CC_SYNTHESIZE(int, officerLastHit, OfficerLastHit);
    CC_SYNTHESIZE(int, m_officercost, OfficerCost);
    CC_SYNTHESIZE(int, m_nextid, OfficerNextId);
    CC_SYNTHESIZE(float, m_exprate, OfficerExpRate);//
    CC_SYNTHESIZE_RETAIN(CCString*, str_req, ReqItems);
    CC_SYNTHESIZE(int, m_visittype, OfficerVisitType);
    CC_SYNTHESIZE(int, m_attackEffect, AttackEffect);
    CC_SYNTHESIZE(int, m_deathEffect, DeathEffect);
    CC_SYNTHESIZE(int, m_skill, Skill);
    CC_SYNTHESIZE(int, soundId, SoundId);
    CC_SYNTHESIZE(int, skillHead,SkillHead);
    //  武将最大星级
    CC_SYNTHESIZE(int, upgradestar, Upgradestar);
    //转生最小等级
    CC_SYNTHESIZE(int, upgradelevel,Upgradelevel);
    //性别
    CC_SYNTHESIZE(int, m_officerGender, OfficerGender);
    //缘分列表
    CC_SYNTHESIZE_PASS_BY_REF(std::vector<int>, m_officerFateList, OfficerFateList);
//    std::vector<int>* m_fateList;
//    std::vector<int>* getFateList() { return m_fateList; };
//    void setFateList(std::vector<int>* vec) { m_fateList = vec;};
    //武将原型ID
    CC_SYNTHESIZE(int, m_officerProtoId, OfficerProtoId);
    //速度修正系数
    CC_SYNTHESIZE(float, m_officerSpeedFactor, OfficerSpeedFactor);//
	
	//headCoin与officerIcon
	CC_SYNTHESIZE(int, m_IconId, IconId);
    //武将计最大等级
    CC_SYNTHESIZE(int, m_skillmaxlevel, SkillMaxLevel);
    //主将计最大等级
    CC_SYNTHESIZE(int, m_lordmaxlevel, LordMaxLevel);
    //武将转生次数+n
    CC_SYNTHESIZE(int, m_advnum, AdvNum);
	//武将占几格
	CC_SYNTHESIZE(int, m_area, Area);

    
public:
    
    SGOfficerDataModel();
    
    ~SGOfficerDataModel();
    
};

#endif /* defined(__GameSanGuo__SGOfficerDataModel__) */
