//
//  SGLogicManager.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 12/28/14.
//
//

#include "SGLogicManager.h"
#include "SGTeamgroup.h"
#include "SGPlayerDataModel.h"
#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGExoticMerchantLayer.h"

//单例
static SGLogicManager* instance = NULL;

//红点检测间隔。
const static float RED_SPOT_CHECKING_INTERVAL = 2.0f;

//ctor
SGLogicManager::SGLogicManager() : gameSpeed_(1)
{
    ;
}

//dtor
SGLogicManager::~SGLogicManager()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

//gi
SGLogicManager* SGLogicManager::gi()
{
    if (!instance)
    {
        instance = new SGLogicManager();
    }
    return instance;
}

//启动业务定时器。
void SGLogicManager::startUp()
{
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(SGLogicManager::redSpotDetection), this, RED_SPOT_CHECKING_INTERVAL , kCCRepeatForever, 0.0f, false);
}

//设置游戏速率。（用于3倍速自动战斗/延时动作的反向调整等）
void SGLogicManager::setGameSpeed(float dt)
{
    gameSpeed_ = dt;
    CCDirector::sharedDirector()->getScheduler()->setTimeScale(gameSpeed_);
}

//获得游戏速率。
float SGLogicManager::getGameSpeed()
{
    return gameSpeed_;
}

//红点判定
void SGLogicManager::redSpotDetection(float dt)
{
    checkExoMechNotice();
    
    checkEmbattleNotice();
}

//西域商人红点检查。
void SGLogicManager::checkExoMechNotice()
{
    bool vis = false;
    
    //剩余时间。
    int& time = SGExoticMerchantLayer::getNoticeTime();
    time--;
    if (time <= 0)
    {
        vis = false;
    }
    else
    {
        //可购买的西域商城物品状态。
        vis = SGExoticMerchantLayer::getIsForcedOpen();
    }
    
    SGMainManager::shareMain()->getMainLayer()->setAdventureNoticeVisible(vis);
}


void SGLogicManager::checkEmbattleNotice()
{
    bool visible = false;
    SGTeamgroup *team = SGTeamgroup::shareTeamgroupInfo();
    CCArray * curEmbattleOfficers =  team->getEmbattleOfficers(team->getiscurrent());
    for( int i=0 ; i < curEmbattleOfficers->count(); ++i)
    {
        SGOfficerCard * temp = (SGOfficerCard*)curEmbattleOfficers->objectAtIndex(i);
        if(temp->canShowNotice())
        {
            visible = true;
            break;
        }
    }
    SGPlayerDataModel * model = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    int teamVolume = model->getPlayerOfficerMax();
    if( curEmbattleOfficers->count() < SGPlayerInfo::sharePlayerInfo()->getOfficerCards()->count() && curEmbattleOfficers->count()< teamVolume)
        visible = true;
    
    if(SGPlayerInfo::sharePlayerInfo()->canSoldierStreng())
        visible = true;
    
    SGMainManager::shareMain()->getMainLayer()->setEmbattleNoticeVisible(visible);
}


