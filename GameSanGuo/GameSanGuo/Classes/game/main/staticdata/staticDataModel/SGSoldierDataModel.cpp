//
//  SGSoldierDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-22.
//
//

#include "SGSoldierDataModel.h"
SGSoldierDataModel::SGSoldierDataModel()
{
    soldierBeanId = NULL;
    soldierId = 0;
    soldierName = NULL;
    soldierRace = 0;
    soldierBaseDef = 0;
    soldierBaseAtk = 0;
    soldierMorale = 0;
    soldierRound = 0;
    soldierWallDef = 0;
    soldierExpType = NULL;
    soldierNum = 0;
    soldierAtkMode = 0;
    soldierAtkMode2 = 0;
    soldierLastHit = 0;
    m_atkrate = 0;
    m_morrate = 0;
    m_maxlvl = 0;
    m_starlvl = 0;
    m_growvalue = 0;
    m_growskill =0;
    m_talentskill=0;
    
    soldierHead =0;
    soldierFileId=0;
    soldierPair=0;
    m_attackEffect=0;
    m_deathEffect=0;
    m_skill=0;
}

SGSoldierDataModel::~SGSoldierDataModel()
{
    CC_SAFE_RELEASE(soldierBeanId);
    CC_SAFE_RELEASE(soldierName);
    CC_SAFE_RELEASE(soldierExpType);
}