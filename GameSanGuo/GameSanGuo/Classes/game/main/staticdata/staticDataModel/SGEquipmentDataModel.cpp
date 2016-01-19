//
//  SGEquipmentDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-22.
//
//

#include "SGEquipmentDataModel.h"
SGEquipmentDataModel::SGEquipmentDataModel()
{
    equipBaseDef = 0;
    equipBaseSpeed = 0;
    equipBeanId = NULL;
    equipCurrStarLevel = 0;
    equipGoverning = 0;
    equipHP = 0;
    equipMaxLevel = 0;
    equipmentId = 0;
    equipName = NULL;
    equipRound = 0;
    equipType = 0;
    equipmentBaseAtk = 0;
    equipExpType = NULL;
    equipNum = 0;
    m_equipgrowvalue = 0;
    str_req = NULL;
    m_exprate = 0;
    m_cost = 0;
    m_nextid = 0;
    m_visittype = 0;
    upgradelevel = 0;
	m_IconId = 0;
    m_advnum = 0;
    
    upgradestar=0;
    m_equipProtoId = 0;
    m_materialDependencyId = -1;
    m_fateDesc = NULL;
}

SGEquipmentDataModel::~SGEquipmentDataModel()
{
    CC_SAFE_RELEASE(equipName);
    CC_SAFE_RELEASE(equipBeanId);
    CC_SAFE_RELEASE(equipExpType);
    CC_SAFE_RELEASE(str_req);
    CC_SAFE_RELEASE(m_fateDesc);

}

int SGEquipmentDataModel::getEquipStarPinzhi()
{
    int starlvl = equipCurrStarLevel;
    if (starlvl>1) {
        starlvl-=1;
    }
    return starlvl;
}