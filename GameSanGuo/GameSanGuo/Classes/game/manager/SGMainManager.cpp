 
//
//  SGMainManager.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-20.
//
//
//#import "SdkReqCallBack.h"



#include "SGChooseSkillCardLayer.h"
#include "SGMainManager.h"
#include "SGBattleManager.h"
#include "SGMainLayer.h"
#include "SGChangePassWordLayer.h"
#include "SGAdvancingLayer.h"
#include "SGSocketClient.h"
#include "GameConfig.h"
#include "SGBarracksLayer.h"
#include "SGEmbattleLayer.h"
#include "SGFirstLayer.h"
#include "SGBaseShow.h"
#include "SGGeneralsLayer.h"
#include "SGGeneralInfoLayer.h"
#include "SGSoldiersLayer.h"
#include "SGSoldierInfoLayer.h"
#include "SGEquipsLayer.h"
#include "SGEquipInfoLayer.h"
#include "SGPropsLayer.h"
#include "SGPropInfoLayer.h"
#include "SGExpandLayer.h"
#include "SGShowAllLayer.h"
#include "SGStrengLayer.h"
#include "SGAdvanceLayer.h"
#include "SGSoulCardInfoLayer.h"
#include "SGStrengLayer.h"
#include "SGAlertBox.h"
#include "SGShopLayer.h"
#include "SGHelpLayer.h"
#include "SGAllCardsLayer.h"
#include "SGSetLayer.h"
#include "SGHelpBook.h"
#include "SGInviteNum.h"
#include "SGStoreLayer.h"
#include "SGGoldChangCoin.h"
#include "SGVipGiftLayer.h"
#include "SGPlayerInfoLayer.h"
#include "SGHelpLayer.h"
#include "SGChat.h"
#include "SGLandingReward.h"
#include "SGChatMessageCenter.h"
#include "SGVisitShowCardLayer.h"
#include "SGStoryMainLayer.h"
#include "SGActivityMainLayer.h"
#include "SGStorySectionLayer.h"
#include "SGFindHelpLayer.h"
//#include "SGVipInfoLayer.h"
#include "SGCardsLayer.h"
#include "SGPvpMainLayer.h"
#include "SGCountrysInfo.h"
#include "SGRewardLayer.h"
#include "SGBattleRankLayer.h"
#include "SGLevelUpGetGold.h"
#include "SGGetSalaryLayer.h"
#include "SGJitianLayer.h"
#include "SGLiandanLayer.h"
#include "SGDrinkWineLayer.h"
#include "SGTaskLayer.h"
#include "SGRewardRegLayer.h"
#include "SGBattleScoreLayer.h"
#include "SGTestSwitch.h"
#include "SGSkillCardInfoLayer.h"
#include "SGSkillCardsLayer.h"
#include "SGFightGuideLayer.h"
#include "SGAIManager.h"
#include "SGGuideManager.h"
#include "SGStaticDataManager.h"
#include "SGAdvanceFinishLayer.h"
#include "PlotModule.pb.h"
#include "SGServerListLoginGame.h"
#include "SGRegisterLayer.h"
#include "SGCantAdvanceBox.h"
#include "SGFindHelpLayer.h"
#include "SGInviterLayer.h"
#include "SGInviteeLayer.h"
#include "SGTeamgroup.h"
#include "SGCreamBattleLayer.h"
#include "SGRewardBindLayer.h"
#include "SGBaseScrollLayer.h"
#include "SGShouJingZhou.h"
#include "SGShouJingZhouRank.h"
#include "ResourceManager.h"
#include "SGAdvancechooseLayer.h"
#include "SGSoulExchangeLayer.h"
#include "SGPiecesLayer.h"
#include "SGRegisterBox.h"
#include "SGServerListNewServerBox.h"
#include "SGModificationPassword.h"
#include "SGConnectTipBox.h"
#include "SGEquipStrengLayer.h"
#include "SGFightRewardLayer.h"

#include "SGComplainLayer.h"
#include "SGLiuyanLayer.h"

#include "SGActivityLayer.h"
#include "CCLuaEngine.h"
#include "ShopModule.pb.h"
#include "SGCardBatchSellLayer.h"
#include "SGTongQueLayer.h"

#include "ServerMoving.pb.h"
#include "SGRewardYueKaLayer.h"
#include "SGJunxianLayer.h"
#include "SGpvpEverydayLayer.h"
#include "SGSeasonListLayer.h"
#include "SGConsumable.h"
#include "SGpvpLastListLayer.h"
#include  "SGJingZhouProcessLayer.h"
#include  "SGRankLayer.h"
#include "SGConsumableInfoLayer.h"
#include "SGBossBattleDetailLayer.h"
#include "SGBossBattleRankListLayer.h"
#include "YuanBaoShopModule.pb.h"
#include "SGBossBattleJoinInLayer.h"
#include "SGBossBattleLayer.h"
#include "SGPiecesInfoLayer.h"
#include "SGBattleData.h"
#include  "SGBattleMainLayer.h"
#include "SGAllTaskLayer.h"
#include "SGStringConfig.h"
#include "RoleModule.pb.h"
#include "SGPvpFightRewardLayer.h"
#include "Invite.pb.h"
#include "SGShareBox.h"
#include "SGGeneralsBagLayer.h"
#include "PlatformAdp.h"
#include "SGEquipAdvancedLayer.h"
#include "SGMaterialLayer.h"
#include "GlobalConfig.h"
#include "SGVsLayer.h"
#include "SGSpecialBattleLayer.h"
#include "SGSpecialBattleSectionLayer.h"
#include "ArenaModule.pb.h"

#include "XiyuModule.pb.h"
#include "SGAdventureLayer.h"


#include "SGUpdateManager.h"
#include "CGPStrengAdvLayer.h"




#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "SdkManager.h"
#include "SdkHandler.h"
#include "AndroidSDKAdp.h"



//#include "TDCCTalkingDataGA.h"
//#include "TDCCVirtualCurrency.h"
//#include "TDCCItem.h"
//#include "TDCCMission.h"
//#include "TDCCAccount.h"
#endif

#define CONNECT_TIP_TAG 9898

#define THIRDPARTYFBPRX "XD_THIRDPARTY_"
#define THIRDPARTYFBID  "FB_"

//是否开启红绿双点调试统计的小按钮。
#define SG_DEBUG_SPOT 0


//是否直接进入TestScene.cpp场景
#if CGPTest
#include "SGTestScene.h"
#endif

static SGMainManager *s_shareMain = NULL;

//是否从来源页面进入闯关 每次使用完成 置false
bool SGMainManager::sourceToStory = false;
int SGMainManager::static_plotId = -1;

SGMainManager* SGMainManager::shareMain(void)
{
    if (!s_shareMain) {
        s_shareMain = new SGMainManager();
    }
    return s_shareMain;
}



SGMainManager::SGMainManager()
:serverIp(NULL)
,m_chatShowPreLayer(0)
,haveChatShowLayer(false)
,m_shareId(-1)
,m_facebookSessionState(false)
,m_isFBLogin(0)
,isExist(false)
,serverid(0)
,_fromShopLayer(false)
,isFbAutologin(false)
,isBossKilled(false)
,_chap2Only1(0)
,_shallSjzGoToSoulMall(false)
,_spBattleChapId(-1)
,_spBattlePlotId(-1)
{
    mainScene = NULL;
    
    if ((GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY ||
         GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD ||
         GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE))
    {
        m_plistName = "sgvisitlayer/sgvisitlayer0.plist";
    }
    else
    {
        m_plistName = "sgvisitlayer/sgvisitlayer1.plist";
    }
    
#if (PLATFORM == IOS)
    OcCaller *oc = OcCaller::getInstance();
	oc->SDKReqAlloc();
    
    //初始化gocpa sdk
    oc->addGoCPASDK();
    
    m_buyString = " ";
    m_buyData = " ";
    m_billId = 0;
#else
	//android code
    /*
     TDCCAccount *c = TDCCAccount::setAccount("1000");
     c->setAge(22);
     
     TDCCVirtualCurrency::onChargeSuccess("11fdgdfg11");
     
     
     EventParamMap pm;
     pm.insert(EventParamPair("level", "34-45"));
     TDCCTalkingDataGA::onEvent("dead", &pm);
     
     AndroidSDKAdp::getInstance()->setAccountSDK("test", "test");
     this->trackItemUse(CCString::create("test"), 2);
     this->trackLevel(23);
     this->trackMsnBegin(34);
     this->trackMsnCompleted(34, true);
     this->trackPurchase(CCString::create("test"), 12, 1200.0f);
     this->trackReward(100, CCString::create("test"));
     this->trackTaskBegin(CCString::create("test"));
     this->trackTaskCompleted(CCString::create("test"));
     */
#endif
	/*
     _mSdkReqCallBack = [[[SdkReqCallBack alloc] init] autorelease];
     [_mSdkReqCallBack retain];
     */
    
    initNotification();
}

void SGMainManager::initNotification()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_PVPENTER_SEASONWAR, this, callfuncO_selector(SGMainManager::showSeasonMainLayer ) );
    notification->addObserver(MSG_CARGO_LIST, this, callfuncO_selector(SGMainManager::showMallLayer)); //新商城 商品列表监听
    //守荆州消息收口。
    notification->addObserver(MSG_SURVIVAL_MAIN_INFO, this, callfuncO_selector(SGMainManager::sjzInfoResponse));
    
    notification->addObserver(MSG_COMMON_SHOW_MESSAGE, this, callfuncO_selector(SGMainManager::showCommonMessage));
    
    notification->addObserver("updateNewViewForFB", this, callfuncO_selector(SGMainManager::doUpdateNewViewForFB));
    
    notification->addObserver(MSG_EXO_MECH_MAIN_INFO, this, callfuncO_selector(SGMainManager::exoMechMainInfoListener));
   
    //android 下的登陆消息注册
#if (PLATFORM == ANDROID)
	//登陆回调
	notification->addObserver(LOGINFLAG, this, callfuncO_selector(SGMainManager::loginReceive));
	//注册回调
	notification->addObserver(REREGISTFLAG, this, callfuncO_selector(SGMainManager::registerReceive));
	//密码修改
	notification->addObserver(PASSWORDLAG, this, callfuncO_selector(SGMainManager::modifyPassword));
	//FASTLOGINFLAG 快速登陆回调
	notification->addObserver(FASTLOGINFLAG, this, callfuncO_selector(SGMainManager::fastLogin));
    
    //第三方登陆回调
	notification->addObserver(THIRDPARTYLOGIN, this, callfuncO_selector(SGMainManager::thirdLoginReceive));
    notification->addObserver(INVALID_INFO_TIP, this, callfuncO_selector(SGMainManager::androidHttpShowTip));
    notification->addObserver(BIND_FAILED, this, callfuncO_selector(SGMainManager::androidBindFacebookFailed));
    
#endif
}

//android 下的登陆
#if (PLATFORM == ANDROID)

//facebook绑定失败情况
void SGMainManager::androidBindFacebookFailed(CCObject *sender)
{
    SG_SHOW_WINDOW("綁定失敗，帳號已綁定過其他遊客");
    SGMainManager::shareMain()->dealFbBindFail();
}



void SGMainManager::androidHttpShowTip(CCObject *sender)
{
    CCInteger *flag = (CCInteger *)sender;
    
    if (flag)
    {
        int type = flag->getValue();
        CCLOG("----state -- sgmainmanager -> type == %d", type);
        
        switch (type)
        {
            case 10:
            {
                this->showMessage(str_UserNmae_tips);
            }
                break;
            case 11:
            {
                this->showMessage(str_Password_tips);
            }
                break;
            case 12:
            {
                this->showMessage(str_User_name_already_exists);
            }
                break;
            case 20:
            {
                this->showMessage(str_Password_erro);
            }
                break;
                
            default:
            {
                CCLOG("errorType [%d]", type);
            }
                break;
        }
    }
}


//android 登陆回调
void SGMainManager::loginReceive(CCObject *sender)
{
	CCLOG("Sender: [%p] in fun [%s]", sender, __FUNCTION__);
	if (sender)
	{
		SdkInfoData * data = (SdkInfoData *)sender;
		
		CCLOG("username[%s] password[%s], isEx[%d]", data->userName.c_str(), data->password.c_str(), data->isEx);
		
		CCUserDefault::sharedUserDefault()->setStringForKey("username",data->userName);
		CCUserDefault::sharedUserDefault()->setStringForKey("password",data->password);
		CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", data->isEx);
		CCUserDefault::sharedUserDefault()->flush();
		
		int userId = data->accountId;
		char account[32] = "\0";
		sprintf(account, "%d", userId);
        std::string accountTmp(account);
        
		std::string loginKey = data->loginKey;
		CCLOG("@@@@loginKey:[%s]####", loginKey.c_str());
		this->doLogin(accountTmp,loginKey.c_str());
		CCLOG("+++++Write User Info Success! in fun [%s]++++", __FUNCTION__);
	}
}


//android 第三方登陆回调
void SGMainManager::thirdLoginReceive(CCObject *sender)
{
	CCLOG("Sender: [%p] in fun [%s]", sender, __FUNCTION__);
	if (sender)
	{
		SdkInfoData * data = (SdkInfoData *)sender;
		
		CCLOG("username[%s] password[%s], isEx[%d]", data->userName.c_str(), data->password.c_str(), data->isEx);
		
		CCUserDefault::sharedUserDefault()->setStringForKey("username",data->userName);
		CCUserDefault::sharedUserDefault()->setStringForKey("password",data->password);
		CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", data->isEx);
		CCUserDefault::sharedUserDefault()->flush();
		
		int userId = data->accountId;
		char account[32] = "\0";
		sprintf(account, "%d", userId);
        std::string accountTmp(account);
        
		std::string loginKey = data->loginKey;
		CCLOG("@@@@loginKey:[%s]####", loginKey.c_str());
		this->doLogin(accountTmp,loginKey.c_str());
		CCLOG("+++++Write User Info Success! in fun [%s]++++", __FUNCTION__);
	}
}


//android 注册回调
void SGMainManager::registerReceive(CCObject *sender)
{
	CCLOG("Sender: [%p] in fun [%s]", sender, __FUNCTION__);
	
	if (sender)
	{
		SdkInfoData *data = (SdkInfoData *)sender;
		CCUserDefault::sharedUserDefault()->setStringForKey("username",data->userName);
		CCUserDefault::sharedUserDefault()->setStringForKey("password",data->password);
		CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", data->isEx);
		CCUserDefault::sharedUserDefault()->flush();
		
		int state = data->state;
		int userId = data->accountId;
		std::string loginKey = data->loginKey;
		
		CCLOG("####loginKey:[%s]####", loginKey.c_str());
		char user[32] = "\0";
		sprintf(user, "%d", userId);
		std::string account(user);
		this->doRegist(state,account,loginKey.c_str());
		CCLOG("+++++Write User Info Success! in fun [%s]++++", __FUNCTION__);
        
	}
}
// android 修改密码回调
void SGMainManager::modifyPassword(CCObject *sender)
{
	CCLOG("Sender: [%p] in fun [%s]", sender, __FUNCTION__);
	
	if (sender)
	{
		SdkInfoData *data = (SdkInfoData *)sender;
		
		int state = data->state;
		int flag = data->flag;
		
		CCLOG("state:[%d], flag:[%d]", state, flag);
		this->doPassword(state, flag);
		
		CCLOG("+++++Modify Success! in fun [%s]++++", __FUNCTION__);
	}
}
//android 快速登陆回调
void SGMainManager::fastLogin(CCObject *sender)
{
	CCLOG("Sender: [%p] in fun [%s]", sender, __FUNCTION__);
	
	if (sender)
	{
		SdkInfoData *data = (SdkInfoData *)sender;
		
		//int state = data->state;
		int userId = data->accountId;
		std::string loginKey = data->loginKey;
		char user[32] = "\0";
		sprintf(user, "%d", userId);
        
		std::string account(user);
		this->doFastLogin(account, loginKey.c_str());
		CCLOG("+++++Modify Success! in fun [%s]++++", __FUNCTION__);
	}
}

#endif
void SGMainManager::setServerInfo(CCString *ip, short post)
{
    serverIp = ip;
    serverIp->retain();
    
    serverPost = post;
}
CCString *SGMainManager::getServerIp()
{
    return serverIp;
}
short SGMainManager::getServerPost()
{
    return serverPost;
}



void SGMainManager::showBox(SGBaseBox *bb,bool isAction)
{
	//bb->setAnchorPoint(ccp(0.5, 0.5));
    if (mainScene->nowBoxTag == (BoxTag)bb->getTag() && mainScene->getChildByTag(mainScene->nowBoxTag) && mainScene->getChildByTag(mainScene->nowBoxTag)->isVisible())
    {
        return;
    }
    
    mainScene->oldBoxTag = mainScene->nowBoxTag;
    mainScene->nowBoxTag = (BoxTag)bb->getTag();

    if(mainScene->nowBoxTag==box_nikeNameBox)
    {
        //bb->setScale(1.0);
    }
    else
    {
        bb->setScale(.4);
        bb->runAction(CCScaleTo::create(ITEMACTIONTIME, 1));
    }
    mainScene->addChild(bb, 90);
    //    }else
    //    {
    //        mainScene->addChild(bb, 100);
    //    }
    //
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(false);
    }
    SGBaseLayer *baseLayer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (baseLayer) {
        baseLayer->setIsCanTouch(false);
    }
}
void SGMainManager::closeBox()
{
    CCLOG("nowLayerTag:%d",mainScene->nowLayerTag);
    SGBaseLayer *layer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (layer)
    {
        layer->setIsCanTouch(true);
    }
    
    if (mainScene->nowBoxTag > 0 && mainScene->getChildByTag(mainScene->nowBoxTag))
        mainScene->removeChildByTag(mainScene->nowBoxTag, true);
    mainScene->nowBoxTag = box_null;
}

void SGMainManager::startGameCGPCallfunc()
{
    mainScene = SGMainScene::create();
    if (mainScene)
    {
        mainScene->nowBoxTag = box_null;
        CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(2.0f, mainScene));
        CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
        CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
#if (PLATFORM == IOS)
        OcCaller::getInstance()->startSDK();
        OcCaller::getInstance()->startHasOffers();
#else
        //android code
        AndroidSDKAdp::getInstance()->startSDK();
#endif
        SGHttpClient::shareHttpClient()->sendStartApp();
    }
    
    //添加按键事件
    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(mainScene);
    
    //开启客户端更新管理。
    SGUpdateManager::gi()->start();
}

void SGMainManager::startGame()
{
#if CGPTest
    SGTestScene*  ptest = new SGTestScene() ;
    ptest->autorelease();
    CCDirector::sharedDirector()->runWithScene(ptest);
//    CCFiniteTimeAction* seq =
//        CCSequence::create(CCDelayTime::create(2000.0f),
//            CCCallFunc::create(this, callfunc_selector(SGMainManager::startGameCGPCallfunc)),
//            nullptr);
//    ptest->runAction(seq);
    return;
    
#else
    mainScene = SGMainScene::create();
    if (mainScene) {
        mainScene->nowBoxTag = box_null;


        CCDirector::sharedDirector()->runWithScene(mainScene);


        //SGGuideManager::shareManager()->isGuide = true;
        
        
        //MMDEBUG: BGN 输出所有图形信息
#if SG_DEBUG_SPOT
        const float btnWidth = 50;
        const float btnHeight = 50;
        
        const float btnWidthHalf = btnWidth * 0.5;
        const float btnHeightHalf = btnHeight * 0.5;
        
        const int tPrior = -500;
        
        //工程方法输出图形信息RED
        SGTouchLayer* stl = SGTouchLayer::create(btnWidth, btnHeight, tPrior, ccc4(255,0,0,255));
        stl->setButtonMode(ResourceManager::sharedInstance(), pf_callback_selector(ResourceManager::showInfo) );
        stl->setPosition(btnWidthHalf, btnHeightHalf);
        mainScene->addChild(stl, 8888);
        
        //用系统方法输出图形信息GREEN
        SGTouchLayer* ssh = SGTouchLayer::create(btnWidth, btnHeight, tPrior, ccc4(0,255,0,255));
        ssh->setButtonMode(CCTextureCache::sharedTextureCache(), pf_callback_selector(CCTextureCache::dumpCachedTextureInfo) );
        CCSize wsize = CCDirector::sharedDirector()->getWinSize();
        ssh->setPosition(wsize.width - btnWidthHalf, btnHeightHalf);
        mainScene->addChild(ssh, 8888);
        
        //工程方法输出图形引用信息BLUE
        SGTouchLayer* stgreen = SGTouchLayer::create(btnWidth, btnHeight, tPrior, ccc4(0,0,255,255));
        stgreen->setButtonMode(ResourceManager::sharedInstance(), pf_callback_selector(ResourceManager::showResRefInfo) );
        stgreen->setPosition(wsize.width - btnWidthHalf, wsize.height - btnHeightHalf);
        mainScene->addChild(stgreen, 8888);
        
        //输出SpriteFrame信息YELLOW
        SGTouchLayer* styellow = SGTouchLayer::create(btnWidth, btnHeight, tPrior, ccc4(255,255,0,255));
        styellow->setButtonMode(CCSpriteFrameCache::sharedSpriteFrameCache(), pf_callback_selector(CCSpriteFrameCache::showInfo) );
        styellow->setPosition(btnWidthHalf, wsize.height - btnHeightHalf);
        mainScene->addChild(styellow, 8888);
#endif
        //MMDEBUG: END
        
        CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
        CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
#if (PLATFORM == IOS)
		OcCaller::getInstance()->startSDK();
        OcCaller::getInstance()->startHasOffers();
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
        doCreatFBSession();
#endif
        
#else
		//android code
        AndroidSDKAdp::getInstance()->startSDK();
#endif
        //        [SdkReqCallBack onStart];
        
        
        //modify by:zyc. 启动游戏app即发送19号消息
        SGHttpClient::shareHttpClient()->sendStartApp();
    }
    
    //添加按键事件
    CCDirector::sharedDirector()->getKeypadDispatcher()->addDelegate(mainScene);
    
    //开启客户端更新管理。
    SGUpdateManager::gi()->start();
#endif
}
void SGMainManager::showLayer(SGBaseLayer *layer, bool isBeforeDel)
{
    mainScene->addShowLayer(layer, isBeforeDel);
}
SGBaseLayer *SGMainManager::getNowShowLayer()
{
    return (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
}
void SGMainManager::showLoadLayer(bool isshow)
{
    mainScene->showLoading(isshow);
}

void SGMainManager::creatMainLayerForGuide()
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (!mainLayer)
    {
        mainLayer = SGMainLayer::create(NULL, sg_mainLayer);
        mainScene->addChild(mainLayer,0);
    }
    
    mainLayer->setVisible(true);
    showFirstLayer();
    
    //MMDBG: UPG 注释勿删，留档。
//    mainScene->updateSecond();
    
}

void SGMainManager::showMainLayer()
{
    SGGuideManager *manager = SGGuideManager::shareManager();
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (!mainLayer)
    {
        mainLayer = SGMainLayer::create(NULL, sg_mainLayer);
        mainScene->addChild(mainLayer,0);
    }else{
        mainLayer->loginAwardList();
    }
    mainLayer->setVisible(true);
    
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    //显示主页面的时候，如果guide_tag_XX还在强制引导的部分中，给出提示未完成
    //UIG_MAX1 强制引导的最大步数
    if (guideId > guide_tag_1 && guideId < guide_tag_49) {
        SGCantAdvanceBox *box = SGCantAdvanceBox::create(NULL, NULL, 36, 0);
        showBox(box);
    }
    else
    {
        if (guideId >= UIG_MAX1)
        {
            SGGuideManager::shareManager()->isGuide = false;
        }
        showFirstLayer();
    }
    //当前没开始第一步引导，开始第一步引导
    if (guideId <= guide_tag_1)
    {
        manager->showSpeaker(1, mainLayer);
    }
    //MMDBG: UPG 注释勿删，留档。
//    mainScene->updateSecond();
}
void SGMainManager::updataUserMsg()
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->updateUserMsg();
    }
}

void SGMainManager::showFirstLayer()
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setVisible(true);
    SGFirstLayer *firstLayer = (SGFirstLayer *)mainScene->getChildByTag(sg_firstLayer);
    if (!firstLayer)
    {
        firstLayer = SGFirstLayer::create(NULL, sg_firstLayer);
    }
    
    if ((SGLandingReward*)mainScene->getChildByTag(sg_landingRewardLayer))
    {
        firstLayer->setIsCanTouch(false);
    }
    
    //CCLog("%d,%d",SGGuideManager::shareManager()->isGuide,SGPlayerInfo::sharePlayerInfo()->getisBossBattleTime());
    if (!SGGuideManager::shareManager()->isGuide && SGPlayerInfo::sharePlayerInfo()->getisBossBattleTime())
    {
        firstLayer->showFirstByTag(2);
    }
    
    
    mainScene->addShowLayer(firstLayer);
    mainLayer->setIsCanTouch(true,true);
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    

    //guide_tag_49之后不再强制引导
    if (guideId >= guide_tag_50)
    {
        //SGGuideManager::shareManager()->checkIsDone(guide_tag_50);
    }
    else if (guideId >= guide_tag_44)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_47);
    }
    else if (guideId >= guide_tag_42)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_44);
    }
    else if (guideId >= guide_tag_40)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_42);
    }
    else if (guideId >= guide_tag_30)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_31);
        if (guideId == guide_tag_32)
        {
            SGGuideManager::shareManager()->checkIsDone(guide_tag_33);
        }
        if (guideId == guide_tag_38)
        {
            SGMainManager::shareMain()->showEmbattleLayer(0,SGTeamgroup::shareTeamgroupInfo()->getiscurrent());
        }

    }
    else if (guideId > guide_tag_21)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_30);
    }
    else if (guideId >= guide_tag_17)
    {
        //暂时修改
//        if (guideId == guide_tag_24) {
//            SGGuideManager::shareManager()->checkIsDone(guide_tag_25);
//        }
//        else if (guideId == guide_tag_29) {
//            SGGuideManager::shareManager()->checkIsDone(guide_tag_30);
//        }
//        else
//        {
//            SGGuideManager::shareManager()->checkIsDone(guide_tag_20);
//        }

        SGGuideManager::shareManager()->checkIsDone(guide_tag_19);
        
    }
    else if (guideId >= guide_tag_10)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_12);
    }
    else if (guideId >= guide_tag_7_3)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_10);
    }
    else if (guideId >= guide_tag_7)//第一关完成，引导到首页，这里进行完成检测
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_7_2);
    }
    else
    {
        if (guideId == guide_tag_4 ||guideId == guide_tag_6 || guideId == guide_tag_9) {
            main::PlotListRequest *request = new main::PlotListRequest();
            request->set_storyid(1);
            request->set_poltaskid(0);
            SGSocketClient::sharedSocketClient()->send(MSG_STORY_SCENE_ENTER, request);
        }else {
            SGGuideManager::shareManager()->checkIsDone(-1);
        }
    }
}
void SGMainManager::showBattleMatchLayer()
{
    SGBattleMatchLayer *bml = SGBattleMatchLayer::create(NULL, sg_battleMatchLayer);
    mainScene->addShowLayer(bml);
    mainScene->removeChildByTag(sg_mainLayer, true);
}

void SGMainManager::showCgBattleLayer(SGBaseLayer *battleLayer)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->setIsCanTouch(false);
        mainLayer->setVisible(false);
    }
    mainScene->addShowLayer(battleLayer);
    SGGuideManager::shareManager()->isGuide = true;
    if (SGGuideManager::shareManager()->isGuide)
    {
        SGAutoGuideLayer *guideLayer = SGGuideManager::shareManager()->getAutoGuideLayer();
        if (guideLayer == NULL) {
            guideLayer = SGAutoGuideLayer::create((SGBattleLayer*)battleLayer);
            mainScene->addChild(guideLayer, 9010);
            SGGuideManager::shareManager()->setAutoGuideLayer(guideLayer);
            guideLayer->setTouchEnabled(false);
        }else {
            guideLayer->setBattleLayer((SGBattleLayer*)battleLayer);
        }
        SGGuideManager::shareManager()->autoStep = 0;
        SGAIManager::shareManager()->aiStep = 0;
    }
}
void SGMainManager::showBattleLayer(SGBaseLayer *battleLayer,bool isPveContinue)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->setIsCanTouch(false);
        mainLayer->setVisible(false);
    }

    mainScene->showBattleLayer(battleLayer);    //modified by cgp
    
    
    if (SGAIManager::shareManager()->isPVE)
    {
        int story = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
        int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
        if (story == 1 || (story == 4 && (plotId == 11 || plotId == 10)) || (story == 6 && (plotId == 17 || plotId == 18 )))
        {
            SGGuideManager::shareManager()->isGuide = true;
        }
        else if (story == 2 && SGPlayerInfo::sharePlayerInfo()->getPlotId() == 4)
        {
            SGGuideManager::shareManager()->isGuide = true;
        }
        else
        {
            SGGuideManager::shareManager()->isGuide = false;
        }
        
        if (isPveContinue)
        {
            SGGuideManager::shareManager()->isGuide = false;
        }
    }
    else
    {
        SGGuideManager::shareManager()->isGuide = false;
    }
    
    if (SGGuideManager::shareManager()->isGuide)
    {
        if (!SGPlayerInfo::sharePlayerInfo()->getIsCgBattle())
        {
            SGFightGuideLayer *guideLayer = SGGuideManager::shareManager()->getFightGuideLayer();
            if (guideLayer == NULL)
            {
                guideLayer = SGFightGuideLayer::create((SGBattleLayer*)battleLayer);
                mainScene->addChild(guideLayer, 9000);
                SGGuideManager::shareManager()->setFightGuideLayer(guideLayer);
                guideLayer->setTouchEnabled(false);
            }
            else
            {
                guideLayer->setBattleLayer((SGBattleLayer*)battleLayer);
            }
            SGGuideManager::shareManager()->step = 0;
            SGAIManager::shareManager()->aiStep = 0;
        }
    }
}



void SGMainManager::showBarrackslayer(bool isBeforeDel)
{
    SGBarracksLayer *barrackslayer = (SGBarracksLayer *)mainScene->getChildByTag(sg_barrackslayer);
    if (!barrackslayer)
    {
        
        barrackslayer = SGBarracksLayer::create();
    }
    else
    {
        
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(false);
    mainScene->addShowLayer(barrackslayer,isBeforeDel);
    
    SGGuideManager::shareManager()->checkIsDone(guide_tag_35);
    //SNSTableView::showScrollItemMultiMoveFromRight(barrackslayer->getSNSTavleView());////1126效果优化
    
}

void SGMainManager::showChatLayer(bool isBeforeDel)
{
    SGChat *chat = (SGChat*)mainScene->getChildByTag(sg_chatLayer);
    if(!chat)
    {
        CCArray *array = SGChatMessageCenter::shareChatMessageCenter()->getArrayCount();
        chat = SGChat::create(array);
    }
    
    SGMainLayer *mainLayer = (SGMainLayer*)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainScene->addShowLayer(chat,isBeforeDel);
}

void SGMainManager::showEmbattleLayer(int type,int emtype,bool isBeforeDel)
{
    SGEmbattleLayer *embattleLayer = (SGEmbattleLayer *)mainScene->getChildByTag(sg_embattleLayer);
    if (!embattleLayer)
    {
        embattleLayer = SGEmbattleLayer::create(type,emtype);
    }
    
    mainScene->addShowLayer(embattleLayer,isBeforeDel);
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->showblack();
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId > guide_tag_36) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_39);
    }else if (guideId > guide_tag_0) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_36);
    }
    
}
void SGMainManager::showGeneralsLayer(int type,bool isBeforeDel,int x,int currentPage,int currentRow)
{
    
    SGGeneralsLayer *layer = (SGGeneralsLayer *)mainScene->getChildByTag(sg_generalsLayer);
    if (!layer)
    {
        
        layer = SGGeneralsLayer::create(type,x,currentPage,currentRow);
        
    }
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
    
    mainScene->addShowLayer(layer, isBeforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
    
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId > guide_tag_18) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_22);
    }else if (guideId > guide_tag_0) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_14);
    }
    
//     CCProfiler::sharedProfiler()->displayTimers();
}

void SGMainManager::showGeneralsBagLayer(int type,bool isBeforeDel,int x,int currentPage,int currentRow)
{
    
    SGGeneralsBagLayer *layer = (SGGeneralsBagLayer *)mainScene->getChildByTag(sg_generalsBagLayer);
    if (!layer)
    {
        
        layer = SGGeneralsBagLayer::create(type,x,currentPage,currentRow);
        
    }
    
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
    
    mainScene->addShowLayer(layer, isBeforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
    
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId > guide_tag_18) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_22);
    }else if (guideId > guide_tag_0) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_14);
    }
    
    //     CCProfiler::sharedProfiler()->displayTimers();
}


void SGMainManager::showBatchSellLayer(int enter, bool beforeDel)
{
	SGCardBatchSellLayer *layer = (SGCardBatchSellLayer *)mainScene->getChildByTag(sg_batchselllayer);
    if (!layer)
    {
        layer = SGCardBatchSellLayer::create(enter);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
	
    mainScene->addShowLayer(layer, beforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
    
}
void SGMainManager::showSoldierslayer(bool isBeforeDel,int x)
{
    SGSoldiersLayer *layer = (SGSoldiersLayer *)mainScene->getChildByTag(sg_soldiersLayer);
    if (!layer) {
        layer = SGSoldiersLayer::create(x);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
    mainScene->addShowLayer(layer,isBeforeDel);
    //SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}
void SGMainManager::removeGeneralInfoLayerForNew()
{
    SGGeneralInfoLayer *generlMsgLayer = (SGGeneralInfoLayer *)mainScene->getChildByTag(sg_generalInfoLayer);
    if (generlMsgLayer)
    {
        generlMsgLayer->removeFromParent();
    }
}
void SGMainManager::showGeneralInfoLayer(SGOfficerCard* card,int type, ERI *info, bool isBeforeDel,SGOfficerCard *officer,int isfromfirst,CCDictionary *dic,int currentPage,int currentRow)
{
    if(isfromfirst!=11&&isfromfirst!= 12)
    {
        CCArray *array =CCArray::create();
        array->retain();
        array =SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
        SGGeneralInfoLayer::setMyOfficerList(array);
    }else{
        SGTeamgroup* sgtg = SGTeamgroup::shareTeamgroupInfo();
        CCArray* ofcList = sgtg->getEmbattleOfficers(sgtg->getiscurrent());
        SGGeneralInfoLayer::setMyOfficerList(ofcList);
    }
    SGGeneralInfoLayer *generlMsgLayer = (SGGeneralInfoLayer *)mainScene->getChildByTag(sg_generalInfoLayer);
    if (!generlMsgLayer) {
        generlMsgLayer = SGGeneralInfoLayer::create(card,type,info, officer,isfromfirst,dic,false,currentPage,currentRow);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->hideblack();
    
    mainScene->addShowLayer(generlMsgLayer,isBeforeDel);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
    
    if (type==25)
    {
        mainLayer->setIsCanTouch(false);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");

    //新手引导第15步，如果是在头像那里点击，进入详情
    if (type == 3 && guideId == guide_tag_12)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_13);
    }
    
    if (guideId >= guide_tag_22_1)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_25);
    }
    else if (guideId > guide_tag_17)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_22_1);
    }
    else
    {
        generlMsgLayer->setIsCanTouch(false);
    }
}

//添加额外的层，一般用于一次性的全屏UI，但是又不想影响MainScene的nowLayerTag。tips：游戏里所有的box的zorder为90。
void SGMainManager::addShowLayer(SGBaseLayer *layer , bool isBeforeDel)
{
    mainScene->addChild(layer);
}

void SGMainManager::showSoldierInfoLayer(SGSoldierCard *card,int type, bool isBeforeDel,SGSoldierCard *before, bool isfirst,CCDictionary *dic)
{
    SGSoldierInfoLayer *soldierMsgLayer = SGSoldierInfoLayer::create(card,type,before,isfirst,dic);
    mainScene->addShowLayer(soldierMsgLayer,isBeforeDel);
    if (type == CHATSHOWENTRYTAG) {
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        mainLayer->hideblack();
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);
    }
}

void SGMainManager::showEquipsLayer(SGOfficerCard *card,int equiptype,int enter,bool isBeforeDel,int x,int currentPage,int currentRow)
{
    
    SGEquipsLayer *layer = NULL;
    layer =  (SGEquipsLayer *)mainScene->getChildByTag(sg_equipsLayer);
    if(layer)
    {
        layer->removeFromParent();
    }
    layer= SGEquipsLayer::create(card,equiptype,enter,x,currentPage,currentRow);
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
    mainScene->addShowLayer(layer,isBeforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}
void SGMainManager::showEquipInfoLayer(SGEquipCard* card,int type,bool isBeforeDel,SGEquipCard *before,bool isfirst,CCDictionary *dic,int currentPage,int currentRow )
{
    SGEquipInfoLayer *equipMsgLayer = SGEquipInfoLayer::create(card,type,before,isfirst,dic,currentPage,currentRow);
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->hideblack();
    mainScene->addShowLayer(equipMsgLayer,isBeforeDel);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
    
    if (type==25) {
        mainLayer->setIsCanTouch(false);
        mainLayer->setPortraitMenuCanTouch(false);
    }
}
void SGMainManager::showPropsLayer(bool isBeforeDel,int currentPage)
{
    SGPropsLayer *layer = (SGPropsLayer *)mainScene->getChildByTag(sg_propsLayer);
    if (!layer) {
        layer = SGPropsLayer::create(currentPage);
    }
    mainScene->addShowLayer(layer,isBeforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}
float SGMainManager::getHeadHeight()
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    //mainLayer->getHeadHeight();
    return mainLayer->getHeadHeight();
}
float SGMainManager::getBottomHeight()
{
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    //mainLayer->getBottomHeight();
    return mainLayer->getBottomHeight();
}

float SGMainManager::fivebuttonhight()
{
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    //mainLayer->getfivebuttonhight();
    return mainLayer->getfivebuttonhight();
}

float SGMainManager::getTotleHdHgt()
{
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    //mainLayer->getTotleUpHgt();
    return mainLayer->getTotleUpHgt();
}
float SGMainManager::getVisitHgt()
{
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    //mainLayer->getVisitHeight();
    return mainLayer->getVisitHeight();
    
}

void SGMainManager::showlastListLayer(int count,CCArray *ary,bool isBeforeDel)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(false);
    SGpvpLastListLayer *layer = (SGpvpLastListLayer *)mainScene->getChildByTag(sg_pvplastlistlayer);
    if (!layer) {
        layer = SGpvpLastListLayer::create(count,ary);
    }
    mainScene->addShowLayer(layer,isBeforeDel);
}

void SGMainManager::showSeasonListLayer(int count,CCArray *ary,bool isBeforeDel)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(false);
    SGSeasonListLayer *layer = (SGSeasonListLayer *)mainScene->getChildByTag(sg_seasonlistlayer);
    if (!layer) {
        layer = SGSeasonListLayer::create(count,ary);
    }
    mainScene->addShowLayer(layer,isBeforeDel);
    
}
void SGMainManager::showpvpEverydayLayer(int count,CCArray *arrary,bool isBeforeDel)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(false);
    SGpvpEverydayLayer *layer = (SGpvpEverydayLayer *)mainScene->getChildByTag(sg_pvpeverydaylayer);
    if (!layer) {
        layer = SGpvpEverydayLayer::create(count,arrary);
    }
    mainScene->addShowLayer(layer,isBeforeDel);
    //    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

void SGMainManager::showExpandLayer(bool isBeforeDel)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(true);
    SGExpandLayer *layer = (SGExpandLayer *)mainScene->getChildByTag(sg_expandLayer);
    if (!layer) {
        layer = SGExpandLayer::create();
    }
    mainScene->addShowLayer(layer,isBeforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

void SGMainManager::showJunxianLayer(bool isBeforeDel)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(false);
    SGJunxianLayer *layer = (SGJunxianLayer *)mainScene->getChildByTag(sg_jitianLayer);
    if (!layer) {
        layer = SGJunxianLayer::create();
    }
    mainScene->addShowLayer(layer,isBeforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}


//显示碎片界面，在此接收服务器的返回的碎片信息，填充数组，传参
void SGMainManager::showPiecesLayer(bool isBeforeDel)
{
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	SGPiecesLayer *layer = (SGPiecesLayer *)mainScene->getChildByTag(sg_piecesLayer);
    
    if (layer)
	{
        layer->removeFromParent();
        layer = NULL;
    }
    
    if (!layer)
	{
        layer = SGPiecesLayer::create();
    }
	mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
	mainScene->addShowLayer(layer,isBeforeDel);
	SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

//显示道具页面
void SGMainManager::showConsumeLayer(bool isBeforeDel)
{
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	SGConsumable *layer = (SGConsumable *)mainScene->getChildByTag(sg_consumablelayer);
    if (!layer)
	{
        layer = SGConsumable::create();
    }
	mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
	mainScene->addShowLayer(layer,isBeforeDel);
	SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

//显示材料页面
void SGMainManager::showMaterialLayer(bool isBeforeDel)
{
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	SGMaterialLayer *layer = (SGMaterialLayer *)mainScene->getChildByTag(sg_materialLayer);
    if (!layer)
	{
        layer = SGMaterialLayer::create();
    }
	mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
	mainScene->addShowLayer(layer,isBeforeDel);
	SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

void SGMainManager::showblack()
{
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
}
void SGMainManager::hideblack()
{
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(true);
}
void SGMainManager::showAllLayer(bool isBeforeDel)
{
    SGShowAllLayer* layer = (SGShowAllLayer *)mainScene->getChildByTag(sg_showAllLayer);
    if (!layer) {
        layer = SGShowAllLayer::create();
    }
    mainScene->addShowLayer(layer);
}
void SGMainManager::showSoulCardInfoLayer(SGPropsCard *card,int enterType,bool delBefore)
{
    SGSoulCardInfoLayer* layer = (SGSoulCardInfoLayer *)mainScene->getChildByTag(sg_soulCardLayer);
    if (!layer) {
        layer = SGSoulCardInfoLayer::create(card,enterType);
    }
    mainScene->addShowLayer(layer,delBefore);
    if (enterType == CHATSHOWENTRYTAG) {
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        mainLayer->hideblack();
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    
}
void SGMainManager::showStrengLayer(SGBaseMilitaryCard *card, int type,int enter, bool isOfficerOnly,int currentPage,ERI *info)
{
    if (type == 2)  //装备强化
    {
        SGEquipStrengLayer* equipstrengLayer = (SGEquipStrengLayer *)mainScene->getChildByTag(sg_equipstrengLayer);
        if (equipstrengLayer == NULL)
        {
            equipstrengLayer = SGEquipStrengLayer::create(card,type,enter,currentPage);
        }
        else
        {
            //如果页面存在，直接设置卡牌
            equipstrengLayer->refreshView(static_cast<SGEquipCard *>(card));
            //为了强化的成功能在本页显示
            //equipstrengLayer->refreshStrengView();
        }
        
        this->showLayer(equipstrengLayer);
    }
    else   //武将强化
    {
        SGStrengLayer* strengLayer = (SGStrengLayer *)mainScene->getChildByTag(sg_strengLayer);
        if (strengLayer == NULL)
        {
            strengLayer = SGStrengLayer::create(card,type,enter,isOfficerOnly,info,currentPage);
        }
        else
        {
            //为了强化的成功能在本页显示
            strengLayer->refreshStrengView();
        }
        
        this->showLayer(strengLayer);
        SGGuideManager::shareManager()->checkIsDone(guide_tag_15);
    }
}

void SGMainManager::showStrengAdvLayer(SGBaseMilitaryCard *card, int type,int enter, bool isOfficerOnly,int currentPage,ERI *info)
{
    if (type == 2)
    {
        SGEquipStrengLayer* equipstrengLayer = (SGEquipStrengLayer *)mainScene->getChildByTag(sg_equipstrengLayer);
        if (equipstrengLayer == NULL)
        {
            equipstrengLayer = SGEquipStrengLayer::create(card,type,enter,currentPage);
        }
        else
        {
            //如果页面存在，直接设置卡牌
            equipstrengLayer->refreshView(static_cast<SGEquipCard *>(card));
            //为了强化的成功能在本页显示
            //equipstrengLayer->refreshStrengView();
        }
        
        this->showLayer(equipstrengLayer);
    }
    else
    {
        CGPStrengAdvLayer* strengLayer = (CGPStrengAdvLayer *)mainScene->getChildByTag(cgp_strengAdvLayer);
        if (strengLayer == NULL)
        {
            strengLayer = CGPStrengAdvLayer::create(card,type,enter,isOfficerOnly,info,currentPage);
        }
        else
        {
            //为了强化的成功能在本页显示   //cgp useful
            //strengLayer->refreshStrengView();
        }

        this->showLayer(strengLayer);
        //SGGuideManager::shareManager()->checkIsDone(guide_tag_15);
    }
}




void SGMainManager::showAdvancechooseLayer(SGBaseMilitaryCard *card, int type,bool enter,bool isOfficerOnly,int currentPage,CCArray *array)
{
    SGAdvancechooseLayer *advancechoose = SGAdvancechooseLayer::create(card,type,enter,isOfficerOnly,currentPage,array);
    this->showLayer(advancechoose);
    //    SGGuideManager::shareManager()->checkIsDone(guide_tag_15);
}

void SGMainManager::showAdvanceLayer(SGBaseMilitaryCard *card,int type,int isfirst,CCArray *array, ERI *info)
{
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId < guide_tag_24 && array == NULL && type == 1) {
        CCObject *general = NULL;
        array = CCArray::create();
        CCArray * generals = SGPlayerInfo::sharePlayerInfo()->getShowOfficerCards(false);
        CCARRAY_FOREACH(generals, general)
        {
            array->addObject(general);
        }
    }
    SGAdvanceLayer *advanceLayer = SGAdvanceLayer::create(card,type,isfirst,array, info);
    this->showLayer(advanceLayer);
    SGGuideManager::shareManager()->checkIsDone(guide_tag_23);
}

//显示装备进阶页面
void SGMainManager::showEquipAdvancedLayer(SGEquipCard *card, bool isBeforeDel, int enterType)
{
    SGEquipAdvancedLayer *equipAdvanced = SGEquipAdvancedLayer::create(card, enterType);
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->hideblack();
    }
    mainScene->addShowLayer(equipAdvanced, isBeforeDel);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
}

void SGMainManager::showAlertBox(SGBoxDelegate *delegate, const char *msg)
{
    SGAlertBox *alertBox = SGAlertBox::create(delegate, msg);
    this->showBox(alertBox);
}
void SGMainManager::showPropInfoLayer(SGPropsCard* card,int type,bool isBeforeDel,int currentPage)
{
    SGPropInfoLayer *propMsgLayer = SGPropInfoLayer::create(card,type,currentPage);
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->hideblack();
    mainScene->addShowLayer(propMsgLayer,isBeforeDel);
    
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
}

void SGMainManager::showConsumableInfoLayer(SGConsumableCard *card, bool isBeforeDel, int enterType, int whichShop, int currentPage)
{
	SGConsumableInfoLayer *comsumeLayer = SGConsumableInfoLayer::create(card, enterType, whichShop);
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->hideblack();
    mainScene->addShowLayer(comsumeLayer,isBeforeDel);
	
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
}


//显示特殊副本（幻境）
void SGMainManager::showSpecialBattleLayer(CCArray *data)
{
    CCLOG("Show Special Battle!");
    SGSpecialBattleLayer *layer = SGSpecialBattleLayer::create(data);
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->hideblack();
    mainScene->addShowLayer(layer, true);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
    
}



void SGMainManager::showSpecialBattleSectionInfoLayer(CCArray *sArray, SpecialBattleData *chapInfo)
{
    CCLOG("Special Battle Section information");
    
    SGSpecialBattleSectionLayer *sectionLayer = (SGSpecialBattleSectionLayer *)mainScene->getChildByTag(sg_specialBattleSecionLayer);
    if (!sectionLayer)
    {
        sectionLayer = SGSpecialBattleSectionLayer::create(sArray, chapInfo);
    }
    else
    {
        sectionLayer->freshBuyCount(chapInfo);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->hideblack();
    mainScene->addShowLayer(sectionLayer, true);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
}

void SGMainManager::showSkillCardsLayer(int type,bool isBeforeDel)
{
    SGSkillCardsLayer *strengLayer = SGSkillCardsLayer::create(type);
    this->showLayer(strengLayer,isBeforeDel);
    
}
void SGMainManager::showSkillCardInfoLayer(SGBaseMilitaryCard *card, int type,bool isBeforeDel,bool state)
{
    SGSkillCardInfoLayer *propMsgLayer = SGSkillCardInfoLayer::create(card,type,state);
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->hideblack();
    mainScene->addShowLayer(propMsgLayer,isBeforeDel);
    
}
void SGMainManager::showShopLayer()
{
    if (mainScene->nowLayerTag == sg_shopLayer)
    {
        return;
    }
    SGShopLayer *layer = (SGShopLayer *)mainScene->getChildByTag(sg_shopLayer);
    if (!layer)
    {
        layer = SGShopLayer::create();
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visitshow();
    mainLayer->setPortraitMenuCanTouch(true);
    mainScene->addShowLayer(layer);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());
}
void SGMainManager::showStoreLayer(int curr,int next,int currrmb,int maxrmb,CCArray *a,int type,CCArray *str,bool isBeforeDel)
{
    SGStoreLayer *layer = (SGStoreLayer *)mainScene->getChildByTag(sg_storeLayer);
    if (!layer)
    {
        layer = SGStoreLayer::create(curr,next,currrmb,maxrmb,a,type,str);
    }
    else
    {
        layer->reSetData(a,str);
    }
    mainScene->addShowLayer(layer,isBeforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}


#pragma mark -
#pragma mark -------story actions-------
void SGMainManager::showStoryMainLayer(CCArray *array, bool isBeforeDel)
{
    if(array == NULL) array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
    SGStoryMainLayer *layer = (SGStoryMainLayer *)mainScene->getChildByTag(sg_storyLayer);
    if (!layer)
    {
        layer = SGStoryMainLayer::create(array);  //cgp_pro
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->setVisible(true);
        mainLayer->setIsCanTouch(true);
        mainLayer->visitshow();
    }
    mainScene->addShowLayer(layer, isBeforeDel);
    
    
    //针对不同的引导步数，要多次进入闯关模式列表中
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");

    //领奖
    if (guideId >= guide_tag_48)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_51);//最后一步引导的时候，返回之后应该引导第guide_tag_51步
    }
    if (guideId >= guide_tag_44)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_45);
    }
    else if (guideId >= guide_tag_21)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_30);
    }
    else if (guideId >= guide_tag_19)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_20);
    }
    else if (guideId >= guide_tag_9)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_10);
    }
    else if (guideId > guide_tag_3)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_7_3);
    }else if (guideId > guide_tag_0)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_3);
    }
    // SNSTableView::showScrollItemMultiMoveSlide(layer->getSNSTavleView());
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

//creamBattle action//george//10.28
void SGMainManager::showCreamBattleLayer(CCArray *array, bool isBeforeDel)
{
	
    if(array == NULL) array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
    SGCreamBattleLayer *layer = (SGCreamBattleLayer *)mainScene->getChildByTag(sg_creamLayer);
    if (!layer) {
        layer = SGCreamBattleLayer::create(array);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setVisible(true);
        mainLayer->setIsCanTouch(true);
        mainLayer->visitshow();
    }
    
    mainScene->addShowLayer(layer, isBeforeDel);
    
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}
//向服务器请求最新的精英副本数据
void SGMainManager::refreshCreamBattleList(int storyId)
{
	SGPlayerInfo::sharePlayerInfo()->setPlotId(20000);//
    main::BossPlotListRequest *request = new main::BossPlotListRequest();
    request->set_storyid(storyId);
	
    SGSocketClient::sharedSocketClient()->send(MSG_CREAM_STORY, request);
}
//显示boss战排行榜
void SGMainManager::showBossBattleRankLayer(CCArray *data , SGKillBossInfo *info, int enterType, bool isBeforeDel)
{
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	SGBossBattleRankListLayer *layer = (SGBossBattleRankListLayer *)mainScene->getChildByTag(sg_bossbattleranklayer);
    if (!layer)
	{
        layer = SGBossBattleRankListLayer::create(data, info, enterType);
    }
	mainLayer->visitshow();
	mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
	mainScene->addShowLayer(layer,isBeforeDel);
	SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

//显示boss战详情
void SGMainManager::showBossBattleDetailLayer(SGBossBattleData *bossData, int enterType, bool del)
{
	SGBossBattleDetailLayer *layer = (SGBossBattleDetailLayer *)mainScene->getChildByTag(sg_bossbattledetailLayer);
    if (!layer) {
        layer = SGBossBattleDetailLayer::create(enterType, bossData);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
	{
		mainLayer->visitshow();
		mainLayer->setPortraitMenuCanTouch(false);
        //        mainLayer->setVisible(true);
        mainLayer->setIsCanTouch(true,false);
        //        mainLayer->visitshow();
    }
	
    mainScene->addShowLayer(layer);
    
}
//刷新Boss详情页面
void SGMainManager::refreshBossInfo(SGBossStateData *data)
{
	SGBossBattleDetailLayer *layer = (SGBossBattleDetailLayer *)mainScene->getChildByTag(sg_bossbattledetailLayer);
    if (!layer) {
        return ;
    }
	layer->refreshBossView(data);
}
//刷新buff值信息
void SGMainManager::freshBuffInfo(int v1, int v2, int v3)
{
	SGBossBattleDetailLayer *layer = (SGBossBattleDetailLayer *)mainScene->getChildByTag(sg_bossbattledetailLayer);
    if (!layer) {
        return ;
    }
	layer->freshBuff(v1, v2, v3);
}


//可参战界面
void SGMainManager::showBossBattleJoinInLayer(BossBattleJoinData *data){
    SGBossBattleJoinInLayer *layer = (SGBossBattleJoinInLayer *)mainScene->getChildByTag(sg_bossBattleJoinInLayer);
    if (!layer) {
        layer = SGBossBattleJoinInLayer::create(data);
    }
    
	
    mainScene->addShowLayer(layer);
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
	{
        mainLayer->setIsCanTouch(true);
		mainLayer->setPortraitMenuCanTouch(false);
        mainLayer->setVisible(true);
    }
    if (SGGuideManager::shareManager()->getLimitGuideSize() > 0)
    {
        layer->setIsCanTouch(false);
    }
}

void SGMainManager::showBossBattleLayer(BossBattleData *bossData,BossReward *storyReward){
    
    SGBossBattleLayer *layer = (SGBossBattleLayer *)mainScene->getChildByTag(sg_bossBattleLayer);
    if (!layer) {
        layer = SGBossBattleLayer::create(bossData,storyReward);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->visitshow();
        mainLayer->setPortraitMenuCanTouch(false);
        mainLayer->setTouchEnabled(false);
        mainLayer->setIsCanTouch(false);
        
    }
    
    mainScene->addShowLayer(layer);
    
}

void SGMainManager::showStorySectionLayer(SGStoryBean* storyBean, int index)
{
    if (storyBean == NULL) //bymm: 2014.08.05 StorySectionLayer里头一定会使用这个bean，空指针则必崩，这里暂时处理为空指针返回，如果有需求，再行优化StorySectionLayer。
    {
        return;
    }
    
    //这里做等级开启的阶段引导的设定
    if (SGGuideManager::shareManager()->getLimitGuideSize() > 0)
    {
        SGGuideManager::shareManager()->isGuide = true;
    }
    
    
    SGStorySectionLayer *layer = (SGStorySectionLayer *)mainScene->getChildByTag(sg_storySectionLayer);
    if (!layer) {
        layer = SGStorySectionLayer::create(storyBean);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    
    if (mainLayer && mainScene->nowBoxTag!=box_quickfight && mainScene->nowBoxTag!=box_canAdvanceTag)
	{
        mainLayer->setVisible(true);
        mainLayer->setIsCanTouch(true);
        mainLayer->visitshow();
    }
    
    mainScene->addShowLayer(layer);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId >= guide_tag_52 && guideId < guide_tag_99) {
        SGGuideManager::shareManager()->closeGuide();
    }
    else if (guideId >= guide_tag_49) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_50);
    }
    else if (guideId >= guide_tag_46) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_47);
    }
    else if (guideId >= guide_tag_45)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_46);
    }
    else if (guideId > guide_tag_21)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_30);//直接开始探访
    }
    else if (guideId > guide_tag_10) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_21);
    }
    else if (guideId > guide_tag_8) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_10);
    }
    else if (guideId > guide_tag_7_1) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_8);
    }
    else if (guideId > guide_tag_4) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_7);
    }
    else if (guideId > guide_tag_0) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_4);
    }
    if (guideId > guide_tag_47)
    {
        layer->showFirstSectionInfo();
    }
    
    //阶段引导
    if (SGGuideManager::shareManager()->getLimitGuideSize() > 0)
    {
        SGGuideManager::shareManager()->startLimitGuide();
    }
}

void SGMainManager::showCreamSectionLayer(SGStoryBean* storyBean)
{
    SGStorySectionLayer *layer = (SGStorySectionLayer *)mainScene->getChildByTag(sg_creamLayer);
    if (!layer) {
        layer = SGStorySectionLayer::create(storyBean);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setVisible(true);
        mainLayer->setIsCanTouch(true);
        mainLayer->visitshow();
    }
    
    mainScene->addShowLayer(layer);
    
    //    SNSTableView::showScrollItemScale(layer->getSNSTavleView());
	SNSTableView::showScrollItemMultiMoveSlide(layer->getSNSTavleView());
    
    
    //针对新手引导的某一大步（某几步构成的引导部分），如果只是完成某一大步引导的小部分，则重置到这一大步的开始，重新引导
//    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
//    if (guideId >= guide_tag_55) {
//        SGGuideManager::shareManager()->checkIsDone(guide_tag_56);
//    }else if (guideId >= guide_tag_52) {
//        SGGuideManager::shareManager()->checkIsDone(guide_tag_53);
//    }else if (guideId >= guide_tag_49) {
//        SGGuideManager::shareManager()->checkIsDone(guide_tag_50);
//    }else if (guideId >= guide_tag_46) {
//        SGGuideManager::shareManager()->checkIsDone(guide_tag_47);
//    }else if (guideId > guide_tag_10) {
//        SGGuideManager::shareManager()->checkIsDone(guide_tag_18);
//    }else if (guideId > guide_tag_8) {
//        SGGuideManager::shareManager()->checkIsDone(guide_tag_10);
//    }else if (guideId > guide_tag_4) {
//		SGGuideManager::shareManager()->checkIsDone(guide_tag_7);
//    }
//    else if (guideId > guide_tag_0) {
//        SGGuideManager::shareManager()->checkIsDone(guide_tag_4);
//    }
}
//显示竞技场界面
void SGMainManager::showAreanLayer(CCArray * array, SGAreanBaseInfoData * baseData,SGAreanItemInfoData * itemData,AreanLayerType type)
{
    //SGAreanLayer
    SGAreanLayer *layer = (SGAreanLayer *)mainScene->getChildByTag(sg_areanLayer);
    if (!layer) {
        layer = SGAreanLayer::create(array,baseData,itemData,type);
    }
    else
    {
        layer->setMyselfData(itemData);
        layer->initDatas(array);
        layer->setBaseInfoData(baseData);
        layer->setAreanLayerType(type);
        layer->updateAreanLayer();
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->visitshow();
        mainLayer->setPortraitMenuCanTouch(false);
        mainLayer->setTouchEnabled(true);
        mainLayer->setIsCanTouch(true,false);
        
    }
    
    mainScene->addShowLayer(layer);
}
//发送到竞技场的消息
void SGMainManager::goToArenaLayerRequest()
{
    int lavel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    if (lavel < SGPlayerInfo::sharePlayerInfo()->getLimitLevelById(limitArena))
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MainLayer_str6403,SGPlayerInfo::sharePlayerInfo()->getLimitLevelById(limitArena))->getCString());
        return;
    }

    main::ArenaMainInfoRequest * request = new main::ArenaMainInfoRequest();
    
    SGSocketClient::sharedSocketClient()->send(MSG_ARENA_MAIN_INFO, request);
}
//显示对战界面
void SGMainManager::showVsLayer()
{
    SGVsLayer * vsLayer = (SGVsLayer * )mainScene->getChildByTag(sg_vsLayer);
    if(!vsLayer)
    {
        vsLayer  = SGVsLayer::create();
    }
    mainScene->addShowLayer(vsLayer);
}
void SGMainManager::skipFindHelp(CCArray *helpers, int plotId, bool storyModel)
{
    CCArray *datas = CCArray::create();
    SGStoryBean *bean = NULL;
    CCArray *array = NULL;
    if (storyModel == true)
    {
        array = SGPlayerInfo::sharePlayerInfo()->getStoryData();
    }
    else
    {
        array = SGPlayerInfo::sharePlayerInfo()->getActivityData();
        
    }
    if (array == NULL)
    { //教学中断
        bean = SGStaticDataManager::shareStatic()->getStoryDataById(1);
    }
    else
    {
        array->retain();
        for (int ii = 0; ii < array->count(); ii++)
        {
            SGStoryBean *story = (SGStoryBean *)array->objectAtIndex(ii);
            if (story->getSceneBeanById(plotId))
            {
                bean = story;
                break;
            }
        }
    }
    if (datas)
    {
        datas->release();
        datas = NULL;
    }
    datas = CCArray::create();
    datas->retain();
    
    for (int ii = 0; ii < helpers->count(); ii++)
    {
        CCObject *obj = helpers->objectAtIndex(ii);
        datas->addObject(obj);
    }
    
    //以上为援军数据，直接默认选择第一个，然后直接开始战斗
    //初始化找朋友帮忙的数据
    SGPveHelper *roler = (SGPveHelper *)datas->objectAtIndex(0);
    //roleID = roler->getRoleId();
    HelpFriendRoleName = roler->getRoleName()->getCString();
    HelpFriendRoleId=roler->getRoleId();
    HelpFriendItemId = roler->getItemId();
    HelpFriendItemLevel = roler->getItemLevel();
    isInMyFriendList = roler->getInMyFriendList();
    
    SGBattleManager::sharedBattle()->pveStart(bean->getStoryId(), plotId, roler->getRoleId());
}

void SGMainManager::showFindHelpLayer(int plotId, CCArray *helpers, bool storyModel)//cgp_pro
{
    SGFindHelpLayer *layer = (SGFindHelpLayer *)mainScene->getChildByTag(sg_findHelplayer);
    if (layer)
    {
        layer->removeFromParent();
        layer = NULL;
    }
    
    if (!layer)
    {
        if (plotId >= 1000)
        {
            storyModel = false;
        }
		if (plotId >= 20000)//精英副本
            
		{
			storyModel = true;
		}

        layer = SGFindHelpLayer::create(plotId, storyModel);//cgp_pro
        
    }
    layer->initDatas(helpers);
    
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");

    //第一个引导，援军列表不再显示
    if (guideId > guide_tag_5)
    {
        mainScene->addShowLayer(layer);  //cgp_pro
    }
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
    
    if (guideId >= guide_tag_57)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_58);
    }
    else if (guideId >= guide_tag_54)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_55);
    }
    else if (guideId >= guide_tag_51)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_52);
    }
    else if (guideId >= guide_tag_48)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_49);
    }
    else if (guideId > guide_tag_21)
    {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_25);
    }
    else if (guideId > guide_tag_19)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_24);
        CCUserDefault::sharedUserDefault()->flush();
        
        //没有援军部分，这里做略过处理，直接读取援军表第一个，开始战斗
        this->skipFindHelp(helpers, plotId, storyModel);
        //SGGuideManager::shareManager()->checkIsDone(guide_tag_9);
    }
    else if (guideId > guide_tag_5)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_9);
        CCUserDefault::sharedUserDefault()->flush();
        
        //没有援军部分，这里做略过处理，直接读取援军表第一个，开始战斗
        this->skipFindHelp(helpers, plotId, storyModel);
        //SGGuideManager::shareManager()->checkIsDone(guide_tag_9);
    }
    else if (guideId > guide_tag_0)
    {
       // int gid = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        
        CCUserDefault::sharedUserDefault()->setIntegerForKey("guideId", guide_tag_6);
        CCUserDefault::sharedUserDefault()->flush();
        //SGGuideManager::shareManager()->checkIsDone(guide_tag_5);//第一大步的引导就是到进入战斗
        
        //没有援军部分，这里做略过处理，直接读取援军表第一个，开始战斗
        this->skipFindHelp(helpers, plotId, storyModel);
        

        
    }
}

void SGMainManager::showActivityLayer(CCArray *array, bool del)
{
    if(array == NULL) array = SGPlayerInfo::sharePlayerInfo()->getActivityData();
    array->retain() ;
    SGActivityMainLayer *layer = (SGActivityMainLayer *)mainScene->getChildByTag(sg_activityLayer);
    if (!layer) {
        layer = SGActivityMainLayer::create(array);
    }
    mainScene->addShowLayer(layer);
}




void SGMainManager::requestFirstActivityLayer()
{
    if (getMainLayer()) {
        getMainLayer()->entryFirstChargeShow();
    }
}

void SGMainManager::showFirstActivityLayer(CCArray *array,CCObject * obj)
{
    SGActivityLayer *layer = (SGActivityLayer *)mainScene->getChildByTag(sg_luaactivity);
    if (layer) {
        layer->removeFromParentAndCleanup(true);
    }
    
    layer = SGActivityLayer::create(array,obj);
    SGMainManager::shareMain()->getMainScene()->addShowLayer(layer);
}


#pragma mark -
#pragma mark - -------pvp actions-------
void SGMainManager::showPvpMainLayer()
{
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    int count = player->getGoodsNumInBag();
    //引导的时候不出现背包满提示
    if (count >= player->getPlayerBagSize() && !SGGuideManager::shareManager()->isGuide)
    {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(SGMainManager::shareMain()->getMainLayer(), NULL, 12, count);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
    }
    else
    {
        showPvpMatchLayer();//直接进入pvp
    }
    
    return;
    
    //    SGPvpMainLayer *layer = (SGPvpMainLayer *)mainScene->getChildByTag(sg_pvpMainLayer);
    //    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    //    if (mainLayer) {
    //        mainLayer->setIsCanTouch(true);
    //        mainLayer->setVisible(true);
    //        mainLayer->visitshow();
    //    }
    //    if (!layer) {
    //        layer = SGPvpMainLayer::create();
    //    }
    //
    //    mainScene->addShowLayer(layer);
    //    //SNSTableView::showScrollItemScale(layer->getSNSTavleView());
    //    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());
}

void SGMainManager::showRankLayer(CCArray *data, int num,int index)
{
    DDLog("tt-%d-------------------------showRankLayer", index);
    SGMainLayer *mainlayer = (SGMainLayer*)mainScene->getChildByTag(sg_mainLayer);
    mainlayer->visitshow();
    mainlayer->setPortraitMenuCanTouch(false);
    SGBattleRankLayer *layer = (SGBattleRankLayer *)mainScene->getChildByTag(sg_rankLayer);
    if (!layer) {
        layer = SGBattleRankLayer::create(data,num);
    }
    layer->setSelecIndex(index);
    
    mainScene->addShowLayer(layer, false);
}

void SGMainManager::showChgCreditCardLayer(CCArray *data, int index,int type)
{
    DDLog("tt-%d--------------------------showChgCreditCardLayer", index);
    SGBattleScoreLayer *layer = (SGBattleScoreLayer *)mainScene->getChildByTag(sg_creditCardLayer);
    if (!layer) {
        layer = SGBattleScoreLayer::create(data,type);
        layer->setSelecIndex(index);//如果没有layer才设置index,要不按钮会一直只热门黑
    }
    
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setIsCanTouch(false);
    mainLayer->setPortraitMenuCanTouch(false);
    
    if(mainScene->nowLayerTag==sg_battleMainLayer)
        mainScene->addShowLayer(layer,false);
    else
        mainScene->addShowLayer(layer);
}

void SGMainManager::showExchangLayer(int lvl,int curr,int max,int gold,int coin)
{
    SGGoldChangCoin *layer = (SGGoldChangCoin *)mainScene->getChildByTag(sg_exchangCoin);
    if (!layer) {
        layer = SGGoldChangCoin::create(lvl,curr,max,gold,coin);
    }
    mainScene->addShowLayer(layer);
}
void SGMainManager::showGiftsLayer(int vipLvl,CCArray *_array,int curr,int max)
{
    CCArray *a = CCArray::create();
    a->addObjectsFromArray(_array);
    
    SGVipGiftLayer *layer = (SGVipGiftLayer *)mainScene->getChildByTag(sg_vipGiftLayer);
    if (!layer) {
        layer = SGVipGiftLayer::create(vipLvl,a,curr,max);
    }
    
    mainScene->addShowLayer(layer);
    
    //这时候输出含有table初始化。
    CCProfiler::sharedProfiler()->displayTimers();
    
    if (vipLvl!=0) {
        SNSIndexPath *index = SNSIndexPath::create(vipLvl - 1, 1, 0);
        layer->tableView->moveToIndexPath(index);
        
    }
    else
    {
        SNSIndexPath *index = SNSIndexPath::create(0, 1, 0);
        layer->tableView->moveToIndexPath(index);
    }
    
}
void SGMainManager::showMessage(const char *messageStr)
{
    mainScene->showMessage(messageStr);
}

void SGMainManager::showEffect(SGEquipCard *card,int type){
    
    mainScene->showEffect(card, type);
}

void SGMainManager::showConnectTip(SGNetworkErrorType type)
{
    mainScene->removeChildByTag(CONNECT_TIP_TAG, true);
    SGConnectTipBox *tipLayer = SGConnectTipBox::create(NULL, type);
    mainScene->addChild(tipLayer, CONNECT_TIP_TAG, CONNECT_TIP_TAG);
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(false);
    }
    SGBaseLayer *baseLayer = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (baseLayer) {
        baseLayer->setIsCanTouch(false);
    }
}

void SGMainManager::showHelpLayer(bool isBeforeDel)
{
    if (mainScene->nowLayerTag == sg_helpLayer)
    {
        return;
    }
    SGHelpLayer *layer = (SGHelpLayer *)mainScene->getChildByTag(sg_helpLayer);
    if (!layer)
    {
        layer = SGHelpLayer::create();
    }
    mainScene->addShowLayer(layer,isBeforeDel);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());
}
void SGMainManager::showAllCards(CCArray *dict)
{
    SGAllCardsLayer *layer = (SGAllCardsLayer *)mainScene->getChildByTag(sg_allCardsLayer);
    if (!layer) {
        layer = SGAllCardsLayer::create(dict);
    }
    mainScene->addShowLayer(layer);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}
void SGMainManager::showSetLayer()
{
    SGSetLayer *layer = (SGSetLayer *)mainScene->getChildByTag(sg_setLayer);
    if (!layer) {
        layer = SGSetLayer::create();
    }
    mainScene->addShowLayer(layer);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}
void SGMainManager::showHelpBook()
{
    SGHelpBook *layer = (SGHelpBook *)mainScene->getChildByTag(sg_helpBook);
    if (!layer) {
        layer = SGHelpBook::create();
    }
    mainScene->addShowLayer(layer);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
}

void SGMainManager::showcomplainlayer()
{
    SGComplainLayer *layer = (SGComplainLayer *)mainScene->getChildByTag(sg_complain);
    if (!layer) {
        layer = SGComplainLayer::create();
    }
    mainScene->addShowLayer(layer);
    
}
void SGMainManager::showliuyanlayer()
{
    SGLiuyanLayer *layer = (SGLiuyanLayer *)mainScene->getChildByTag(sg_liuyan);
    if (!layer) {
        layer = SGLiuyanLayer::create();
    }
    layer->setZOrder(-10);
    mainScene->addShowLayer(layer);
    
    
}




void SGMainManager::showVisitLayer(int visitHuo,int a,int b,int c,int d,int j,std::vector<VisitActivity>& visitActivityList,const char*lotteryName, const char * activityLotteryName)
{
    SGVisitLayer *layer = (SGVisitLayer *)mainScene->getChildByTag(sg_visitLayer);
    if (!layer)
    {
        layer = SGVisitLayer::create(visitHuo,a,b,c,d,j,visitActivityList,lotteryName,activityLotteryName);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->visithide();
    mainLayer->setPortraitMenuCanTouch(false);
    mainScene->addShowLayer(layer);
    
    SGGuideManager::shareManager()->checkIsDone(guide_tag_32);
}

void SGMainManager::showPlayerInfo()
{
    SGPlayerInfoLayer *layer = (SGPlayerInfoLayer *)mainScene->getChildByTag(sg_playerInfoLayer);
    if (!layer) {
        layer = SGPlayerInfoLayer::create();
    }
    getMainLayer()->setPortraitMenuCanTouch(false);
    mainScene->addShowLayer(layer);
    
}

void SGMainManager::showInviteNumLayer()
{
    SGInviteNum *layer = (SGInviteNum *)mainScene->getChildByTag(sg_inviteNum);
    if (!layer) {
        layer = SGInviteNum::create();
    }
    mainScene->addShowLayer(layer);
}
void SGMainManager::showVisitCards(CCArray *array,CCArray *huoArray,SGBaseMilitaryCard *comboCard,SGBaseMilitaryCard *progressCard,bool useGoldDoor,bool isBeforeDel, SGPropsCard *buyPropCard)
{
    SGVisitShowCardLayer *Layer = (SGVisitShowCardLayer *)mainScene->getChildByTag(sg_visitShowCard);
    if (!Layer)
    {
        Layer = SGVisitShowCardLayer::create(array,huoArray,comboCard,progressCard,useGoldDoor, buyPropCard);
    }
    mainScene->addShowLayer(Layer,true);
    
}
void SGMainManager::showRewardLayer(SGBoxDelegate *dg)
{
    SGFirstLayer *baseLayer = (SGFirstLayer*)mainScene->getChildByTag(sg_firstLayer);
    if (baseLayer) {
        baseLayer->setIsCanTouch(false);
    }
    
    SGLandingReward *layer = (SGLandingReward *)mainScene->getChildByTag(sg_landingRewardLayer);
    if (!layer)
    {
        layer = SGLandingReward::create(dg);
    }
    mainScene->addChild(layer,10);
}

void SGMainManager::showTotalRewardLayer(main::LoginAwardListResponse &response,SGBoxDelegate *dg){
    
    SGTotalLoginRewardLayer *layer=(SGTotalLoginRewardLayer *)mainScene->getChildByTag(sg_totalLoginReward);
    if (!layer) {
        layer=SGTotalLoginRewardLayer::create(response,dg);
        layer->setTag(sg_totalLoginReward);
    }
    
    mainScene->addShowLayer(layer,true);
    //    mainScene->addChild(layer, 1);
    SGFirstLayer *baseLayer = (SGFirstLayer*)mainScene->getChildByTag(sg_firstLayer);
    if (baseLayer) {
        baseLayer->setIsCanTouch(false);
    }
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(false);
    }
    SGBaseLayer *baseLayer1 = (SGBaseLayer*)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (baseLayer1) {
        //        baseLayer1->setIsCanTouch(false);
        
    }
    
}

void SGMainManager::showCardsInfo(CCArray *array,int type,bool isBeforeDel)
{
    SGCardsLayer *layer = (SGCardsLayer *)mainScene->getChildByTag(sg_cardsLayer);
    if (layer)
    {
        layer->removeFromParent();
        layer = NULL;
    }
    if (!layer) {
        layer = SGCardsLayer::create(array,type);
    }
    mainScene->addShowLayer(layer,isBeforeDel);
    
}
void SGMainManager::updateEmbattleInfo()
{
    SGEmbattleLayer *Layer = (SGEmbattleLayer *)mainScene->getChildByTag(sg_embattleLayer);
    if (Layer)
    {
        Layer->updateEmbattle();
    }
}
void SGMainManager::showCountriesLayer(int type,bool isBeforeDel,int country)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setPortraitMenuCanTouch(false);
    SGCountrysInfo *layer = (SGCountrysInfo *)mainScene->getChildByTag(sg_countrysInfo);
    if (!layer) {
        layer = SGCountrysInfo::create(type,country);
    }
    mainScene->addShowLayer(layer,isBeforeDel);
}
void SGMainManager::showRewardsLayer(bool willReload)
{
    SGRewardLayer *layer = (SGRewardLayer *)mainScene->getChildByTag(sg_rewardsLayer);
    if (!layer)
    {
        layer = SGRewardLayer::create();
    }
    mainScene->addShowLayer(layer);
    if (willReload && layer != NULL)
    {
        layer->hideInviteeItem();
    }
}
void SGMainManager::showLvlUpGetGold(CCArray *array)
{
    SGLevelUpGetGold *layer = (SGLevelUpGetGold *)mainScene->getChildByTag(sg_levelUpGetGold);
    if (!layer) {
        layer = SGLevelUpGetGold::create(array);
    }
    mainScene->addShowLayer(layer);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());////1126效果优化
}
void SGMainManager::showSalaryLayer(int a,int b,int c,int d,int e,int f,int g)
{
    SGGetSalaryLayer *layer = (SGGetSalaryLayer *)mainScene->getChildByTag(sg_getsalaryLayer);
    if (!layer) {
        layer = SGGetSalaryLayer::create(a,b,c,d,e,f,g);
    }
    mainScene->addShowLayer(layer);
}
void SGMainManager::showJitianLayer(int current, int max,int cando,CCArray*array)
{
    SGJitianLayer *layer = (SGJitianLayer *)mainScene->getChildByTag(sg_jitianLayer);
    if (!layer) {
        layer = SGJitianLayer::create(current,max,cando,array);
    }
    mainScene->addShowLayer(layer);
}
void SGMainManager::showLiandanLayer(int time,int index)
{
    SGLiandanLayer *layer = (SGLiandanLayer *)mainScene->getChildByTag(sg_liandanLayer);
    if (!layer) {
        layer = SGLiandanLayer::create(time,index);
    }
    mainScene->addShowLayer(layer);
}
void SGMainManager::showDrinkWineLayer(int power,int iscan)
{
    SGDrinkWineLayer *layer = (SGDrinkWineLayer *)mainScene->getChildByTag(sg_drinkWineLayer);
    if (!layer) {
        layer = SGDrinkWineLayer::create(power,iscan);
    }
    mainScene->addShowLayer(layer);
}
void SGMainManager::showTaskLayer(CCArray *array)
{
    if (SGGuideManager::shareManager()->getGuideLayer()!=NULL) {
        SGGuideManager::shareManager()->getGuideLayer()->removeHighLightRect();
    }
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setPortraitMenuCanTouch(false);
    SGTaskLayer *layer = (SGTaskLayer *)mainScene->getChildByTag(sg_taskLayer);
    if (!layer) {
        layer = SGTaskLayer::create(array);
    }
    mainScene->addShowLayer(layer);
    
    SGGuideManager::shareManager()->checkIsDone(guide_tag_45);
    layer->showFirstTask();
    
}

////kanata
void SGMainManager::showAllTaskLayer(CCArray *array)
{
    if (SGGuideManager::shareManager()->getGuideLayer()!=NULL) {
        SGGuideManager::shareManager()->getGuideLayer()->removeHighLightRect();
    }
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setPortraitMenuCanTouch(false);
    SGAllTaskLayer *layer = (SGAllTaskLayer *)mainScene->getChildByTag(sg_alltasklayer);
    if (!layer) {
        layer = SGAllTaskLayer::create(array);
    }
    mainScene->addShowLayer(layer);
    layer->firstShow();
    
    SGGuideManager::shareManager()->checkIsDone(guide_tag_45);
}








////
void SGMainManager::showRegLayer(int chance, int time, int cost,CCArray *bcsArr)
{
    SGRewardRegLayer *layer = (SGRewardRegLayer *)mainScene->getChildByTag(sg_regLotteryLayer);
    if (!layer) {
        layer = SGRewardRegLayer::create(chance, time, cost,bcsArr);
    }
    mainScene->addShowLayer(layer);
}

void SGMainManager::showBindLayer(CCArray *array,int type,CCArray *array2,int type2,const char * mail)
{
    SGRewardBindLayer *layer = (SGRewardBindLayer *)mainScene->getChildByTag(sg_bindLayer);
    if (layer)
    {
        layer->removeFromParent();
        layer = NULL;
    }
    if (!layer) {
        layer = SGRewardBindLayer::create(array,type,array2,type2,mail);
    }
    mainScene->addShowLayer(layer);
}

void SGMainManager::showInviterLayer(CCString* inviteCode,int inviteNum,int inviteMax,CCArray *array)
{
    SGInviterLayer *layer = (SGInviterLayer *)mainScene->getChildByTag(sg_inviterLayer);
    if (!layer) {
        layer = SGInviterLayer::create(inviteCode,inviteNum,inviteMax,array);
    }
    mainScene->addShowLayer(layer);
    
}
void SGMainManager::showInviteeLayer()
{
    SGInviteeLayer *layer = (SGInviteeLayer *)mainScene->getChildByTag(sg_inviteeLayer);
    if (!layer) {
        layer = SGInviteeLayer::create();
    }
    mainScene->addShowLayer(layer);
}
void SGMainManager::showYueKaRewardLayer(CCArray *array)
{
    SGRewardYueKaLayer *layer = (SGRewardYueKaLayer *)mainScene->getChildByTag(sg_yuekaRewardLayer);
    if (!layer) {
        layer = SGRewardYueKaLayer::create(array);
    }
    mainScene->addShowLayer(layer);
}

//mainscene
SGMainScene* SGMainManager::getMainScene()
{
    return mainScene;
}

TouchGroup*  SGMainManager::getTouchGroup()
{
    return mainScene->m_pUIGroup;
}

SGMainLayer* SGMainManager::getMainLayer()
{
    SGMainLayer *layer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    return layer;
}

void SGMainManager::showTestLayer(cocos2d::CCLayer *layer)
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer)
    {
        mainLayer->setIsCanTouch(false);
        mainLayer->setVisible(false);
    }
    mainScene->addChild(layer, TEST_HERO_LAYER_TAG, TEST_HERO_LAYER_TAG);
}

void SGMainManager::showAdvanceTexiao(int itemid,SGBaseMilitaryCard *card_ , int type,int star,bool isfromfirst)
{
    //    SGAdvanceFinishLayer *layer = (SGAdvanceFinishLayer *)mainScene->getChildByTag(sg_advancetexiao);
    //    if (!layer) {
    //        layer = SGAdvanceFinishLayer::create(itemid,card_,type,isfromfirst);
    //    }
    //    mainScene->addShowLayer(layer);
    SGAdvancingLayer *layer = (SGAdvancingLayer *)mainScene->getChildByTag(sg_advancetexiao);
    if (!layer) {
        layer = SGAdvancingLayer::create(itemid,card_,type,star,isfromfirst);
    }
    
    EFFECT_PLAY(MUSIC_35);
    
    mainScene->addShowLayer(layer);
    
}

void SGMainManager::showChooseSkillLayer()
{
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->showblack();
    mainLayer->setPortraitMenuCanTouch(false);
    
    SGChooseSkillCardLayer *layer = SGChooseSkillCardLayer::create();
    mainScene->addShowLayer(layer);
}
void SGMainManager::showloginLayer()
{
    //    mainScene->removeNowLayer(sg_loginLayer);
    SGServerListLoginGame *layer = (SGServerListLoginGame *)mainScene->getChildByTag(sg_listLoginLayer);
    if (!layer) {
        layer = SGServerListLoginGame::create();
    }
    mainScene->addShowLayer(layer);
}

//发送获取守荆州详情。
void SGMainManager::sendGetSjzInfoRequest(bool shallGoToSoulMall)
{
    _shallSjzGoToSoulMall = shallGoToSoulMall;
    //发送获取详情
    main::SurvivalMainInfoRequest* req = new main::SurvivalMainInfoRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_SURVIVAL_MAIN_INFO, req);
}

void SGMainManager::sjzInfoResponse(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::SurvivalMainInfoResponse *rsp = (main::SurvivalMainInfoResponse *) sr->m_msg;
        if (rsp)
        {
            if (rsp->state() == 1) //如果有效，开启守荆州UI
            {
                //解析荆州详情数据
                SGSjzData* data = SGPlayerInfo::sharePlayerInfo()->getSjzData();
                data->setLevelCur(rsp->levels());
                data->setCountCur(rsp->fightcount());
                data->setCountMax(rsp->maxfightcount());
                data->setRankCur(rsp->rank());
                data->setDamageMax(rsp->maxdamage());
                data->setVIPCountMax(rsp->maxcanbuycount());
                data->setVIPCountCur(rsp->buycount());
                data->setVIPGold(rsp->price());
                
                if (_shallSjzGoToSoulMall) //去商店
                {
                    SGMainLayer* layer = this->getMainLayer();
                    if (layer)
                    {
                        layer->sjzSoulExchangeRequest(2);
                    }
                }
                else //去首页
                {
                    this->showshoujingzhou();
                }
            }
            else
            {
                SGMainManager::shareMain()->showMessage(str_FirstLayer_str14);
            }
        }
    }
}

void SGMainManager::showshoujingzhou()  //1001守荆州
{
    int lavel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    if (lavel < SGPlayerInfo::sharePlayerInfo()->getLimitLevelById(limitShouJinZhou))
    {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MainLayer_str604,SGPlayerInfo::sharePlayerInfo()->getLimitLevelById(limitShouJinZhou))->getCString());
        return;
    }
    
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->setVisible(true);
    mainLayer->setPortraitMenuCanTouch(true);
    mainLayer->setIsCanTouch(true);
    SGShouJingZhou *layer = (SGShouJingZhou *)mainScene->getChildByTag(sg_shoujingzhou);
    
    if (!layer)
    {
        layer = SGShouJingZhou::create();
    }
    mainScene->addShowLayer(layer);
}


//void SGMainManager::showsjzrank()
//{
//    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
//    mainLayer->setPortraitMenuCanTouch(false);
//    SGShouJingZhouRank *layer = (SGShouJingZhouRank *)mainScene->getChildByTag(sg_sjzrank);
//
//    if (!layer) {
//        layer = SGShouJingZhouRank::create();
//    }
//    mainScene->addShowLayer(layer);
//
//}

void SGMainManager::showsjzrank(CCArray *data, int pageItemsNum, int pageMax, int nowRank, int coins, int rewardNum, int time)
{
    SGMainLayer *mainlayer = (SGMainLayer*)mainScene->getChildByTag(sg_mainLayer);
    
    mainlayer->setPortraitMenuCanTouch(false);
    SGShouJingZhouRank *layer = (SGShouJingZhouRank *)mainScene->getChildByTag(sg_sjzrank);
    if (!layer) {
        layer = SGShouJingZhouRank::create(data, pageItemsNum, pageMax, nowRank, coins, rewardNum, time);
    }
    //layer->setSelecIndex(index);
    mainScene->addShowLayer(layer);
}

void SGMainManager::showwelcomelayer(bool conn)
{
    SGWelComeLayer *layer = (SGWelComeLayer *)mainScene->getChildByTag(sg_welComeLayer);
    if (!layer) {
        layer = SGWelComeLayer::create();
        layer->loginstart();
        mainScene->addShowLayer(layer);
    }else{
        layer->setVisible(true);
        layer->setIsCanTouch(true);
    }
    if (conn) {
        layer->fastStartHandler();
    }
    
}
void SGMainManager::showregisterLayer(int type,bool beforeDel)
{
    SGRegisterLayer *layer = (SGRegisterLayer *)mainScene->getChildByTag(sg_registerLayer);
    if (!layer) {
        layer = SGRegisterLayer::create(type);
    }
    mainScene->addShowLayer(layer,beforeDel);
}
void SGMainManager::showChangePassWord()
{
    SGChangePassWordLayer *layer = (SGChangePassWordLayer *)mainScene->getChildByTag(sg_changepassword);
    if (!layer) {
        layer = SGChangePassWordLayer::create();
    }
    mainScene->addShowLayer(layer);
    
}

void SGMainManager::showTongQueLayer(int spendMedal,int spendGold,int medal,int spendCoins,int refreshTime,int blessValue, std::vector<SGItemInfo> &tongqueItem, SGItemInfo &yangxing)
{
    SGTongQueLayer *layer = (SGTongQueLayer *)mainScene->getChildByTag(sg_tongQueLayer);
    if (!layer) {
        layer = SGTongQueLayer::create(spendMedal,spendGold, medal, spendCoins, refreshTime, blessValue,tongqueItem,yangxing);
    }
    mainScene->addShowLayer(layer);
    getMainLayer()->setPortraitMenuCanTouch(false);
    
}

void SGMainManager::showPveFinish()
{
    if (SGPlayerInfo::sharePlayerInfo()->getHasPveFight())
    {
        mainScene->backFight();
        
    }
}

//加载特殊战斗节列表中的底图
void SGMainManager::addSpHeadBgPlistByNum(int numId, LayerTag lTag)
{
    if (numId < 0)
    {
        return ;
    }
    
    if (numId > 18)
    {
        ResourceManager::sharedInstance()->bindTexture("sgspecialbattle/sphead19-24.plist", RES_TYPE_LAYER_UI, lTag);
    }
    else if (numId > 12)
    {
        ResourceManager::sharedInstance()->bindTexture("sgspecialbattle/sphead13-18.plist", RES_TYPE_LAYER_UI, lTag);
    }
    else if (numId > 6)
    {
        ResourceManager::sharedInstance()->bindTexture("sgspecialbattle/sphead7-12.plist", RES_TYPE_LAYER_UI, lTag);
    }
    else if (numId >= 1)
    {
        ResourceManager::sharedInstance()->bindTexture("sgspecialbattle/sphead1-6.plist", RES_TYPE_LAYER_UI, lTag);
    }
    
}

//加载章节列表中上部banner的底畋
void SGMainManager::addSpTopHeadBgPlistByNum(int numId, LayerTag lTag)
{
    if (numId < 0)
    {
        return ;
    }
    if (numId >= 1)
    {
        ResourceManager::sharedInstance()->bindTexture("sgspecialbattle/sptophead1-8.plist", RES_TYPE_LAYER_UI, lTag);
    }
}


//所有icon资源加载，以ID 为索引，每100张图像在一个plist中 evan 20140825
void SGMainManager::addHeadIconPlistByNum(int numId,LayerTag layerTag)
{
    if (numId == -1) {
        return;
    }
    int minID = ((int)((numId-1)/100)) * 100+1;
    int maxId = minID+99;
    CCString * skillName = CCString::createWithFormat("headIcon/head%d-%d.plist",minID,maxId);
    ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
    
}
//武将大图资源加载，以ID为索引，从1756开始，每5张图像在一个plist中，1756之前的分组沿用以前不规范的区间规则 evan 20140825
void SGMainManager::addOfficerPng(int numId,LayerTag layerTag)
{
    if (numId < 0) {
        return;
    }
    if (numId > 1760) {
        int minID = ((int)((numId-1)/5)) * 5+1;
        int maxId = minID+4;
        CCString * skillName = CCString::createWithFormat("officerIcon/officerIcon%d-%d.plist",minID,maxId);
        ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
    }
    if (numId > 1755) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1756-1760.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1732) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1736-1755.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1344) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1401-1411.plist", RES_TYPE_LAYER_UI, layerTag);
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1345-1732.plist", RES_TYPE_LAYER_UI, layerTag);
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1412.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1165) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1166-1344.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1121) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1122-1158.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1119) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1120-1121.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1110) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1111-1119.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1074) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1075-1110.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1058) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1059-1074.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1047) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1048-1058.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1036) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1037-1047.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1026) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1027-1036.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1016) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1017-1026.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1013) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1014-1016.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 1007) {
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1008-1013.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else{
        ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1001-1007.plist", RES_TYPE_LAYER_UI, layerTag);
    }
}
//装备大图资源加载，以ID（4001开始）为索引，每10张图像在一个plist中 evan 20140825
void SGMainManager::addEquipPng(int numId,LayerTag layerTag)
{
    int minID = ((int)((numId-1)/10)) * 10+1;
    int maxId = minID+9;
    CCString * skillName = CCString::createWithFormat("equip/equip%d-%d.plist",minID,maxId);
    ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
}

//道具大图资源加载，以ID为索引，每20张图像在一个plist中 evan 20140825
void SGMainManager::addPropPng(int numId,LayerTag layerTag)
{
    int minID = ((int)((numId-1)/20)) * 20+1;
    int maxId = minID+19;
    CCString * skillName = CCString::createWithFormat("prop/prop%d-%d.plist",minID,maxId);
    ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
}

//VShead（战斗开始页面）武将头像资源加载，以ID为索引，每20张图像在一个plist中 evan 20140825
void SGMainManager::addVSHeadPlistByNum(int numId,LayerTag layerTag)
{
    int minID = ((int)((numId-1)/20)) * 20+1;
    int maxId = minID+19;
    CCString * skillName = CCString::createWithFormat("skillHead/skillHead%d-%d.plist",minID,maxId);
    ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
}

////流光（战斗中cut in头像）头像资源加载，以ID为索引，每5张图像在一个plist中,170之前的部分沿用以前的不规则区间 evan 20140825
//void SGMainManager::addSkillPlistByNum(int numId, LayerTag layerTag){
//    if(numId >170){
//        int minID = ((int)((numId-1)/5)) * 5+1;
//        int maxId = minID+4;
//        CCString * skillName = CCString::createWithFormat("skill_head/skill_head%d-%d.plist",minID,maxId);
//        ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 167)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head168-170.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 157)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head158-167.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 149)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head150-157.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 134)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head135-149.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 113)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head114-134.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 102)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head103-113.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 95)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head96-102.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 89)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head90-95.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 84)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head85-89.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 51)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head52-84.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 39)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head40-51.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 30)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head31-39.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 20)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head21-30.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 11)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head12-20.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 5)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head6-11.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else if (numId > 0)
//    {
//        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head1-5.plist", RES_TYPE_LAYER_UI, layerTag);
//    }
//    else
//    {
//        int minID = ((int)(numId/10)) * 10;
//        int maxId = minID+9;
//        CCString * skillName = CCString::createWithFormat("skill_head/skill_head%d-%d.plist",minID,maxId);
//        ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
//    }
//    
//}

//modified by cgp
//流光（战斗中cut in头像）头像资源加载，以ID为索引，每5张图像在一个plist中,170之前的部分沿用以前的不规则区间 evan 20140825
void SGMainManager::addSkillPlistByNum(int numId, LayerTag layerTag){
    if(numId >170){
        int minID = ((int)((numId-1)/5)) * 5+1;
        int maxId = minID+4;
        CCString * skillName = CCString::createWithFormat("skill_head/skill_head%d-%d.plist",minID,maxId);
        ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 167)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head168-170.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 157)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head158-167.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 149)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head150-157.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 134)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head135-149.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 113)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head114-134.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 102)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head103-113.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 95)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head96-102.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 89)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head90-95.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 84)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head85-89.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 51)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head52-84.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 39)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head40-51.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 30)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head31-39.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 20)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head21-30.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 11)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head12-20.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 5)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head6-11.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else if (numId > 0)
    {
        ResourceManager::sharedInstance()->bindTexture("skill_head/skill_head1-5.plist", RES_TYPE_LAYER_UI, layerTag);
    }
    else
    {
        int minID = ((int)(numId/10)) * 10;
        int maxId = minID+9;
        CCString * skillName = CCString::createWithFormat("skill_head/skill_head%d-%d.plist",minID,maxId);
        ResourceManager::sharedInstance()->bindTexture(skillName->getCString(), RES_TYPE_LAYER_UI, layerTag);
    }
    
}




//FIXME: 刷新详情界面的六大属性，暂时使用，杯具。
void SGMainManager::refreshGilOfficerInfo()
{
    SGGeneralInfoLayer *generlMsgLayer = (SGGeneralInfoLayer *)mainScene->getChildByTag(sg_generalInfoLayer);
    if (generlMsgLayer) {
        generlMsgLayer->renewSimpleInfo();
    }
}

//显示军魂换礼界面。
void SGMainManager::showSoulExchangeLayer(int et)
{
    
    SGSoulExchangeLayer* sgsel = (SGSoulExchangeLayer *)mainScene->getChildByTag(sg_soulExchangeLayer);
    if (!sgsel)
        sgsel = SGSoulExchangeLayer::create(et);
    //上场
    SGMainLayer* mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainLayer->hideblack();
    mainLayer->setIsCanTouch(false);
    mainLayer->setPortraitMenuCanTouch(false);
    
    bool del;
    if (1 == sgsel->getEnterType()) //守荆州UI过来，不销毁。
        del = false;
    else if (2 == sgsel->getEnterType()) //战败UI过来，销毁。
        del = true;
    else //其他情况
        del = true;
    mainScene->addShowLayer(sgsel,del);
}

/*
 id SGMainManager::getSdkdelegate()
 {
 return _mSdkReqCallBack;
 }
 */

void SGMainManager::doRegist(int state,std::string& accountId,const char* loginKey)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("userId",accountId);
    CCUserDefault::sharedUserDefault()->setStringForKey("loginKey",std::string(loginKey));
    if (mainScene->getChildByTag(sg_registerLayer))
    {
        ((SGRegisterLayer *)mainScene->getChildByTag(sg_registerLayer))->requestFinished(MSG_HTTP_REGISTER, CCString::createWithFormat("%d",state));
    }
    else if (mainScene->getChildByTag(sg_registerBox))
    {
        ((SGRegisterBox *)mainScene->getChildByTag(sg_registerBox))->requestFinished(MSG_HTTP_REGISTER, CCString::createWithFormat("%d",state));
    }
    else if (mainScene->getChildByTag(sg_welComeLayer))
    {
        ((SGWelComeLayer *)mainScene->getChildByTag(sg_welComeLayer))->clearGust();
    }
}


void SGMainManager::dealFbBindFail()
{
    if (mainScene->getChildByTag(sg_welComeLayer))
    {
        ((SGWelComeLayer *)mainScene->getChildByTag(sg_welComeLayer))->doFbBindFail();
    }
}

void SGMainManager::doLogin(std::string& accountId,const char* loginKey, int channelId, int subChannelId, const char *nickName)
{
	if (isFbAutologin)
    {
         CCUserDefault::sharedUserDefault()->setStringForKey("FBaccountId",accountId);
    }
    else
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("userId",accountId);
    }
    
    //CCUserDefault::sharedUserDefault()->setStringForKey("userId",accountId);
    
    CCUserDefault::sharedUserDefault()->setStringForKey("loginKey",std::string(loginKey));
	CCUserDefault::sharedUserDefault()->setIntegerForKey("channelId",channelId);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("subChannelId",subChannelId);
	CCUserDefault::sharedUserDefault()->setStringForKey("nickName",std::string(nickName));
    

	//CCUserDefault::sharedUserDefault()->flush();
    
    if (mainScene->getChildByTag(sg_welComeLayer))
    {
        ((SGWelComeLayer *)mainScene->getChildByTag(sg_welComeLayer))->requestFinished(MSG_HTTP_LOGIN,NULL);
        
    }
    else if (mainScene->getChildByTag(sg_serverListNewServerBox))
    {
        ((SGServerListNewServerBox *)mainScene->getChildByTag(sg_serverListNewServerBox))->requestFinished(MSG_HTTP_LOGIN,NULL);
    }
    else if (mainScene->getChildByTag(sg_listLoginLayer))
    {
        ((SGServerListLoginGame *)mainScene->getChildByTag(sg_listLoginLayer))->requestFinished(MSG_HTTP_LOGIN,NULL);
    }
    else if (mainScene->getChildByTag(sg_helpLayer))
    {
        ((SGHelpLayer*)mainScene->getChildByTag(sg_helpLayer))->requestFinished(MSG_HTTP_LOGIN,NULL);
    }
    else if (mainScene->getChildByTag(sg_mainLayer))
    {
        ((SGMainLayer *)mainScene->getChildByTag(sg_mainLayer))->requestFinished(MSG_HTTP_LOGIN,NULL);
    }
    
}
void SGMainManager::doFastLogin(std::string& accountId,const char* loginKey)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("userId",accountId);
    CCUserDefault::sharedUserDefault()->setStringForKey("loginKey",std::string(loginKey));
    if (mainScene->getChildByTag(sg_welComeLayer))
    {
        ((SGWelComeLayer *)mainScene->getChildByTag(sg_welComeLayer))->requestFinished(MSG_HTTP_FAST_LOGIN,NULL);
    }
    else if (mainScene->getChildByTag(sg_listLoginLayer))
    {
        ((SGServerListLoginGame *)mainScene->getChildByTag(sg_listLoginLayer))->requestFinished(MSG_HTTP_FAST_LOGIN,NULL);
    }
}
void SGMainManager::doPassword(int state,int flag)
{
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::createWithFormat("%d",state), "isSccess");
    dict->setObject(CCString::createWithFormat("%d",flag), "flag");
    
    if (mainScene->getChildByTag(sg_changepassword))
    {
        ((SGChangePassWordLayer *)mainScene->getChildByTag(sg_changepassword))->requestFinished(MSG_HTTP_PASSWORD,dict);
    }
    else if (mainScene->getChildByTag(sg_modifypassword))
    {
        ((SGModificationPassword *)mainScene->getChildByTag(sg_modifypassword))->requestFinished(MSG_HTTP_PASSWORD,dict);
    }
}

void SGMainManager::trackLevel(int level)
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->setSdkReqLevel(level);
#else
	//android code
	AndroidSDKAdp::getInstance()->setSdkReqLevel(level);
#endif
    //    [_mSdkReqCallBack setLevel:level];
}

void SGMainManager::trackReward(long moneyNum,CCString *reason)
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->trackSDKReward(moneyNum, reason->getCString());
#else
	//android code
	AndroidSDKAdp::getInstance()->trackSDKReward(moneyNum, reason->getCString());
#endif
    //[SdkReqCallBack onReward:moneyNum reason:[NSString stringWithUTF8String:reason->getCString()]];
}

void SGMainManager::trackPurchase(CCString *item,int num ,long price)
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->trackSDKPurchase(item->getCString(), num, price);
#else
	//android code
	AndroidSDKAdp::getInstance()->trackSDKPurchase(item->getCString(), num, price);
#endif
    //    [SdkReqCallBack onPurchase:[NSString stringWithUTF8String:item->getCString()] itemNumber:num
    //                    priceInVirtualCurrency:price ];
}

void SGMainManager::trackItemUse(CCString *item,int num )
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->trackSDKItemUse(item->getCString(), num);
#else
	//android code
	AndroidSDKAdp::getInstance()->trackSDKItemUse(item->getCString(), num);
#endif
    //    [SdkReqCallBack onUse:[NSString stringWithUTF8String:item->getCString()] itemNumber:num ];
}

void SGMainManager::trackMsnBegin(int sceneId)
{
    SGStoryBean *storyBean = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(sceneId,m_trackIsPve);
    CCString *stroyInfo = NULL;
    if (storyBean == NULL) {
        stroyInfo = CCString::createWithFormat("%d",sceneId);
    }
    else
    {
        stroyInfo = CCString::createWithFormat("%d%s",sceneId,storyBean->getSceneBeanById(sceneId)->getSceneName()->getCString());
    }
#if (PLATFORM == IOS)
	OcCaller::getInstance()->trackSDKMsnBegin(stroyInfo->getCString());
#else
	//android code
	AndroidSDKAdp::getInstance()->trackSDKMsnBegin(stroyInfo->getCString());
#endif
	
    //    [SdkReqCallBack onBegin:[NSString stringWithUTF8String:stroyInfo->getCString()]];
}

void SGMainManager::trackMsnCompleted(int sceneId,bool isSpot)
{
    SGStoryBean *storyBean = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(sceneId,isSpot);
    CCString *stroyInfo = NULL;
    if (storyBean == NULL) {
        stroyInfo = CCString::createWithFormat("%d",sceneId);
    }
    else
    {
        stroyInfo = CCString::createWithFormat("%d%s",sceneId,storyBean->getSceneBeanById(sceneId)->getSceneName()->getCString());
    }
#if (PLATFORM == IOS)
	OcCaller::getInstance()->trackSDKMsnCompleted(stroyInfo->getCString());
#else
	//android code
	AndroidSDKAdp::getInstance()->trackSDKMsnCompleted(stroyInfo->getCString());
#endif
    //    [SdkReqCallBack onCompleted:[NSString stringWithUTF8String:stroyInfo->getCString()]];
}

void SGMainManager::trackMsnFailed(int sceneId ,CCString *cause)
{
    SGStoryBean *storyBean = SGStaticDataManager::shareStatic()->getStoryDataByPlotId(sceneId,m_trackIsPve);
    CCString *stroyInfo = NULL;
    if (storyBean == NULL) {
        stroyInfo = CCString::createWithFormat("%d",sceneId);
    }
    else
    {
        stroyInfo = CCString::createWithFormat("%d%s",sceneId,storyBean->getSceneBeanById(sceneId)->getSceneName()->getCString());
    }
#if (PLATFORM == IOS)
	OcCaller::getInstance()->trackSDKMsnFailed(stroyInfo->getCString(), cause->getCString());
#else
	//android code
	AndroidSDKAdp::getInstance()->trackSDKMsnFailed(stroyInfo->getCString(), cause->getCString());
    
#endif
	
    //    [SdkReqCallBack onFailed:[NSString stringWithUTF8String:stroyInfo->getCString()]
    //                       failedCause:[NSString stringWithUTF8String:cause->getCString()]];
}

void SGMainManager::trackTaskBegin(CCString *taskName)
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->trackSDKTaskBegin(taskName->getCString());
#else
	//android code
	AndroidSDKAdp::getInstance()->trackSDKTaskBegin(taskName->getCString());
#endif
    //   [SdkReqCallBack onBegin:[NSString stringWithUTF8String:taskName->getCString()]];
}

void SGMainManager::trackTaskCompleted(CCString *taskName)
{
#if (PLATFORM == IOS)
	OcCaller::getInstance()->trackSDKTaskCompleted(taskName->getCString());
#else
	//android code
	AndroidSDKAdp::getInstance()->trackSDKTaskCompleted(taskName->getCString());
#endif
    //    [SdkReqCallBack onCompleted:[NSString stringWithUTF8String:taskName->getCString()]];
}

void SGMainManager::showExchangeCodeLayer(CCString *desprition){
    
    SGExchangeCodeLayer *exchangeCodelayer=(SGExchangeCodeLayer *)mainScene->getChildByTag(sg_exchangeCodeLayer);
    if (!exchangeCodelayer) {
        exchangeCodelayer=SGExchangeCodeLayer::create(desprition);
    }
    SGMainManager::shareMain()->showLayer(exchangeCodelayer);
}

// android 调用这个方法像服务器发送充值成功消息。bId为订单ID，bNum为充值金额。
void SGMainManager::chargeSuccessForAndroid(const char* bId, int bNum)
{
    CCLOG("chargeSuccessForAndroid || bId=%s, bNum=%d", bId, bNum);
    //MMDEBUG: CHARGE 安卓当前由对方billing server转发消息到我方server，故这里不再处理。
#if PLATFORM == ANDROID
    //先构造一次，否则android会莫名丢掉bId的内容
    std::string tmp(bId);
    AndroidSDKAdp::getInstance()->onChargeSuccessSDK(tmp, 0, 0);
#endif
    
}

//购买成功向服务器发送消息
void SGMainManager::buySuccessAndSendMsg(const char *order, int channelId, int isApp)
{
	int serverId = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
	int userID = SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId();
	
	main::ShopChargeRequest *request = new main::ShopChargeRequest;
    request->set_gameid(1);
    request->set_areaid(serverId);
    request->set_accountid(userID);
    request->set_roleid(SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId());
    request->set_receipt(order);
	request->set_channelid(channelId);
	request->set_isapp(isApp);
    SGSocketClient::sharedSocketClient()->send(MSG_STORE_BUY, request);
}
//新手引导玩家进度消息发送
void SGMainManager::sendNewPlayerProcess(GUIDE_STEP step, int ext)
{
	//向服务器发送新手玩家到哪一步
//	CCLOG("send process msg to server");
//	main::NewComerRecordRequest *ncrp = new main::NewComerRecordRequest();
//	ncrp->set_recordtypeid(step);
//	ncrp->set_mac(std::string(GameConfig::getUdit()) );
//	
//	SGSocketClient::sharedSocketClient()->send(MSG_GUIDE_STEP, ncrp,false);
	
}

//打开聊天展示页面
void SGMainManager::openChatShowLayerByType(SGBaseMilitaryCard *showChatItem,int showType,int nowLayersTag)
{
    switch (showType) {
        case 1:
        {
            //武将
            showGeneralInfoLayer((SGOfficerCard*)showChatItem, CHATSHOWENTRYTAG,NULL,false);
        }
            break;
        case 2:
        {
            //装备
            showEquipInfoLayer((SGEquipCard*)showChatItem, CHATSHOWENTRYTAG,false);
        }
            break;
        case 3:
        {
            //士兵
            showSoldierInfoLayer((SGSoldierCard*)showChatItem, CHATSHOWENTRYTAG,false);
        }
            break;
        case 4:
        {
            //杂物
            if (((SGPropsCard *)showChatItem)->getType() == 3) {
                //军魂卡
                showSoulCardInfoLayer((SGPropsCard *)showChatItem,CHATSHOWENTRYTAG,false);
            }
            else if (((SGPropsCard *)showChatItem)->getType() == 4 ||((SGPropsCard *)showChatItem)->getType() == 5)
            {
                //技能卡
                showSkillCardInfoLayer((SGPropsCard *)showChatItem,CHATSHOWENTRYTAG,false);
            }
            else
            {
                showPropInfoLayer((SGPropsCard *)showChatItem, CHATSHOWENTRYTAG,false);
            }
            
        }
            break;
        case 5:
        {
            showPiecesInfoLayer((SGPiecesCard *)showChatItem, CHATSHOWENTRYTAG, false);
            //碎片
        }
            break;
		case 6:
        {
            //消耗品
			this->showConsumableInfoLayer((SGConsumableCard *)showChatItem, false,CHATSHOWENTRYTAG);
        }
            break;
        default:
            break;
    }
    m_chatShowPreLayer = nowLayersTag;
    haveChatShowLayer = true;
	
}


void SGMainManager::showjingzhouprocesslayer(CCArray *array)
{
    SGJingZhouProcessLayer *layer = (SGJingZhouProcessLayer *)mainScene->getChildByTag(sg_jingzhouprocess);
    if (!layer) {
        layer = SGJingZhouProcessLayer::create(array);
    }
    mainScene->addShowLayer(layer);
    SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());////1126效果优化
	
	layer->showFirstSectionInfo();
}

//关闭聊天展示页面
void SGMainManager::closeChatShowLayer()
{
    SGBaseLayer*layer = (SGBaseLayer *)mainScene->getChildByTag(m_chatShowPreLayer);
    if (layer) {
        SGMainManager::shareMain()->showLayer(layer);
    }
    else
    {
        SGMainManager::shareMain()->showFirstLayer();
    }
    haveChatShowLayer = false;
}


void SGMainManager::showRankLayerLayer(CCArray *array,int value ,int ranking,int maxitemnum)
{    
    SGRankLayer *layer = (SGRankLayer *)mainScene->getChildByTag(sg_rank);
    if (!layer) {
        layer = SGRankLayer::create(array,value,ranking,maxitemnum);
    }
    mainScene->addShowLayer(layer);
    
}

void SGMainManager::showPiecesInfoLayer(SGPiecesCard* card,int type,bool isBeforeDel)
{
    SGPiecesInfoLayer *layer = (SGPiecesInfoLayer *)mainScene->getChildByTag(sg_piecesinfolayer);
    if (!layer) {
        layer = SGPiecesInfoLayer::create(card,type);
    }
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    mainScene->addShowLayer(layer,isBeforeDel);
    mainLayer->setIsCanTouch(true);
    mainLayer->setPortraitMenuCanTouch(false);
    
    if(type==25)
        mainLayer->setIsCanTouch(false);
    if (type == CHATSHOWENTRYTAG) {
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        mainLayer->hideblack();
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    
    
}
//进boss排行
void SGMainManager::sendGetLastBossRankList()
{
	main::BossDamageRankRequest *rankReq = new main::BossDamageRankRequest();
	SGSocketClient::sharedSocketClient()->send(MSG_ENTER_BOSS_RANK, rankReq);
}
// 获取好友武将情况
void SGMainManager::getFriendOfficerInfo()
{
	main::FriendInfoRequest *fInfo = new main::FriendInfoRequest();
	fInfo->set_friendid(HelpFriendRoleId);
	SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, fInfo);
}
void SGMainManager::gotoMatchRequest()
{
    servername = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
    serverid = (int)CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
    
    main::PvpEntryRequest *request = new main::PvpEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
}

void SGMainManager::showSeasonMainLayer(CCObject *sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::PvpEntryResponse *response = (main::PvpEntryResponse *)request->m_msg;
    SGBattleData *battleData = NULL;
    SGRankPlayer *selfPlayer = NULL;
    if (response)
    {
        
        float var1 = response->winratio();//当前胜率
        int var2 = response->prestige();//天梯声望
        int var3 = response->continuouswin();//连胜
        int var4 = response->maxcontinuouswin();//最大连胜
        int var5 = response->awardscore();//天梯积分
        //int var6 = response->prestigerank();//声望排名
        int var7 = response->pvpcount();//第几界
        int var8 = response->pvprank();//天梯排名
        //int var9 = response->pvpscore();//天梯等级
        int var10= response->pvpfightcount();//参战次数
        
        bool hasDayReward = response->hasdayreward();//每日奖励是否可领取
        bool hasSeasonReward = response->hasseasonreward();//赛季奖励是否可领取

        
        std::string str = response->pvprankname();//当前军衔
        std::string str2 = response->pvpenddate();//截止时间
        
        CCString *name = SGPlayerInfo::sharePlayerInfo()->getNickName();
        int iconId = SGPlayerInfo::sharePlayerInfo()->getPlayerImageID();
        
        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, var8,var2,var10, var1,serverid,servername.c_str());
        battleData = SGBattleData::create(str2.c_str(), var4, var3, var5, var2,var1,var8 ,str.c_str(),var7 , hasDayReward , hasSeasonReward);//debug by:zyc, bvattledata arguments error.
        
        
        //        selfPlayer = SGRankPlayer::create(name->getCString(), iconId, 0, 0, var4, winr);
        battleData->setStartTime(CCString::create(response->specstarttime()));
        battleData->setEndTime(CCString::create(response->specendtime()));
        SGPlayerInfo::sharePlayerInfo()->setPvpRank(selfPlayer);
        if (selfPlayer)
        {
            SGPlayerInfo::sharePlayerInfo()->setplayerPvpLevel(selfPlayer->getLevel());
        }
        SGPlayerInfo::sharePlayerInfo()->setplayerPvpNum(response->awardscore());
    }
    if (battleData != NULL)
    {
        if (mainScene->getChildByTag(sg_fightrewardLayer))
        {
            ((SGFightRewardLayer *)mainScene->getChildByTag(sg_fightrewardLayer))->stopRequestPvpMain();
        }
        //SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGBattleMainLayer *layer = (SGBattleMainLayer *)mainScene->getChildByTag(sg_battleMainLayer);
        SGMainLayer *main = (SGMainLayer*)mainScene->getChildByTag(sg_mainLayer);
        main->setIsCanTouch(true);
        main->setVisible(true);
        main->setPortraitMenuCanTouch(false);
        if (!layer)
        {
            layer = SGBattleMainLayer::create(battleData);
        }
        mainScene->setVisible(true);
        mainScene->addShowLayer(layer);
        if (SGGuideManager::shareManager()->getLimitGuideSize() > 0)
        {
            layer->setIsCanTouch(false);
        }
    }
    
    
}
void SGMainManager::showPvpMatchLayer()
{
    
    int lavel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    if (lavel < SGPlayerInfo::sharePlayerInfo()->getLimitLevelById(limitPvpBattle)) {
        SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_MainLayer_str64,SGPlayerInfo::sharePlayerInfo()->getLimitLevelById(limitPvpBattle))->getCString());
        return;
    }
    
    //            CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGPvpMainLayer::hideScrollItemScale));
    /////kanata  MT效果
    //    CCCallFunc *speedFun=CCCallFunc::create(this,  callfunc_selector(SGPvpMainLayer::hideScrollItemSingleMove));
    
    //    CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGMainManager::gotoMatchRequest));
    //
    //    //            CCCallFunc *speedFun2=CCCallFunc::create(this,  callfunc_selector(SGPvpMainLayer::showFieldLayer));
    //    this->runAction(CCSequence::create(speedFun,CCDelayTime::create(ITEMACTIONTIME*2),speedFun2,NULL));
    //
    //            main::PvpEntryRequest *request = new main::PvpEntryRequest();
    //            SGSocketClient::sharedSocketClient()->send(MSG_PVPENTER_SEASONWAR, request);
    
    
    gotoMatchRequest();
}


void SGMainManager::setEmbattleKey(bool val)
{
	CCUserDefault::sharedUserDefault()->setBoolForKey(CCString::createWithFormat("%s_show_embattle_flag",
																				 SGPlayerInfo::sharePlayerInfo()->getNickName()->getCString())->getCString(), val);
	CCUserDefault::sharedUserDefault()->flush();
	
}
bool SGMainManager::getEmbattleKey()
{
	return CCUserDefault::sharedUserDefault()->getBoolForKey(CCString::createWithFormat("%s_show_embattle_flag",
                                                                                        SGPlayerInfo::sharePlayerInfo()->getNickName()->getCString())->getCString());
}


#if (PLATFORM == IOS)
void SGMainManager::setBuyData(const char *data)
{
    m_buyData = std::string(data);
}


void SGMainManager::sendCreatInvoice(float amount,const char * buyString)
{
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    OcCaller::getInstance()->sendCreatInvoice(SGPlayerInfo::sharePlayerInfo()->getNickName()->getCString(), amount, SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId());
    m_buyString = std::string(buyString);
    
}
void SGMainManager::sendInvoiceRequst()
{
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();
    OcCaller::getInstance()->sendInvoiceStatus(m_billId);
}

void SGMainManager::doCreatInvoice(int state,int billId)
{
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
    
    if (state == 0)
    {
        m_billId = billId;
        OcCaller::getInstance()->iapBuyItem(m_buyString.c_str());
    }
}

void SGMainManager::doReceiveInvoice(int state)
{
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
    
    if (state == 0)
    {
        int serverId = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
        
        int userID = SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId();
        
        main::ShopChargeRequest *request = new main::ShopChargeRequest;
        request->set_gameid(GO2PLAYGAMEID);
        request->set_areaid(serverId);
        request->set_accountid(userID);
        request->set_roleid(SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId());
        request->set_receipt(m_buyData);
        SGSocketClient::sharedSocketClient()->send(MSG_STORE_BUY, request);
    }
}

void SGMainManager::doInvoiceFail()
{
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
    //SGMainManager::showMessage("Buy Fail");
}
#endif

//发送进入具体商店的消息
void SGMainManager::sendEnterMallLayerRequest(ShopGUID shopId , ShopEnterType type)
{
    SGPlayerInfo * player = SGPlayerInfo::sharePlayerInfo();
    switch(shopId)
    {
        case SHOPID_PVP:
            if(!player->canBreakLimitById(limitPvpBattle))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_GoldShopLimitTimeSell_str21,player->getLimitLevelById(limitPvpBattle))->getCString() );
                return;
            }
            break;
            
        case SHOPID_JING_ZHOU:
            if(!player->canBreakLimitById(limitShouJinZhou))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_GoldShopLimitTimeSell_str22,player->getLimitLevelById(limitShouJinZhou))->getCString() );
                return;
            }
            break;
            
        case SHOPID_LUO_YANG://只有vip等级达到才显示该条目  不需条件。
            break;
            
        case SHOPID_SPECIAL_BATTLE:
            if(!player->canBreakLimitById(limitSpecailBattle))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_GoldShopLimitTimeSell_str24,player->getLimitLevelById(limitSpecailBattle))->getCString() );
                return;
            }
            break;
            
        case SHOPID_ARENA:
            if(!player->canBreakLimitById(limitArena))
            {
                SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_GoldShopLimitTimeSell_str25,player->getLimitLevelById(limitArena))->getCString() );
                return;
            }
            break;
            
        default://无条件限制
            break;
    }
    SGMallLayer::setEnterTyppe(type);
    
    main::ShopCargoListRequest *request = new main::ShopCargoListRequest;
    request->set_shopid(shopId);
    SGSocketClient::sharedSocketClient()->send(MSG_CARGO_LIST, request);
}

//接收到商品列表消息
void SGMainManager::showMallLayer( CCObject * sender)
{
    SGSocketRequest *request = (SGSocketRequest *)sender ;
    main::ShopCargoListResponse *response = (main::ShopCargoListResponse *)request->m_msg;
    
    if ( response)
    {
        
        SGMallData * mallData = new SGMallData();
        mallData->_mallId = response->shopid();
        mallData->_mallTitle = response->shopname();
        mallData->_everydayUpdateTime = response->nextfreshtime();
        mallData->_refreshNeedMoney = response->freshneedgold();
        mallData->_refreshNeedPayKind = response->freshgoldtype();
        
        int count = response->cargoinfo_size(); //数量
        if ( 0 == count )
        {
            //todo: 当服务器发来0个物品
            
        }
        else
        {
            CargoInformation * pCargoInfo = NULL;
            for ( int i=0 ; i< count ; ++i)
            {
                pCargoInfo = new CargoInformation();
                pCargoInfo->cargoId = response->cargoinfo(i).cargoid();
                pCargoInfo->payKind = response->cargoinfo(i).pricetype();
                mallData->_vecPayKind.push_back(pCargoInfo->payKind); //所有都加入vec中，使用时用 unique函数去除相同的。
                
                pCargoInfo->oldPrice = response->cargoinfo(i).price();
                pCargoInfo->currentPrice = response->cargoinfo(i).curprice();
                pCargoInfo->itemKind = response->cargoinfo(i).itemtype();
                pCargoInfo->itemId = response->cargoinfo(i).itemid();
                pCargoInfo->itemCount = response->cargoinfo(i).itemcount();
                pCargoInfo->cargoTag = response->cargoinfo(i).cargotag();
                if ( 0 == mallData->_mallId)
                {
                    mallData->_goodsKind[pCargoInfo->cargoTag] = 1;
                }
                pCargoInfo->cargoState = response->cargoinfo(i).cargostate();
                
                
                mallData->_allCargo->addObject(pCargoInfo);
            }
            SGMallLayer * mallLayer = SGMallLayer::create(mallData);
            mainScene->addShowLayer(mallLayer,true);
            SNSTableView::showScrollItemMultiMoveFromRight(mallLayer->getSNSTavleView());
        }
    }
    CCLog("no msg.");
}

void SGMainManager::shareGameById(int shareId,int shareInfo,std::string &sharePicture )
{
    m_shareId = shareId;
    switch (shareId) {
        case 1:
        {
            
        }
            break;
        case 2:
        {
            doFBShare(shareInfo,sharePicture );
        }
            break;
        case 3:
        {
            
        }
            break;
        case 4:
        {
            
        }
            break;
        default:
            break;
    }
}


#if (PLATFORM == ANDROID)

std::string SGMainManager::getShareInfoFromShareId(int shareId)
{
    std::string info = "";
    switch (shareId)
    {
        case 0:
        {
            //SGShareTypeGeneral
            info = str_SGShareBoxInfo_genral;
        }
            break;
        case 1:
        {
            //SGShareTypeVisit
            info = str_SGShareBoxInfo_visit;
        }
            break;
        case 2:
        {
            //SGShareTypeBossBattel
            info = str_SGShareBoxInfo_boss;
        }
            break;
        case 3:
        {
            //SGShareTypeShoujingzhou
            info = str_SGShareBoxInfo_jinzhou;
        }
            break;
        case 4:
        {
            //SGShareTypeGeneralFight
            info = str_SGShareBoxInfo_pvefight;
        }
            break;
        default:
        {
            //SGShareTypeGeneral
            info = str_SGShareBoxInfo_genral;
        }
            break;
    }
    return info;
}


#endif


void SGMainManager::doFBShare(int shareInfo,std::string &sharePicture )
{
    m_sharePicture = CCFileUtils::sharedFileUtils()->getWritablePath() + sharePicture;
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
#if (PLATFORM == IOS)
    OcCaller::getInstance()->startFBShare(shareInfo,m_sharePicture);
#else
    //android code
    std::string shareInfoString = getShareInfoFromShareId(shareInfo);
    AndroidSDKAdp::getInstance()->fbShareGame(m_sharePicture, shareInfoString);
#endif
#else
    if (getMainScene()->nowBoxTag == box_shareGame )
    {
        SGShareBox * nowBox = (SGShareBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
        nowBox->boxCloseWithOutCallBack();
    }
    if (remove(m_sharePicture.c_str()) != 0)
    {
        CCLOG("can not remove downloaded zip file %s", m_sharePicture.c_str());
    }
#endif
}

void SGMainManager::finishFBShare()
{
    if (m_shareId > 0)
    {
        main::ShareRewardRequest *request = new main::ShareRewardRequest();
        request->set_platformid(m_shareId);
        SGSocketClient::sharedSocketClient()->send(MSG_SHARE_REWARD, request);
        m_shareId = -1;
        showMessage(str_ShareGame_str1);
        
        if (getMainScene()->nowBoxTag == box_shareGame )
        {
            SGShareBox * nowBox = (SGShareBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
            nowBox->boxCloseWithOutCallBack();
        }
        if (remove(m_sharePicture.c_str()) != 0)
        {
            CCLOG("can not remove downloaded zip file %s", m_sharePicture.c_str());
        }
        
        SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
    }
}
void SGMainManager::shareFbDidFail()
{
    m_shareId = -1;
    showMessage(str_ShareGame_str2);
    
    if (getMainScene()->nowBoxTag == box_shareGame )
    {
        SGShareBox * nowBox = (SGShareBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
        nowBox->setIsCanTouch(true);
    }
    
    if (remove(m_sharePicture.c_str()) != 0)
    {
        CCLOG("can not remove downloaded zip file %s", m_sharePicture.c_str());
    }
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
}

void SGMainManager::setFBSessionState(bool sessionState)
{
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
    m_facebookSessionState = sessionState;
#endif
}


void SGMainManager::setChatBtnVisiable(bool isvisble)
{
    isvisble = false;    //added by cgp
    SGButton *mainLayerChat = (SGButton *)SGMainManager::shareMain()->getMainLayer()->getBtnByTag(CHATBTNTAG);
    if (mainLayerChat) {
        mainLayerChat->setVisible(isvisble);
        mainLayerChat->setEnabled(isvisble);
    }
}
void SGMainManager::doCloseFBlogin(int fbValue)
{
#if (PLATFORM == IOS)
    OcCaller::getInstance()->closeFBlogin(fbValue);
#endif
}

void SGMainManager::clearFBUserData(int fbValue )
{
    m_isFBLogin = fbValue;
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_welComeLayer))
    {
        ((SGWelComeLayer *)mainScene->getChildByTag(sg_welComeLayer))->updateViewForFB();
    }
    bool savedFbState = CCUserDefault::sharedUserDefault()->getBoolForKey("XdFBLoginState");
    if(savedFbState)
    {
        //玩家强退，未退出facebook
        CCUserDefault::sharedUserDefault()->setStringForKey("userId","");
        CCUserDefault::sharedUserDefault()->setStringForKey("username","");
        CCUserDefault::sharedUserDefault()->setStringForKey("password","");
    }
    CCUserDefault::sharedUserDefault()->setBoolForKey("XdFBLoginState",false);
}



void SGMainManager::doUpdateNewViewForFB()
{
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_welComeLayer))
    {
        ((SGWelComeLayer *)mainScene->getChildByTag(sg_welComeLayer))->updateViewForFB();
    }
}

void SGMainManager::updateNewViewForFB()
{
    SGNotificationCenter::sharedNotificationCenter()->postNotification("updateNewViewForFB");
}

void SGMainManager::setFBUserData(const char * userId,const char* loginKey,const char *userName,bool setFbState)
{
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
    //CCUserDefault::sharedUserDefault()->setStringForKey("loginKey",std::string(loginKey));
    
    bool savedFbState = CCUserDefault::sharedUserDefault()->getBoolForKey("XdFBLoginState");
    
    CCUserDefault::sharedUserDefault()->setBoolForKey("XdFBLoginState",setFbState || savedFbState);
    
    if (setFbState || savedFbState)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("userId",std::string(userId));
        CCUserDefault::sharedUserDefault()->setStringForKey("username",std::string(userName));
        
        CCUserDefault::sharedUserDefault()->setStringForKey("nickName","");
        CCUserDefault::sharedUserDefault()->flush();
        
        m_isFBLogin = 1;
        
        this->updateNewViewForFB();
    }


#endif
}

void SGMainManager::doFBLogin()
{
	
	CCUserDefault::sharedUserDefault()->setIntegerForKey("channelId",1);
	CCUserDefault::sharedUserDefault()->setIntegerForKey("subChannelId",0);
    
    CCUserDefault::sharedUserDefault()->setStringForKey("password","password");
    
    std::string accountId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
    std::string acountName = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    
    
	CCUserDefault::sharedUserDefault()->flush();
    
    std::string un = std::string(THIRDPARTYFBPRX) + std::string(THIRDPARTYFBID) + accountId;
    std::string pw  = accountId; 

#if (PLATFORM == IOS)
    OcCaller::getInstance()->pyUserThirdPartySDKLoginReq(un.c_str(), pw.c_str());
#else
    //android code
    //AndroidSDKAdp::getInstance()->fbLogin();
    SdkManager::shareSdkManager()->userThirdPartyLogin(un, pw);
#endif
//    //登入请求
//    std::string serverip = CCUserDefault::sharedUserDefault()->getStringForKey("serverip");
//    int serverpost = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverpost");
//    
//    //记录服务器ip和端口号
//    SGMainManager::shareMain()->setServerInfo(CCString::create(serverip.c_str()) , serverpost);
//    //连接socket
//    SGSocketClient::sharedSocketClient()->startConn(serverip.c_str() , serverpost);
}

void SGMainManager::dealIosOKLogin()
{
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
    if (m_facebookSessionState)
    {
        //facebook 登入
        doFBLogin();
    }
    else
    {
        SGMainManager::shareMain()->getMainScene()->plat->showConnection();
#if (PLATFORM == IOS)
        OcCaller::getInstance()->okLoginSDK();
#else
        SdkManager::shareSdkManager()->oKLogin();
#endif
    }
    
#else
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();

#if (PLATFORM == IOS)
    OcCaller::getInstance()->okLoginSDK();
#else
    //android code here
    SdkManager::shareSdkManager()->oKLogin();
#endif
#endif
    
}

void SGMainManager::dealIosLogin(std::string &un,std::string &pw)
{
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
    if (m_isFBLogin == 1)
    {
        //facebook 登入
        doFBLogin();
    }
    else
    {
        SGMainManager::shareMain()->getMainScene()->plat->showConnection();
        
#if (PLATFORM == IOS)
        OcCaller::getInstance()->pyUserSDKLoginReq(un.c_str(), pw.c_str());
#else
        SdkManager::shareSdkManager()->userLogin(un, pw);
#endif
    }
    
#else
    SGMainManager::shareMain()->getMainScene()->plat->showConnection();

#if (PLATFORM == IOS)
    OcCaller::getInstance()->pyUserSDKLoginReq(un.c_str(), pw.c_str());
#else
    //android code
    SdkManager::shareSdkManager()->userLogin(un, pw);
#endif
#endif
}

void SGMainManager::doCreatFBSession()
{
    m_facebookSessionState = false;
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
#if (PLATFORM == IOS)
    OcCaller::getInstance()->createFBSession();
#else
    AndroidSDKAdp::getInstance()->callFBCreateSession();
#endif
#endif
}

void SGMainManager::cleanAccount()
{
    CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount", true);
		// 移动至外围ocCaller中,集中使用cpp调用
#if (PLATFORM == IOS)
		OcCaller::getInstance()->pyUserSDKCallReq();
#else
		//android code here
#endif
}



void SGMainManager::sendOperatorRecord(int stepId , bool isShowCircle)
{
    main::NewComerRecordRequest *res = new main::NewComerRecordRequest();
    res->set_recordtypeid(stepId);
    res->set_mac(GameConfig::getUdit());
    SGSocketClient::sharedSocketClient()->send(MSG_GUIDE_STEP, res ,isShowCircle );

}

//战斗相关的统计处理 然后发送消息    actionType 意思是1=战斗中点击按钮  2=移动士兵 3=删除士兵成功  4=本关胜利 5=长按精灵 直到出现可以点击的删除按钮
void SGMainManager::preProcessDataInFightSceneForSend(int actionType)
{
    int storyId = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
    int sceneId = SGPlayerInfo::sharePlayerInfo()->getCurrentSceneId();
    int step = SGGuideManager::shareManager()->step;

    if ( 1 == actionType)
    {
        switch (storyId)
        {
            case 1:
                switch (sceneId)
                {
                    case 1: //1-1
                        if (step == 16) //1-1  点击 援兵
                        {
                            sendOperatorRecord(201005);
                        }
                        else if (step == 21) //1-1 点击行动力
                        {
                            sendOperatorRecord(201007);
                        }
                        else if (step == 22 ) // 1-1 点击结束回合
                        {
                            sendOperatorRecord(201008);
                        }
                        break;
                    case 2://1-2
                        if ( 14 == step ) //1-2 点击援兵
                        {
                            sendOperatorRecord(202007);
                        }
                        break;
                    case 3: //1-3
                        if ( 11 == step)// 1-3 点击援兵
                        {
                            sendOperatorRecord(204014);
                        }
                        break;
                }
                break;
                
                
                
                //在下边添加 story>1 的逻辑
            case 2:
                break;
                
        }
    }
    else if ( 2 == actionType)
    {
        
        switch ( storyId)
        {
            case 1:
                switch (sceneId)
                {
                    case 1:
                        if (8 ==step ) //1-1 第一次移动
                        {
                            sendOperatorRecord(201003);
                        }
                        else if (11 == step)//1-1 第二次移动
                        {
                            sendOperatorRecord(201004);
                        }
                        else if (18== step)//1-1 第3次移动
                        {
                            sendOperatorRecord(201006);
                        }
                        break;
                    case 2:
                        if (5 == step) //1-2 第一次移动
                        {
                            sendOperatorRecord(202004);
                        }
                        else if ( 7 == step ) //1-2 第二次移动
                        {
                            sendOperatorRecord(202005);
                        }
                        else if ( 9 == step) //1-2 第三次移动
                        {
                            sendOperatorRecord(202006);
                        }
                        else if (18 == step )//1-2 第四次移动
                        {
                            sendOperatorRecord(202008);
                        }
                        else if (20 == step )//1-2 第五次移动
                        {
                            sendOperatorRecord(202009);
                        }
                        break;
                    //在此添加 节
                }
                break;
            case 2:
                break;
            //在此添加 章
        }
    }
    else if ( 3==actionType)
    {
        switch(storyId)
        {
            case 1:
                switch(sceneId)
                {
                    case 3://1-3 目前从1-3 开始有删除操作
                        if ( 3 == step)//1-3 第一次删除
                        {
                            sendOperatorRecord(204005);
                        }
                        else if (5 == step)//1-3 第2次删除
                        {
                            sendOperatorRecord(204007);
                        }
                        else if (7 == step)//1-3 第3次删除
                        {
                            sendOperatorRecord(204009);
                        }
                        else if (8 == step)//1-3 第4次删除
                        {
                            sendOperatorRecord(204011);
                        }
                        else if (9 == step)//1-3 第5次删除
                        {
                            sendOperatorRecord(204013);
                        }
                        break;
                }
                break;
            case 2:
                break;
        }
    }
/*
    else if ( 4 == actionType) //胜利 其他胜利消息  在sgguidemanager  checkisdone中
    {
        if (1 == storyId)
        {
            if (1 == sceneId )// 1-1
            {
                sendOperatorRecord(201009);
            }
//            else if (2 == sceneId ) //1-2
//            {
//                sendOperatorRecord(202008);
//            }
//            else if (3 == sceneId) //1-3
//            {
//                sendOperatorRecord(203008);
//            }
        }
    }
 */
    else if ( 5 == actionType)
    {
        if ( 1 == storyId)
        {
            if (3 == sceneId) //目前只考虑1-3
            {
                switch(step)
                {
                    case 3:
                        sendOperatorRecord(204004);
                        break;
                    case 5:
                        sendOperatorRecord(204006);
                        break;
                    case 7:
                        sendOperatorRecord(204008);
                        break;
                    case 8:
                        sendOperatorRecord(204010);
                        break;
                    case 9:
                        sendOperatorRecord(204012);
                        break;
                }
            }
        }
    }
 
}

//void SGMainManager::setCanAdvPartNum(SGOfficerCard * officer)
//{
//    if (officer)
//    {
//        officer->retain();
//        
//        //设置多少个可使用转生材料
//        int usednum =0;
//        if (officer->getMaxStar())
//        {
//            if (officer->getCurrStar()>=1)
//            {
//                
//                CCArray* array =SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
//                for(int i=0;i<array->count();i++)
//                {
//                    SGBaseMilitaryCard *baseMilitaryCard = (SGBaseMilitaryCard *)array->objectAtIndex(i);
//                    SGOfficerCard *card1 = (SGOfficerCard *)baseMilitaryCard;
//                    int po1 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(1,card1);
//                    int po2 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(2,card1);
//                    int po3 = SGTeamgroup::shareTeamgroupInfo()->getPositionId(3,card1);
//                    if ( po1 == 0 && po2 ==0 && po3 == 0 && card1->getIsProt() == 0 && card1->getSsid()!=officer->getSsid() && card1->getProtoId() ==((SGOfficerCard*)officer)->getProtoId() && officer->getCurrStar() == card1->getCurrStar())
//                    {
//                        usednum++;
//                    }
//                }
//                
//                
//            }
//        }
//        officer->setAdvanceStuffNum(usednum);
//
//        
//        officer->release();
//    }
//}

//DEBUG黄点的测试
void SGMainManager::yellowTest()
{
    return;
    
    //吕布
    CCNode* layer = this->getMainScene()->getChildByTag(sg_welComeLayer);
    
    bool isme = false;
    
    
    float time = 1.2;
    float sbTime = time/12;
    for (int i = 0; i < mapList; i++) {
        //和业务逻辑无关
        CCString *name = NULL;
        CCString *name1 = NULL;
        name = CCString::createWithFormat("animationFile/jian_%d.plist", 106);
        name1 = CCString::createWithFormat("animationFile/jian_%d.scml", 106);
        
        ResourceManager::sharedInstance()->bindTexture(name->getCString(), RES_TYPE_BATTLE_IMAGE, sg_battleLayer);
        CCSpriterX *fermEffect = CCSpriterX::create(name1->getCString(), true, true);
        fermEffect->setanimaID(0);
        fermEffect->setisloop(true);
        fermEffect->setPosition( GameConfig::getGridPoint(isme ? 5 : -5 - 2, abs(isme ? -i :  (mapList - 1 - i)), true) );
        fermEffect->play();
        layer->addChild(fermEffect, SKILLS_BULLET_TAG, SKILLS_BULLET_TAG + i);
        
        if (!isme) {
            //MMDEBUG: FLIPY
            fermEffect->setScaleY(-1);
//            fermEffect->setPosition(ccp(0, GameConfig::getGridSize().height * 1.5));
//            fermEffect->setCCSpriterXFlipY(true);
        }
        
        CCDelayTime *delaytime = CCDelayTime::create(sbTime*0.5*i);
        CCMoveTo *move = CCMoveTo::create(time, GameConfig::getGridPoint(isme ? -mapRow : mapRow - 2, abs(isme ? -i : (mapList - 1 - i)) ,true));
        fermEffect->runAction(CCSequence::create(delaytime, move, NULL));
        
        //        int a = (fermEffect->getAnimaTimeLength(0, 0) * ANIMA_TIME_NUM) * 10000;
        //
        //
        //        CCDelayTime *time = CCDelayTime::create(a);
        //        CCCallFuncND *call = CCCallFuncND::create(this, callfuncND_selector(SGSkillManager::sbSkill_20005), (void*)a);
        //        sb->runAction(CCSequence::create(time, call, NULL));
    }
}

//快速开启原型级物品信息BOX
void SGMainManager::showCommonInfoBoxInProto(SGBaseMilitaryCard* bmc, SGBoxDelegate* boxDeg, int otherEnterType, bool otherIsBeforeDel, int myEnterType)
{
    if (!bmc)
        return;
    SGCommonInfoBox* cib = SGCommonInfoBox::create(bmc, boxDeg, otherEnterType, otherIsBeforeDel, myEnterType);
    this->showBox(cib);
}


//快速开启原型物品信息BOX，需要物品的TYPE/ID。
void SGMainManager::showCommonInfoBoxInProto(SGBaseItemType type, int itemId, SGBoxDelegate* boxDeg, int otherEnterType, bool otherIsBeforeDel, int myEnterType)
{
    SGStaticDataManager* sdm = SGStaticDataManager::shareStatic();
    SGBaseMilitaryCard* bmc = sdm->getOneBmcByTypeAndId(type, itemId);
    if (bmc)
    {
        this->showCommonInfoBoxInProto(bmc, boxDeg, otherEnterType, otherIsBeforeDel, myEnterType);
    }
}

//快速开启带有“获取”途径的CIB。
void SGMainManager::showCommonInfoBoxGoToArena(SGBaseItemType type, int itemId, SGBoxDelegate* boxDeg,  int otherEnterType, bool otherIsBeforeDel, int myEnterType)
{
    SGStaticDataManager* sdm = SGStaticDataManager::shareStatic();
    SGBaseMilitaryCard* bmc = sdm->getOneBmcByTypeAndId(type, itemId);
    if (bmc)
    {
        SGCommonInfoBox* cib = SGCommonInfoBox::create(bmc, boxDeg, otherEnterType, otherIsBeforeDel, myEnterType, SGCSM_GOTO_ARENA);
        this->showBox(cib);
    }
}

//true 为TW， false不是台湾
bool SGMainManager::getCurrentChannelIsTW()
{
    return (GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_GOOGLE_PLAY ||
            GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_ADR_THIRD ||
            GAME_CHANNEL_TYPE == GCCT_TAIWAN_GO2PLAY_IOS_APPLE_APPSTORE);
}

std::string SGMainManager::getPlistWithChannel()
{
    return m_plistName;
}

int SGMainManager::getShowFriendlyTipsChapId()
{
    int storyId = 10;
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 = (CCDictionary *) dic->objectForKey("50");
    
    if (dic1)
    {
        storyId = dic1->valueForKey("value")->intValue();
    }
    return storyId;
}

//西域商人总览req
void SGMainManager::exoMechMainInfoRequest()
{
    //MMDBG: 偽造數據
//    SGSocketRequest* sreq = new SGSocketRequest();
//    
//    main::XiyuInfoResponse* xiresp = new main::XiyuInfoResponse();
//    xiresp->set_state(1);
//    xiresp->set_time(1500);
//    
//    main::GoodsDataProto* gdp = NULL;
//    for (int k = 0; k < 4; k++)
//    {
//        gdp = xiresp->add_goods();
//        main::ItemDataProto* idp = gdp->mutable_item();
//        idp->set_itemtype(6);
//        idp->set_itemid(1021);
//        idp->set_itemnum(1234);
//        idp->set_itemlv(20);
//        
//        gdp->set_pricetype(0);
//        gdp->set_pricevalue(k * 78);
//        if (k % 2 == 0)
//        {
//            gdp->set_state(0);
//        }
//        else
//        {
//            gdp->set_state(1);
//        }
//        
//        gdp->set_mark("VIP双倍");
//    }
//    sreq->m_msg = xiresp;
//    this->exoMechMainInfoListener(sreq);
//    CC_SAFE_DELETE(sreq);
//    
//    return;
    
    SGAdventureLayer* al = dynamic_cast<SGAdventureLayer*>(this->mainScene->getChildByTag(sg_adventureLayer));
    if (al)
    {
        return;
    }
    
    main::XiyuInfoRequest* req = new main::XiyuInfoRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_EXO_MECH_MAIN_INFO, req);
}

//西域商人总览resp
void SGMainManager::exoMechMainInfoListener(CCObject* obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::XiyuInfoResponse* resp = dynamic_cast<main::XiyuInfoResponse*>(sr->m_msg);
        if (resp)
        {
            SGAdventureLayer* al = dynamic_cast<SGAdventureLayer*>(this->mainScene->getChildByTag(sg_adventureLayer));
            if (!al)
            {
                al = SGAdventureLayer::create();
                this->mainScene->addShowLayer(al);
            }

            if (al)
            {
                al->showPageByIndex(SGAT_EXOTIC_MERCHANT, resp);
            }
        }
    }
}

void SGMainManager::showCommonMessage(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::ShowCommonMessage* res = dynamic_cast<main::ShowCommonMessage*>(sr->m_msg);
        showMessage(res->msg().c_str());
    }
}


