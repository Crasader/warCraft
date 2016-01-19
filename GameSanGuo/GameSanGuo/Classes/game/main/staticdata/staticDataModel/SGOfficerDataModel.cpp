//
//  SGOfficerDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-21.
//
//

#include "SGOfficerDataModel.h"
SGOfficerDataModel::SGOfficerDataModel()
{
    officerId = 0;
    officerGoverning = 0 ;
    officerBaseSpeed = 0 ;
    officerMaxLevel = 0 ;
    officerType = 0;
    officerBaseDef =0;
    officerRace = 0;
    officerRound =0;
    officerName = NULL;
    officerBaseAtk =0;
    officerGeneralsSkill = NULL;
    officerCurrStarLevel =0;
    officerLordSkill = 0;
    officermorale =0;
    officerName  =0;
    officerExpType =0;
    m_officerAtkRate =0;
    m_officerMorRate =0;
    m_officergrowvalue =0;
    officerAtkMode =0;
    officerAtkMode2 =0;
    officerHead =0;
    officerFileId=0;
    officerPair=0;
    officerLastHit=0;
    m_officercost=0;
    m_nextid=0;
    m_exprate=0;
    str_req=0;
    m_visittype=0;
    m_attackEffect=0;
    m_deathEffect=0;
    m_skill=0;
    soundId=0;
    skillHead=0;
    upgradestar=0;
    upgradelevel=0;
    m_officerGender=0;
    m_officerProtoId=0;
    m_officerSpeedFactor=0;
    m_IconId=0;
    m_skillmaxlevel=0;
    m_lordmaxlevel=0;
    m_advnum=0;
    m_area=0;
    
    
    
    
//    officerBaseAtk = 0;
//    officerBaseDef = 0;
//    officerBaseSpeed = 0;
//    officerCurrStarLevel = 0;
//    officerGoverning = 0;
//    officerId = 0;
//    officerLordSkill = NULL;
//    officerMaxLevel = 0;
//    officerName = NULL;
//    officerRace = 0;
//    officerRound = 0;
//    officerType = 0;
//    officermorale = 0;
//    officerGeneralsSkill = NULL;
//    officerExpType = NULL;
//    officerNum = 0;
//    officerLastHit = 0;
//    m_officerAtkRate =0;
//    m_officerMorRate = 0;
//    m_officergrowvalue = 0;
//    m_officercost = 0;
//    m_nextid = 0;
//    m_exprate = 0;
//    str_req = NULL;
//    m_visittype = 0;
//    soundId=0;
//    skillHead=0;
//    upgradelevel = 0;
//	m_IconId = 0;
//    m_advnum =0;
//	m_area = 0;
}

SGOfficerDataModel::~SGOfficerDataModel()
{
    CC_SAFE_RELEASE(officerLordSkill);
    CC_SAFE_RELEASE(officerName);
    CC_SAFE_RELEASE(officerGeneralsSkill);
    CC_SAFE_RELEASE(officerExpType);
    CC_SAFE_RELEASE(str_req);
}