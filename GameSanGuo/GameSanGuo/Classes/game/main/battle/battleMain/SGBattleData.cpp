//
//  SGBattleData.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-8.
//
//

#include "SGBattleData.h"
USING_NS_CC;

SGBattleData::SGBattleData(void)
{
    _timeDesc = NULL;
    _maxlian =0;
    _lian = 0;
    _score =0;
    _shengwang =0;
    _winrate=0;
    _rankshengwang=0;
    _junxian=NULL;
    _jieshu=0;
    _hasDayReward = false;
    _hasSeasonReward = false;
    m_pStartTime = NULL;
    m_pEndTime = NULL;
}
SGBattleData::~SGBattleData(void)
{
//    CC_SAFE_DELETE(player);
    if(m_pEndTime)
    {
        CC_SAFE_DELETE(m_pEndTime);
    }
    if(m_pStartTime)
    {
        CC_SAFE_DELETE(m_pStartTime);
    }
}

SGBattleData* SGBattleData::create(const char *tdesc, int maxl, int lian, int score, int shengw,int winrate,int rankshengw,const char* junxian,int jieshu , bool hasDayReward , bool hasSeasonReward)
{
    SGBattleData *data = new SGBattleData();
    if (data && data->initWithInfo(tdesc, maxl, lian, score, shengw,winrate,rankshengw,junxian,jieshu , hasDayReward , hasSeasonReward))
    {
        data->autorelease();
        return data;
    }
    CC_SAFE_DELETE(data);
    return NULL;
}

bool SGBattleData::initWithInfo(const char *tdesc, int maxl, int lian, int score, int shengw,int winrate,int rankshengw,const char* junxian,int jieshu , bool hasDayReward , bool hasSeasonReward)
{
    _timeDesc = CCString::create(tdesc)->getCString();
    _maxlian =maxl;
    _lian = lian;
    _score =score;
    _shengwang =shengw;
    _winrate=winrate;
    _rankshengwang=rankshengw;
    _junxian=CCString::create(junxian)->getCString();
    _jieshu=jieshu;
    
    _hasDayReward = hasDayReward;
    _hasSeasonReward = hasSeasonReward;
    // player and rank array init
    return true;
}
