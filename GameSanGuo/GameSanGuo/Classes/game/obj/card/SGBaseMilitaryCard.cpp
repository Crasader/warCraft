//
//  SGBaseMilitaryCard.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#include "SGBaseMilitaryCard.h"

SGBaseMilitaryCard::SGBaseMilitaryCard()
:g_atk(0)
,g_governing(0)
,g_expRate(0.f)
,g_currExp(0)
,g_currLevel(0)
,g_currStar(0)
,g_def(0)
,g_round(0)
,g_morale(0)
,g_itemId(0)
,g_maxLevel(0)
,g_isprot(0)
,g_sell(0)
,g_ssid(0)
,g_race(0)
,g_maxExp(0)
,g_maxstar(0)
,g_speed(0.f)
,g_OfficerName(NULL)
,m_cost(0)
,m_itemType(0)
,upgradestar(0)
,upgradelevel(0)
,skillmaxlevel(0)
,lordmaxlevel(0)
,m_select(0)
,m_headSuffixNum(0)
,m_adnum(0)
,m_canAddNum(1)
,m_sellPrice(0)
,m_itemNum(0)
{
}
SGBaseMilitaryCard::~SGBaseMilitaryCard()
{
    CC_SAFE_RELEASE(g_OfficerName);
}