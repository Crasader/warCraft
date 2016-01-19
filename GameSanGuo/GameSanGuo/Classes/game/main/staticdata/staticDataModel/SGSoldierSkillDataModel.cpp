//
//  SGSoldierSkillDataModel.cpp
//  GameSanGuo
//
//  Created by dongboqian on 14-2-17.
//
//

#include "SGSoldierSkillDataModel.h"
SGSoldierSkillDataModel::SGSoldierSkillDataModel()
{
    soldierBeanId = NULL;
    m_skillid = 0;
    m_skilllevel = 0;
    skillinfo =NULL;
    skillname = NULL;
    m_nextid = 0;
}

SGSoldierSkillDataModel::~SGSoldierSkillDataModel()
{
    CC_SAFE_RELEASE(soldierBeanId);
    CC_SAFE_RELEASE(skillname);
    CC_SAFE_RELEASE(skillinfo);
}