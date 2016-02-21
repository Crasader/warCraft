//
//  MainSceneLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-26.
//
//

#include "SGMainLayer.h"
#include "SGMainManager.h"
#include "SGHttpClient.h"
#include "SGPlayerInfo.h"
#include "itemmodule.pb.h"
#include "SGOfficerCard.h"
#include "SGSoldierCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGChatMessage.h"
#include "SGChat.h"
#include "SGChatMessageCenter.h"
#include "LotteryModule.pb.h"
#include "SGCantAdvanceBox.h"
#include "SGStaticDataManager.h"
#include "SGPlayerInfoLayer.h"
#include "SGPlatFormIos.h"
#include "PlotModule.pb.h"
#include "ServerMoving.pb.h"
#include "SGAnnouncementLayer.h"
#include "SGFirstLayer.h"
#include "RewardModule.pb.h"
#include "SGBMatchAlertBox.h"
#include "SGBMatchEndBox.h"
#include "PvpFight.pb.h"
#include "SGTestSwitch.h"
#include "SGStoryReward.h"
#include "SGStoryRewardBox.h"
#include "SGStoryRewardItem.h"
#include "LoginModule.pb.h"
#include "SGBattleManager.h"
#include "SGHeartbeatDetection.h"
#include "SGBMatchStartBox.h"
#include "ActivityModule.pb.h"
#include "SGBattleData.h"
#include "SGBattleMainLayer.h"
#include "SGGuideManager.h"
#include "SGOverBattleBox.h"
#include "SGFriendCountFull.h"
#include "ShopModule.pb.h"
#include "SGFightRewardLayer.h"
#include "SGVisitLayer.h"
#include "SGCreamBattleLayer.h"
#include "SGFriend.h"
#include "SGCoinRewardBox.h"
#include "SGRewardSubLayer.h"
#include "SGPropRewardBox.h"
#include "SGTeamgroup.h"
#include "ResourceManager.h"
#include "SurvivalModule.pb.h"
#include "SGSoulExchangeLayer.h"
#include "SGGuideManager.h"
#include "SGAIManager.h"
//#import "SdkReqCallBack.h"
#include "SGEmbattleLayer.h"
#include "LuaActivityModule.pb.h"
#include "SGActivityData.h"
#include "YuanBaoShopModule.pb.h"
#include "SGActGiftInfoBox.h"
#include "SGStringConfig.h"
#include "SGLottery9ViewItem.h"
#include "PlatformAdp.h"
#include "Invite.pb.h"
#include "RoleShopModule.pb.h" //新商城 by：zyc
#include "SGUpdateManager.h"
#include "SGLootWiKiLayer.h"
#include "SGAdvancedEffectLayer.h"
#include "SGRankAllPlayer.h"
#include "RankModule.pb.h"
#include "ArenaModule.pb.h"
#if (PLATFORM == ANDROID)
#include "SdkHandler.h"
#include "SdkManager.h"
#endif

#include "DayTaskModule.pb.h"
#include "SGAllTaskLayer.h"
#include "SpecialBattleModule.pb.h"
#include "SGMallLayer.h"
#include "SGLogicManager.h"
#include <algorithm>

#include "CGPTools.h"
#include "GlobalConfig.h"

//是不是在二方的好友列表里
int isInMyFriendList = 0;
int HelpFriendPlayLev = 0;
int HelpFriendRoleId = 0;
const char *HelpFriendRoleName;
//在援军加好友处显示武将用
int HelpFriendItemId = 0;
int HelpFriendItemLevel = 0;

static int vsresult = 0;
static int ipPort = 0;
static std::string ipAddr = "";
static std::string fightId = "";


#define ALLSHOWLOGIN 0

SGMainLayer::SGMainLayer()
:name(NULL),
chat(NULL),
experience(NULL),
tili(NULL),
gold(NULL),
copper(NULL),
level(NULL),
//layer(NULL),
dynamic(NULL),
center(NULL),
vipRank(NULL),
oldVipLevel(0),
openByActivity(false)
,canChatShown(true)
,chongzhi(NULL)
,chatMore(NULL)
,money_bg(NULL)
,money_bg1(NULL)
,expprog(NULL)
,combatValue(NULL)
,portrait_bg(NULL)
,jl(NULL)
,portraitMenu(NULL)
,gonggaobg(NULL)
,tilicd(NULL)
,junlcd(NULL)
,tilicdbg(NULL)
,junlcdbg(NULL)
,upheight(0)
,btmheight(0)
,btnheight(0)
,totaluphgt(0)
,visitHeight(0)
,m_nPveLeftTime(0)
,m_nPveCircleTime(0)
,m_nPvpLeftTime(0)
,m_nPvpCircleTime(0)
,mailManager(NULL)
,chatShowItemSsid(0)
,chatShowRoleId(0)
,chatShowType(0)
,money_Button(NULL)
,money_Button1(NULL)
,m_name(NULL)
,m_isbattelAddFriend(false)
,m_embattleNotice(NULL)
,m_adventureNotice(NULL)
, m_hand(NULL)
, embattleBtn(NULL)
{
	//10级到45级每升5级,开了一个布阵格子
    for (int i = 0, j = 10; i < 8 && j < 46; i++, j+=5)
	{
		levelArray[i] = j;
	}
}

SGMainLayer::~SGMainLayer()
{
    CCLOG("~SGMainLayer");
    
    this->unschedule(schedule_selector(SGMainLayer::updateLayerData));
//    this->unschedule(schedule_selector(SGMainLayer::showRRInfo));
    SGNotificationCenter *notificationCenter = SGNotificationCenter::sharedNotificationCenter();
    notificationCenter->removeObserver(this, MSG_VISIT_ENTER);
    notificationCenter->removeObserver(this, MSG_STORY_MAIN_ENTER);
    notificationCenter->removeObserver(this, MSG_SEASONWAR_APPLY);
    notificationCenter->removeObserver(this, MSG_SEASONWAR_APPLYS);
    notificationCenter->removeObserver(this, MSG_SEASONWAR_READYS);
    notificationCenter->removeObserver(this, MSG_ANNOUCEMENT_MESSAGE);
    notificationCenter->removeObserver(this, MSG_REWARD_NOTICE);
    notificationCenter->removeObserver(this, MSG_UPDATE_PLAYERINFO);
    notificationCenter->removeObserver(this, MSG_STORY_BATTLE_QEND);
    notificationCenter->removeObserver(this, MSG_STORY_ACT_LIST);
//    notificationCenter->removeObserver(this, MSG_PVPENTER_SEASONWAR);
    notificationCenter->removeObserver(this, MSG_FRIEND_ADDFRIEND);
    notificationCenter->removeObserver(this, MSG_PURCHPVEPOWE_RENTER);
    notificationCenter->removeObserver(this, MSG_PURCHPVPPOWE_RENTER);
    notificationCenter->removeObserver(this, MSG_UPDATEPOWER);
 
   // notificationCenter->removeObserver(this, MSG_REWARDLIST_LOGIN);
      
    notificationCenter->removeObserver(this, MSG_CREAM_BATTLE);
    notificationCenter->removeObserver(this, MSG_UPDATETEAMINF);
    
//    notificationCenter->removeObserver(this, MSG_JUNHUN_INFO); //被商城替代
    notificationCenter->removeObserver(this, MSG_SURVIVAL_DAILY_REWARD);
    notificationCenter->removeObserver(this, MSG_REWARD_LVLUPENTER);
      notificationCenter->removeObserver(this, MSG_UPDATEQAINFO);
     notificationCenter->removeObserver(this, MSG_CHAT_SHOWITEM);
    notificationCenter->removeObserver(this, MSG_LUA_ACTIVITY);
    notificationCenter->removeObserver(this, MSG_FIRSTCHARGEGIFT);
    notificationCenter->removeObserver(this, MSG_TONGQUE_REQUEST);
 
     notificationCenter->removeObserver(this, MSG_BOSSBATTLE_JOIN);
   
 
   notificationCenter->removeObserver(this, MSG_ENTER_BOSS_RANK);
 
    //notificationCenter->removeObserver(this, MSG_CREAM_STORY);
//	notificationCenter->removeObserver(this, MSG_FLUSH_BOSS_INFO);
	notificationCenter->removeObserver(this, MSG_ENTER_BOSS);
    notificationCenter->removeObserver(this, MSG_STARTBOSSTIME);
    notificationCenter->removeObserver(this, MSG_BOSSEND_REWARD);
	notificationCenter->removeObserver(this, MSG_STORE_ENTER);
    notificationCenter->removeObserver(this, MSG_EXCHANGE_ENTER);
    notificationCenter->removeObserver(this, MSG_REWARD_INTIVER);
    notificationCenter->removeObserver(this, MSG_OFFICER_HOLY_CRASH);
    notificationCenter->removeObserver(this,MSG_RANK);
    notificationCenter->removeObserver(this, MSG_VIPGIFT_BAGS);
    notificationCenter->removeObserver(this, MSG_PVP_EVERYDAY);
    notificationCenter->removeObserver(this, MSG_DAYTASK_LIST);
    notificationCenter->removeObserver(this, MSG_SPECIAL_BATTLE_CHAP);
    notificationCenter->removeObserver(this, MSG_SPECIAL_BATTLE_SEC);
    notificationCenter->removeObserver(this, MSG_ARENA_MAIN_INFO);
    notificationCenter->removeObserver(this, MSG_FIGHT_REPORT);
    notificationCenter->removeObserver(this, MSG_ARENA_RANK_LIST);

#if (PLATFORM == ANDROID)
#include "PlatformAdp.h"
#endif
    center->release();
    
 
    mailManager->release();
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_mainLayer);
}

SGMainLayer *SGMainLayer::create(const char *bgFile, LayerTag tag_)
{
    SGMainLayer *mainLayer = new SGMainLayer();
    if (mainLayer && mainLayer->init(bgFile, tag_))
    {
        mainLayer->autorelease();
        return mainLayer;
    }
    CC_SAFE_DELETE(mainLayer);
    return NULL;
}

void SGMainLayer::buttonClickFirstLayer()
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (SGMainManager::shareMain()->getMainScene()->nowLayerTag == sg_firstLayer && guideId > guide_tag_20) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_44);
        return;
    }
    EFFECT_PLAY(MUSIC_BTN);

    SGMainManager::shareMain()->showFirstLayer();
}

//点击增加元宝按钮响应

void SGMainLayer::onClickAddYuanbao()
{
    if (SGMainManager::shareMain()->getMainScene()->nowLayerTag == sg_strengLayer)
    {
        return ;
    }
    
    
    if(!SGPlayerInfo::sharePlayerInfo()->getHaveCharged())
    {
        main::LuaActivityDetailRequest* req = new main::LuaActivityDetailRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_LUA_ACTIVITY, req);
    }
    else
    {
        main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);
    }
    
    CCLOG("点击增加元宝");
}
//点击增加铜钱按钮响应
void SGMainLayer::onClickAddCopperMoney()
{
    //防穿透
    if (SGMainManager::shareMain()->getMainScene()->nowLayerTag == sg_strengLayer)
    {
        return ;
    }
//    main::ShopExchangeCoinEntryRequest *request = new main::ShopExchangeCoinEntryRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_EXCHANGE_ENTER, request);
    //removed by cgp
   
    CCLOG("点击增加铜钱");

}
void SGMainLayer::exchangeCopperListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopExchangeCoinEntryResponse *response = (main::ShopExchangeCoinEntryResponse *)sr->m_msg;
    
    if (sr)
    {
        int vipLvl = response->viplevel();
        int useChance = response->usedchance();
        int max = response->maxchance();
        int gold = response->gold();
        int coin = response->coin();
        SGMainManager::shareMain()->showExchangLayer(vipLvl,max,useChance,gold,coin);
    }

}
void SGMainLayer::storeEnterListener(CCObject *obj)
{
    //如果在引导中，不响应点击
    if (SGGuideManager::shareManager()->isGuide)
    {
        return;
    }
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopChargeEntryResponse *response = (main::ShopChargeEntryResponse *)sr->m_msg;
    if (sr)
    {
        
        main::VIPInfo vip = response->vipinfo();
        int leng = response->chargeitems_size();
        CCArray *arrary = CCArray::create();
		CCArray *strary = CCArray::create();
        for (int i = 0;i<leng;i++) {
            main::ChargeItem item = response->chargeitems(i);
            CCString *str = CCString::createWithFormat("%d-%d-%d-%d-%d-%d-%d-%d",item.showtype(),item.chargetype(),item.discount(),item.gold(),item.rmb(),item.goldperday(),item.leftday(),item.rewardday());
            //CCLOG("%d-%d-%d-%d",item.type(),item.discount(),item.gold(),item.rmb());
            arrary->addObject(str);
            CCString *str2 = CCString::createWithFormat("%s",item.code().c_str());
            strary->addObject(str2);
            
        }
		
        
		SGMainManager::shareMain()->showStoreLayer(vip.currentviplevel(),vip.nextviplevel(),vip.currentrmb(),vip.maxrmb(),arrary,0,strary);
    }
}

void SGMainLayer::inviterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::InviteDetailResponse *response = (main::InviteDetailResponse *)sr->m_msg;
        if (response != NULL)
        {
            CCString* inviteCode = CCString::create(response->invitecode());//发送邀请的邀请码
            int inviteNum = response->invitenum();//已邀请
            int inviteMax = response->invitemax();   //最都邀请
            
            int rewardsListCount = response->rewards_size();//奖励列表
            CCArray *arr = CCArray::create();
            for(int i=0;i<rewardsListCount;i++)
            {
                main::InviteRewardMessage rewardinfo = response->rewards(i);
                arr->addObject(CCString::createWithFormat("%d - %d - %d - %d - %d",rewardinfo.num(),rewardinfo.itemtype(),
                                                          rewardinfo.itemid(), rewardinfo.itemnum(), rewardinfo.state()));
            }
            SGMainManager::shareMain()->showInviterLayer(inviteCode,inviteNum,inviteMax,arr);
        }
        
    }
}

void SGMainLayer::taskListListener(cocos2d::CCObject *obj)
{
    //如果在任务页面接收返回消息，mainlayer中的直接忽略掉
    if (dynamic_cast<SGAllTaskLayer *>(SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_alltasklayer)))
    {
        return ;
    }
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        CCArray *array = CCArray::create();
        array->retain();
        main::DayTaskListResponse *response = (main::DayTaskListResponse *)sr->m_msg;
        int len = response->daytaskmodel_size();
        for (int i = 0; i<len; i++) {
            main::DayTaskModel info = response->daytaskmodel(i);
            SGTaskObj *taskobj = new SGTaskObj;
            taskobj->settaskId(info.taskid());
            taskobj->settaskCount(info.count());
            taskobj->setgold(info.gold());
            taskobj->setisAward(info.isaward());
            array->addObject(taskobj);
            taskobj->autorelease();
        }
        SGMainManager::shareMain()->showAllTaskLayer(array);
    }
}

//战斗节列表
//进入特殊战斗节列表消息处理
void SGMainLayer::specialBattlePlostListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        CCArray *plotArr = CCArray::create();
        
        main::SpecialBattlePlotListResponse *response = (main::SpecialBattlePlotListResponse *)sr->m_msg;
        if (response)
        {
            main::SpecialBattleStoryModel model = response->storymodel();
            //节列表消息中携带本章信息
            SpecialBattleData *chapData = new SpecialBattleData;
            chapData->autorelease();
            chapData->storyId = model.storyid();
            chapData->pos = model.position();
            chapData->chapClosedDesc = model.closedesc();
            chapData->chapOpenDesc = model.opendesc();
            chapData->chapName = model.name();
            chapData->chapDesc = model.description();
            chapData->bannerBgId = model.banaerid();
            chapData->freePlayTimes = model.canjoincount();
            chapData->itemId = model.iconid();
            chapData->itemType = model.icontype();
            chapData->isOpen = model.state();
            chapData->maxPlayTimes = model.maxcount();
            
            int plotSize = response->plotmodel_size();
            for (int i = 0; i < plotSize; i++)
            {
                SpecialBattleSecData *secData = new SpecialBattleSecData;
                secData->autorelease();
                
                //节信息填充
                main::SpecialBattlePlotModel plotModel = response->plotmodel(i);
                secData->plotId = plotModel.plotid();
                secData->sectionName = plotModel.name();
                secData->canJoin = plotModel.state();
                secData->conditionTips = plotModel.closedesc();
                secData->bgIconId = plotModel.icon();
                secData->suggestPower = plotModel.combatvalue();
                
                int dropItemSize = plotModel.items_size();
                //加入掉落预览
                for (int i = 0; i < dropItemSize; i++)
                {
                    DropList dlist;
                    dlist.itemId = plotModel.items(i).itemid();
                    dlist.itemType = plotModel.items(i).itemtype();
                    secData->dropVec.push_back(dlist);
                }
                
                plotArr->addObject(secData);
            }
            SGMainManager::shareMain()->showSpecialBattleSectionInfoLayer(plotArr,chapData);
        }
    }
    
}

//排序回调
static int sortByPos(CCObject *p1, CCObject *p2)
{
    SpecialBattleData *sbd1 = static_cast<SpecialBattleData *>(p1);
    SpecialBattleData *sbd2 = static_cast<SpecialBattleData *>(p2);
    if (sbd1 == NULL || sbd2 == NULL) {
        return false;
    }
    return sbd1->pos < sbd2->pos;
}


//特殊战斗章列表
void SGMainLayer::specialBattleListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;

    if (sr)
    {
        CCArray *storyArr = CCArray::create();

        main::SpecialBattleStoryListResponse *response = (main::SpecialBattleStoryListResponse *)sr->m_msg;
        //
        if (response)
        {
            int storySize = response->storymodel_size();
            
            for (int i = 0; i < storySize; i++)
            {
                SpecialBattleData *storyData = new SpecialBattleData;
                storyData->autorelease();
                
                main::SpecialBattleStoryModel storyModel = response->storymodel(i);
                
                //单个章节的数据
                storyData->storyId = storyModel.storyid();
                storyData->isOpen = storyModel.state();
                storyData->freePlayTimes = storyModel.canjoincount();
                storyData->chapOpenDesc = storyModel.opendesc();
                storyData->chapClosedDesc = storyModel.closedesc();
                storyData->chapName = storyModel.name();
                storyData->bannerBgId = storyModel.banaerid();
                storyData->maxPlayTimes = storyModel.maxcount();
                storyData->pos = storyModel.position();
                storyData->itemId = storyModel.iconid();
                storyData->itemType = storyModel.icontype();
                storyData->chapDesc = storyModel.description();
                
                storyArr->addObject(storyData);
            }
        }
        
        //根据pos排序
        if (storyArr && storyArr->data && storyArr->data->arr)
        {
            std::sort(storyArr->data->arr, storyArr->data->arr + storyArr->data->num, sortByPos);
        }
        SGMainManager::shareMain()->showSpecialBattleLayer(storyArr);
    }
}


void SGMainLayer::addNotifications()
{
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_CREAM_BATTLE, this, callfuncO_selector(SGMainLayer::creamBattleListener));
    notification->addObserver(MSG_VISIT_ENTER,this,callfuncO_selector(SGMainLayer::visitListener));
    notification->addObserver(MSG_STORY_MAIN_ENTER, this, callfuncO_selector(SGMainLayer::storyListener));
    notification->addObserver(MSG_SEASONWAR_APPLY, this, callfuncO_selector(SGMainLayer::responseVSApply));
    notification->addObserver(MSG_SEASONWAR_APPLYS, this, callfuncO_selector(SGMainLayer::showBMatchBox));
    notification->addObserver(MSG_SEASONWAR_READYS, this, callfuncO_selector(SGMainLayer::responseVSReady));
    notification->addObserver(MSG_SEASONWAR_MATCHS, this, callfuncO_selector(SGMainLayer::flushVSerInfo));
    notification->addObserver(MSG_ANNOUCEMENT_MESSAGE, this, callfuncO_selector(SGMainLayer::showAnnoucementListener));
    notification->addObserver(MSG_REWARD_NOTICE, this, callfuncO_selector(SGMainLayer::rewardListener));
    notification->addObserver(MSG_UPDATE_PLAYERINFO, this, callfuncO_selector(SGMainLayer::updatePlayerInfo));
    notification->addObserver(MSG_STORY_BATTLE_QEND, this, callfuncO_selector(SGMainLayer::showRewardBox));
    notification->addObserver(MSG_STORY_ACT_LIST, this, callfuncO_selector(SGMainLayer::activityListener));
    notification->addObserver(MSG_FRIEND_ADDFRIEND, this, callfuncO_selector(SGMainLayer::addFriendListener));
    notification->addObserver(MSG_PURCHPVPPOWE_RENTER,this, callfuncO_selector(SGMainLayer::purchPvpPowerEnterListener));
    notification->addObserver(MSG_PURCHPVEPOWE_RENTER,this, callfuncO_selector(SGMainLayer::purchPvePowerEnterListener));
    notification->addObserver(MSG_STORY_SCENE_ENTER, this, callfuncO_selector(SGMainLayer::showSceneLayer));
    notification->addObserver(MSG_UPDATEPOWER, this, callfuncO_selector(SGMainLayer::updatepvppve));
    notification->addObserver(MSG_UPDATETEAMINF, this, callfuncO_selector(SGMainLayer::updateTeam));
    notification->addObserver(MSG_SURVIVAL_DAILY_REWARD, this, callfuncO_selector(SGMainLayer::sjzDailyRewardResponse)); //军魂换礼响应
    
    notification->addObserver(MSG_REWARD_LVLUPENTER,this,callfuncO_selector(SGMainLayer::lvlUpEnterListener));
    notification->addObserver(MSG_UPDATEQAINFO,this,callfuncO_selector(SGMainLayer::updatebattlebtn));
    notification->addObserver(MSG_CHAT_SHOWITEM, this, callfuncO_selector(SGMainLayer::getShowItemListener));
    notification->addObserver(MSG_LUA_ACTIVITY, this, callfuncO_selector(SGMainLayer::entryFirstChargeShowListener));
    
    
    notification->addObserver(MSG_FIRSTCHARGEGIFT, this, callfuncO_selector(SGMainLayer::showFirstChargeGift));
    notification->addObserver(MSG_TONGQUE_REQUEST, this, callfuncO_selector(SGMainLayer::showTongQueLisenter));
    notification->addObserver(MSG_ENTER_BOSS_RANK, this, callfuncO_selector(SGMainLayer::entryBossRankList));
    notification->addObserver(MSG_BOSSBATTLE_JOIN,
                              this,
                              callfuncO_selector(SGMainLayer::bossListener));
    notification->addObserver(MSG_ENTER_BOSS, this, callfuncO_selector(SGMainLayer::enterBossFightListener));
    notification->addObserver(MSG_BOSSEND_REWARD, this, callfuncO_selector(SGMainLayer::showBossEndRemind));
    
    notification->addObserver(MSG_STORE_ENTER,
                              this,
                              callfuncO_selector(SGMainLayer::storeEnterListener));
    
    notification->addObserver(MSG_EXCHANGE_ENTER,
                              this,
                              callfuncO_selector(SGMainLayer::exchangeCopperListener));
    notification->addObserver(MSG_REWARD_INTIVER,
                              this,
                              callfuncO_selector(SGMainLayer::inviterListener));
    notification->addObserver(MSG_ALAN_WAKE, this, callfuncO_selector(SGMainLayer::alanWakeListener));
    notification->addObserver(MSG_OFFICER_HOLY_CRASH, this, callfuncO_selector(SGMainLayer::officerHolyCrashListener));
    
    //vip 礼包
    notification->addObserver(MSG_VIPGIFT_BAGS, this, callfuncO_selector(SGMainLayer::onVipGiftResponse));
    
    //firstLayer中的排行功能与商城功能互换位置
    notification->addObserver(MSG_RANK, this, callfuncO_selector(SGMainLayer::rankListener));
    
    //pvp每日奖励
    notification->addObserver(MSG_PVP_EVERYDAY, this, callfuncO_selector(SGMainLayer::onPvpDayReward));
    notification->addObserver(MSG_DAYTASK_LIST,
                              this,
                              callfuncO_selector(SGMainLayer::taskListListener));
    //特殊副本章列表
    notification->addObserver(MSG_SPECIAL_BATTLE_CHAP, this, callfuncO_selector(SGMainLayer::specialBattleListener));
    //特殊副本节列表
    notification->addObserver(MSG_SPECIAL_BATTLE_SEC, this, callfuncO_selector(SGMainLayer::specialBattlePlostListener));
    
    notification->addObserver(MSG_ARENA_MAIN_INFO, this, callfuncO_selector(SGMainLayer::enterAreanListener));
    notification->addObserver(MSG_FIGHT_REPORT, this, callfuncO_selector(SGMainLayer::enterFightRoad));
    notification->addObserver(MSG_ARENA_RANK_LIST, this, callfuncO_selector(SGMainLayer::enterZhuHouGeJu));
}


void SGMainLayer::initMsg()
{
    ResourceManager::sharedInstance()->bindTexture("headIcon/headCountry.plist", RES_TYPE_LAYER_UI, sg_mainLayer);
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    addNotifications();
    //启动聊天分发器
    center = SGChatMessageCenter::shareChatMessageCenter();
    if(!center)
    {
        CCLOG("SGChatMessageCenter error");
    }
    
    //启动定时器
    m_nPveCircleTime = SGPlayerInfo::sharePlayerInfo()->getPveCircleTime();
    m_nPveLeftTime = SGPlayerInfo::sharePlayerInfo()->getPveLeftTime();
    m_nPvpLeftTime = SGPlayerInfo::sharePlayerInfo()->getPvpLeftTime();
    m_nPvpCircleTime = SGPlayerInfo::sharePlayerInfo()->getPvpCircleTime();
    this->schedule(schedule_selector(SGMainLayer::updateLayerData), 1);
    
    //启动资源计数器监控
//    this->schedule(schedule_selector(SGMainLayer::showRRInfo), 5);
    
 
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_mainLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_mainLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_mainLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_mainLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_mainLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_LAYER_UI, sg_mainLayer);
  //  ResourceManager::sharedInstance()->bindTexture("particle/hua.plist", RES_TYPE_LAYER_UI, sg_mainLayer);

    int modelTag = 0;

   // bool isDayTime = Tools::isDayOrLight();
    
    
    
    

    
    //主页全屏大背景
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCSprite *background2 = CCSprite::createWithSpriteFrameName(Tools::isDayOrLight() ? "barrack_bg.png" : "nightbarrack_bg.png");
    background2->setScaleX(s.width/background2->getContentSize().width * 1.01);
    background2->setAnchorPoint(ccp(0.5, 0.5));
    background2->setPosition(ccpAdd(center, ccp(0, 0)));
    this->addChild(background2,-10);
    //////留言页面专设的tag
    background2->setTag(10000);
    
    CCSprite *background3 = CCSprite::createWithSpriteFrameName(Tools::isDayOrLight() ? "barrack_bg2.png" : "nightbarrack_bg2.png");
    background3->setScaleX(s.width/background3->getContentSize().width * 1.01);
    background3->setAnchorPoint(ccp(0.5, 0.5));
    background3->setPosition(ccpAdd(center, ccp(0, 0)));
    this->addChild(background3,-6);
    //////留言页面专设的tag
    background3->setTag(10001);
    
    CCSkeletonAnimation*  boguang  = CCSkeletonAnimation::createWithFile("effSpine/boguang.json", "effSpine/boguang.atlas");
    if (!Tools::isDayOrLight())
    {
         CCSkeletonAnimation*  nightGuang = CCSkeletonAnimation::createWithFile("effSpine/nightguang.json", "effSpine/nightguang.atlas");
        
        this->addChild(nightGuang, -8);
        nightGuang->setAnimation("animation", true);
        nightGuang->setPosition(ccp(320, 568));
    }

    boguang->setAnimation(Tools::isDayOrLight() ? "animation" :"animation2", true);
    this->addChild(boguang, -8);
    boguang->setPosition(ccp(320, 568));
    
    CCSkeletonAnimation*  people;
    if (Tools::isDayOrLight())
    {
        people = CCSkeletonAnimation::createWithFile("effSpine/fengmian001.json", "effSpine/fengmian001.atlas");
    }
    else
    {
         people = CCSkeletonAnimation::createWithFile("effSpine/fengmian002.json", "effSpine/fengmian002.atlas");
    }
    
    people->setAnimation("animation", true);
    this->addChild(people, -4);
    people->setPosition(ccp(490, 250));

    mYunCai1 = CCSkeletonAnimation::createWithFile("effSpine/yuncai.json", "effSpine/yuncai.atlas", 1);
    mYunCai1->setAnimation(Tools::isDayOrLight() ? "animation" :"animation2", true);
    mYunCai1->setPosition(ccp(320, 600));
    this->addChild(mYunCai1, -8);
 
    
    
    CCDelayTime*  delayYun = CCDelayTime::create(16);
    CCCallFunc*   callYun = CCCallFunc::create(this,  callfunc_selector(SGMainLayer::delayYuncai));
    this->runAction(CCSequence::create(delayYun, callYun, nullptr));
    
    
    
    
    

    
    
    //首页
    SGButton *homeBtn = SGButton::create("homeBtn.png",NULL,  this, menu_selector(SGMainLayer::buttonClickFirstLayer),ccp(0, 0),false, true, SGBE_GRAY_MAGNIFY);
    this->addBtn(homeBtn);
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_11);//对应的modelTag == 511
    homeBtn->setTag(modelTag);
    
    //布阵
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_34);
    embattleBtn = SGButton::create("buZhen.png",NULL,  this, menu_selector(SGMainLayer::onEmbattleBtnClicked),ccp(0, 0),false, true, SGBE_GRAY_MAGNIFY);
    embattleBtn->setTag(modelTag);
    this->addBtn(embattleBtn);
    
    //背包
    SGButton *barracksBtn = SGButton::create("barrackBtn.png",NULL, this, menu_selector(SGMainLayer::barracksHandler),ccp(0, 0),false, true, SGBE_GRAY_MAGNIFY);
    this->addBtn(barracksBtn);
    modelTag  = SGStaticDataManager::shareStatic()->getimitTagById(limitOpenBox,2);
    barracksBtn->setTag(modelTag);
    
    //拜访
    modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_31);
    SGButton *visitBtn = SGButton::create("visitBtn.png",NULL,  this, menu_selector(SGMainLayer::visitHandler),ccp(0, 0),false,  true, SGBE_GRAY_MAGNIFY);
    this->addBtn(visitBtn);
    visitBtn->setTag(modelTag);
    
    //奇遇（原好友）
    SGButton *friendBtn = SGButton::create("adventure_btn.png",NULL, this, menu_selector(SGMainLayer::friendHandler),ccp(0, 0),false, true, SGBE_GRAY);
    this->addBtn(friendBtn);
    modelTag = 0;
    friendBtn->setTag(modelTag);
    
    //系统
    SGButton *battleBtn = SGButton::create("battleBtn.png",NULL,  this, menu_selector(SGMainLayer::systemHandler),ccp(0, 0),false, true, SGBE_GRAY);
    this->addBtn(battleBtn);

    //底部6个按钮的大背景
    CCSprite *Btnbackground = Btnbackground =  CCSprite::createWithSpriteFrameName("mainBtn_background_1.png");
    Btnbackground->setAnchorPoint(ccp(0.5, 0));
    Btnbackground->setPosition(SGLayout::getPoint(kBottomCenter));
    Btnbackground->setScaleX(s.width/Btnbackground->getContentSize().width);
    this->addChild(Btnbackground,-1);
    
    float wid = (s.width - homeBtn->getContentSize().width*6)/7;
    
    homeBtn->setPosition(ccp(wid + homeBtn->getContentSize().width * 0.5,Btnbackground->getContentSize().height*.47));
    embattleBtn->setPosition(ccp(wid*2 + homeBtn->getContentSize().width*1.5, Btnbackground->getContentSize().height*.47));    //布阵
    barracksBtn->setPosition(ccp(wid*3 + homeBtn->getContentSize().width*2.5, Btnbackground->getContentSize().height*.47));  //背包
    visitBtn->setPosition(ccp(wid*4 + homeBtn->getContentSize().width*3.5, Btnbackground->getContentSize().height*.47));  //拜访
    friendBtn->setPosition(ccp(wid*5 + homeBtn->getContentSize().width*4.5, Btnbackground->getContentSize().height*.47));  //奇遇
    battleBtn->setPosition(ccp(wid*6 + homeBtn->getContentSize().width*5.5, Btnbackground->getContentSize().height*.47)); //系统
    

    

    
    
    //最上面的公告背景深蓝色
    gonggaobg =  CCSprite::createWithSpriteFrameName("mainlayer_gongg_bg.png");
    gonggaobg->setAnchorPoint(ccp(0.5,1));
    gonggaobg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, 5)));
    this->addChild(gonggaobg,2);
    
    //黄色的公告装饰小框
    CCScale9Sprite *zs = CCScale9Sprite::createWithSpriteFrameName("gonggaozhuangshi.png");
    if (s.width>640)
        zs->setPreferredSize(CCSizeMake(768, 43));

    zs->setPosition(ccpAdd(gonggaobg->getPosition(), ccp(0, -5)));
    zs->setAnchorPoint(ccp(0.5,1));
    zs->setZOrder(1000);
    this->addChild(zs);
    
    //褐色的公告背景
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-gonggaobg->getContentSize().height)));
    this->addChild(gonggao_3_2,2);
    
    //金线
    CCSprite *gonggao_2 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    gonggao_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_2->setScaleX(s.width/gonggao_2->getContentSize().width);
    gonggao_2->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(gonggao_2,2);
    
    //公告左边渐变背景
    CCSprite *gonggao_left = CCSprite::createWithSpriteFrameName("gonggao_LR.png");
    gonggao_left->setAnchorPoint(ccp(0, 0));
    gonggao_left->setPosition(ccp(0, gonggao_2->getPosition().y));
    this->addChild(gonggao_left,2);
    
    //公告右边渐变背景
    CCSprite *gonggao_right = CCSprite::createWithSpriteFrameName("gonggao_LR.png");
    gonggao_right->setFlipX(true);
    gonggao_right->setAnchorPoint(ccp(1, 0));
    gonggao_right->setPosition(ccp(s.width, gonggao_2->getPosition().y));
    this->addChild(gonggao_right,2);
    
    
    //玩家信息的大背景
    CCSprite *player_bg_2 = CCSprite::createWithSpriteFrameName("playerinfo_bg_2.png");
    player_bg_2->setAnchorPoint(ccp(0.5, 1));
    player_bg_2->setPosition(ccpAdd(gonggao_3_2->getPosition(), ccp(0,  - gonggao_3_2->getContentSize().height)));
    this->addChild(player_bg_2, -2);
    
    //左上装饰
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-gonggaobg->getContentSize().height)));
    
    //右上装饰
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-gonggaobg->getContentSize().height)));
    
    //右下装饰
    CCSprite *rightcorner = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    rightcorner->setAnchorPoint(ccp(1, 0));
    rightcorner->setFlipX(true);
    rightcorner->setFlipY(true);
    rightcorner->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height - gonggao_3_2->getContentSize().height)));
    
    //左下装饰
    CCSprite *leftcorner = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    leftcorner->setFlipY(true);
    leftcorner->setAnchorPoint(ccp(0, 0));
    leftcorner->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height  - gonggao_3_2->getContentSize().height)));
    
    this->addChild(gonggao_bg_3_r,3);
    this->addChild(gonggao_bg_3_l,3);
    this->addChild(rightcorner,1);
    this->addChild(leftcorner,1);
    
    //上边装饰的金边
    CCSprite *titlecenterUp = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenterUp->setAnchorPoint(ccp(0.5, 0));
    titlecenterUp->setPosition(ccpAdd(gonggao_bg_3_l->getPosition(), ccp(s.width/2 , -13 )));
    titlecenterUp->setScaleX(6);

    //下边装饰的金边
    CCSprite *titlecenterDown = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenterDown->setAnchorPoint(ccp(0.5, 0));
    titlecenterDown->setPosition(ccpAdd(leftcorner->getPosition(), ccp(s.width/2,-10)));
    this->addChild(titlecenterDown,1);
    if (s.height==1136)
    {
        titlecenterDown->setScaleX(6.0);
    }
    else
    {
        titlecenterDown->setScaleX(5.1);
    }
    titlecenterDown->setVisible(false);
    titlecenterUp->setVisible(false);

    //下边的边线
    CCSprite *btmbian =CCSprite::createWithSpriteFrameName("new_xian.png");
    btmbian->setAnchorPoint(ccp(0.5, 0));
    btmbian->setScaleX((s.width - leftcorner->getContentSize().width*2)/btmbian->getContentSize().width);
    this->addChild(btmbian);
    btmbian->setPosition(ccp(s.width/2, rightcorner->getPosition().y));
    
    //玩家信息栏的大矩形框背景，金色
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(player_bg_2->getPosition(), ccp(0, - player_bg_2->getContentSize().height + leftcorner->getContentSize().height/25)));

    //头像背景
    portrait_bg = SGButton::create("player_portrait.png", CCString::createWithFormat( "countryIconbig_%d.png",playerInfo->getPlayerCountry())->getCString(),
                                   this, menu_selector(SGMainLayer::showPlayerInfo),ccp(0.5, -6),false,true);
    portrait_bg->setAnchorPoint(ccp(0, 0));
    portrait_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(s.width/70, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height - gonggao_3_2->getContentSize().height+5)));
    portraitMenu = CCMenu::create(portrait_bg,NULL);
    this->addChild(portraitMenu,-1);
    portraitMenu->setPosition(CCPointZero);

    //临时
    if(SGPlayerInfo::sharePlayerInfo()->getShowvip())
    {
        if (playerInfo->getplayerVipLevel() > 0) {
            vipRank =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("svip%d.png", playerInfo->getplayerVipLevel())->getCString());
            this->addChild(vipRank);
            vipRank->setAnchorPoint(ccp(0, 0));
            vipRank->setPosition(ccpAdd(portrait_bg->getPosition(), ccp(-vipRank->getContentSize().width/4,portrait_bg->getContentSize().height*0.6)));
            oldVipLevel = playerInfo->getplayerVipLevel();
        }//vip

    }
    int flength = playerInfo->getNickName()->length();
    
    unsigned int flen = flength / 3 + flength % 3;
    int fontSize = 13;
    if (flen > 7)
    {
        fontSize = 11;
    }
    
    //昵称
    name = SGCCLabelTTF::create(playerInfo->getNickName()->getCString(), FONT_PANGWA, FONTSIZE(fontSize) , ccWHITE);
    name->setAnchorPoint(ccp(0, 0.5));
    this->addChild(name);
    name->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(portrait_bg->getPosition().x + portrait_bg->getContentSize().width*1.07, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height/3.5)));

    //经验
    SGCCLabelTTF *exppower = SGCCLabelTTF::create(str_mainjingyan, FONT_PANGWA, 26);
    this->addChild(exppower);
    exppower->setAnchorPoint(ccp(0, 0.5));
    exppower->setPosition(ccpAdd(name->getPosition(), ccp(0,-name->getContentSize().height)));//1.4
    
    CCSprite *expbg = CCSprite::createWithSpriteFrameName("mainlayer_exp_bg.png");
    this->addChild(expbg);
    expbg->setPosition(ccpAdd(exppower->getPosition(), ccp(exppower->getContentSize().width + expbg->getContentSize().width/2,0)));
    expprog = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("mainlayer_exp.png"));
    expprog->setType(kCCProgressTimerTypeBar);
    expprog->setMidpoint(ccp(0,0));
    expprog->setBarChangeRate(ccp(1, 0));
    expprog->setPosition(expbg->getPosition());
    this->addChild(expprog);
    int currLevelExperienceMax = SGStaticDataManager::shareStatic()->getPlayerByLevel(playerInfo->getPlayerLevel())->getPlayerExp();//当前等级最大经验
    float rate = (float)playerInfo->getPlayerExp()/currLevelExperienceMax;
    expprog->setPercentage(rate*100);
    
    // 金元宝
    float posYRate = 0;
    if (s.width >640)
    {
        //pad
        posYRate = .9;//1.1
    }
    else
    {
        //phone
        posYRate = 0.9;//1.3
        
    }
    
    money_bg =  CCSprite::createWithSpriteFrameName("mainlayer_moneybg.png");
    money_bg->setAnchorPoint(ccp(0, 0.5));
    money_bg->setPosition(ccpAdd(exppower->getPosition(), ccp(0,-money_bg->getContentSize().height*posYRate)));
    this->addChild(money_bg,-1);
    CCLayerColor * layercolor = CCLayerColor::create(ccc4(1,0,0,0), money_bg->getContentSize().width, money_bg->getContentSize().height);
    CCLayerColor * layercolor_a = CCLayerColor::create(ccc4(1,0,0,0), money_bg->getContentSize().width, money_bg->getContentSize().height);
    
    CCLayerColor * layercolor1 = CCLayerColor::create(ccc4(0,255,0,0), money_bg->getContentSize().width, money_bg->getContentSize().height);
    CCLayerColor * layercolor1_a = CCLayerColor::create(ccc4(0,255,0,0), money_bg->getContentSize().width, money_bg->getContentSize().height);
    
    money_Button = SGButton::createFromSprite((CCSprite*)layercolor1,(CCSprite*)layercolor1_a,  this, menu_selector(SGMainLayer::onClickAddYuanbao),ccp(0, 0),false,true,false);
    money_Button->setPosition(money_bg->getPosition());
    money_Button->setAnchorPoint(ccp(0, 0.5));
    this->addBtn(money_Button);

    money_Button1 = SGButton::createFromSprite((CCSprite*)layercolor,(CCSprite*)layercolor_a,  this, menu_selector(SGMainLayer::onClickAddCopperMoney),ccp(0, 0),false,true,false);
    money_bg1 = CCSprite::createWithSpriteFrameName("mainlayer_moneybg.png");
    money_bg1->setAnchorPoint(ccp(0, 0.5));
    money_bg1->setPosition(ccpAdd(money_bg->getPosition(),ccp(money_bg->getContentSize().width*1.08,0)));
    this->addChild(money_bg1,-1);
     money_Button1->setAnchorPoint(ccp(0, 0.5));
    money_Button1->setPosition(money_bg1->getPosition());
        this->addBtn(money_Button1);
    
    CCSprite *goldicon = CCSprite::createWithSpriteFrameName("goldicon.png");
    goldicon->setPosition(ccpAdd(money_bg->getPosition(), ccp(money_bg->getContentSize().width-goldicon->getContentSize().width/2,0)));
    this->addChild(goldicon);
    //加号
    CCSprite *plusflag1 = CCSprite::createWithSpriteFrameName("firstpageplus.png");
    plusflag1->setPosition(ccpAdd(money_bg->getPosition(), ccp(plusflag1->getContentSize().width/2,0)));
    this->addChild(plusflag1);
    

    //铜钱 金币
    int nGold = playerInfo->getPlayerGold();
    CCString* str_gold = NULL;
    if(nGold >= 10000000)
    {
        nGold = nGold/10000;
        str_gold = CCString::createWithFormat(str_wan_,nGold);
    }
    else
    {
        str_gold = CCString::createWithFormat("%d",playerInfo->getPlayerGold());
    }

    gold = SGCCLabelTTF::create(str_gold->getCString(), FONT_BOXINFO, FONTSIZE(13) , ccWHITE);
    gold->setAnchorPoint(ccp(0, 0.5));
    this->addChild(gold);
    gold->setPosition(ccpAdd(money_bg->getPosition(), ccp(money_bg->getContentSize().width/2-gold->getContentSize().width/2,0)));
    CCSprite *silvericon = CCSprite::createWithSpriteFrameName("coinicon.png");
    silvericon->setPosition(ccpAdd(money_bg1->getPosition(),ccp(money_bg1->getContentSize().width-silvericon->getContentSize().width,0)));
    this->addChild(silvericon);
    
    CCSprite *plusflag2 = CCSprite::createWithSpriteFrameName("firstpageplus.png");
    plusflag2->setPosition(ccpAdd(money_bg1->getPosition(), ccp(plusflag2->getContentSize().width/2,0)));
    this->addChild(plusflag2);
    //铜钱
    int nCoins = playerInfo->getPlayerCoins();
    CCString* str_coins = NULL;
    if(nCoins >= 10000000)
    {
        nCoins = nCoins / 10000;
         str_coins = CCString::createWithFormat(str_wan_,nCoins);
    }
    else
    {
         str_coins = CCString::createWithFormat("%d",playerInfo->getPlayerCoins());
    }
    copper = SGCCLabelTTF::create(str_coins->getCString(), FONT_BOXINFO, FONTSIZE(13));
    copper->setAnchorPoint(ccp(0, 0.5));
    this->addChild(copper);
    copper->setPosition(ccpAdd(money_bg1->getPosition(),ccp(money_bg1->getContentSize().width/2-copper->getContentSize().width/2,0)));
    
    
    //全服动态公告
    dynamic = SGDynamicCirculateLayer::create();
    dynamic->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -gonggaobg->getContentSize().height + 13)));
    addChild(dynamic,999);
    
    
    //聊天背景 长条
    CCSprite *chatbackground =  CCSprite::createWithSpriteFrameName("chat_background.png");
    chatbackground->setAnchorPoint(ccp(0.5, 0));
    chatbackground->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, Btnbackground->getContentSize().height)));
    this->addChild(chatbackground,-1,CHATBTNTAG + 101);
    chatbackground->setVisible(false);   //modified by cgp
    
    //聊天按钮
    SGButton *chatBtn = SGButton::create("chatbtn.png",
                                         "public_font_chat.png",
                                         this,
                                         menu_selector(SGMainLayer::buttonClickChatRoom),
                                         ccp(0, 0),
                                         false,
                                         true);
    this->addBtn(chatBtn);
    chatBtn->setEnabled(false);
    chatBtn->setVisible(false);
    chatBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(s.width/2.5, chatbackground->getPosition().y + chatbackground->getContentSize().height*.55)));
    chatBtn->setTag(CHATBTNTAG);
    //聊天
    CCSize winSize=CCDirector::sharedDirector()->getWinSize();
    chatMore = SGCCLabelTTF::create("....", FONT_BOXINFO,30);
    this->addChild(chatMore);
    chatMore->setAnchorPoint(ccp(0,0.5));
    chatMore->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(-s.width/4-10, Btnbackground->getContentSize().height + chatbackground->getContentSize().height/2)));
    chatMore->setVisible(false);

    chat = SGCCLabelTTF::create(str_TTMainLayer_str3, FONT_BOXINFO,30,CCSizeMake(winSize.width/4*3-20,50),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    this->addChild(chat);
    chat->setAnchorPoint(ccp(0,0.5));
    chat->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(s.width/40, Btnbackground->getContentSize().height + chatbackground->getContentSize().height/2)));
   
    
    //体力字
    SGCCLabelTTF * font_tili = SGCCLabelTTF::create(str_maintili, FONT_PANGWA, 26);
    this->addChild(font_tili);
    font_tili->setAnchorPoint(ccp(0, 0.5));
    font_tili->setPosition(ccp(money_bg1->getPosition().x, name->getPosition().y));
    //体力数值
    tili = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(13) , ccWHITE);
    tili->setAnchorPoint(ccp(0, 0.5));
    this->addChild(tili);
    tili->setPosition(ccpAdd(font_tili->getPosition(), ccp(font_tili->getContentSize().width*1.2, 0)));

    //军粮
    SGCCLabelTTF * junliang = SGCCLabelTTF::create(str_mainjunliang, FONT_PANGWA, 26);
    this->addChild(junliang);
    junliang->setAnchorPoint(ccp(0, 0.5));
    junliang->setPosition(ccp(font_tili->getPosition().x, exppower->getPosition().y ));
    
    jl = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(13));
    jl->setAnchorPoint(ccp(0, 0.5));
    this->addChild(jl);
    jl->setPosition(ccp(tili->getPosition().x+10, junliang->getPosition().y));
    
    //级别
    CCSprite *lvlbg = CCSprite::createWithSpriteFrameName("levelbg.png");
    this->addChild(lvlbg);
    lvlbg->setPosition(ccpAdd(portrait_bg->getPosition(), ccp(portrait_bg->getContentSize().width*.9, +portrait_bg->getContentSize().height*.155)));

    CCString* str_level = CCString::createWithFormat("%d",playerInfo->getPlayerLevel());
    level = SGCCLabelTTF::create(str_level->getCString(), FONT_BOXINFO, FONTSIZE(13) , ccWHITE);
    this->addChild(level);
    level->setPosition(ccp(lvlbg->getPositionX(),lvlbg->getPositionY()+ lvlbg->getContentSize().height/8));
    
    //体力，军粮冷却时间
    tilicdbg =  CCSprite::createWithSpriteFrameName("touming_kuang.png");
    this->addChild(tilicdbg);
    tilicdbg->setAnchorPoint(ccp(0.4, 0.5));
    tilicdbg->setPosition(ccpAdd(font_tili->getPosition(), ccp(money_bg->getContentSize().width, 0)));
    junlcdbg =  CCSprite::createWithSpriteFrameName("touming_kuang.png");
    this->addChild(junlcdbg);
    junlcdbg->setAnchorPoint(ccp(0.4, 0.5));
    junlcdbg->setPosition(ccp(tilicdbg->getPosition().x, junliang->getPosition().y));
    
    tilicd = CCLabelTTF::create(str_TTMainLayer_str4, FONT_BOXINFO, FONTSIZE(11));
    tilicd->setAnchorPoint(ccp(0, 0.5));
    tilicd->setColor(ccBLACK);
    this->addChild(tilicd);
    tilicd->setPosition(ccpAdd(tili->getPosition(), ccp(tili->getContentSize().width +tilicd->getContentSize().width + 10 , 0)) );
    
    junlcd = CCLabelTTF::create(str_TTMainLayer_str4, FONT_BOXINFO, FONTSIZE(11));
    junlcd->setColor(ccBLACK);
    junlcd->setAnchorPoint(ccp(0, 0.5));
    this->addChild(junlcd);
    junlcd->setPosition(ccp(tilicd->getPosition().x , junliang->getPosition().y ));
    
    //隐藏冷却时间
    junlcdbg->setVisible(false);
    junlcd->setVisible(false);
    tilicd->setVisible(false);
    tilicdbg->setVisible(false);
    
    visitHeight = gonggaobg->getContentSize().height + title_bg->getContentSize().height;
    upheight =  gonggaobg->getContentSize().height ;
    btmheight = Btnbackground->getContentSize().height + chatbackground->getContentSize().height;
    btnheight = Btnbackground->getContentSize().height ;
    totaluphgt = gonggaobg->getContentSize().height + player_bg_2->getContentSize().height + title_bg->getContentSize().height + gonggao_3_2->getContentSize().height - leftcorner->getContentSize().height/25 ;
    
    
    
    if (!SGPlayerInfo::sharePlayerInfo()->getHaveCharged()) {
        chongzhi = SGButton::create("firstCharge3.png", NULL, this, menu_selector(SGMainLayer::entryFirstChargeShow));
        CCSprite * chongzhiGuan = CCSprite::createWithSpriteFrameName("firstChargeGuang.png");
        if (chongzhiGuan) {
            chongzhiGuan->setAnchorPoint(ccp(0.5,0.5));
            chongzhiGuan->ignoreAnchorPointForPosition(false);
            chongzhiGuan->setPosition(ccp(chongzhi->getContentSize().width/2,chongzhi->getContentSize().height/2));
            chongzhiGuan->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
            chongzhi->addChild(chongzhiGuan,-10,-10);
        }
        chongzhi->setPosition(ccpAdd(money_bg1->getPosition(), ccp( (winSize.width - 640)/2 + money_bg1->getContentSize().width *0.9, money_bg1->getContentSize().height*0.3 +chongzhi->getContentSize().height/2))  );
        addBtn(chongzhi);
    }
    
    
    //配置聊天栏中的信息,如果有聊天消息，就把最后一条消息放在聊天栏中
    CCArray *chatArray = SGChatMessageCenter::shareChatMessageCenter()->getArrayCount();
    if(chatArray->count() != 0){
        sendMessage(chatArray->lastObject());
    }else{
        chat->setString("");
    }
    
    CCSprite *spr_visit =  CCSprite::createWithSpriteFrameName("publc_notice.png");
    this->addChild(spr_visit,10,1111);
    spr_visit->setPosition(ccpAdd(visitBtn->getPosition(), ccp(visitBtn->getContentSize().width * 0.38, visitBtn->getContentSize().height*.38)));
    spr_visit->setVisible(false);
    
    
	//MM: 好友请求提醒，由于“好友”并入“系统”，所以
	CCSprite *friend_notice = CCSprite::createWithSpriteFrameName("publc_notice.png");
	this->addChild(friend_notice, 10, 1235);
    SGButton* redSpotRefer = battleBtn; //原为friendBtn
	friend_notice->setPosition(ccpAdd(redSpotRefer->getPosition(), ccp(redSpotRefer->getContentSize().width*0.38, redSpotRefer->getContentSize().height * .38)));
	friend_notice->setVisible(false);
	//updateFriendBtn();//是否有好友请求
     this->schedule(schedule_selector(SGMainLayer::updateFriendBtn),1);
    
    //客服投诉提醒 //kanata
	CCSprite *complain_notice = CCSprite::createWithSpriteFrameName("publc_notice.png");
	this->addChild(complain_notice, 10, 1236);
	complain_notice->setPosition(ccpAdd(battleBtn->getPosition(), ccp(0, battleBtn->getContentSize().height * .38)));
	complain_notice->setVisible(false);
    setupdatebattlebtnvisible();
    
    //布阵提醒
    m_embattleNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
    m_embattleNotice->setPosition(ccpAdd(embattleBtn->getContentSize(), ccp(-6 , -6)));
    m_embattleNotice->setVisible(false);
    embattleBtn->addChild(m_embattleNotice , 10);
    
    //奇遇提醒
    m_adventureNotice = CCSprite::createWithSpriteFrameName("publc_notice.png");
    m_adventureNotice->setPosition(ccpAdd(embattleBtn->getContentSize(), ccp(-6 , -6)));
    m_adventureNotice->setVisible(false);
    friendBtn->addChild(m_adventureNotice , 10);

    
    //邮件
    mailManager = new SGMailManager();
    
    show9RewardLogin();
    this->schedule(schedule_selector(SGMainLayer::updatehomebtn),.5);
    
 
    
    //开启业务管理器。
    SGLogicManager::gi()->startUp();
    
    
    //added by cgp for test particle
    
     CCTextureCache::sharedTextureCache()->setLoadingMode((LIM_OPTION)CGP_OTH);
    CCParticleSystemQuad*   particle = CCParticleSystemQuad::create("particle/yinghua.plist");
     CCTextureCache::sharedTextureCache()->setLoadingMode((LIM_OPTION)CGP_PLATFORM);
    
    
    particle->setPosition(ccp(320, 1100));
    this->addChild(particle);
    

    
}

void SGMainLayer::loginAwardList()
{
    //FIXME: 记得打开屏蔽
//    main::LoginAwardListRequest *request=new main::LoginAwardListRequest();
//    SGSocketClient::sharedSocketClient()->send(MSG_REWARDLIST_LOGIN, request);

}



void SGMainLayer::updateTeam(cocos2d::CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    if(!request)
    {
        return;
    }
     main::TeamInfo* response = (main::TeamInfo*)request->m_msg;
    int length = response->teaminfoitem_size();
    for (int i = 0; i < length; i++) {
        main::TeamInfoItem item = response->teaminfoitem(i);
        SGTeamgroup *group =SGTeamgroup::shareTeamgroupInfo();
        int index = group->getteamindex(item.teamid()+1);
        group->setiscurrent(index,item.current());
        group->setlordId(index,item.lordofficercardid());
        group->setsoldierblue(index,item.bluesoldier());
        group->setsoldiergreen(index,item.greensoldier());
        group->setsoldierred(index,item.redsoldier());
        int length2 = item.officerlist_size();
        for (int j = 0; j<length2; j++) {
            group->setEmbattleOfficers(index,j,item.officerlist(j));
        }
        
    }
    
}

void SGMainLayer::showAnnoucementListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("showAnnoucementListener error !!! ");
    }
    else{
        if( SGUpdateManager::gi()->getLocalIsBigReady() )
        {
            //cgpAnnounce
//        main::AnnouncementResponse response = *(main::AnnouncementResponse*)request->m_msg;
//             //显示公告界面
//         SGAnnouncementLayer *annLayer = SGAnnouncementLayer::create(response, this);
//
//		annLayer->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height / 2));
//        SGMainManager::shareMain()->showBox(annLayer);
            
            SGFirstLayer *firstLayer = (SGFirstLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer);
            
            if (firstLayer)
            {
                firstLayer->setIsCanTouch(true);
            }
            
            //   this->boxClose();
            BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
            if(BT_PEACE != btp  &&  BT_LADDER!= btp  && BT_ARENA!=btp )
            {
                SGMainManager::shareMain()->showPveFinish();
            }
        }
    }
}
//显示九宫格奖励登陆框
void SGMainLayer::show9RewardLogin()
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    CCLOG("SGMainLayer::show9RewardLogin() guideId=%d", guideId);
    if (guideId >= UIG_MAX1)
    {
        if (SGPlayerInfo::sharePlayerInfo()->getis9Show() ) {
            if( SGUpdateManager::gi()->getLocalIsBigReady() )
            {
                SGMainManager::shareMain()->showRewardLayer(this);
            }
        }
        
        else
        {
            if( SGUpdateManager::gi()->getLocalIsBigReady() )
            {
                //打开公告界面
                main::AnnouncementRequest *request = new main::AnnouncementRequest();
                SGSocketClient::sharedSocketClient()->send(MSG_ANNOUCEMENT_MESSAGE, request);
            }
        }
    }
}
void SGMainLayer::showRewardLoginListener(cocos2d::CCObject *obj){
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("showRewardLoginListener error !!! ");
    }
    else{
        main::LoginAwardListResponse response =*(main::LoginAwardListResponse *)request->m_msg;
        
        int length=response.model_size();
        bool  has=false;
        int min=1000;
        
        //检查是否有可领取的或者没有登入够15天
        for (int i=length-1; i>=0; i--) {
            main::LoginAwardModel lam=response.model(i);
            if (lam.isfinish()==-1) {//-1代表已领取，0；代表可领取，其他代表还有几天可领取
                continue;
                
            }
            if (lam.isfinish()==0) {
                min=lam.isfinish();
                has=true;
                break;
            }
        }
        
        bool LoginAwardGold=false;
        LoginAwardGold=SGPlayerInfo::sharePlayerInfo()->getLoginAwardGold();
        openByActivity=true;
        int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        if (guideId >= UIG_MAX1)
        {
            if (ALLSHOWLOGIN || (has && (!LoginAwardGold || openByActivity)))
            {
                SGMainManager::shareMain()->showTotalRewardLayer(response,this);

            }
            else
            {
   
                //  if (SGPlayerInfo::sharePlayerInfo()->getis9Show() && !LoginAwardGold) {   //15天登陆奖励后再次登陆显示九宫格
                if (SGPlayerInfo::sharePlayerInfo()->getis9Show() ) {
                    SGMainManager::shareMain()->showRewardLayer(this);
                }
                else
                {
                    //打开公告界面
                    main::AnnouncementRequest *request = new main::AnnouncementRequest();
                    SGSocketClient::sharedSocketClient()->send(MSG_ANNOUCEMENT_MESSAGE, request);
                }
            }
        }
    }

}

#pragma mark - 刷新数据，倒计时

void SGMainLayer::delayYuncai()
{
    mYunCai2 = CCSkeletonAnimation::createWithFile("effSpine/yuncai.json", "effSpine/yuncai.atlas", 1);
    mYunCai2->setAnimation(Tools::isDayOrLight() ? "animation" :"animation2", true);
    mYunCai2->setPosition(ccp(320, 650));
    this->addChild(mYunCai2, -9);
}


//定时器
void SGMainLayer::updateLayerData()
{
    SGPlayerInfo *playerInfo =  SGPlayerInfo::sharePlayerInfo();
    
    int pvpPower = playerInfo->getPvpPower();      //军粮
    int pvePower = playerInfo->getPlayerPower();   //体力
    int playerLevel = playerInfo->getPlayerLevel();
    int tiliMax = SGStaticDataManager::shareStatic()->getPlayerByLevel(playerLevel)->getPlayerPower();
    int limitJunliang = ((CCString*)  ((CCDictionary *)SGStaticDataManager::shareStatic()->getDisplayValueDict()->objectForKey("13"))->objectForKey("value"))->intValue();
//    int compatV= playerInfo->getcombatValue();
    
    //now is max power
    if (playerInfo->getHaveCharged())
    {
        tilicd->setVisible(pvePower < tiliMax);
    }
   
    //now is max junliang
    if (playerInfo->getHaveCharged())
    {
        junlcd->setVisible(pvpPower != limitJunliang);//5改30
    }
    //刷新体力
    if (pvePower < tiliMax) {
        if(m_nPveLeftTime <= 0)
        {
            m_nPveLeftTime = m_nPveCircleTime;
            pvePower++;
            if(pvePower >= tiliMax)
            {
                pvePower = tiliMax;
                
            }
            SGPlayerInfo::sharePlayerInfo()->setPlayerPower(pvePower);

        }else
        {
            m_nPveLeftTime --;
        }
    }
    char tiliBuf[64];
    sprintf(tiliBuf, "%d/%d",pvePower,tiliMax);
    tili->setString(tiliBuf);
    if (tilicd->isVisible())
    {
         tilicd->setString(getStringByTime(m_nPveLeftTime));
    }
    //刷新军粮

    if (pvpPower < limitJunliang) {//5改30
        if(m_nPvpLeftTime == 0)
        {
            m_nPvpLeftTime = m_nPvpCircleTime;
            pvpPower++;
            if(pvpPower >= limitJunliang)
            {
                pvpPower = limitJunliang;
            }
            playerInfo->setPvpPower(pvpPower);
            
        }
        else
        {
            m_nPvpLeftTime--;
        }
    }
    jl->setString(CCString::createWithFormat("%d/%d",pvpPower,limitJunliang)->getCString());
    if (junlcd->isVisible())
    {
        junlcd->setString(getStringByTime(m_nPvpLeftTime));
    }
    
    SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_playerInfoLayer);
    if(layer)
    {
        //刷新用户信息界面
        std::string tili_left_time = std::string(getStringByTime(m_nPveLeftTime));
        std::string junliang_left_time = std::string(getStringByTime(m_nPvpLeftTime));
        
        //回复体力
        int revertTiliTime = 0;
        std::string tili_revert_time;
        if(tiliMax <= pvePower)
        {
            tili_revert_time = "00:00";
        }else{
            revertTiliTime = m_nPveLeftTime+(tiliMax-pvePower-1)*m_nPveCircleTime;
            tili_revert_time = std::string(getStringByTime(revertTiliTime));
        }
        
        //回复军粮
        int revertJunliangTime = 0;
        std::string junliang_revert_time;
        if(pvpPower >= limitJunliang)//5改30
        {
            //revertJunliangTime = 0;
            junliang_revert_time = "00:00";
        }else{
            revertJunliangTime = m_nPvpLeftTime+(limitJunliang-pvpPower-1)*m_nPvpCircleTime;//5改30
            junliang_revert_time = std::string(getStringByTime(revertJunliangTime));
        }
        //CCLOG("%s",junliang_revert_time.c_str());
        //刷新人物信息界面
        SGPlayerInfoLayer *playerInfoLayer = (SGPlayerInfoLayer*)layer;
        playerInfoLayer->updateInfo(tili_left_time.c_str(), tili_revert_time.c_str(), junliang_left_time.c_str(), junliang_revert_time.c_str());
    }
    
    //刷新玩家信息
    updateUserMsg();
}

const char *SGMainLayer::getStringFromNow(int time)
{
    struct cocos2d::cc_timeval now;  // 秒，毫秒
    cocos2d::CCTime::gettimeofdayCocos2d(&now, NULL);
    struct tm * tm;
    tm = localtime(&now.tv_sec);
    
    int hour = tm->tm_hour;
    int minute = tm->tm_min;
    int secend = tm->tm_sec;
    
    return getStringByTime(hour*3600+minute*60+secend+time);
}

const char *SGMainLayer::getStringByTime(int leftTime)
{
    int time_hour = leftTime/3600;
    int time_mimute = leftTime%3600/60;
    int  time_secend= leftTime%3600%60;
    
    CCString * newTimeStr = CCString::createWithFormat("%02d:%02d:%02d",time_hour,time_mimute,time_secend);

    return newTimeStr->getCString();
}

void SGMainLayer::updateUserMsg()
{
    //GPCCLOG("SGMainLayer::updateUserMsg\n");
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    int nGold = playerInfo->getPlayerGold();
    CCString* str_gold = NULL;
    if(nGold>=10000000)
    {
        nGold = nGold/10000;
        str_gold = CCString::createWithFormat(str_wan_,nGold);
    }else{
        str_gold = CCString::createWithFormat("%d",playerInfo->getPlayerGold());
    }
    gold->setString(str_gold->getCString());
    gold->setPosition(ccpAdd(money_bg->getPosition(), ccp(money_bg->getContentSize().width/2-gold->getContentSize().width/2,0)));
    name->setString(playerInfo->getNickName()->getCString());
    int nCoins = playerInfo->getPlayerCoins();
    CCString* str_coins = NULL;
    if(nCoins>=10000000)
    {
        nCoins = nCoins/10000;
        str_coins = CCString::createWithFormat(str_wan_,nCoins);
    }else{
        str_coins = CCString::createWithFormat("%d",playerInfo->getPlayerCoins());
    }
    copper->setString(str_coins->getCString());
    copper->setPosition(ccpAdd(money_bg1->getPosition(),ccp(money_bg1->getContentSize().width/2-copper->getContentSize().width/2,0)));
    level->setString(CCString::createWithFormat("%d", playerInfo->getPlayerLevel())->getCString());
    if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
        if (oldVipLevel !=  playerInfo->getplayerVipLevel() && playerInfo->getplayerVipLevel() > 0) {
            ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_mainLayer, LIM_PNG_AS_PNG);
            if (vipRank == NULL) {
                vipRank =  CCSprite::createWithSpriteFrameName(CCString::createWithFormat("svip%d.png", playerInfo->getplayerVipLevel())->getCString());
                this->addChild(vipRank);
                vipRank->setAnchorPoint(ccp(0, 0));
                vipRank->setPosition(ccpAdd(portrait_bg->getPosition(), ccp(-vipRank->getContentSize().width/4,portrait_bg->getContentSize().height*0.6)));
            }
            else{
                vipRank->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("svip%d.png", playerInfo->getplayerVipLevel())->getCString())->displayFrame());
            }
            oldVipLevel = playerInfo->getplayerVipLevel();
        }

    }
   
}

void SGMainLayer::battleHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
#if(AllTaskOpen == 1)
        int lavel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
        //用最小的那个竞技场的来控制外层
        if (lavel < SGPlayerInfo::sharePlayerInfo()->getLimitLevelById(limitArena)) {
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MainLayer_str6403,SGPlayerInfo::sharePlayerInfo()->getLimitLevelById(limitArena))->getCString());
            return;
        }
    
    
        SGMainManager::shareMain()->showVsLayer();
#else
    SGMainManager::shareMain()->showMessage("功能暂未开启!");
#endif
    

    
   
}

void SGMainLayer::friendHandler()
{
    //cgpUI
    //奇遇界面，默认进入西域商人。
    //SGMainManager::shareMain()->exoMechMainInfoRequest();
    //SGMainManager::shareMain()->getMainScene()->cocostudioTest();
}

void SGMainLayer::systemHandler()
{
    EFFECT_PLAY(MUSIC_ITEM);
    SGMainManager::shareMain()->showHelpLayer();
}

void SGMainLayer::storyHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    int count = player->getGoodsNumInBag();
    
    if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide)
    {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(SGMainManager::shareMain()->getNowShowLayer(), NULL, 12, count);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
    }
    else
    {
        //在引导中
        this->requestStory();
    }
    
}
void SGMainLayer::requestStory()
{
    main::StoryListRequest *request = new main::StoryListRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_MAIN_ENTER, request);
}

//10.28//数据
void SGMainLayer::requestCream()
{
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    if (player->getMaxPlotId() < player->getLimitLevelById(limitJinyinFuben))
    {
        SGSpotDataModel* spotData = SGStaticDataManager::shareStatic()->getSpotById(player->getLimitLevelById(limitJinyinFuben), true);
          SGMainManager::shareMain()->showMessage( CCString::createWithFormat(str_MainLayer_str152,spotData->getSpotSectionIndex())->getCString());
    }
    else
    {
        main::BossStoryListRequest *request = new main::BossStoryListRequest();
        //    SGSocketClient::sharedSocketClient()->send(MSG_CREAM_BATTLE, request);
        SGSocketClient::sharedSocketClient()->send(MSG_CREAM_BATTLE, request);
    }
}

void SGMainLayer::showSceneLayer(CCObject *sender)
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    //GPCCLOG("^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^showSceneLayer:GuildId  %d", guideId);
    
    if (guideId == guide_tag_30) return;
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PlotListResponse *response = (main::PlotListResponse *)req->m_msg;
    if (response)
    {
        int storyId = response->storyid();
        CCArray *array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
        SGStoryBean *story = NULL;
        if (array == NULL)
        { //战斗中断
            story = SGStaticDataManager::shareStatic()->getStoryDataById(1);
        }
        else
        {
            for (int ii = 0; ii < array->count(); ii++)
            {
                story = (SGStoryBean *)array->objectAtIndex(ii);
                if (storyId == story->getStoryId())
                {
                    break;
                }
            }
        }
        if (story ==NULL)
        {
            return;
        }
        story->setStoryStars(response->star());
        story->setMaxStar(response->maxstar());
        story->setCanReward(response->canreward());
        story->setRewardId(response->rewardid());

        if (response->state() == 0)
        {
            // story 场景为空，开启第一个场景
            int size = response->plotmodel_size();
            if (size > 0)
            {
                for (int ii = 0; ii < size; ii++)
                {
                    main::PlotModel model = response->plotmodel(ii);
                    SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                    if (scene == NULL)
                    {
                        continue;
                    }
                    else
                    {
                        scene->setVisible(true);
                        scene->setRank(model.rank());
                        scene->setOpenState(model.openstate());
                        scene->setPower(model.power());
                        scene->setZhandou(model.zhandou());
                        scene->setExp(model.exp());
                        scene->setGold(model.gold());
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++)
                        {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                    }
                }
            }
        }
        else if (response->state() == 1)  //走的这边
        {
            // 初始化 场景数值章节
            int size = response->plotmodel_size();
            if (size > 0)
            {
                int maxSceneId = 0;
                for (int ii = 0; ii < size; ii++)
                {
                    main::PlotModel model = response->plotmodel(ii);
                    
                    CCLOG("scene ID=================%d", model.plotid());
                    SGStorySceneBean *scene = story->getSceneBeanById(model.plotid());
                    if (scene == NULL)
                    {
                        continue;
                    }
                    else
                    {
                        if (maxSceneId < model.plotid())
                        {
                            maxSceneId = model.plotid();
                        }
                        scene->setVisible(true);
                        scene->setRank(model.rank());
                        scene->setOpenState(model.openstate());
                        scene->setPower(model.power());
                        scene->setZhandou(model.zhandou());
                        scene->setExp(model.exp());
                        scene->setGold(model.gold());
                        int itemNum = model.itemids_size();
                        scene->itemIds.clear();
                        for (int jj =0;jj<itemNum; jj++)
                        {
                            scene->itemIds.push_back(model.itemids(jj));
                        }
                    }
                }
                //V5: 设置已玩过的最大普通关卡ID。
                SGPlayerInfo::sharePlayerInfo()->setMaxPlotId(maxSceneId);
            }
        }
        else
        {
            return;
        }
        
        //不显示武将介绍的 第一关是引导，第二关有限制引导
        if (response->plotaskid() == 0 || story->getStoryId() == 1
            || story->getStoryId() == 2)
        {
            SGMainManager::shareMain()->m_trackIsPve = true;
            SGMainManager::shareMain()->showStorySectionLayer(story);
            
            
            
            if (SGGuideManager::shareManager()->getLimitGuideSize() > 0)
            {
                SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
                SGBaseLayer *nowLayer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                nowLayer->setIsCanTouch(false);
            }

            
        }
        else
        { // 有故事对话
            //对应类型中的章id，要进行对话引导

            if (SGPlayerInfo::sharePlayerInfo()->isGuideStory(4, story->getStoryId()))
            {
                //先显示节列表
                SGMainManager::shareMain()->showStorySectionLayer(story);

                //触发引导
                SGGuideManager::shareManager()->setLimitGuideTag(SGPlayerInfo::sharePlayerInfo()->checkStartLimitGuide(4,story->getStoryId()));
                SGGuideManager::shareManager()->startLimitGuide();
            }
            else
            {
                SGStorySceneBean *scene = (SGStorySceneBean *)story->getStorySections()->objectAtIndex(0);
                SGGuideManager::shareManager()->showStorySpeaker(scene->getPlotId(), this, story);//序章问题
            }
        }
    }
    else
        SGMainManager::sourceToStory = false;
}

void SGMainLayer::sendActivityHandler()
{
    main::ActivityStoryListRequest *request = new main::ActivityStoryListRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_ACT_LIST, request);
}
void SGMainLayer::activityHandler()
{
#if(AllTaskOpen == 1)
        if (SGPlayerInfo::sharePlayerInfo()->getIspvp()) {
            SG_SHOW_WINDOW(str_MainLayer_str16);
            return;
        }
        EFFECT_PLAY(MUSIC_BTN);
    
        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
        if (!player->canBreakLimitById(limitXianshiFuben)) {
           SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MainLayer_str17,player->getLimitLevelById(limitXianshiFuben))->getCString() );
        }
        else
        {
            int count = player->getGoodsNumInBag();
            //在引导中不会出现这个背包满提示
            if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide) {
                SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(SGMainManager::shareMain()->getNowShowLayer(), NULL, 12, count);
                SGMainManager::shareMain()->showBox(cantadvanceBox);
            }
            else
            {
                sendActivityHandler();
            }
        }
#else
    SGMainManager::shareMain()->showMessage("功能暂未开启!");
#endif

    
}


void SGMainLayer::activityListener(CCObject *sender)
{
    CCArray *activitys = SGPlayerInfo::sharePlayerInfo()->getActivityData();
    if (activitys == NULL) {
        SGPlayerInfo::sharePlayerInfo()->setActivityData(CCArray::create());
        activitys = SGPlayerInfo::sharePlayerInfo()->getActivityData();
        activitys ->retain();
        SGPlayerInfo::sharePlayerInfo()->getActivityData()->retain();
    }else {
        activitys->removeAllObjects();
    }
    
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::ActivityStoryListResponse *response = (main::ActivityStoryListResponse *)request->m_msg;
    if (request) {
        // 活动ID：10000-10009 场景ID：10000-10027
        int size = response->storymodel_size();
        DDLog("ss------------------%d", size);
        if (size == 0) { // 无活动
            // do nothing
        }else {
            for (int ii = 0 ; ii <size; ii++) {
                main::ActivityStoryModel model = response->storymodel(ii);
                int storyId = model.storyid();
                DDLog("ss_id------------------%d", storyId);
                SGStoryBean *storyBean = SGStaticDataManager::shareStatic()->getStoryDataById(storyId, false);
                if (storyBean) {
                    storyBean->setVisible(true);
                    storyBean->setActivityState(model.state());
                    storyBean->setActivityStartTime(model.starttime());
                    storyBean->setActivityEndTime(model.endtime());
                    storyBean->setChallengeCount(model.joincount());
                    storyBean->setMaxChallengeCount(model.maxcount());
                    activitys->addObject(storyBean);
                }
            }
        }
    }
    if (activitys->count()) {
        //按storyid排序
        for (int i = 0; i < activitys->count() - 1; i++)
        {
            for (int j = i + 1; j < activitys->count(); j++)
            {
                SGStoryBean *s1 = (SGStoryBean *)activitys->objectAtIndex(i);
                SGStoryBean *s2 = (SGStoryBean *)activitys->objectAtIndex(j);
                
                if (s1->getStoryId() > s2->getStoryId())
                {
                    activitys->exchangeObject(s1, s2);
                }
            }
        }
        
        SGMainManager::shareMain()->showActivityLayer(activitys);
    }else
    {
        SGMainManager::shareMain()->showMessage(str_MainLayer_str18);
    }
}


//10.28//george精英副本监听
void SGMainLayer::creamBattleListener(CCObject *sender)
{
	//后添加
    
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::BossStoryListResponse *response = (main::BossStoryListResponse *)request->m_msg;
	
	
    if (request) {
        
               // 故事
        int size = response->bossstorymodel_size();
        CCLog("精英副本Size == %d", size);
        if (size == 0)
		{
            //不能参加精英副本
            SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
            int limit = player->getLimitLevelById(limitJinyinFuben);
            SGStoryBean *story = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(limit);
            if (story != NULL) {
                SGStorySceneBean *plot = story->getSceneBeanById(limit);
                if (plot != NULL) {
                   SGMainManager::shareMain()->showMessage((CCString::createWithFormat(str_MainLayer_str19,str_MainLayer_str191))->getCString());
                }
            }
            else
            {
               SGMainManager::shareMain()->showMessage(str_MainLayer_str20);
            }
			
        }
		else
		{
            
            CCArray *creams = SGPlayerInfo::sharePlayerInfo()->getStoryData();
            if (creams == NULL) {
                SGPlayerInfo::sharePlayerInfo()->setStoryData(CCArray::create());
                creams = SGPlayerInfo::sharePlayerInfo()->getStoryData();
                SGPlayerInfo::sharePlayerInfo()->getStoryData()->retain();
            }else {
                creams->removeAllObjects();
            }

			//精英副本内容
			for (int ii = 0; ii < size; ii++) {
				main::BossStoryModel model = response->bossstorymodel(ii);
				int plotId = model.storyid();//精英副本ID
				SGStoryBean *plotBean = SGStaticDataManager::shareStatic()->getStoryDataById(plotId, false);//测试添加
				
				if (plotBean)
				{
					plotBean->setVisible(true);
					creams->addObject(plotBean);
				}
			}
            
            //设置精英副本阶段引导的数据
            CCUserDefault::sharedUserDefault()->setIntegerForKey("bossChap", 20000 + creams->count());
            CCUserDefault::sharedUserDefault()->flush();
            //第二个条件：从来源进入普通闯关后 点击精英闯关按钮--黑了。具体细节结果请删除第二个条件自测。坑。。
            if(SGMainManager::sourceToStory && NULL==(SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_storySectionLayer)))
            {
                SGPlayerInfo::sharePlayerInfo()->setPlotId(SGMainManager::static_plotId);
                SGLootWiKiLayer::sendMsg();
            }
            else
            {
                SGMainManager::shareMain()->showCreamBattleLayer(creams);
                SGMainManager::sourceToStory = false;
            }
		}
      
	}
    else
        SGMainManager::sourceToStory = false;
    
}
//========================================================================

//10.28
void SGMainLayer::creamBattleHandler()
{
	
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showMessage("功能暂未开启!");
//    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//    int count = player->getGoodsNumInBag();
//    if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide)
//    {
//        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(SGMainManager::shareMain()->getNowShowLayer(), NULL, 12, count);
//        SGMainManager::shareMain()->showBox(cantadvanceBox);
//    }
//    else
//        this->requestCream();
}
//end 精英副本


void SGMainLayer::storyListener(CCObject *sender)
{
    CCArray *storys = SGPlayerInfo::sharePlayerInfo()->getStoryData();
    if (storys == NULL)
    {
        SGPlayerInfo::sharePlayerInfo()->setStoryData(CCArray::create());
        storys = SGPlayerInfo::sharePlayerInfo()->getStoryData();
        SGPlayerInfo::sharePlayerInfo()->getStoryData()->retain();
    }
    else
    {
        storys->removeAllObjects();
    }
    
	
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::StoryListResponse *response = (main::StoryListResponse *)request->m_msg;
    if (request)
    {
        // 故事ID：1-22 场景ID：1-96
        int size = response->storymodel_size();
        CCLog("ss------------------%d", size);
        if (size == 0)
        { // 默认开启第一项关卡的第一个场景
            // 正式代码
            SGStoryBean *storyBean = SGStaticDataManager::shareStatic()->getStoryDataById(1);
            if (storyBean)
            {
                storyBean->setSceneVisibleById(1, true);
                storys->addObject(storyBean);
            }
        }
        else
        {
            int allDone = 0, maxStoryId = 0;
            
            for (int ii = 0 ; ii <size; ii++)
            {
                main::StoryModel model = response->storymodel(ii);
                int storyId = model.storyid();
                SGStoryBean *storyBean = SGStaticDataManager::shareStatic()->getStoryDataById(storyId);
                if (storyBean)
                {
                    storyBean->setVisible(true);
                    storyBean->setIsWin(model.iswin());
                    storyBean->setRank(model.star());
                    storyBean->setCoinRate(model.coinsrate());
                    storyBean->setExpRate(model.exprate());
                    storyBean->setConTime(model.time());
                    storyBean->setRankStars(model.star());
                    storyBean->setRankReward(model.reward());

                    if (model.iswin() == 1)
                    {
                        allDone++;
                    }

                    if (maxStoryId <= storyId)
                    {
                        maxStoryId = storyId;
                    }
                    storys->insertObject(storyBean, 0);
                }
            }
            if (allDone == size)
            { // 全部通过，开启下个故事
                //int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
                if (maxStoryId < MaxSceneId /*&& guideId >= guide_tag_45*/)
                {
                    SGStoryBean *storyBean = SGStaticDataManager::shareStatic()->getStoryDataById(maxStoryId+1);
                    if (storyBean)
                    {
                        storyBean->setVisible(true);
                        storyBean->setIsWin(0);
                        storys->insertObject(storyBean, 0);
                    }
                }
            }
        }
        
    }
    
    
    
    if (SGPlayerInfo::sharePlayerInfo()->getHasPveFight())
    {
        SGPlayerInfo::sharePlayerInfo()->setHasPveFight(false);
        SGBattleManager::sharedBattle()->pveContinuStart(1);
        
    }
    else
    {
        if(SGMainManager::sourceToStory)
        {
            SGPlayerInfo::sharePlayerInfo()->setPlotId(SGMainManager::static_plotId);
            SGSpotDataModel * spot = SGStaticDataManager::shareStatic()->getSpotById(SGMainManager::static_plotId);
            int section = spot->getSpotSectionIndex();
            //int index = spot->getSpotSpotIndex();
            main::PlotListRequest *request = new main::PlotListRequest();
            request->set_storyid(section );
            request->set_poltaskid( section );
            SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
        }
        else
        {
            SGMainManager::shareMain()->showStoryMainLayer(storys);   //cgp_pro
            
            SGMainManager::sourceToStory = false;
        }

    }
}


void SGMainLayer::barracksHandler()
{
    if (SGMainManager::shareMain()->getMainScene()->nowLayerTag == sg_barrackslayer)
    {
        return;
    }
    EFFECT_PLAY(MUSIC_BTN);
    
//#if(AllTaskOpen == 1)
//    SGMainManager::shareMain()->showBarrackslayer();
//#else
    SGMainManager::shareMain()->showBarrackslayer();
    // SGMainManager::shareMain()->showMessage("功能暂未开放！");
//#endif

}

void SGMainLayer::showblack()
{

}
void SGMainLayer::hideblack()
{

}
float SGMainLayer::getBottomHeight()
{
    return btmheight;
}
float SGMainLayer::getfivebuttonhight()
{
    return btnheight;
}
float SGMainLayer::getHeadHeight()
{
    return upheight;
}
float SGMainLayer::getTotleUpHgt()
{
    return totaluphgt;
}
float SGMainLayer::getVisitHeight()
{
    return visitHeight;
}
void SGMainLayer::visithide()
{
    showblack();
}
void SGMainLayer::visitshow()
{
    hideblack();
}
void SGMainLayer::visitHandler()
{
    if (SGMainManager::shareMain()->getMainScene()->nowLayerTag == sg_visitLayer)
    {
        return;
    }
    EFFECT_PLAY(MUSIC_BTN);
  //  SGMainManager::shareMain()->showMessage("功能暂未开放！");

    DDLog("##########################======================%d", guideId);
    main::LotteryEntryRequest *request = new main::LotteryEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_VISIT_ENTER, request);
    
}
void SGMainLayer::visitListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::LotteryEntryResponse *response = (main::LotteryEntryResponse *)sr->m_msg;
        int a = response->legendofficertime();
        int b = response->legendequiptime();
        int c = response->advanceofficertime();
        int d = response->advanceequiptime();
        int j = response->activitylotteryviplv();
        int visitHuo = response->isactivityopen();
        
        std::vector<VisitActivity> visitActivityList;
        visitActivityList.clear();
        VisitActivity visitActivity;

        int len = response->scoutactivityinfomodel_size();
        for(int i=0;i<len;i++)
        {
            main::ScoutActivityInfoModel mailData = response->scoutactivityinfomodel(i);
            visitActivity.subType = mailData.subtype();
            visitActivity.starTime = mailData.starttime();
            visitActivity.endTime = mailData.endtime();
            visitActivity.description = mailData.description();
            visitActivityList.push_back(visitActivity);
        }
        SGMainManager::shareMain()->showVisitLayer(visitHuo,a,b,c,d,j,visitActivityList,response->lotteryname().c_str(),response->activitylotteryname().c_str());
        
    }
    
}
//得到 SGChatMessageCenterDelegate 的回调 ， 把最新的聊天消息传递过来
void SGMainLayer::sendMessage(CCObject *chatObject)
{

    SGChatMessage *message = (SGChatMessage*)chatObject;
    
    if (message->getIsChatShowHaHa()) {
        CCString *showStr=CCString::createWithFormat("%s:",message->getRoleNickName()->getCString());
        
        int len=showStr->length()/3 +showStr->length()%3;
        
        if (len>15) {
            chatMore->setVisible(true);
        }
        else
        {
            chatMore->setVisible(false);
        }
        
        
        chat->setString(showStr->getCString());

        chatShowItemSsid = message->getChatShowSSid();
        chatShowRoleId = message->getRoleId();
        chatShowType = message->getChatShowType();
        SGCCLabelTTF *label_ShowName = SGCCLabelTTF::create(message->getChatShowyName()->getCString(), FONT_PANGWA, 24);
 
        int showStar = message->getChatShowStarLevel();
        showStar -= 1;
        
        if (showStar >= 0 && showStar < 5) {
            label_ShowName->setInsideColor(ccStarLevelColor[showStar]);
        }
        else
        {
            label_ShowName->setInsideColor(ccWHITE);
            
        }
         menu->removeChildByTag(CHATSHOWENTRYTAG+1111, true);
         CCMenuItemSprite *chatShowItem = CCMenuItemSprite::create(label_ShowName, label_ShowName, this, menu_selector(SGMainLayer::chatShowItemClickSend));
        chatShowItem->setAnchorPoint(ccp(0, 0.5));
        chatShowItem->setPosition(ccpAdd( chat->getPosition(), ccp(chat->getContentSize().width/2,0)));
        menu->addChild(chatShowItem,0,CHATSHOWENTRYTAG+1111);
    }
    else
    {
 
         menu->removeChildByTag(CHATSHOWENTRYTAG+1111, true);
        CCString *showStr=CCString::createWithFormat("%s:%s",message->getRoleNickName()->getCString(),message->getContent()->getCString());
        
        int len=showStr->length()/3 +showStr->length()%3;
        
        if (len>15) {
            chatMore->setVisible(true);
        }
        else
        {
            chatMore->setVisible(false);
        }
        chat->setString(showStr->getCString());
    }
    
    
}
void SGMainLayer::chatShowItemClickSend(CCObject *obj)
{
    main::ViewSpectacularItemRequest *info = new main::ViewSpectacularItemRequest();
    info->set_ssid(chatShowItemSsid);
    info->set_roleid(chatShowRoleId);
    info->set_type(chatShowType);
    SGSocketClient::sharedSocketClient()->send(MSG_CHAT_SHOWITEM, info);
}

void SGMainLayer::getShowItemListener(cocos2d::CCObject *object)
{
    portraitMenu->setTouchEnabled(false);
    
    SGSocketRequest *request = (SGSocketRequest*)object;
    if(!request)
    {
        CCLOG("getPlayerInfoListener error !!! ");
        return;
    }
    main::ViewSpectacularItemResponse *chatShowInfo = (main::ViewSpectacularItemResponse*)request->m_msg;
    if (chatShowInfo) {
        showChatItemByType(chatShowInfo);
    }
}
void SGMainLayer::showChatItemByType(main::ViewSpectacularItemResponse *chatShowInfo)
{
    SGBaseMilitaryCard * chatShowItem = NULL;
    int type = chatShowInfo->type();
    switch (type) {
        case -1:
        {
            SG_SHOW_WINDOW(str_MainLayer_str21);
        }
            break;
        case 1:
        {
            //武将
            SGOfficerCard *officerCard = new SGOfficerCard();
            chatShowItem = officerCard;
            chatShowItem->autorelease();
            //CCDictionary *dicc = SGCardAttribute::getValue(chatShowInfo->currlevel(), chatShowInfo->itemid());
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById( chatShowInfo->itemid());
            
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(chatShowInfo->itemid());
            
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type,chatShowInfo->currlevel()+1);
            
            officerCard->setItemId(chatShowInfo->itemid());
            officerCard->setCurrStar(chatShowInfo->currstar());
            officerCard->setCurrLevel(chatShowInfo->currlevel());
            officerCard->setCurrExp(chatShowInfo->currexp());
            officerCard->setAtk(chatShowInfo->atk());
            officerCard->setDef(chatShowInfo->def());
            officerCard->setSpeed(chatShowInfo->speed());
            officerCard->setMorale(chatShowInfo->hp());
            
            officerCard->setLordId(chatShowInfo->lordskillid());
            officerCard->setLordExp(chatShowInfo->lordskillexp());
            officerCard->setOfficerSkil(chatShowInfo->geneskillid());
            officerCard->setSkillExp(chatShowInfo->geneskillexp());
           officerCard->setGoverning(chatShowInfo->governing());
            
            int fLeng = chatShowInfo->reasoninfo_size();
            std::vector<int> fateList;
            for (int k = 0;k<fLeng; k++)
            {
                main::ReasonInfo protoFate = chatShowInfo->reasoninfo(k);
                int fateId = protoFate.reasonid();
                int fateState = protoFate.isactivate();
                
                fateList.push_back(fateId *10 + fateState);
            }
            officerCard->setFateList(fateList);
            
            if (temp->getOfficerNextId()) {
                officerCard->setCost(temp->getOfficerCost());
            }
            
            
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
            if (expdata != NULL) {
                officerCard->setMaxExp(expdata->getExpValue());
            }
            officerCard->setOfficerName(temp->getOfficerName());
            officerCard->setMaxLevel(temp->getOfficerMaxLevel());
            
            officerCard->setRound(temp->getOfficerRound());
            
            
            
            officerCard->setRace(temp->getOfficerRace());
            officerCard->setItemType(temp->getOfficerType());
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            
            //转生次数
            officerCard->setAdNum(temp->getAdvNum());
            //武将原型ID
            officerCard->setProtoId(temp->getOfficerProtoId());
            //速度修正系数
            officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
            
            officerCard->setHeadSuffixNum(temp->getIconId());//12.05
            
            if (strcmp(temp->getOfficerGeneralsSkill()->getCString(), "0") &&temp->getOfficerGeneralsSkill()->length()>3)
            {
                std::string aaa(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
                
                std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
                //officerCard->setOfficerSkil(atoi(generalskill.c_str()));//这段代码具体不知是什么作用,就是这里把被动技置1了,现注掉 angel
            }
            if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
            {
                std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                officerCard->setLordSkill(atoi(lordskill.c_str()));
            }
            
            //身上装备
            int eleng = chatShowInfo->equipid_size();
            for (int j = 0;j<eleng; j++) {
                int equipId = chatShowInfo->equipid(j);
                SGEquipCard *equipCard = new SGEquipCard();
                equipCard->autorelease();
                CCDictionary *dicc = SGCardAttribute::getValue(1, equipId);
                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(equipId);
                int type = SGStaticDataManager::shareStatic()->getStrengEquipType(equipId);
                SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, 2);
                
                equipCard->setSsid(CHATSHOWENTRYTAG);
                //装备原型ID
                equipCard->setProtoId(temp->getEquipProtoId());
                equipCard->setItemId(equipId);
                equipCard->setCurrLevel(1);
                equipCard->setCurrExp(CHATSHOWENTRYTAG);
                equipCard->setOfficerCardId(officerCard->getItemId());
                equipCard->setCurrStar(temp->getEquipCurrStarLevel());
                equipCard->setItemType(temp->getEquipType());
                equipCard->setOfficerName(temp->getEquipName());
                officerCard->addEquip(equipCard);
                
                //转生等级
                equipCard->setUpgradelevel(temp->getUpgradelevel());
                
                //转生次数
                equipCard->setAdNum(temp->getAdvNum());
                //转生最大星级
                equipCard->setUpgradestar(temp->getUpgradestar());
                equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                equipCard->setMaxExp(expdata->getExpValue());
                equipCard->setRound(temp->getEquipRound());
                equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                equipCard->setExpRate(temp->getEquipExpRate());
                equipCard->setGoverning(temp->getEquipGoverning());
                equipCard->setMaxStar(temp->getEquipNextId());
                equipCard->setMaxLevel(temp->getEquipMaxLevel());
                equipCard->setHeadSuffixNum(temp->getIconId());//12.06
                if (temp->getEquipNextId()) {
                    equipCard->setCost(temp->getEquipCost());
                }
            }
        }
            break;
        case 2:
        {
            //装备
            
            SGEquipCard *equipCard = new SGEquipCard();
            chatShowItem = equipCard;
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(chatShowInfo->itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(chatShowInfo->itemid());
            
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, chatShowInfo->currlevel()+1);
            if (expdata==NULL) {
                expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, chatShowInfo->currlevel());
            }
            
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            
            equipCard->setItemId(chatShowInfo->itemid());
            equipCard->setCurrStar(chatShowInfo->currstar());
            equipCard->setCurrLevel(chatShowInfo->currlevel());
            //equipCard->setCurrExp(chatShowInfo->currexp());
            
            equipCard->setAtk(chatShowInfo->atk());
            equipCard->setDef(chatShowInfo->def());
            equipCard->setSpeed(chatShowInfo->speed());
            equipCard->setMorale(chatShowInfo->hp());
            
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            equipCard->setMaxExp(expdata->getExpValue());
            equipCard->setRound(temp->getEquipRound());
            equipCard->setExpRate(temp->getEquipExpRate());
            
            equipCard->setGoverning(temp->getEquipGoverning());
            equipCard->setItemType(temp->getEquipType());
            equipCard->setMaxStar(temp->getEquipNextId());
            equipCard->setOfficerName(temp->getEquipName());
            equipCard->setMaxLevel(temp->getEquipMaxLevel());
			
			equipCard->setHeadSuffixNum(temp->getIconId());//12.06
			
            if (temp->getEquipNextId()) {
                equipCard->setCost(temp->getEquipCost());
            }
            equipCard->autorelease();
        }
            break;
        case 3:
        {
            //士兵
            SGSoldierCard *soldierCard = new SGSoldierCard();
            chatShowItem = soldierCard;
            SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(chatShowInfo->itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, chatShowInfo->currlevel()+1);
            
            soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
            
            soldierCard->setItemId(chatShowInfo->itemid());
            soldierCard->setCurrStar(chatShowInfo->currstar());
            soldierCard->setCurrLevel(chatShowInfo->currlevel());
            soldierCard->setCurrExp(chatShowInfo->currexp());
            
            soldierCard->setAtk(chatShowInfo->atk());
            soldierCard->setDef(chatShowInfo->def());
            soldierCard->setMorale(chatShowInfo->hp());
            
            soldierCard->setMaxExp(expdata->getExpValue());
            soldierCard->setOfficerName(temp->getSoldierName());
            soldierCard->setRound(temp->getSoldierRound());
            
            soldierCard->setsoldiertalentskill(temp->getTalentSkill());//士兵增加
            soldierCard->setsoldiergrowskill(temp->getGrowSkill());
            
            soldierCard->autorelease();
        }
            break;
        case 4:
        {
            //杂物
            SGPropsCard *propCard = new SGPropsCard();
            chatShowItem = propCard;
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(chatShowInfo->itemid());
            
            propCard->setItemId(chatShowInfo->itemid());
            propCard->setCurrStar(chatShowInfo->currstar());
            
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
			propCard->setHeadSuffixNum(prop->getIconId());//12.05
			
            propCard->autorelease();
        }
            break;
        case 5:
        {
            
            SGPiecesCard*piecesCard=new SGPiecesCard();
            chatShowItem = piecesCard;
            
            
            SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(chatShowInfo->itemid());
                
                    piecesCard->setSell(pieces->getPiecesPrice());//价格
                    piecesCard->setOfficerName(pieces->getPiecesName());//名称
                    piecesCard->setPiecesName(pieces->getPiecesName());
                    piecesCard->setItemType(pieces->getPiecesType());
                 //   piecesCard->setItemType(itemType);//类型
                    piecesCard->setCurrLevel(pieces->getStarLevel());//星级
                    piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
                    piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
                    piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
                    piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
                    piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
                    piecesCard->setCurrStar(pieces->getStarLevel());

                    piecesCard->autorelease();

            
            //碎片
        }
            break;
		case 6:
		{
			SGConsumableCard *consumeCard = new SGConsumableCard();
			chatShowItem = consumeCard;
			SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(chatShowInfo->itemid());
			
			consumeCard->setItemId(chatShowInfo->itemid());
			
			
			consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
			consumeCard->setSell(consumeModel->getConsumePrice());
			consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
			consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
			consumeCard->setOfficerName(consumeModel->getConsumeName());
			consumeCard->setMoneyType(consumeModel->getMoneyType());
			consumeCard->setItemType(consumeModel->getConsumeType());
			consumeCard->setHeadSuffixNum(consumeModel->getIconId());
			consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
			consumeCard->setPicId(consumeModel->getConsumePicId());
			consumeCard->autorelease();
		}
			break;
        case 7://材料
        {
            SGMaterialCard *materialCard = new SGMaterialCard();
            chatShowItem = materialCard;
            SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(chatShowInfo->itemid());
            
            materialCard->setItemId(chatShowInfo->itemid());
            
            materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
            materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
            materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
            materialCard->setOfficerName(materialModel->getMaterialName());
            materialCard->setSell(materialModel->getMaterialSoldPrice());
            materialCard->autorelease();
        }
            break;
        default:
            SG_SHOW_WINDOW(str_MainLayer_str22);
            break;
    }
    if (chatShowItem) {
        SGMainManager::shareMain()->openChatShowLayerByType(chatShowItem,type,SGMainManager::shareMain()->getMainScene()->nowLayerTag);
    }
}


//进入聊天室
void SGMainLayer::buttonClickChatRoom()
{
    LayerTag layerTag = SGMainManager::shareMain()->getMainScene()->nowLayerTag;
    if (layerTag == sg_visitLayer
        || layerTag == sg_playerInfoLayer
        || layerTag == sg_generalInfoLayer
        || layerTag == sg_equipInfoLayer
        || layerTag == sg_soulCardLayer
        || layerTag == sg_soldierInfoLayer
        || layerTag == sg_propInfoLayer
        || layerTag == sg_embattleLayer
        || layerTag == sg_skillcardInfoLayer
        || layerTag == sg_tongQueLayer
        || layerTag == sg_vipGiftLayer
        || layerTag == sg_strengLayer)
   {
        CCLOG("Can't touch chatBt");
    }
    else
    {
        EFFECT_PLAY(MUSIC_BTN);
        portraitMenu->setEnabled(false);
        if(money_Button)
        {
            money_Button->setVisible(false);
            money_Button->setEnabled(false);
        }
        if(money_Button1)
        {
            money_Button1->setVisible(false);
            money_Button1->setEnabled(false);
        }
        if (chongzhi) {
            //chongzhi->setVisible(false);
            chongzhi->setEnabled(false);
        }
        SGChatMessageCenter::shareChatMessageCenter()->openChatRoom();
    }

    
}

void SGMainLayer::showPlayerInfo()
{
    if (SGMainManager::shareMain()->getMainScene()->nowLayerTag == sg_strengLayer)
    {
        return ;
    }
    
    EFFECT_PLAY(MUSIC_BTN);
   // SGMainManager::shareMain()->showPlayerInfo();
    
    
}
void SGMainLayer::bossListener(CCObject *obj)
{
    SGSocketRequest *sr=(SGSocketRequest *)obj;
    if (sr) {
        main::MainBossResponse *rs=(main::MainBossResponse *)sr->m_msg;
        if (rs->state()==0 || rs->state()==1) {
            BossBattleJoinData *data =new BossBattleJoinData();
            data->canJoin=rs->state();
            data->bossLev=rs->bosslevel();
            data->description=CCString::create(rs->slayinfo());
            data->formName=CCString::create(rs->thefirstthree());
            
            if(rs->state()==1)
            {
                data->startMin=rs->startedtime();
                MUSIC_STOP;
                MUSIC_PLAY(MUSIC_BOSSBATTLE);
            }
            else
                data->startMin=rs->residuetime();
            SGMainManager::shareMain()->showBossBattleJoinInLayer(data);
        }
        else
        {
            
            CCLog("错误");
        }
    }
}

#pragma mark -
#pragma mark - -----------pvp battle actions-----------
void SGMainLayer::responseVSApply(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::PvpApplyResponse *response = (main::PvpApplyResponse *)request->m_msg;
    CCLog("Apply==================%d", response->result());
    if (response) {
        if (response->result() != 9) {
            //如果不是重复报名，修改vsresult
            vsresult = response->result();
        }
        /*
         vsresult
         0 报名成功
         1 等级不够
         2 背包已满
         3 军食不够
         4 胜利次数不够20场
         5 时段不开放
         6 赛季未开放
         9 已经报名
         */
        switch (response->result()) {
            case 0:
            {
                // 不做任何事情, 等待响应 CheckReadyResponse
                // 显示 cant box
                SGBMatchStartBox *box = SGBMatchStartBox::create(this);
                SGMainManager::shareMain()->showBox(box);
            }
                break;
            case 1:
            {
                SGMainManager::shareMain()->showMessage(str_MainLayer_str23);
            }
                break;
            case 2:
            {
                SGMainManager::shareMain()->showMessage(str_CantAdvance_tips24);
            }
                break;
            case 3:
            {
                SGMainManager::shareMain()->showMessage(str_MainLayer_str24);
            }
                break;
            case 4:
            {
                SGMainManager::shareMain()->showMessage(str_MainLayer_str25);
            }
                break;
            case 5:
            {
                SGMainManager::shareMain()->showMessage(str_MainLayer_str26);
            }
                break;
            case 6:
            {
                SGMainManager::shareMain()->showMessage(str_MainLayer_str27);
            }
                break;
            case 9:
            {
                SGMainManager::shareMain()->showMessage(str_MainLayer_str28);
            }
                break;
            default:
                SGMainManager::shareMain()->showMessage(str_MainLayer_str29);
                break;
        }
    }
}

void SGMainLayer::showBMatchBox(CCObject *sender)
{
    CCLog("Match==================OK");
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::CheckReadyResponse *response = (main::CheckReadyResponse *)request->m_msg;
    
    if (response) {
        if (vsresult == 0) { // apply success
            SGMainManager::shareMain()->closeBox();
            int result = response->result();
            if (result == 1) { // 判断速配是否成功
                SGMainManager::shareMain()->getMainScene()->showPvpMatch(false);
                SGBMatchAlertBox *box = SGBMatchAlertBox::create(this);
                SGMainManager::shareMain()->showBox(box);
                SGMainScene * mainScene = SGMainManager::shareMain()->getMainScene();
                if (mainScene->nowLayerTag == sg_visitShowCard) {
                    SGBaseLayer *baseLayer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
                    baseLayer->setIsCanTouch(true);
                    
                }
            }else {
                CCLog("Match==================NOT OK");
            }
        }
    }
}

void SGMainLayer::responseVSReady(CCObject *sender)
{
    // no response
    // MSG_SEASONWAR_READYS
}

void SGMainLayer::flushVSerInfo(CCObject *sender)
{
    CCLog("tt------------------MSG_SEASONWAR_MATCHS");
    
    // 获取到vs双方数据并刷新界面
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PvpMatchingResponse *response = (main::PvpMatchingResponse *)req->m_msg;
    
    if (response && response->result() == 1) {
        int var1 = response->icon();
        int var2 = response->pos();
        int var3 = response->level();
        int var4 = response->fightcounter();
        float var5 = response->winratio();
        std::string name = response->name();
        int var6 = response->serverid();
        std::string servername = response->servername();
       // CCLOG("%s",servername.c_str());//cgpLogDel
        SGRankPlayer *armyInfo = SGRankPlayer::create(name.c_str(), var1, var2, var3, var4, var5,var6,servername.c_str());
        
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGBMatchEndBox *box = (SGBMatchEndBox *)mainScene->getChildByTag(box_matchEnd);
        if (box) {
            box->setArmyData(armyInfo);
        }else {
            SGBMatchEndBox *box = SGBMatchEndBox::create(this);
            SGRankPlayer *selfInfo = SGPlayerInfo::sharePlayerInfo()->getPvpRank();
            box->setSelfData(selfInfo);
            box->setArmyData(armyInfo);
            SGMainManager::shareMain()->showBox(box);
        }
        //        SGBattleManager::sharedBattle()->closeReceive();
        
        ipPort = response->port();
        ipAddr = response->ip();
        fightId = response->fightid();
        CCUserDefault::sharedUserDefault()->setStringForKey("pvp_fightId", fightId);
        CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_tran", true);
        DDLog("ip-----------------%d, %s", ipPort, ipAddr.c_str());
        //        CCAction *action = CCSequence::create(CCDelayTime::create(1.0f),
        //                                              CCCallFunc::create(this, callfunc_selector(SGMainLayer::loginPvpServer)),
        //                                              NULL);
        //        this->runAction(action);
    }else {
        // get player error, show error box
        DDLog("cc========================match fail");
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        SGBaseLayer *baseLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
        SGBMatchEndBox *box = (SGBMatchEndBox *)mainScene->getChildByTag(box_matchEnd);
        
        if (box) box->boxClose();
        if (baseLayer) baseLayer->setIsCanTouch(true);
        if (mainLayer) mainLayer->setIsCanTouch(true);
        
        SGCantAdvanceBox *newBox = SGCantAdvanceBox::create(NULL, NULL, 38, 0);
        SGMainManager::shareMain()->showBox(newBox);
    }
}

void SGMainLayer::loginPvpServer()
{
    SGBattleManager::sharedBattle()->isMatchSucc = false;
    SGSocketClient::sharedSocketClient()->startConn(ipAddr.c_str(), ipPort);
}

void SGMainLayer::requestFinished(int msgid, cocos2d::CCObject *data)
{
    if (msgid == MSG_HTTP_LOGIN){
        SGSocketClient::sharedSocketClient()->startConn(ipAddr.c_str(), ipPort);
    }
}

void SGMainLayer::requestFailed()
{
    CCLOG("pvp login error ############## ");
}

void SGMainLayer::showRewardBox(CCObject *sender)
{
    SGSocketRequest *req = (SGSocketRequest *)sender;
    main::PlotQuitEndResponse *response = (main::PlotQuitEndResponse *)req->m_msg;
    SGPlayerInfo *playInfo = SGPlayerInfo::sharePlayerInfo();
    int _plotId = playInfo->getPlotId();
    SGBattleManager::sharedBattle()->isWin = true;
    SGAIManager::shareManager()->isInitPve = true;
   
    CCLog("===============q_reward-----------%d, %d", response->state(), _plotId);
    if (response && response->state() == 1) {
        int jexp =  response->meritorious();
        ////////////////////////////接受标记战斗类型的字段，原逻辑是只有进入战斗才会设置战斗类型
//        main::CooperationModel cooperationModel=(main::CooperationModel )response->coo();
        
//        HelpFriendPlayLev=cooperationModel.rolelv();
//        HelpFriendRoleName = cooperationModel.name().c_str();
//        CCLog("%s",HelpFriendRoleName);
//        HelpFriendRoleId=cooperationModel.roleid();
//        HelpFriendItemId = cooperationModel.itemid();
//        HelpFriendItemLevel = cooperationModel.itemlevel();
        
        HelpFriendRoleId=response->cooid();
        
        int typetype=response->type();
        if(typetype==0)
        {
             SGBattleManager::sharedBattle()->setBattleType(BT_ADVENTURE);
        }
        else
        {
             SGBattleManager::sharedBattle()->setBattleType(BT_SURVIVAL);
            playInfo->setismaxlevel(0);
//             playInfo->setismaxlevel(response->ismaxlevels()); //mm: 这个字段已经删掉，改成了oldLevel，代表玩家之前的等级，原本的值也一直是0。
        }
        
        /////////////////////////////////
//    int    currGold = playInfo->getPlayerGold();
//    int    currCoin = playInfo->getPlayerCoins();
        
        
        
        
        ////////////////////////////////
        int var1 = response->rating();
        int var2 = response->exp();
        int var3 = response->gold();
		
		
		int soul = 0;// @wangyang, 其实军魂,这里忽略掉
		
        //是否跳转西域商人
        bool isShowExoMerch = response->xiyu() > 0;
        
		
        CCArray *var4 = CCArray::create();
        var4->retain();
        SGStoryReward *reward = SGStoryReward::create(var1, var2, var3, jexp, soul, 0, 0, var4);
        //每中结算都在外围retain，在FightRewadLayer中无需retain，只在析构中safe release。
        reward->retain();
        
        reward->setBeforeLevel(response->oldlevel());
        reward->setLianDan(response->liandan());
        reward->setExtExp(response->addexp());
        int size = response->plotdropitems_size();
        if (size > 0) { // 先将卡片加入背包中
            CCArray *rItems = reward->getItems();
            rItems->retain() ;
            for (int ii = 0; ii < size; ii++) {
                SGStoryRdItem *rdModel = new SGStoryRdItem();
                SGBaseMilitaryCard *card = new SGBaseMilitaryCard();
                main::PlotDropItemMessage itemmsg = response->plotdropitems(ii);
                CCArray *rewardItem = CCArray::create();
                rewardItem ->retain();
                
                for(int iii = 0; iii < itemmsg.item_size(); iii++)
                {
                    bool isHave= false;
                    main::PlotDropItems item = itemmsg.item(iii);
                    if (item.officercards().itemid() > 0) {
                        int id = item.officercards().itemid();
                        SGOfficerDataModel *off = SGStaticDataManager::shareStatic()->getOfficerById(item.officercards().itemid());
                        //int tag = off->getOfficerRace();
                        int tag = off->getOfficerVisitType();
                        isHave = detectIsHaveItem(rewardItem,id);
                        if (!isHave)
                        {
                            rdModel = SGStoryRdItem::create(id, tag, 0);
                            rdModel->addCount();
                        }
                        //rdModel = SGStoryRdItem::create(id, tag, 0);
                        card = createOfficerCardWithInfo(item.officercards());
                        //playInfo->addOfficerCard((SGOfficerCard *)card);
                    }else if (item.equipmentcards().itemid() > 0) {
                        int id = item.equipmentcards().itemid();
                        SGEquipmentDataModel *equ = SGStaticDataManager::shareStatic()->getEquipById(id);
                        //int tag = equ->getEquipType();
                        
                        ///
                        int size = rewardItem->count();
                        if(size==0)
                        {
                            isHave =  false;
                        }else
                        {
                            for(int i = 0; i < size; i++)
                            {
                                int nItemId =  id;
                                SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                                int nItemid2 = item_->getItemId();
                                if(nItemId==nItemid2)
                                {
                                    item_->setCount(item_->getCount()+1);
                                    isHave =  true;
                                    break;
                                }
                                
                            }
                            
                        }
                        
                        ///

                        int tag = equ->getEquipVisitType();
                       // isHave = detectIsHaveItem(rewardItem,id);
                        if (!isHave)
                        {
                            rdModel = SGStoryRdItem::create(id, tag, 1);
                            rdModel->addCount();
                        }
                        
                    }else if (item.propscards().itemid() > 0) {
                        int id = item.propscards().itemid();
                        ///
                        int size = rewardItem->count();
                        if(size==0)
                        {
                            isHave =  false;
                        }else
                        {
                            for(int i = 0; i < size; i++)
                            {
                                int nItemId =  id;
                                SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                                int nItemid2 = item_->getItemId();
                                if(nItemId==nItemid2)
                                {
                                    item_->setCount(item_->getCount()+item.propscards().curritemnumber());
                                    isHave =  true;
                                    break;
                                }
                                
                            }
                            
                        }
                        
                        ///
                        
                        //isHave = detectIsHaveItem(rewardItem,id);
                        if (!isHave)
                        {
                            rdModel = SGStoryRdItem::create(id, 0, 3);
                            rdModel->setCount(item.propscards().curritemnumber());
                        }
                        

                    }//加入碎片掉落处理
                        else if (item.fragmentcards().itemid() > 0)
                        {
                            int id = item.fragmentcards().itemid();
                            
                            ///
                            int size = rewardItem->count();
                            if(size==0)
                            {
                                isHave =  false;
                            }else
                            {
                                for(int i = 0; i < size; i++)
                                {
                                    int nItemId =  id;
                                    SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                                    int nItemid2 = item_->getItemId();
                                    if(nItemId==nItemid2)
                                    {
                                        item_->setCount(item_->getCount()+item.fragmentcards().curritemnumber());
                                        isHave =  true;
                                        break;
                                    }
                                    
                                }
                                
                            }
                            
                            ///
                            
                            //isHave = detectIsHaveItem(rewardItem,id);
                            if (!isHave)
                            {
                                rdModel = SGStoryRdItem::create(id, 0, 5);
                                rdModel->setCount(item.fragmentcards().curritemnumber());
                            }
                            
                        }
                    //加入消耗品
                        else if (item.consumablecards().itemid() > 0)
                        {
                            //消耗品掉落
                            int id = item.consumablecards().itemid();
                            ///
                            int size = rewardItem->count();
                            if(size==0)
                            {
                                isHave =  false;
                            }else
                            {
                                for(int i = 0; i < size; i++)
                                {
                                    int nItemId =  id;
                                    SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                                    int nItemid2 = item_->getItemId();
                                    if(nItemId==nItemid2)
                                    {
                                        item_->setCount(item_->getCount()+item.consumablecards().curritemnumber());
                                        isHave =  true;
                                        break;
                                    }
                                    
                                }
                                
                            }
                            if (!isHave)
                            {
                                rdModel = SGStoryRdItem::create(id, 0, 6);
                                rdModel->setCount(item.consumablecards().curritemnumber());
                            }
                            
                            
                        }
                    //材料
                        else if (item.materialitemprotos().itemid() > 0)
                        {
                            //iType 7
                            //材料掉落
                            int id = item.materialitemprotos().itemid();
                            ///
                            int size = rewardItem->count();
                            if(size==0)
                            {
                                isHave =  false;
                            }else
                            {
                                for(int i = 0; i < size; i++)
                                {
                                    int nItemId =  id;
                                    SGStoryRdItem* item_ = (SGStoryRdItem*)rewardItem->objectAtIndex(i);
                                    int nItemid2 = item_->getItemId();
                                    if(nItemId==nItemid2)
                                    {
                                        item_->setCount(item_->getCount()+item.materialitemprotos().itemnum());
                                        isHave =  true;
                                        break;
                                    }
                                    
                                }
                                
                            }
                            if (!isHave)
                            {
                                rdModel = SGStoryRdItem::create(id, 0, 7);
                                rdModel->setCount(item.materialitemprotos().itemnum());
                            }
                            
                        }
                    

                    if (!isHave)
					   rewardItem->addObject(rdModel);
                }
                rItems->addObject(rewardItem);
                
            }
        }

        DDLog("q_reward===============-----------=%d", response->liandan());
        SGPlayerInfo::sharePlayerInfo()->setSkipFightCount(response->plotharrycount());
        SGPlayerInfo::sharePlayerInfo()->updateStoryData(_plotId, response->rating());
        
        SGMainScene *mainScen = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScen->getChildByTag(sg_mainLayer);
        mainLayer->setIsCanTouch(false);
       ////////kanatakanata
            SGFightRewardLayer * prolayer =  (SGFightRewardLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_fightrewardLayer);
        if(prolayer)
        {
            prolayer->setjump(true);
            prolayer->updateAnimation(reward);
            
            //  layer->setIsCanTouch(true);
        }
        else
        {
			////////////
			SGFightRewardLayer *layer = SGFightRewardLayer::create(reward,1, isShowExoMerch);//1115
			BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
			if(btp==BT_SURVIVAL)
        {
			layer->setjump(true);
        }
                
                
                
    

        SGMainManager::shareMain()->showLayer(layer);
        }
    }
}
bool SGMainLayer::detectIsHaveItem(CCArray *arr, int  nid)
{
    int size = arr->count();
    if(size==0)
        return false;
    for(int i = 0; i < size; i++)
    {
        int nItemId =  nid;
        SGStoryRdItem* item = (SGStoryRdItem*)arr->objectAtIndex(i);
        int nItemid2 = item->getItemId();
        if(nItemId==nItemid2)
        {
            item->addCount();
            return true;
        }
        
    }
    return  false;
}
void SGMainLayer::addFriendListener(CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest*)obj;
    if(!req)
    {
        CCLOG("AddFriend Error");
        return;
    }
    
    main::AddFriendResponse *response = (main::AddFriendResponse*)req->m_msg;
    
    //0; 异常失败
    //1; 自己的好友列表满了
    //2; 对方的好友满了
    //3; 已经在你的好友列表中
    //4; 已经在你的黑名单中,解除黑名单并申请
    //5; 已经在他的黑名单中
    //6; 已经申请过了,在他的申请列表中
    //7; 正常添加
    if(m_isbattelAddFriend)
    {
         SGMainManager::shareMain()->showMessage(str_FriendBase_str8);
         m_isbattelAddFriend = false;
         return;
    }
    
    switch(response->state())
    {
        case 0:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str2);
            break;
        }
        case 1:
        {
            //弹窗提示
            SGFriendCountFull *full = SGFriendCountFull::create(this);
            SGMainManager::shareMain()->showBox(full);
            
            break;
        }
        case 2:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str3);
            break;
        }
        case 3:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str4);
            break;
        }
        case 5:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str5);
            break;
        }
        case 6:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str6);
            break;
        }
        case 4:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str7);
            break;
        }
        case 7:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str8);
            break;
        }
    }
    
}

#pragma mark -
#pragma mark - -----------end pvp battle actions-----------

void SGMainLayer::rewardListener(cocos2d::CCObject *obj)
{
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    SGSocketRequest *request = (SGSocketRequest *)obj;
    if(!request)
    {
        return;
    }
    playerInfo->getrewardNotice()->removeAllObjects();
    main::RewardNoticeMessage* response = (main::RewardNoticeMessage*)request->m_msg;
    int leng = response->update_size();
    for (int i = 0; i<leng; i++) {
        //CCString *str = CCString::createWithFormat("%d",response->update(i));
        main::RewardTypeState notice = ( main::RewardTypeState )response->update(i);
       // CCLOG("A IS %d",response->update(i));
        SGRewardNoticeItem * noticeItem = new  SGRewardNoticeItem();
        noticeItem->settypeItem(notice.type());
        noticeItem->setstateItem(notice.state());
        playerInfo->addrewardNotice(noticeItem);
    }
}
void SGMainLayer::showSeasonMainLayer(CCObject *sender)
{
    
//    DDLog("box------------------SEASON");
//    SGMainManager::shareMain()->showPvpMainLayer();
//    
//    return;
//    
//    SGSocketRequest *request = (SGSocketRequest *)sender;
//    main::PvpEntryResponse *response = (main::PvpEntryResponse *)request->m_msg;
//    SGBattleData *battleData = NULL;
//    SGRankPlayer *selfPlayer = NULL;
//    if (response) {
//        int var1 = response->isopen();
//        int var2 = response->toppos();
//        int var3 = response->toplevel();
//        int var4 = response->fighttimes();
//        int var5 = response->score();
//        int var6 = response->joinmax();
//        int var7 = response->jointoday();
//        
//        std::string str = response->seasontime();
//        float winr = response->winratio();
//        
//        CCString *name = SGPlayerInfo::sharePlayerInfo()->getNickName();
//        int iconId = SGPlayerInfo::sharePlayerInfo()->getPlayerImageID();
//        // create(const char *name, int hid, int rank, int levl, int ftime, float rwin)
//        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var2, var3, var4, winr);
//        battleData = SGBattleData::create(str.c_str(), var1, var5, var7, var6);
//        battleData->setPlayer(selfPlayer);
//        
//        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var2, var3, var4, winr);
//        SGPlayerInfo::sharePlayerInfo()->setPvpRank(selfPlayer);
//    }
//    if (battleData != NULL) {
//        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//        SGBattleMainLayer *layer = (SGBattleMainLayer *)mainScene->getChildByTag(sg_battleMainLayer);
//        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mailLayer);
//        if (!layer) {
//            layer = SGBattleMainLayer::create(battleData);
//        }
//        if (mainLayer) {
//            mainLayer->setVisible(true);
//            mainLayer->setIsCanTouch(true);
//            mainLayer->visitshow();
//        }
//        mainScene->setVisible(true);
//        mainScene->addShowLayer(layer);
//    }
//    SGMainManager::shareMain()->closeBox();
}




//根据不同的级数,确定是否显示布阵界面的问号,提示开启新的布阵格子
void SGMainLayer::setEmbattleFlag(int level)
{
	for (int i = 0; i < 8; i++)
	{
		if (levelArray[i] == level)
		{
			// 升级到某个开布阵武将位置的级数,就设置本地标志,需要显示叹号
			//CCUserDefault::sharedUserDefault()->setBoolForKey("show_embattle_flag", true);
			SGMainManager::shareMain()->setEmbattleKey(true);
			CCUserDefault::sharedUserDefault()->setBoolForKey(CCString::createWithFormat("%s%d", SGPlayerInfo::sharePlayerInfo()->getNickName()->getCString(),
																						 level)->getCString(), true);
			CCUserDefault::sharedUserDefault()->flush();
		}
	}
}

void SGMainLayer::updatePlayerInfo(cocos2d::CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;

    main::updateRoleResponse* response = (main::updateRoleResponse*)request->m_msg;
    if(!request)
    {
        return;
    }
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    if(playerInfo->getPlayerLevel()!=response->level())
    {
        HelpFriendPlayLev = playerInfo->getPlayerLevel();
    }
    
    playerInfo->setPlayerPower(response->power());
    playerInfo->setPvpPower(response->pvppower());
    playerInfo->setPlayerExp(response->exp());
    playerInfo->setPlayerLevel(response->level());
	
	//此处记录级数变化,来确定是否开启新的布阵格数
	if (!CCUserDefault::sharedUserDefault()->getBoolForKey(CCString::createWithFormat("%s%d", SGPlayerInfo::sharePlayerInfo()->getNickName()->getCString() ,
																response->level())->getCString()))
	{
		this->setEmbattleFlag(response->level());
	}
    playerInfo->setPlayerCoins(response->coins());
    playerInfo->setPlayerGold(response->gold());
//    playerInfo->setplayerAward(response->award());
    playerInfo->setPveCircleTime(response->pvecircletime());
    playerInfo->setPveLeftTime(response->pvelefttime());
    playerInfo->setPvpCircleTime(response->pvpcircletime());
    playerInfo->setPvpLeftTime(response->pvplefttime());
    playerInfo->setcombatValue(response->combatvalue());
    playerInfo->setCanRewardDayTask(response->canrewarddaytask());
	playerInfo->setBestEquip(response->bestequip());
    playerInfo->setJunHun(response->junhun());
    playerInfo->setplayerPvpNum(response->pvptopscore());
    playerInfo->setSkipFightCount(response->plotharrycount());
    
    //更新勾玉信息
    playerInfo->setPlayerJewels(response->gouyu());
    //更新竞技场币(征服点)
    playerInfo->setPlayerArenaScore(response->arenacoin());
    
    m_nPveCircleTime = SGPlayerInfo::sharePlayerInfo()->getPveCircleTime();
    m_nPveLeftTime = SGPlayerInfo::sharePlayerInfo()->getPveLeftTime();
    m_nPvpLeftTime = SGPlayerInfo::sharePlayerInfo()->getPvpLeftTime();
    m_nPvpCircleTime = SGPlayerInfo::sharePlayerInfo()->getPvpCircleTime();
	
	////@warning 更新上阵数据，this is a hole ，big deep hole ^_*，为了更新战斗力
	if (SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer))
	{
		((SGEmbattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer))->updateEmbattle(1);
        ((SGEmbattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer))->updateBestEquipBt(playerInfo->getBestEquip());
	}
    //更新最强装备
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer))
    {
        ((SGFirstLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer))->updateNoticeBuZhen();
    }
    
    
}


void SGMainLayer::updatepvppve(cocos2d::CCObject *sender)
{
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    //HelpFriendPlayLev = playerInfo->getPlayerLevel();//不需要刷新等级，在updatePlayerInfo刷
    SGSocketRequest *request = (SGSocketRequest *)sender;
    if(!request)
    {
        return;
    }
    main::updateRolePowerResponse* response = (main::updateRolePowerResponse*)request->m_msg;
    playerInfo->setPlayerPower(response->power());
    playerInfo->setPvpPower(response->pvppower());
    
    // playerInfo->setPlayerGovering(response->governing());
    playerInfo->setPveCircleTime(response->pvecircletime());
    playerInfo->setPveLeftTime(response->pvelefttime());
    playerInfo->setPvpCircleTime(response->pvpcircletime());
    playerInfo->setPvpLeftTime(response->pvplefttime());
    
    m_nPveCircleTime = SGPlayerInfo::sharePlayerInfo()->getPveCircleTime();
    m_nPveLeftTime = SGPlayerInfo::sharePlayerInfo()->getPveLeftTime();
    m_nPvpLeftTime = SGPlayerInfo::sharePlayerInfo()->getPvpLeftTime();
    m_nPvpCircleTime = SGPlayerInfo::sharePlayerInfo()->getPvpCircleTime();
    
    
}

void SGMainLayer::updateFriendBtn()
{
    //好友挪到系统里面，所以"好友"的红点现在改为加在“系统”上。
    SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
	CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getApplyList();
	if (temp->data && temp->data->arr && temp->count() != 0  && playerInfo->getPlayerLevel() >= playerInfo->getShowPlayerNoticeLimitLev())
	{
		this->getChildByTag(1235)->setVisible(true);
	}
	else
	{
		this->getChildByTag(1235)->setVisible(false);
	}
}

void SGMainLayer::updatehomebtn()
{
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
//  有可免费探访机会
    if (playerInfo->getFreevisit() == 1 && playerInfo->getPlayerLevel() >= playerInfo->getShowPlayerNoticeLimitLev() )
    {
        this->getChildByTag(1111)->setVisible(true);
    }else
    {
        this->getChildByTag(1111)->setVisible(false);
    }
//    if(STATE_YES == playerInfo->getPlayerNoticeById(4 , true) )
//        this->getChildByTag(1111)->setVisible(true);
//    else
//        this->getChildByTag(1111)->setVisible(false);
    
    int count = 0;
    SGFirstLayer *firstLayer = (SGFirstLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer);
    if(firstLayer)
    {
        for(int i=0;i<playerInfo->getFriendMails()->count();i++)
        {
            SGMailDataModel *mailData = (SGMailDataModel*)playerInfo->getFriendMails()->objectAtIndex(i);
            if(!mailData->getMailIsRead())
            {
                count ++;
            }
        }
        
        int systemCount = playerInfo->getSystemMails()->count();
        for(int i=0;i<systemCount;i++)
        {
            SGMailSystemDataModel *mailData = (SGMailSystemDataModel*)playerInfo->getSystemMails()->objectAtIndex(i);
            if(!mailData->getIsRead())
            {
                count ++;
            }
        }
        
        for(int i=0;i<playerInfo->getGMMails()->count();i++)
        {
            SGMailDataModel *mailData = (SGMailDataModel*)playerInfo->getGMMails()->objectAtIndex(i);
            if(!mailData->getMailIsRead())
            {
                count ++;
            }
        }
        firstLayer->setMailCount(count);
    }
    
    
    count += playerInfo->getApplyList()->count();
    CCArray *_array = playerInfo->getrewardNotice();
    CCObject *obj = NULL;
    int x = 0;
    CCARRAY_FOREACH(_array, obj)
    {
        SGRewardNoticeItem * item =(SGRewardNoticeItem *)obj;
        
        //CCString *str = (CCString *)obj;
        if (item->getstateItem()== 1 && x!=7 && x!=8)
        {
            count++;
            break;
        }
        x++;
    }
}
/////////////////////////////////////暂时就这样吧

 void  SGMainLayer::setupdatebattlebtnvisible()
{
    CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();
    int count=0;
    for(int i=0;i<temp->count();i++)
    {
        SGQAConcent * qa = (SGQAConcent*)temp->objectAtIndex(i);
        
        if(!qa->getState())
        {
            count ++;
        }
        
    }

    SGPlayerInfo * playerInfo = SGPlayerInfo::sharePlayerInfo();
    if(count!=0 && playerInfo->getPlayerLevel() >= playerInfo->getShowPlayerNoticeLimitLev())
    {
        this->getChildByTag(1236)->setVisible(true);
        
    }
    else
    {
        this->getChildByTag(1236)->setVisible(false);
        
    }

}



 void SGMainLayer::updateqa(SGQAConcent *player)
{
    
    
    CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();
    CCArray* array=CCArray::create();
    int m=temp->count();
    
    for(int i=0;i<m;i++)
    {
        
        SGQAConcent * qa = (SGQAConcent*)temp->objectAtIndex(i);
        
        array->addObject(qa);
        //   newtemp->addOb;ject(qa);       /////////新顺序

    }

    SGPlayerInfo::sharePlayerInfo()->getQAContent()->removeAllObjects();////清空原有信息
    
    
    ////////添加新信息
    
    int qqq=1;
    if(qqq==1)
    {
        SGPlayerInfo::sharePlayerInfo()->addQAContent(player);
        qqq=0;
    }
    
    for(int i=0;i<m;i++)
    {
        SGQAConcent * qa = (SGQAConcent*)array->objectAtIndex(i);
        
        //   newtemp->addObject(qa);       /////////新顺序
        
        SGPlayerInfo::sharePlayerInfo()->addQAContent(qa);
        
    }
    
}


/////////////////////
void SGMainLayer::updatebattlebtn(CCObject*obj)
{
//	CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();
//	if (/*temp->data && temp->data->arr && */temp->count() != 0)
//	{
//		this->getChildByTag(1236)->setVisible(true);
//	}
//	else
//	{
//		this->getChildByTag(1236)->setVisible(false);
//	}
    SGSocketRequest *req = (SGSocketRequest *)obj;
    main::UpdateQAResponse *response = (main::UpdateQAResponse *)req->m_msg;

    
    CCArray *temp = SGPlayerInfo::sharePlayerInfo()->getQAContent();
    int count=0;
    for(int i=0;i<temp->count();i++)
    {
        SGQAConcent * qa = (SGQAConcent*)temp->objectAtIndex(i);
        
        if(!qa->getState())
        {
            count ++;
        }
        
        if(qa->getID()==response->id())
        {
            qa->setState(0);
        }
        
    }

    
    if(count!=0)
    {
       this->getChildByTag(1236)->setVisible(true);
    
    }
    else
    {
        this->getChildByTag(1236)->setVisible(false);
    
    }

}

////
void SGMainLayer::purchPvePowerEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest *)obj;
    main::ShopPurchPvEPowerEntryResponse *response = (main::ShopPurchPvEPowerEntryResponse *)req->m_msg;
    if (response) {
        int left = response->leftcounter();
        int total = response->countermax();
        
        if (left > 0)
        {
            int recoverPower = response->power();//每次回复的体力
            
            CCString *data = CCString::createWithFormat("%d/%d", (total-left+1), total);
            int cost = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
            
            if (cost >= response->cost()) { // 有钱有次数
                
                SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 27, response->cost(), recoverPower, data);
                SGMainManager::shareMain()->showBox(box);

                SGMainManager::shareMain()->trackPurchase(CCString::create(str_MainLayer_str30), 1, response->cost());
            }else { // 无钱有次数
                SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 29, response->cost(), recoverPower, data);
                SGMainManager::shareMain()->showBox(box);
            }
        }else { // 无次数
            //            SGMainManager::shareMain()->showMessage("体力回满次数已用尽");
            SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 28, response->cost());
            if(box)
            SGMainManager::shareMain()->showBox(box);
        }
    }
}

void SGMainLayer::purchPvpPowerEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest *)obj;
    main::ShopPurchPvPPowerEntryResponse *response = (main::ShopPurchPvPPowerEntryResponse *)req->m_msg;
    if (response) {
        int left = response->leftcounter();
        int total = response->countermax();
        
        if (left > 0) {

            CCString *data = CCString::createWithFormat("%d/%d", (total-left+1), total);
            int cost = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
            
            if (cost >= response->cost()) { // 有钱有次数
                SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 31, response->cost(), response->power(), data);
                if (box)
                    SGMainManager::shareMain()->showBox(box);
            }else { // 无钱有次数
                SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 33, response->cost(),0, data);
                if (box)
                    SGMainManager::shareMain()->showBox(box);
            }
        }else { // 无次数
            SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 32, response->cost());
            if (box)
                SGMainManager::shareMain()->showBox(box);
        }
    }
}




void SGMainLayer::setIsCanTouch(bool touch, bool btn)
{
    SGBaseLayer::setIsCanTouch(touch);
    if (btn)
    {
       setPortraitMenuCanTouch(touch);
    }
}


void SGMainLayer::setPortraitMenuCanTouch(bool touch)
{
    portraitMenu->setEnabled(touch);
    portraitMenu->setTouchEnabled(touch);
    if (menu->getChildByTag(CHATSHOWENTRYTAG+1111))
    {
        chat->setVisible(touch);
        menu->getChildByTag(CHATSHOWENTRYTAG+1111)->setVisible(touch);
    }
    if (chongzhi)
    {
        chongzhi->setEnabled(touch);
    }
    if(money_Button)
    {
        money_Button->setVisible(touch);
        money_Button->setEnabled(touch);
    }
    if(money_Button1)
    {
        money_Button1->setVisible(touch);
        money_Button1->setEnabled(touch);
    }

}

int SGMainLayer::getMailSelectId()
{
    if (mailManager != NULL) {
        return mailManager->selectIndex;
    }
    return -1;
}

void SGMainLayer::setMailSelectId(int mailId)
{
    if (mailManager != NULL) {
        mailManager->selectIndex = mailId;
    }
}




//军魂换礼req。et:1=守荆州UI导入，2=战绩UI导入。
void SGMainLayer::sjzSoulExchangeRequest(int et)
{
    SGMainManager::shareMain()->sendEnterMallLayerRequest(SHOPID_JING_ZHOU , SHOU_JING_ZHOU_LAYER);
}

//军魂换礼rsp
void SGMainLayer::sjzSoulExchangeResponse(CCObject* obj)
{
    CCLOG("--sjz-->>>> sjzSoulExchangeResponse(CCObject* obj)");
    //接收
    SGSocketRequest *sr = (SGSocketRequest *) obj;
    if (sr)
    {
        main::JunhunInfoResponse* rsp = (main::JunhunInfoResponse*) sr->m_msg;
        //解析四个数字，并储存起来。
        SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
        data->setSoulAmount(rsp->junhun());
        data->setSoulCurOfS(rsp->index1());
        data->setSoulCurOfA(rsp->index2());
        data->setSoulCurOfB(rsp->index3());
        data->setSoulMaxOfS(rsp->index1max());
        data->setSoulMaxOfA(rsp->index2max());
        data->setSoulMaxOfB(rsp->index3max());
        //显示军魂页面
        SGMainManager::shareMain()->showSoulExchangeLayer(sjzSeEt);
    }
}

//每5秒更新一次显示资源计数器信息
void SGMainLayer::showRRInfo()
{
    ResourceManager::sharedInstance()->showResRefInfo();
}

//2013.12.16 守荆州当日奖励，随时可能推送，只有接收，没有发送。
void SGMainLayer::sjzDailyRewardResponse(CCObject* obj)
{
    SGSocketRequest* sr = (SGSocketRequest*) obj;
    if (sr)
    {
        main::SurvivalDailyRewardResponse* rsp = (main::SurvivalDailyRewardResponse*) sr->m_msg;
        if (rsp)
        {
            //设置排名和元宝
            SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
            data->setDailyRank(rsp->rank());
            data->setDailyIngot(rsp->rewardgold());
            //显示提示
            SGCantAdvanceBox *box = SGCantAdvanceBox::create(NULL, NULL, 56, 0, NULL);
            SGMainManager::shareMain()->showBox(box);
        }
    }
}

void SGMainLayer::lvlUpEnterListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::RewardLevelUpEntryResponse *response = (main::RewardLevelUpEntryResponse *)sr->m_msg;
        CCArray *array = CCArray::create();

        int len = response->records_size();
        for (int i = 0; i<len; i++)
        {
            array->addObject(CCString::createWithFormat("%d",response->records(i)));
        }
		
        SGMainManager::shareMain()->showLvlUpGetGold(array);
    }
}

void SGMainLayer::sendChatShowContent(CCString * itemName ,int showType,int showStarsLevel,int itemSsid)
{
    //修改小喇叭数量
    if (canChatShown)
    {
        
        if (SGPlayerInfo::sharePlayerInfo()->getchatTime() <30)
        {
            //CCLog("%d",SGPlayerInfo::sharePlayerInfo()->getchatTime());
            SGMainManager::shareMain()->showMessage(str_MainLayer_str31);
            return;
        }
              
        
        canChatShown = false;
        schedule(schedule_selector(SGMainLayer::resetChatShown),1.5);
        
//        int smallTrumpet  = SGPlayerInfo::sharePlayerInfo()->getSpeakerCount();
//        smallTrumpet --;
//        if (smallTrumpet > 0)
//        {
//            SGPlayerInfo::sharePlayerInfo()->setSpeakerCount(smallTrumpet);
        
            if (showType != 2 &&showStarsLevel>1)
                showStarsLevel-=1;
            
            CCString * chatContent = CCString::createWithFormat("<pengyou.com> - %s - %d - %d - %d",itemName->getCString(),showType,showStarsLevel,itemSsid);
            main::SendMessageRequest *chat = new main::SendMessageRequest();
            chat->set_chid(1);
            chat->set_content(chatContent->getCString());
            SGSocketClient::sharedSocketClient()->send(MSG_CHAT_MESSAGE, chat);
            SG_SHOW_WINDOW(str_MainLayer_str32);
        
        
            
            SGPlayerInfo::sharePlayerInfo()->setchatTime(0);
            this->schedule(schedule_selector(SGMainLayer::computeChateTime), 1);

    }
    
}

void SGMainLayer::resetChatShown()
{
    canChatShown = true;
    unschedule(schedule_selector(SGMainLayer::resetChatShown));
}

void SGMainLayer::hideFirstCharge()
{
    if (chongzhi) {
        chongzhi->removeFromParentAndCleanup(true);
        chongzhi = NULL;
    }
}

void SGMainLayer::entryFirstChargeShowListener(CCObject *obj)
{
//    SGSocketRequest *sr = (SGSocketRequest *)obj;
//    CCArray *array = NULL;
//    if (sr)
//    {
//        main::LuaActivityDetailResponse *response = (main::LuaActivityDetailResponse *)sr->m_msg;
//        SGActivityData * activityData = NULL;
//         SGActivityBriefData * activityBriefData = NULL;
//        int len = response->activitybrief_size();
//        int labelLen = 0;
//        int btnLen = 0;
//        int ItemLen = 0;
//        int stringLen = 0;
//        int imageLen = 0;
//        if (len > 0) {
//            array = CCArray::create();
//        }
//        else
//        {
//            return;
//        }
//        for(int i = 0; i< len; i++)
//        {
//            main::ActivityBrief activityBrief = response->activitybrief(i);
//            activityBriefData = SGActivityBriefData::create(activityBrief.luaid(), activityBrief.luastate(),activityBrief.luaname(), activityBrief.btnname(), activityBrief.notice());
//            array->addObject(activityBriefData);
//            
//        }
//        
//        main::ActivityDetailResponse activityDetail= response->activitydetail();//activitydetail(i);
//        main::ActivityBrief activityBrief = activityDetail.activitybrief();
//            activityData = SGActivityData::create(activityBrief.luaid(), activityBrief.luaname(), activityBrief.btnname(), activityBrief.notice());
//            
//            //label
//            labelLen = activityDetail.labelinfo_size();
//            for (int la=0; la<labelLen; la++) {
//                main::LabelInfo labelInfo = activityDetail.labelinfo(la);
//                ccColor4B color(ccc4(0,0,0,0));
//                if(labelInfo.has_fontcolor())
//                {
//                    main::ElementColor elementColor = activityDetail.labelinfo(la).fontcolor();
//                    color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
//                }
//                int labelValue = -1;
//                if(labelInfo.has_labelvalue())
//                {
//                    labelValue = labelInfo.labelvalue();
//                }
//                activityData->addLabelInfo(labelInfo.labelid(), labelValue,color);
//            }
//            //btn
//            btnLen = activityDetail.btninfo_size();
//            for (int bt=0; bt<btnLen; bt++) {
//                main::BtnInfo btntInfo = activityDetail.btninfo(bt);
//                std::string strFont = "";
//                if(btntInfo.has_fontstr())
//                {
//                    strFont = btntInfo.fontstr();
//                }
//                ccColor4B fontColor (ccc4(0,0,0,0));
//                if(btntInfo.has_fontcolor())
//                {
//                    main::ElementColor elementColor = btntInfo.fontcolor();
//                    fontColor =  ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
//                }
//                std::string strBg = "";
//                if(btntInfo.has_bgstr())
//                {
//                    strBg = btntInfo.bgstr();
//                }
//                std::string strFg = "";
//                if(btntInfo.has_fgstr())
//                {
//                    strFg = btntInfo.fgstr();
//                }
//                ccColor4B imageColor (ccc4(0,0,0,0));
//                if(btntInfo.has_imagecolor())
//                {
//                    main::ElementColor elementColor = btntInfo.imagecolor();
//                    imageColor =  ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
//                }
//                int state = 1;
//                if(btntInfo.has_btnstate())
//                {
//                    state = btntInfo.btnstate();
//                }
//                
//                activityData->addBtnInfo(btntInfo.btnid(), strFont,fontColor,strBg,strFg,imageColor,state);
//                
//                //activityData->addBtnInfo(btntInfo.btnid(), strFont,fontColor,strBg,strFg,imageColor,btntInfo.btnstate());
//            }
//            //item
//            ItemLen = activityDetail.iteminfo_size();
//            for (int it = 0; it<ItemLen; it++) {
//                activityData->addItemInfo(activityDetail.iteminfo(it).itemtype(),
//                                          activityDetail.iteminfo(it).itemid(),
//                                          activityDetail.iteminfo(it).itemcount(),
//                                          activityDetail.iteminfo(it).itemlevel(),
//                                          activityDetail.iteminfo(it).itembtnid());
//            }
//            stringLen = activityDetail.labelstringinfo_size();
//            for(int i = 0;i<stringLen;i++)
//            {
//                main::LabelStringInfo labelStringInfo = activityDetail.labelstringinfo(i);
//                ccColor4B color(ccc4(0,0,0,0));
//                if(labelStringInfo.has_fontcolor())
//                {
//                    main::ElementColor elementColor = labelStringInfo.fontcolor();
//                    color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
//                }
//                std::string labelString = "";
//                if(labelStringInfo.has_labelvalue())
//                {
//                    labelString = labelStringInfo.labelvalue();
//                }
//                activityData->addLabelStringInfo(labelStringInfo.labelid(), labelString,color);
//            }
//        imageLen = activityDetail.imageinfo_size();
//        for(int i = 0; i < imageLen; i++)
//        {
//            main::ImageInfo imageInfo = activityDetail.imageinfo(i);
//            std::string imageStr = "";
//            if(imageInfo.has_imagestr())
//            {
//                imageStr = imageInfo.imagestr();
//            }
//            ccColor4B color(ccc4(0,0,0,0));
//            if(imageInfo.has_imagecolor())
//            {
//                main::ElementColor elementColor = imageInfo.imagecolor();
//                color = ccc4(elementColor.red(),elementColor.green(),elementColor.blue(),elementColor.alpha());
//            }
//            activityData->addImageInfo(imageInfo.imageid(), imageStr, color);
//        }
//            //array->addObject(activityData);
//        
//		
//        SGMainManager::shareMain()->showFirstActivityLayer(array,activityData);
//    }
}


void SGMainLayer::entryTongQueShow()
{
    main::LuaActivityDetailRequest* req = new main::LuaActivityDetailRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_TONGQUE_REQUEST, req);
}
void SGMainLayer::showTongQueLisenter(CCObject *obj)
{
    SGSocketRequest *req = (SGSocketRequest *)obj;
    main::TongQueLotteryDropResponse *response = (main::TongQueLotteryDropResponse *)req->m_msg;
	
	if (response->ishide())
	{
		SG_SHOW_WINDOW(str_MainLayer_str33);
		return;
	}
	
    if (response)
    {
        int spendMedal = response->spendmedal();//当前翻牌所需要的勋章数目
        int spendGold = response->spendgold();//消耗元宝数
        int medal = response->medal();//当前勋章数
        int spendCoins = response->spendcoins();//刷新消耗铜钱数
        int refreshTime = response->refreshtime();//倒计时时间
        int blessValue = response->blessvalue();//祝福值
        
        //item
        int itemLen = response->itemlist_size();
        std::vector<SGItemInfo> tongqueItem;
        tongqueItem.clear();
        for (int it = 0; it<itemLen; it++) {
        
            SGItemInfo itemInfo(response->itemlist(it).itemtype(),
                                response->itemlist(it).itemid(),
                                response->itemlist(it).itemcount()
                                ,response->itemlist(it).itemlevel(),
                                response->itemlist(it).itembtnid());
            tongqueItem.push_back(itemInfo);
        }
        SGItemInfo yangxing(response->rewardinfo().itemtype(),
                            response->rewardinfo().itemid(),
                            response->rewardinfo().itemcount(),
                            response->rewardinfo().itemlevel(),
                            response->rewardinfo().itembtnid());
        
        SGMainManager::shareMain()->showTongQueLayer(spendMedal,spendGold,medal,spendCoins,refreshTime,blessValue, tongqueItem, yangxing);
        
    }
}

void SGMainLayer::entryFirstChargeShow()
{
    if (SGMainManager::shareMain()->getMainScene()->nowLayerTag == sg_strengLayer)
    {
        return ;
    }
    
    main::LuaActivityDetailRequest* req = new main::LuaActivityDetailRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_LUA_ACTIVITY, req);
}


void SGMainLayer::showFirstChargeGift(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::FirstPayRewardResponse *response = (main::FirstPayRewardResponse *)sr->m_msg;
    if (sr)
    {
        SGVipGift *vipGift = new SGVipGift();
        vipGift->setActivityId(0);
        vipGift->setBtnId(0);
        vipGift->setGiftCoin(0);
        vipGift->setGiftAward(0);
        vipGift->setGiftSpeak(0);
        vipGift->setGiftLiandan(0);
        vipGift->setHaveBuy(FIRSTCHARGESHOWTAG);
        int giftNum = response->firstpayrewarditemproto_size();
        int itemType = 0;
        int itemId = 0;
        int itemCount = 0;
        int itemLevel = 0;

        for (int i=0;i<giftNum; i++) {
            main::FirstPayRewardItemProto item = response->firstpayrewarditemproto(i);
            itemType = item.itemtype();
            itemId = item.itemid();
            itemCount = item.itemcount();
            itemLevel = item.itemlevel();
            
            switch (itemType) {
                case 0:
                {
                    SGGift *gift = new SGGift;
                    gift->setType(itemType);
                    gift->setIconid(10000);
                    gift->setNum(itemCount);//问题
                    gift->setStar(0);
                    gift->setName(CCString::create(str_yuanbao));
                    vipGift->addGiftList(gift);
                    gift->autorelease();
                }
                    break;
                case 1:
                {
                    SGGift *gift = new SGGift;
                    gift->setType(itemType);
                    gift->setIconid(9999);
                    gift->setNum(itemCount);
                    gift->setStar(0);
                    gift->setName(CCString::create(str_Tong_money));
                    vipGift->addGiftList(gift);
                    gift->autorelease();
                }
                    break;
                case 2:
                {

                }
                    break;
                case 3:
                {
                    SGGift *gift = new SGGift;
                    gift->setType(itemType);
                    gift->setIconid(9997);
                    gift->setNum(itemCount);
                    gift->setStar(0);
                    gift->setName(CCString::create(str_jungong));
                    vipGift->addGiftList(gift);
                    gift->autorelease();
                }
                    break;
                case 4:
                {
                    SGGift *gift = new SGGift;
                    gift->setType(itemType);
                    gift->setIconid(9998);
                    gift->setNum(itemCount);
                    gift->setStar(0);
                    gift->setName(CCString::create(str_Small_laba));
                    vipGift->addGiftList(gift);
                    gift->autorelease();
                }
                    break;
                case 5:
                {
                    SGGift *gift = new SGGift;
                    gift->setType(itemType);
                    gift->setIconid(9996);
                    gift->setNum(itemCount);
                    gift->setStar(0);
                    gift->setName(CCString::create(str_liandianmifang));
                    vipGift->addGiftList(gift);
                    gift->autorelease();
                }
                    break;
                case 6:
                {
                    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(itemId);
                    SGGift *gift = new SGGift;
                    gift->setType(itemType);
                    gift->setItemId(itemId);
                    gift->setIconid(temp->getIconId());
                    gift->setAdvnum(temp->getAdvNum());
                    gift->setNum(itemCount);
                    gift->setName(temp->getOfficerName());
                     gift->setStar( temp->getOfficerCurrStarLevel());
                    
                    vipGift->addGiftList(gift);
                    gift->autorelease();
                }
                    break;
                case 7:
                {
                    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(itemId);
                    SGGift *gift = new SGGift;
                    gift->setType(itemType);
                    gift->setItemId(itemId);
                    gift->setIconid(temp->getIconId());
                    gift->setAdvnum(temp->getAdvNum());
                    gift->setNum(itemCount);
                    gift->setName(temp->getEquipName());
                    gift->setStar( temp->getEquipCurrStarLevel());
                    
                    vipGift->addGiftList(gift);
                    gift->autorelease();
                }
                    break;
                case 9:
                {
                    SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(itemId);
                    SGGift *gift = new SGGift;
                    gift->setType(itemType);
                    gift->setItemId(itemId);
                    gift->setIconid(temp->getIconId());
                    gift->setNum(itemCount);
                    gift->setName(temp->getPropsName());
                    gift->setStar( temp->getPropsCurrStarLevel());
                    
                    vipGift->addGiftList(gift);
                    gift->autorelease();
                }
                    break;
                default:
                    break;
            }


        }
        
        CCString *name = CCString::create(str_MainLayer_str34);
        CCString *info= CCString::create(str_MainLayer_str35);
        SGActGiftInfoBox *GiftInfoBox = SGActGiftInfoBox::create(this, vipGift,1,name,info);
        SGMainManager::shareMain()->showBox(GiftInfoBox);
    }
    
}

void SGMainLayer::requestStoreLayer(CCObject *obj)
{
    if (SGPlayerInfo::sharePlayerInfo()->getHaveCharged()) {
        main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);
    }
    else
    {
        if (obj) {
            this->setIsCanTouch(true, false);
            ((CCNode*) obj)->removeFromParentAndCleanup(true);
        }
        entryFirstChargeShow();
    }
}
/////////////////
#pragma mark boss 战消息
void SGMainLayer::showBossKilledMsg(SGBoxDelegate *bdg)
{
    if (SGMainManager::shareMain()->getIsBossKilled())
    {
        SGMainManager::shareMain()->setIsBossKilled(false);
        SGCantAdvanceBox *box = SGCantAdvanceBox::create(bdg, NULL, 57, 0,0, NULL,0,SGMainManager::shareMain()->getBosskillMsg().c_str());
        SGMainManager::shareMain()->showBox(box);
    }
}
void SGMainLayer::showBossEndRemind(CCObject *sender){

    SGSocketRequest *req=(SGSocketRequest *)sender;
    if (req)
    {
        main::BossEndRewardResponse *bossEnd=(main::BossEndRewardResponse *)req->m_msg;
        if (bossEnd)
        {
            std::string showMessage=bossEnd->rewardstring();
            
            if (SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bossBattleLayer) == NULL)
            {
                SGCantAdvanceBox *box = SGCantAdvanceBox::create(NULL, NULL, 57, 0,0, NULL,0,showMessage.c_str());
                SGMainManager::shareMain()->showBox(box);
            }
            else
            {
                SGMainManager::shareMain()->setIsBossKilled(true);
                SGMainManager::shareMain()->setBosskillMsg(showMessage);
            }
//            SGCantAdvanceBox *box = SGCantAdvanceBox::create(NULL, NULL, 57, 0,0, NULL,0,showMessage.c_str());
//            SGMainManager::shareMain()->showBox(box);
        }
    }
}

//进入boss战排行
void SGMainLayer::entryBossRankList(CCObject *sender)
{
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::BossDamageRankResponse *rankRes = (main::BossDamageRankResponse *)req->m_msg;
		if (rankRes)
		{
			CCArray *data = CCArray::create();
			data->retain();
			SGKillBossInfo *kill = new SGKillBossInfo();
			kill->autorelease();
			kill->mins = rankRes->minute();
			kill->killDamage = (int)rankRes->damage();
			kill->playerName = rankRes->playername();
			kill->selfDamage = rankRes->selfdamage();
			
			int rankSize = rankRes->bossrankinfo_size();
			for (int i = 0; i < rankSize; i++)
			{
				SGBossRankListData *r = new SGBossRankListData();
				r->autorelease();
				main::BossRankInfo info = rankRes->bossrankinfo(i);
				r->roleId = info.roleid();
				r->roleName = info.rolename();
				r->icon = info.icon();
				r->damage = info.damage();
				r->country = info.country();
				r->rank = i;
				r->level = info.level();
				r->loc = info.rankindex();
				data->addObject(r);
			}
            //逻辑有问题  需要修改。
			if(data->count() == 0 )
            {
            }
            else
                SGMainManager::shareMain()->showBossBattleRankLayer(data, kill);
		}
		
	}
}


#pragma mark "==============BOSS战======================="
//发送参战消息监听
void SGMainLayer::enterBossFightListener(CCObject *sender)
{
    
    
	SGSocketRequest *req = (SGSocketRequest *)sender;
	if (req)
	{
		main::EnterBossResponse *bossRes = (main::EnterBossResponse *)req->m_msg;
		if (bossRes)
		{
			if (bossRes->state() == 1)
			{
				CCLOG("Success");
				//加入boss有信息
				SGBossBattleData *bossData = new SGBossBattleData();
				bossData->autorelease();
				bossData->bossCurrHp = bossRes->bosshp();
				bossData->bossMaxHp = bossRes->bossbasehp();
				bossData->bossHurtTimes = bossRes->bossdamagecount();
				bossData->residueSec = bossRes->time();
				bossData->currentHurt = bossRes->damage();// 玩家自己对boss的伤害
				
				bossData->value1 = bossRes->value1();
				bossData->value2 = bossRes->value2();
				bossData->value3 = bossRes->value3();
				
				//加入buff信息
				bossData->buffData = CCArray::create();
				bossData->buffData->retain();
				int buffSize = bossRes->bossbuffmodel_size();
				for (int i = 0; i < buffSize; i++)
				{
					main::BossBuffModel buffModel = bossRes->bossbuffmodel(i);
					SGBossBuffData *buff = new SGBossBuffData();
					buff->autorelease();
					buff->buffId = buffModel.buffid();
					buff->buffName = buffModel.buffname();
					buff->buffNeedMoney = buffModel.buffgold();
					buff->buffValue = buffModel.buffbasvalue();
					bossData->buffData->addObject(buff);
				}
				//加入排名信息
				bossData->rankData = CCArray::create();
				bossData->rankData->retain();
				int rankSize = bossRes->bossrankmodel_size();
				for (int i = 0; i < rankSize; i++)
				{
					main::BossRankModel rankModel = bossRes->bossrankmodel(i);
					SGBossRankData *rank = new SGBossRankData();
					rank->autorelease();
					rank->roleId = rankModel.roleid();
					rank->roleName = rankModel.rolename();
					rank->roleLvl = rankModel.level();
					rank->roleHurt = rankModel.damage();
					bossData->rankData->addObject(rank);
				}
				
				SGMainManager::shareMain()->showBossBattleDetailLayer(bossData, 0, true);
			}
			else
			{
				//CCLOG("%s", bossRes->errmsg().c_str());//cgpLogDel
			
				//
				main::MainBossRequest *req = new main::MainBossRequest();
				SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_JOIN, req);
			}
		}
	}
  
	
}

void SGMainLayer::computeChateTime(){
    if(SGPlayerInfo::sharePlayerInfo()->getchatTime()>30)
    {
        this->unschedule(schedule_selector(SGMainLayer::computeChateTime));
        return;
    }
    SGPlayerInfo::sharePlayerInfo()->setchatTime(SGPlayerInfo::sharePlayerInfo()->getchatTime()+1);
}

//敲醒客户端
void SGMainLayer::alanWakeListener(CCObject* obj)
{
    SGSocketRequest *req = (SGSocketRequest *)obj;
	if (req)
	{
		main::AlanWake *aw = (main::AlanWake *)req->m_msg;
        if (aw)
        {
            CCLOG("AlanWake! %d, %s", aw->arg1(), aw->arg2().c_str());
            SGMainManager::shareMain()->showMessage(aw->arg2().c_str());
        }
    }
}

//武将碎裂动画
void SGMainLayer::officerHolyCrashListener(CCObject* obj)
{
    SGSocketRequest *req = (SGSocketRequest *)obj;
	if (req)
	{
		main::OfficerCardItemProto *ocip = (main::OfficerCardItemProto *)req->m_msg;
        if (ocip)
        {
            if (ocip->iscrash())
            {
                SGMainManager::shareMain()->getAfterAdvancedItemId();
                SGAdvancedEffectLayer* ael =SGAdvancedEffectLayer::create(BIT_OFFICER, ocip->itemid(), ocip->itemid(), 3, ocip->crashfragmentnum());
                ael->setZOrder(200);
                //没有返回页面，所以选择添加临时层。
                SGMainManager::shareMain()->addShowLayer(ael, false);
            }
        }
    }
}

//排行按钮回调
void SGMainLayer::onRankBtnClicked()
{
    main::RankRequest* req = new main::RankRequest();
    req->set_type(0);
    req->set_nationid(0);
    req->set_page(1);
    SGSocketClient::sharedSocketClient()->send(MSG_RANK, req);
}
//群雄争霸监听
void SGMainLayer::enterAreanListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::ArenaMainInfoResponse *rsp = (main::ArenaMainInfoResponse *) sr->m_msg;
        
        
        
        CCArray * array = CCArray::create();
        for(int i = 0; i < rsp->matchlist_size(); i++)
        {
            main::ArenaRoleInfo itemInfo = rsp->matchlist(i);
            
            SGAreanItemInfoData * item = SGAreanItemInfoData::create(itemInfo.roleid(),
                                                                     CCString::create(itemInfo.nickname().c_str()),
                                                                     itemInfo.nationid(),
                                                                     itemInfo.officeitemid(),
                                                                     itemInfo.level(),
                                                                     itemInfo.rank(),
                                                                     itemInfo.combatvalue(),
                                                                     itemInfo.zhengfudian());
            array->addObject(item);
        }
        main::ArenaRoleInfo myInfo = rsp->selfinfo();
        SGAreanItemInfoData * itemMyInfo = SGAreanItemInfoData::create(myInfo.roleid(),
                                                                       CCString::create(myInfo.nickname().c_str()),
                                                                       myInfo.nationid(),
                                                                       myInfo.officeitemid(),
                                                                       myInfo.level(),
                                                                       myInfo.rank(),
                                                                       myInfo.combatvalue(),
                                                                       myInfo.zhengfudian());
        main::ArenaBaseInfo  baseInfo =  rsp->baseinfo();
        SGAreanBaseInfoData * base = SGAreanBaseInfoData::creat(baseInfo.zhengfudian(),
                                                                baseInfo.count(),
                                                                baseInfo.maxcount(),
                                                                CCString::create(baseInfo.info().c_str()),
                                                                baseInfo.surplustime(),
                                                                baseInfo.cdtime(),
                                                                baseInfo.canrewardcount());
        //base->retain();
        SGMainManager::shareMain()->showAreanLayer(array, base, itemMyInfo, Arean_qun);
        
    }
    
    
    
    
    
    
}
//征战之路监听
void SGMainLayer::enterFightRoad(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::ArenaFightReportResponse *rsp = (main::ArenaFightReportResponse *) sr->m_msg;
        
        
        
        CCArray * array = CCArray::create();
        for(int i = 0; i < rsp->reportlist_size(); i++)
        {
            main::ArenaFightReport itemInfo = rsp->reportlist(i);
            
            SGAreanFightLogData * item = SGAreanFightLogData::create(itemInfo.roleid(),
                                                                     CCString::create(itemInfo.nickname().c_str()),
                                                                     itemInfo.nationid(),
                                                                     itemInfo.officeitemid(),
                                                                     itemInfo.level(),
                                                                     itemInfo.beforerank(),
                                                                     itemInfo.afterrank(),
                                                                     itemInfo.iswin(),
                                                                     itemInfo.isattact(),
                                                                     itemInfo.time());
            array->addObject(item);
        }
        
        main::ArenaBaseInfo  baseInfo =  rsp->baseinfo();
        SGAreanBaseInfoData * base = SGAreanBaseInfoData::creat(baseInfo.zhengfudian(),
                                                                baseInfo.count(),
                                                                baseInfo.maxcount(),
                                                                CCString::create(baseInfo.info().c_str()),
                                                                baseInfo.surplustime(),
                                                                baseInfo.cdtime(),
                                                                baseInfo.canrewardcount());
        //base->retain();
        SGMainManager::shareMain()->showAreanLayer(array, base, NULL, Arean_log);
        
    }
    

}
//诸侯割据监听
void SGMainLayer::enterZhuHouGeJu(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::ArenaRanksResponse *rsp = (main::ArenaRanksResponse *) sr->m_msg;
        
        
        
        CCArray * array = CCArray::create();
        for(int i = 0; i < rsp->ranks_size(); i++)
        {
            main::ArenaRoleInfo itemInfo = rsp->ranks(i);
            
            SGAreanItemInfoData * item = SGAreanItemInfoData::create(itemInfo.roleid(),
                                                                     CCString::create(itemInfo.nickname().c_str()),
                                                                     itemInfo.nationid(),
                                                                     itemInfo.officeitemid(),
                                                                     itemInfo.level(),
                                                                     itemInfo.rank(),
                                                                     itemInfo.combatvalue(),
                                                                     itemInfo.zhengfudian());
            array->addObject(item);
        }
        main::ArenaRoleInfo myInfo = rsp->selfinfo();
        SGAreanItemInfoData * itemMyInfo = SGAreanItemInfoData::create(myInfo.roleid(),
                                                                       CCString::create(myInfo.nickname().c_str()),
                                                                       myInfo.nationid(),
                                                                       myInfo.officeitemid(),
                                                                       myInfo.level(),
                                                                       myInfo.rank(),
                                                                       myInfo.combatvalue(),
                                                                       myInfo.zhengfudian());
        main::ArenaBaseInfo  baseInfo =  rsp->baseinfo();
        SGAreanBaseInfoData * base = SGAreanBaseInfoData::creat(baseInfo.zhengfudian(),
                                                                baseInfo.count(),
                                                                baseInfo.maxcount(),
                                                                CCString::create(baseInfo.info().c_str()),
                                                                baseInfo.surplustime(),
                                                                baseInfo.cdtime(),
                                                                baseInfo.canrewardcount());
        //base->retain();
        SGMainManager::shareMain()->showAreanLayer(array, base, itemMyInfo, Arean_zhu);
        
    }

}
//排行消息回调
void SGMainLayer::rankListener(cocos2d::CCObject *sender)
{
    
    
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    if (sr)
    {
        main::RankResponse *rsp = (main::RankResponse *) sr->m_msg;
        if (rsp)
        {
            int size = rsp->rankinfos_size();
            //自己的信息
            main::RankProto myrankinfo=rsp->rankinfo();
            int myrank=myrankinfo.value();
            int rankstate=myrankinfo.ranking();
            CCArray*array=CCArray::create();
            for (int i = 0; i < rsp->rankinfos_size(); i++)
            {
                
                main::RankProto rankinfo= rsp->rankinfos(i);
                SGRankAllPlayer*player=new SGRankAllPlayer();
                player->setplayerId(rankinfo.roleid());
                std::string str(rankinfo.nickname());
                CCString* ns=CCString::createWithFormat("%s",str.c_str());
                player->setName(ns);
                player->setHeadId(rankinfo.nation());
                ////////
                player->setRank(rankinfo.value());
                player->setServerNum(rankinfo.areaid());
                std::string str2(rankinfo.areaname());
                CCString* ns2=CCString::createWithFormat("%s",str2.c_str());
                player->setServerName(ns2);
                
                player->setIsIn(rankinfo.ranking()); ///是否进榜
                array->addObject(player);
            }
            SGMainManager::shareMain()->showRankLayerLayer(array,myrank,rankstate,size);
        }
    }
     
}

void SGMainLayer::onEmbattleBtnClicked()
{
    SGMainManager::shareMain()->showEmbattleLayer(1,SGTeamgroup::shareTeamgroupInfo()->getiscurrent());
}

void SGMainLayer::setEmbattleNoticeVisible(bool isVisible)
{
    m_embattleNotice->setVisible(isVisible);
}

//设置奇遇红点的显隐
void SGMainLayer::setAdventureNoticeVisible(bool vis)
{
    m_adventureNotice->setVisible(vis);
}

void SGMainLayer::onVipGiftResponse(CCObject * obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopVipGiftBagEntryResponse *response = (main::ShopVipGiftBagEntryResponse *)sr->m_msg;
    int vipLvl = 0;
    CCArray * _vipGifts = CCArray::create();
    if (sr)
    {
        vipLvl = response->viplevel();
        int curr = response->currentrmb();
        int max = response->maxrmb();
        int len = response->list_size();
        for (int i = 0 ; i <len; i++)
        {
            SGVipGift *vipGift = new SGVipGift();
            main::VipGiftBag gifts = response->list(i);
            vipGift->setGiftCost(gifts.cost());
            vipGift->setGiftID(gifts.ssid());
            vipGift->setGiftName(CCString::create(gifts.name()));
            vipGift->setGiftInfo(CCString::create(gifts.info()));
            vipGift->setGiftDiscount(gifts.discount());
            vipGift->setGiftVipLvl(gifts.viplevel());
            vipGift->setHaveBuy(gifts.isbuy());
            int lenth = gifts.list_size();
            for (int j = 0; j <lenth; j ++)
            {
                SGGift *gift = new SGGift;
                main::GiftItem GiftItem = gifts.list(j);
                gift->setItemId(GiftItem.itemid());
                gift->setNum(GiftItem.num());
                gift->setType(GiftItem.itemtype());
                int enumType = (SGBaseItemType)(GiftItem.itemtype() );
                switch (enumType)
                {
                    case BIT_INGOT:
                    {
                        gift->setName(CCString::create(str_yuanbao));
                        gift->setItemId(10000);
                    }
                        break;
                    case BIT_GOLD:
                    {
                        gift->setName(CCString::create(str_Tong_money) );
                        gift->setItemId(9999);
                    }
                        break;
                    case BIT_GRADE:
                    {
                        gift->setName(CCString::create(str_Currency_TianTiJiFen));
                        gift->setItemId(9995);
                    }
                        break;
                    case BIT_RECIPE:
                    {
                        gift->setName(CCString::create(str_Small_laba));
                        gift->setItemId(9996);
                    }
                        break;
                    case BIT_OFFICER:
                    {
                        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(GiftItem.itemid());
                        gift->setName(temp->getOfficerName());
                        gift->setItemId(temp->getOfficerId());
                    }
                        
                        break;
                    case BIT_EQUIP:
                    {
                        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(GiftItem.itemid());
                        gift->setName(temp->getEquipName());
                        gift->setItemId(temp->getEquipmentId());
                    }
                        
                        break;
                    case BIT_PROP:
                    {
                        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(GiftItem.itemid());
                        gift->setName(temp->getPropsName());
                        gift->setItemId(temp->getPropsId());
                        
                    }
                        break;
                    case BIT_PIECE:
                    {
                        SGPiecesDataModel * temp = SGStaticDataManager::shareStatic()->getPiecesDictById(GiftItem.itemid() );
                        gift->setName(temp->getPiecesName());
                        gift->setItemId(temp->getPiecesId());
                    }
                        break;
                    case BIT_SOUL:
                    {
                        gift->setName(CCString::create(str_Currency_JunHun));
                        gift->setItemId(9994);
                    }
                        break;
                    case BIT_VIGOUR:
                    {
                        gift->setName(CCString::create(str_Format_tili__));
                        gift->setItemId(9993);
                    }
                        break;
                    case BIT_GRAIN:
                    {
                        gift->setName(CCString::create(str_Format_junliang__));
                        gift->setItemId(9992);
                    }
                        break;
                    case BIT_CONSUMABLE:
                    {
                        SGConsumableDataModel * temp = SGStaticDataManager::shareStatic()->getConsumableById(GiftItem.itemid());
                        gift->setName(temp->getConsumeName());
                        gift->setItemId(temp->getConsumId());
                    }
                        break;
                    case BIT_MEDAL:
                    {
                        gift->setName(CCString::create(str_Format_xunzhang__));
                        gift->setItemId(9991);
                    }
                        break;
                    case BIT_MATERIAL:
                    {
                        SGMaterialDataModel * temp = SGStaticDataManager::shareStatic()->getMaterialById(GiftItem.itemid());
                        gift->setName(temp->getMaterialName());
                        gift->setItemId(temp->getMaterialId());
                    }
                        break;
                    default:
                        CCAssert(1==0, "error type.");
                        break;
                        
                }
                
                vipGift->addGiftList(gift);
                gift->autorelease();
            }
            _vipGifts->addObject(vipGift);
            vipGift->autorelease();
        }
        _vipGifts->retain();
        SGMainManager::shareMain()->showGiftsLayer(vipLvl,_vipGifts,curr,max);
        _vipGifts->release();
    }

}

void SGMainLayer::onPvpDayReward( CCObject * sender)
{
    //0:金子 1:铜钱 2:天梯积分  5:炼丹秘方 6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌  10:碎片 11军魂 12体力 13军粮 14: 15消耗品 16 勋章 17 材料
    SGSocketRequest *sr = (SGSocketRequest *)sender;
    main::PvPDayAwardGiftEntryResponse *response = (main::PvPDayAwardGiftEntryResponse *)sr->m_msg;
    if (sr)
    {
        CCArray *arrary = CCArray::create();
        
        int len = response->list_size();
        int count = response->fightcounter();
        for (int i = 0 ; i <len; i++) {
            SGVipGift *vipGift = new SGVipGift();
            main::PvPDayAwardGiftBag gifts = response->list(i);
            vipGift->setGiftName(CCString::create(gifts.name()));
            vipGift->setGiftInfo(CCString::create(gifts.info()));
            vipGift->setGiftDiscount(gifts.giftcostdiscount());
            vipGift->setPvpgifttype(gifts.count());
            vipGift->setGiftCoin(gifts.giftcoin());
            vipGift->setGiftAward(gifts.giftaward());
            vipGift->setGiftSpeak(gifts.speaker());
            vipGift->setGiftLiandan(gifts.liandan());
            vipGift->setHaveBuy(gifts.isbuy());
            
            int lenth = gifts.list_size();
            for (int j = 0; j <lenth; j ++) {
                SGGift *gift = new SGGift;
                main::PvPDayAwardGiftItem GiftItem = gifts.list(j);
                gift->setItemId(GiftItem.itemid());
                gift->setNum(GiftItem.num());
                
                
                int type = GiftItem.itemtype();
                SGBaseItemType enumType = (SGBaseItemType)type;
                gift->setType(type);
                switch (enumType)
                {
                    case BIT_INGOT:
                        gift->setItemId(10000);
                        gift->setName(CCString::create(str_Currency_YanBao));
                        break;
                    case BIT_GOLD: //coin
                        gift->setItemId(9999);
                        gift->setName(CCString::create(str_Currency_TongQian));
                        break;
                    case BIT_GRADE:// 天梯积分  图片名称 store_tianti.png 在public3中
                        gift->setItemId(9995);
                        gift->setName(CCString::create(str_Currency_TianTiJiFen));
                        break;
                    case BIT_RECIPE: //炼丹
                        gift->setItemId(9996);
                        gift->setName(CCString::create(str_liandianmifang));
                        break;
                        
                    case BIT_SOUL://军魂
                        gift->setItemId(9994);
                        gift->setName(CCString::create(str_FriendMailLayer_str8));
                        break;
                    case BIT_VIGOUR: //体力
                        gift->setItemId(9993);
                        gift->setName(CCString::create(str_CantAdvance_tips64));
                        break;
                    case BIT_GRAIN: //军粮
                        gift->setItemId(9992);
                        gift->setName(CCString::create(str_CantAdvance_tips65));
                        break;
                    case BIT_MEDAL://勋章
                        gift->setItemId(9991);
                        gift->setName(CCString::create(str_Currency_XunZhang));
                        break;
                    case BIT_OFFICER:
                    {
                        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(GiftItem.itemid());
                        gift->setItemId(temp->getOfficerId());
                        
                        //服务器不再发送， 从表中读取。
                        gift->setName(temp->getOfficerName());
                        gift->setStar(temp->getOfficerCurrStarLevel());
                        gift->setAdvnum(temp->getAdvNum());
                    }
                        
                        break;
                    case BIT_EQUIP:
                    {
                        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(GiftItem.itemid());
                        gift->setItemId(temp->getEquipmentId());
                        
                        //服务器不再发送， 从表中读取。
                        gift->setName(temp->getEquipName());
                        gift->setStar(temp->getEquipCurrStarLevel());
                        gift->setAdvnum(temp->getAdvNum());
                    }
                        
                        break;
                    case BIT_PROP:
                    {
                        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(GiftItem.itemid());
                        gift->setItemId(temp->getPropsId());
                        
                        //服务器不再发送， 从表中读取。
                        gift->setName(temp->getPropsName());
                        gift->setStar(temp->getPropsCurrStarLevel());
                    }
                        
                        break;
                    case BIT_CONSUMABLE:
                    {
                        SGConsumableDataModel *temp = SGStaticDataManager::shareStatic()->getConsumableById(GiftItem.itemid());
                        gift->setItemId(temp->getConsumId());
                        
                        //服务器不再发送， 从表中读取。
                        gift->setName(temp->getConsumeName());
                        gift->setStar(temp->getConsumeStarLvl());
                    }
                        break;
                    case BIT_PIECE:
                    {
                        SGPiecesDataModel * temp = SGStaticDataManager::shareStatic()->getPiecesDictById(GiftItem.itemid());
                        gift->setItemId(temp->getPiecesId());
                        
                        //
                        gift->setStar(temp->getStarLevel());
                        gift->setName(temp->getPiecesName());
                    }
                        break;
                    case BIT_MATERIAL:
                    {
                        SGMaterialDataModel * temp = SGStaticDataManager::shareStatic()->getMaterialById(GiftItem.itemid());
                        gift->setItemId(temp->getMaterialId());
                        gift->setName(temp->getMaterialName());
                    }
                        break;
                        
                    default:
                        CCAssert(false,"the value of type is wrong.");
                        break;
                }
                
                vipGift->addGiftList(gift);
                gift->autorelease();
            }
            
            arrary->addObject(vipGift);
            vipGift->autorelease();
        }
        arrary->retain();
        SGMainManager::shareMain()->showpvpEverydayLayer(count,arrary);
        arrary->release();
    }
}

