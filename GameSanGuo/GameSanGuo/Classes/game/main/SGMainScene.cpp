//
//  SGMainScene.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-20.
//
//
#include "ResourceManager.h"
#include "SGMainScene.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "LoginModule.pb.h"
#include "SGMsgId.h"
#include "SGProtocolToMsg.h"
#include "SGBattleMatchLayer.h"
#include "SGBattleManager.h"
#include "cocos2d.h"
#include "SGWelComeLayer.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGCountryLayer.h"
#include "SGGamePlayer.h"
#include "UpdateLayer.h"
#include "SGLottery9ViewItem.h"
#include "SGStaticDataManager.h"
#include "SGMainLayer.h"
#include "SGHeartbeatDetection.h"
#include "SGTestSwitch.h"
#include "SGTTMainLayer.h"
#include "SGBattlePreNLayer.h"
#include "CGPBattleLoadingLayer.h"

#include "SGOverBattleBox.h"
#include "GameMusicmanage.h"
#include "SGGeneralBox.h"
#include "SGBMatchAlertBox.h"
#include "SGBMatchStartBox.h"
#include "SGBMatchEndBox.h"
#include "RewardModule.pb.h"
#include "SGCantAdvanceBox.h"
#include "SGGuideManager.h"
#include "DayTaskModule.pb.h"
#include "SGFightRewardLayer.h"
#include "SGStoryRewardItem.h"
#include "SGBattleCallFriendBox.h"
#include "SGBattleReviveLayer.h"
#include "SGQFightBox.h"
#include "SGBattleLeaderSkillBox.h"
#include "SGBattleRoundPromptLayer.h"
#include "BackFightBox.h"
#include "SGFirstLayer.h"
#include "ActivityMessage.pb.h"
#include "SGAnnouncementDataModel.h"
#include "SGGeneralInfoLayer.h"
#include "SGTeamgroup.h"
#include "ResourceManager.h"
#include "SGPiecesCard.h"
#include "SGCGMovieLayer.h"
#include "SGAIManager.h"
#include "SGEmbattleLayer.h"
#include "SGStringConfig.h"
#include "ExtClass.h"
#include "Invite.pb.h"
#include "SGObtainTili.h"
#include "SGUpdateManager.h"
#include "SGUpdateClientBox.h"
#include "GlobalConfig.h"
#include "SGExoticMerchantLayer.h"
#include "SGConnectLoading.h"
#include "SGLogicManager.h"
#if (PLATFORM == ANDROID)
#include "SdkHandler.h"
#include "SdkManager.h"
#include "AndroidSDKAdp.h"
#elif (PLATFORM == IOS)
#include "OcCaller.h"
#endif


SGMainScene::SGMainScene()
: nowLayerTag(sg_LayerNull)
, nowBoxTag(box_null)
, oldBoxTag(box_null)
, plat(NULL)
, usedMemory(NULL)
, availableMemory(NULL)
, pvpMatch(NULL)
, message(NULL)
, friendManager(NULL)
, conLayer_(NULL)
, m_pvpDelay(0.0f)
, m_bossDelay(0.0f)
{
    nowIsCocostudio = false;
}


SGMainScene::~SGMainScene()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_mainScene);
    friendManager->release();
}


void SGMainScene::onEnter()
{
    CCScene::onEnter();
  

}


bool SGMainScene::init()
{
    if (!CCScene::init())
    {
        return false;
    }
    
    //added by cgp
    m_pUIGroup = TouchGroup::create();
    this->addChild(m_pUIGroup, 1);
    
	//初始化各SDK平台
	ExtClassOfSDK::sharedSDKInstance()->initSDK();

	
	
    nowBoxTag = box_null;
    oldBoxTag = box_null;
    nowLayerTag = sg_LayerNull;
    CCMenuItemLabel *bindLabel = CCMenuItemLabel::create(
                                                         CCLabelTTF::create("log", "Arial", 34),
                                                         this,
                                                         menu_selector(SGMainScene::logCache));
    
    //bindLabel->setVisible(false);
    bindLabel->setVisible(true);
    bindLabel->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(-bindLabel->getContentSize().width/2,
																	-bindLabel->getContentSize().height/2)));
    //this->addChild(bindLabel);
    this->addChild(SGNotificationCenter::sharedNotificationCenter());
    
    this->initNotifications();
	
   
    //启动好友消息收发器
    friendManager = new SGFriendManager();
//    mailManager = new SGMailManager();
    //跨平台
    plat = SGPlatFormIos::create();
    addChild(plat);
    
    //对战文字
    SGCCLabelTTF * label = SGCCLabelTTF::create(str_MainLayer_str36, FONT_PANGWA, 26 ,CCSizeZero,kCCTextAlignmentCenter , kCCVerticalTextAlignmentCenter );
    pvpMatch =  CCSprite::createWithSpriteFrameName("public_blackBoard.png");
    CCSize size = pvpMatch->getContentSize();
    label->setPosition(ccp(size.width*0.5f , size.height * 0.5f));
    //pvpMatch->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(- size.width * 0.5f, -size.height*0.6f)));
    pvpMatch->setPosition( ccpAdd(SGLayout::getPoint(kUpCenter) , ccp(0 , -size.height * 0.5) ));

    pvpMatch->addChild(label);
    this->addChild(pvpMatch, 999);
    pvpMatch->setVisible(false);
    
    
    std::string str;
    str = CCUserDefault::sharedUserDefault()->getStringForKey("isplaymusic");
    int isplay = CCString::create(str)->intValue();
    SET_MUSIC_ON(isplay == 0);
    std::string str1;
    str1 = CCUserDefault::sharedUserDefault()->getStringForKey("isplayeffect");
    int isplay1 = CCString::create(str1)->intValue();
    SET_EFFECT_ON(isplay1==0);

    MUSIC_PLAY(MUSIC_SCENE);
    
    usedMemory = NULL;
    availableMemory = NULL;
#if SHOW_MEM_INFO
    float fontSize = 8.0f;
    CCLabelTTF *version = CCLabelTTF::create(CCString::createWithFormat("version>:%s", BUNDLE_VERSION)->getCString(), "", FONTSIZE(fontSize));
	version->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width / 2, CCDirector::sharedDirector()->getWinSize().height - 50));
    version->setColor(ccGREEN);
    int um = GameConfig::usedMemory();
    CCString *mo = CCString::createWithFormat("usedMemory: %d M", um);
    usedMemory = CCLabelTTF::create(mo->getCString(), "", FONTSIZE(fontSize));
    usedMemory->setPosition(ccpAdd(version->getPosition(), ccp(0, - usedMemory->getContentSize().height + 18)));
    usedMemory->setColor(ccGREEN);
    int am = GameConfig::availableMemory();
    CCString *amo = CCString::createWithFormat("availableMemory: %d M", am);
    availableMemory = CCLabelTTF::create(amo->getCString(), "", FONTSIZE(fontSize));
    availableMemory->setPosition(ccpAdd(usedMemory->getPosition(), ccp(0, -availableMemory->getContentSize().height + 30)));
    availableMemory->setColor(ccGREEN);
    
    CCMenuItemLabel *item = CCMenuItemLabel::create(CCLabelTTF::create(str_MainLayer_str37, "", FONTSIZE(fontSize)), this, menu_selector(SGMainScene::showAllTextureCache));
    item->setPosition(ccpAdd(availableMemory->getPosition(), ccp(0, -item->getContentSize().height)));
    CCMenu *menu = CCMenu::create(item, NULL);
    menu->setPosition(CCPointZero);

#endif
	
#if (PLATFORM == ANDROID)
    SdkManager::shareSdkManager()->setSdkHandle(SdkHandler::shareSdkHandler());
#endif
    
    //初始化心跳检测。
    SGHeartbeatDetection *hd = SGHeartbeatDetection::shareHeartbeatDetection();
//    hd->setIsHbdWorking(true);
    this->addChild(hd);
    CCLOG("MY HEART WILL GO ON.");
    return true;
}
                                                    
void SGMainScene::showAllTextureCache()
{
    CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
}

void SGMainScene::upDateMemory()
{
    int um = GameConfig::usedMemory();
    CCString *mo = CCString::createWithFormat("usedMemory: %d M", um);
    usedMemory->setString(mo->getCString());
    int am = GameConfig::availableMemory();
    CCString *amo = CCString::createWithFormat("availableMemory: %d M", am);
    availableMemory->setString(amo->getCString());
    if (am <= 10) {
        usedMemory->setColor(ccRED);
        availableMemory->setColor(ccRED);
    }else{
        usedMemory->setColor(ccGREEN);
        availableMemory->setColor(ccGREEN);
    }
}

void SGMainScene::showPvpMatch(bool visible)
{
    pvpMatch->stopAllActions();
    if (visible == true)
    {
        pvpMatch->setVisible(true);
        pvpMatch->runAction(CCRepeatForever::create(CCBlink::create(0.7 ,1) ));
    }
    else
    {
        pvpMatch->setVisible(false);
    }
}

void SGMainScene::resetMatchVisible(CCObject *sender)
{
    pvpMatch->setVisible(true);
}

//弹出黑条提示
void SGMainScene::showMessage(const char *messageStr)
{
    if (!message)
    {
        message = SGMessageLayer::create();
        this->addChild(message, INT16_MAX);
    }
    message->showMessage(messageStr);
}

//显示加载动画。
void SGMainScene::showLoading(bool bl)
{
    if (!conLayer_)
    {
        conLayer_ = SGConnectLoading::create();
        this->addChild(conLayer_, 88888888);
    }
    conLayer_->setisshow(bl);
}

void SGMainScene:: showEffect(SGEquipCard *card,int type){

     SGGeneralInfoLayer *baseLayer = (SGGeneralInfoLayer*)this->getChildByTag(sg_generalInfoLayer);
    if (baseLayer) {
        baseLayer->changeEquipEffectLayer->showEffectall(card,type);
    }
    
}

void SGMainScene::welcome()
{
    //可能会出现RUC（建议更新客户端BOX）。
    EFFECT_PLAY(MUSIC_BTN);
    //关闭更新UI。
    SGUpdateManager::gi()->switchUpdateLayer(false);

    SGWelComeLayer *welComeLayer = SGWelComeLayer::create();
    this->addShowLayer(welComeLayer);
}

void SGMainScene::initNotifications()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->isPostData = true;
    notification->addObserver(NC_NETWORK_LINK_SUCCESS, this, callfuncO_selector(SGMainScene::socketConnSuccess));
    notification->addObserver(NC_NETWORK_LINK_FAIL, this, callfuncO_selector(SGMainScene::socketConnFail));
    notification->addObserver(NC_NETWORK_RECEIVE_ERROR, this, callfuncO_selector(SGMainScene::socketRecvError));
    notification->addObserver(NC_NETWORK_RECEIVE_BREAK, this, callfuncO_selector(SGMainScene::socketRecvBreak));
    notification->addObserver(MSG_MAIN_LOGIN, this, callfuncO_selector(SGMainScene::socketRegListener));
    notification->addObserver(MSG_MAIN_CREATEROLE, this, callfuncO_selector(SGMainScene::createRoleListener));
    notification->addObserver(MSG_REWARD_REGPLAERS, this, callfuncO_selector(SGMainScene::rewardRegListener));
    //notification->addObserver(MSG_DAYTASK_FINISH, this, callfuncO_selector(SGMainScene::taskFinishListener));
    notification->addObserver(MSG_ANNOUNCEMSG, this, callfuncO_selector(SGMainScene::announceListener));
    notification->addObserver(MSG_STOP_SERVICE,this,callfuncO_selector(SGMainScene::stopServiceMsgProcess) );
    notification->addObserver(MSG_SHARE_REWARD,this,callfuncO_selector(SGMainScene::shareRewardProcess) );
    SGBattleManager::sharedBattle()->initNotification();
    notification->addObserver(MSG_UPDATEBAGINF,
                              this,
                              callfuncO_selector(SGMainScene::upDateBagInfoListener));
    notification->addObserver(MSG_MAIL_REFRESHSYSTEMMAIL,
                              this, callfuncO_selector(SGMainScene::updateMailListener));
    notification->addObserver(MSG_UPDATE_MAINTASK,
                              this, callfuncO_selector(SGMainScene::updateMainTaskListener));
    notification->addObserver(MSG_STARTBOSSTIME, this, callfuncO_selector(SGMainScene::upDateBossStartState));
    
    notification->addObserver(MSG_FRIEND_GETPWOWERGIFT, this, callfuncO_selector(SGMainScene::addGiftPower));
    
    
}

void SGMainScene::alertViewClickedButtonAtIndex(int buttonIndex)
{
    
}
void SGMainScene::startLoading()
{
//    SGLoadLayer *loadLayer = (SGLoadLayer *)this->getChildByTag(sg_loadLayer);
//    
//    loadLayer->updateShowMsg("start loading...");
//    loadLayer->loadFinish();
}

void SGMainScene::socketConnSuccess()
{
    CCLOG("socketConnSuccess");

    
    bool pvpLogin = CCUserDefault::sharedUserDefault()->getBoolForKey("pvp_tran");
    std::string fId = CCUserDefault::sharedUserDefault()->getStringForKey("pvp_fightId");
    
    std::string username = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    if(!strcmp(username.c_str(), ""))
    {
        username = GameConfig::getUdit();
    }
   
    std::string loginKey =CCUserDefault::sharedUserDefault()->getStringForKey("loginKey");
    std::string password = CCUserDefault::sharedUserDefault()->getStringForKey("password");
	
	//设置sdk帐户信息
	std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
	std::string nickName = CCUserDefault::sharedUserDefault()->getStringForKey("nickName");
	int channelId = CCUserDefault::sharedUserDefault()->getIntegerForKey("channelId");
	if (channelId == 0)//渠道未知,重新获取
	{
		channelId = ExtClassOfSDK::sharedSDKInstance()->getChannelId();
        CCUserDefault::sharedUserDefault()->setIntegerForKey("channelId", channelId);
        CCUserDefault::sharedUserDefault()->flush();
	}
    
    //MMDEBUG: LOGIN 这里截断channelId的设置，IOS=1，ADR=2，以后所有channelId将会统一处理。目前其他地方可能使用各种奇怪的channelId，但是暂时无法考证。
    channelId = GlobalConfig::gi()->getRealChannelId();
    CCUserDefault::sharedUserDefault()->setIntegerForKey("channelId", channelId);
    
    
	int subChannelId = CCUserDefault::sharedUserDefault()->getIntegerForKey("subChannelId");
	
    if(!strcmp(password.c_str(), ""))
    {
        password = GameConfig::getUdit();
    }
     CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", true);
    CCUserDefault::sharedUserDefault()->flush();
    short sid = (short)CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
    if(pvpLogin == false)
    {
        main::LoginRequest *lr = new main::LoginRequest();
		
        lr->set_loginkey(loginKey);
		lr->set_channelid(channelId);
		lr->set_subchannelid(subChannelId);
		lr->set_nickname(nickName);
		lr->set_userid(userId);
        lr->set_serverid(sid);
		
        std::string token = CCUserDefault::sharedUserDefault()->getStringForKey("deviceToken");
        lr->set_token(token);
 
        SGSocketClient::sharedSocketClient()->send(MSG_MAIN_LOGIN, lr, true, true);
    }else {
        DCLog("PVP------------------------Login");
        int channelId = CCUserDefault::sharedUserDefault()->getIntegerForKey("channelId");
        std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
        main::PvPLoginRequest *req = new main::PvPLoginRequest();
        req->set_channelid(channelId);
        req->set_accountid(userId);
        req->set_serverid(sid);
        req->set_fightid(fId);
        req->set_type(SGPlayerInfo::sharePlayerInfo()->getBattleType());
        req->set_roleid(SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId());
        SGSocketClient::sharedSocketClient()->send(MSG_PVPFIGHT_LOGIN, req);
        
        CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_tran", false);  //重置PVP标签
        CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_retran", true); //PVP结束后重新登陆
        
//        SGMainScene *mainScene =  SGMainManager::shareMain()->getMainScene();
//        mainScene->removeChildByTag(mainScene->nowLayerTag, true); // 先删掉，战斗结束后刷新
//        mainScene->removeChildByTag(sg_mainLayer, true);
        CCUserDefault::sharedUserDefault()->flush();
        
        CCAction *action = CCSequence::create(CCDelayTime::create(15.0f),
                                              CCCallFunc::create(this, callfunc_selector(SGMainScene::pvpLoginFail)),
                                              NULL);
        this->runAction(action);
    }
    
    SGHeartbeatDetection *hd = SGHeartbeatDetection::shareHeartbeatDetection();
    hd->setIsHbdWorking(true);
}

void SGMainScene::pvpLoginFail()
{
    if(SGBattleManager::sharedBattle()->isMatchSucc == true) return;
    
    SGMainManager::shareMain()->closeBox();
    
    SGCantAdvanceBox *box = SGCantAdvanceBox::create(NULL, NULL, 37, 0);
    SGMainManager::shareMain()->showBox(box);
}

void
SGMainScene::socketConnFail()
{
    //连接失败。
    SGSocketClient::sharedSocketClient()->stopSocketConn(SGNET_CANNOT_REACH_SERVER);
}

//接收数据错误
void SGMainScene::socketRecvError()
{
    SGSocketClient::sharedSocketClient()->stopSocketConn(SGNET_RECEIVE_ERROR);
}

//接收数据中断
void SGMainScene::socketRecvBreak()
{
    SGSocketClient::sharedSocketClient()->stopSocketConn(SGNET_RECEIVE_BREAK);
}

void SGMainScene::initRoleInfo(main::RoleResponse msg)
{
    //获取单例。请参考SGLogicManager头文件注释。
    SGLogicManager::gi();
    
    if (&msg)
    {
        SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();

        //清除所有数据，避免掉线初始化时再次加进重复数据
        playerInfo->getOfficerCards()->removeAllObjects();
        playerInfo->getSoldierCards()->removeAllObjects();
        playerInfo->getEquipCards()->removeAllObjects();
        playerInfo->getPropCards()->removeAllObjects();
		playerInfo->getConsumableCards()->removeAllObjects();
		//去掉所有的碎片信息
		playerInfo->getPiecesCards()->removeAllObjects();
		playerInfo->getPiecesCommonCards()->removeAllObjects();
		playerInfo->setPlayerRoleId(msg.roleid());
        playerInfo->setPlayerName(CCString::create(msg.name()));
        playerInfo->setNickName(CCString::create(msg.nickname()));
        playerInfo->setPlayerSsid(CCString::create(msg.ssid()));
        playerInfo->setPlayerExp(msg.exp());
        playerInfo->setPlayerLevel(msg.level());
        playerInfo->setPlayerCoins(msg.coins());
//        playerInfo->setPlayerSilver(msg.silver());
        playerInfo->setPlayerGold(msg.gold());
        playerInfo->setPlayerCountry(msg.country());
        playerInfo->setPlayerImageID(msg.image());
        playerInfo->setPlayerPower(msg.power());
        playerInfo->setPlayerBagSize(msg.bagsize());
        playerInfo->setFriendSize(msg.friendsize());
        playerInfo->setExpandCount(msg.expandcount());
//        playerInfo->setSpeakerCount(msg.speakercount());
//        playerInfo->setplayerAward(msg.award());
        playerInfo->setPlayerModifyCount(msg.modifynicknamecount());
        playerInfo->setPveCircleTime(msg.pvecircletime());
        playerInfo->setPveLeftTime(msg.pvelefttime());
        playerInfo->setPvpCircleTime(msg.pvpcircletime());
        playerInfo->setPvpLeftTime(msg.pvplefttime());
        playerInfo->setPvpPower(msg.pvppower());
        playerInfo->setis9Show(msg.haslottery9());
        playerInfo->setLoginChance(msg.lottery9chance());
        playerInfo->setLoginDays(msg.loginday());
        
        //for test
        playerInfo->setGuideId(msg.beginnersguideid());
        
        
        playerInfo->setFreevisit(msg.hasfreelottery());
//        playerInfo->setIsFirstFour(msg.newhandpross());
        playerInfo->setplayerVipLevel(msg.viplevel());
        playerInfo->setplayerPvpLevel(msg.pvptoplevel());
        playerInfo->setplayerPvpNum(msg.pvptopscore());
        
        playerInfo->setHasPveFight(msg.haspvefight());
        
        playerInfo->setMilitaryRank(msg.pvprank());
        playerInfo->setMilitaryRankName(CCString::create(msg.rankname()));
        playerInfo->setLoginAwardGold(msg.loginawardgold());
        playerInfo->setCanRewardDayTask(msg.canrewarddaytask());
        playerInfo->setInvitationCode(CCString::create(msg.invitecode()));
        playerInfo->setSearchfriendcode(CCString::create(msg.searchfriendcode()));
        playerInfo->setShowvip(msg.showvipstate());
//        playerInfo->setShowvip(1);
        playerInfo->setAddPlayerGovering(msg.addgoverning());
        playerInfo->setcombatValue(msg.combatvalue());
        playerInfo->setBattleType(0);
        playerInfo->setPvpWinPvp(msg.pvpwinpvp());
        playerInfo->setWinPvp(msg.enternewpvpwin());
        playerInfo->setPlayerliandan(msg.liandangold());
        playerInfo->setHaveCharged(msg.havecharged() == 1);
  
        //
        playerInfo->setBestEquip(msg.bestequip());
        playerInfo->setJunHun(msg.junhun());//军魂
        
        //玩家当前已打过的关卡ID设置，普通/精英。
        playerInfo->setMaxPlotId(msg.maxplotid());
        playerInfo->setMaxBossPlotId(msg.maxbossplotid());
        //活动是不是显示红点提示
        playerInfo->setIsActiviNotAccess(msg.activitynotice());
        //加入勾玉信息
        playerInfo->setPlayerJewels(msg.gouyu());
        //加入西域商人信息
        SGExoticMerchantLayer::setIsForcedOpen(msg.xiyustate() > 0);
        SGExoticMerchantLayer::setNoticeTime(msg.xiyutime());
        
        
        
        //客户端继续战斗参数处理
        playerInfo->setIsNeedVeriFy(msg.needverify());
        SGBattleManager::sharedBattle()->setIsBattlePve(!msg.needverify());
        if ( playerInfo->getHasPveFight() && !msg.needverify())
        {
            CCDictionary *totalSbsDic = CCDictionary::createWithContentsOfFileThreadSafe(SGBattleManager::sharedBattle()->getClientSaveBattleDataName());
            CCString *severBattleId = CCString::createWithFormat("%lld",msg.fpid());
            
            if (totalSbsDic == NULL|| !severBattleId->isEqual(totalSbsDic->objectForKey(BATTLESID)))
            {
                //客户端没有该玩家的战斗保存，不继续战斗
                playerInfo->setHasPveFight(false);
                SGBattleManager::sharedBattle()->setIsBattlePve(false);

            }
        }
        
        
        //设置上次未完成战斗类型
        SGBattleManager::sharedBattle()->setBattleType((BATTLE_TYPE) msg.branchtype());
 

        
        HelpFriendPlayLev = playerInfo->getPlayerLevel();
        
        //清理已有文件
        if(playerInfo->getFriendMails()->count())
            playerInfo->getFriendMails()->removeAllObjects();
        if(playerInfo->getSystemMails()->count())
            playerInfo->getSystemMails()->removeAllObjects();
        if(playerInfo->getGMMails()->count())
            playerInfo->getGMMails()->removeAllObjects();
        
        //好友邮件
        int friendMailCount = msg.mainmails_size();
        for(int i=0;i<friendMailCount;i++)
        {
            playerInfo->getFriendMails()->addObject(SGMailManager::getMailDataModelByMailProto(msg.mainmails(i)));
        }
        
        //系统邮件
        int systemMailCount = msg.systemmails_size();
        for(int i=0;i<systemMailCount;i++)
        {
            playerInfo->getSystemMails()->addObject(SGMailManager::getSystemMailByProto(msg.systemmails(i)));
        }
        
        //GM邮件
        int gmMailCount = msg.gmmails_size();
        for(int i=0;i<gmMailCount;i++)
        {
            playerInfo->getGMMails()->addObject(SGMailManager::getMailDataModelByMailProto(msg.gmmails(i)));
        }
        
        
        if (msg.haslottery9())
        {

            int length = msg.lotteryitems_size();
            for (int i = 0; i<length; i++)
            {
                main::Lottery9ViewItem item = msg.lotteryitems(i);
                SGLottery9ViewItem *lottery9ViewItem = new SGLottery9ViewItem();
                lottery9ViewItem->setItemIDItem(item.itemid());
                lottery9ViewItem->setItemTypeItem(item.itemtype());
                lottery9ViewItem->setItemNum(item.itemnum());
                playerInfo->add9Item(lottery9ViewItem);
                lottery9ViewItem->autorelease();

            }
        }
        int leng = msg.rewardnotice_size();
        playerInfo->getrewardNotice()->removeAllObjects();
        for (int i = 0; i<leng; i++) {
            //CCString *str = CCString::createWithFormat("%d",msg.rewardnotice(i));
            main::RewardTypeState notice = ( main::RewardTypeState)msg.rewardnotice(i);
            SGRewardNoticeItem * noticeItem = new  SGRewardNoticeItem();
            noticeItem->settypeItem(notice.type());
            noticeItem->setstateItem(notice.state());
            playerInfo->addrewardNotice(noticeItem);
        }
        
        //精英闯关次数
        leng = msg.bossplotcount_size();
        for(int i =0 ; i<leng ; ++i)
        {
            main::BossPlotCountProto proto = (main::BossPlotCountProto)msg.bossplotcount(i);
            playerInfo->setCreamData(proto.bossplotid(), proto.canjoincount(), 1);
        }
        
        
        main::BaseBagInfo bagInfo = msg.basebaginfo();
        
        int  officerlen = bagInfo.baseofficercardinfoproto_size();
        for (int i = 0; i<officerlen; i++)
        {
#if DATA_LOADING
            if (i != 0) {
                return;
            }
#endif
            main::OfficerCardItemProto proto = bagInfo.baseofficercardinfoproto(i);
            SGOfficerCard *officerCard = new SGOfficerCard();
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
//            if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
			
			
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1 > 99 ? 100 : proto.currlevel() + 1);

            officerCard->setSsid(proto.ssid());
            officerCard->setItemId(proto.itemid());
            officerCard->setCurrExp(proto.currexp());
            officerCard->setLordId(proto.lordskillid());
            officerCard->setIsProt(proto.isprotection());
            officerCard->setCurrLevel(proto.currlevel());
            officerCard->setOfficerSkil(proto.generalsskillid());
            officerCard->setSkillExp(proto.generalsskillcurrexp());
            officerCard->setLordExp(proto.lordskillcurrexp());
            //武将攻防血速差异性，这几个属性改由服务器发送
            officerCard->setRealAtk(proto.atk());
            officerCard->setRealDef(proto.def());
            officerCard->setRealMorale(proto.hp());
            officerCard->setRealSpeed(proto.sp());
            
            //剩余潜力点值
            officerCard->setDevPoint(proto.leftdevpoint());
            //是否保存培养
            officerCard->setHasSvaedDev(proto.saveddev());
            
            if (temp)
            {
                if (temp->getOfficerNextId()) {
                officerCard->setCost(temp->getOfficerCost());

//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
            }
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
            officerCard->setMaxExp(expdata->getExpValue());
            officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
            officerCard->setOfficerName(temp->getOfficerName());
            officerCard->setMaxLevel(temp->getOfficerMaxLevel());
            officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            officerCard->setRound(temp->getOfficerRound());
            officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            officerCard->setGoverning(temp->getOfficerGoverning());
            officerCard->setRace(temp->getOfficerRace());
            officerCard->setItemType(temp->getOfficerType());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            //转生次数
            officerCard->setAdNum(temp->getAdvNum());
            officerCard->setFateList(temp->getOfficerFateList());
            
            //武将原型ID
            officerCard->setProtoId(temp->getOfficerProtoId());
            //速度修正系数
            officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
			
			officerCard->setHeadSuffixNum(temp->getIconId());//12.05

            if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
            {
                std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                 officerCard->setLordSkill(atoi(lordskill.c_str()));            
            }
            
            playerInfo->addOfficerCard(officerCard);
            }
            officerCard->autorelease();
        }
        
        int equiplen = bagInfo.baseequipmentcarditemproto_size();
        for(int i = 0; i<equiplen; i++)
        {
#if DATA_LOADING
            if (i != 0) {
                return;
            }
#endif
            main::EquipmentCardItemProto cardInfo = bagInfo.baseequipmentcarditemproto(i);
            SGEquipCard *equipCard = new SGEquipCard();
            CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());

            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
            if (expdata==NULL) {
                expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel());
            }
           
            equipCard->setSsid(cardInfo.ssid());
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            equipCard->setItemId(cardInfo.itemid());
//            equipCard->setSetNum(cardInfo.setnum());
            equipCard->setCurrLevel(cardInfo.currlevel());
            //equipCard->setCurrExp(cardInfo.currexp());
            equipCard->setOfficerCardId(cardInfo.officercarditemid());
            equipCard->setIsProt(cardInfo.isprotection());
            if (cardInfo.officercarditemid() >0)
            {
                SGOfficerCard *officer = playerInfo->getOfficerCard(cardInfo.officercarditemid());
                if (officer)
                {
                    officer->addEquip(equipCard);
                }
            }
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            equipCard->setMaxExp(expdata->getExpValue());
            equipCard->setRound(temp->getEquipRound());
            equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            equipCard->setExpRate(temp->getEquipExpRate());
            equipCard->setCurrStar(temp->getEquipCurrStarLevel());
            equipCard->setGoverning(temp->getEquipGoverning());
            equipCard->setItemType(temp->getEquipType());
            equipCard->setMaxStar(temp->getEquipNextId());
            equipCard->setOfficerName(temp->getEquipName());
            equipCard->setMaxLevel(temp->getEquipMaxLevel());
			equipCard->setMaterialDependencyId(temp->getMaterialDependencyId());
            if (temp->getFateDesc())
            {
                equipCard->setFateDesc(temp->getFateDesc());
            }
			equipCard->setHeadSuffixNum(temp->getIconId());//12.06
			
            equipCard->setSellPrice(cardInfo.cost());
            if (temp->getEquipNextId()) {
                equipCard->setCost(temp->getEquipCost());
                

            }

            playerInfo->addEquipCard(equipCard);
            equipCard->autorelease();
        }
        int soldierlen = bagInfo.basesoldiercarditemproto_size();
        for(int i = 0; i<soldierlen; i++)
        {
#if DATA_LOADING
            if (i != 0) {
                return;
            }
#endif
            main::SoldierCardItemProto proto = bagInfo.basesoldiercarditemproto(i);
            SGSoldierCard *soldierCard = new SGSoldierCard();
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(proto.itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, proto.currlevel()+1);
 
            soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
            soldierCard->setSsid(proto.ssid());
            soldierCard->setItemId(proto.itemid());
            soldierCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            soldierCard->setMaxExp(expdata->getExpValue());
            soldierCard->setOfficerName(temp->getSoldierName());
            soldierCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            soldierCard->setRound(temp->getSoldierRound());
            soldierCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());           
            soldierCard->setCurrLevel(proto.currlevel());
            soldierCard->setCurrStar(temp->getSoldierStarLevel());
            soldierCard->setCurrExp(proto.currexp());
//            soldierCard->setItemType(proto.itemtype());
            soldierCard->setsoldiertalentskill(temp->getTalentSkill());//士兵增加
            soldierCard->setsoldiergrowskill(temp->getGrowSkill());
            
            playerInfo->addSoldierCard(soldierCard);
            soldierCard->autorelease();
        }
        
        int proplen = bagInfo.basepropscarditemproto_size();
        for(int i = 0; i<proplen; i++)
        {
#if DATA_LOADING
            if (i != 0) {
                return;
            }
#endif
            main::PropsCardItemProto cardInfo = bagInfo.basepropscarditemproto(i);
            SGPropsCard *propCard = new SGPropsCard();
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
            
            propCard->setSsid(cardInfo.ssid());
            propCard->setItemId(cardInfo.itemid());
            propCard->setIsProt(cardInfo.isprotection());
    
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
			propCard->setHeadSuffixNum(prop->getIconId());//12.05
			
            playerInfo->addPropCard(propCard);
            propCard->autorelease();
        }
		
//加入碎片////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int piecesLen = bagInfo.baseofficerfragmentcarditemproto_size();//碎片数量//12.2
		for (int i = 0; i < piecesLen; i++)
		{
#if DATA_LOADING
            if (i != 0) {
                return;
            }
#endif
			main::OfficerFragmentCardItemProto piecesInfo = bagInfo.baseofficerfragmentcarditemproto(i);
			SGPiecesCard *piecesCard = new SGPiecesCard();
			SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(piecesInfo.itemid());
			piecesCard->setSsid(piecesInfo.ssid());//ssid
			piecesCard->setItemId(piecesInfo.itemid());//碎片id
			piecesCard->setPiecesCurNum(piecesInfo.curritemnumber());// 碎片数量

			
			piecesCard->setSell(pieces->getPiecesPrice());//价格
			piecesCard->setOfficerName(pieces->getPiecesName());//名称
			piecesCard->setPiecesName(pieces->getPiecesName());
			piecesCard->setItemType(pieces->getPiecesType());//类型
			piecesCard->setCurrLevel(pieces->getStarLevel());//星级
			piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
			piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
			piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
			piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
			piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
			piecesCard->setCurrStar(pieces->getStarLevel());
            
			//
//			if (piecesInfo.itemid() == 340000)//如果碎片为通用碎片
			if (pieces->getCallingCardId() == -1)//通用碎片
			{
				playerInfo->addPiecesCommonCard(piecesCard);
				continue;
			}
			
			playerInfo->addPiecesCard(piecesCard);
			piecesCard->autorelease();
		}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//加入道具(消耗品)
		int consumeLen = bagInfo.baseconsumablecarditemproto_size();
		for (int i = 0; i < consumeLen; i++)
		{
#if DATA_LOADING
            if (i != 0) {
                return;
            }
#endif
			
			main::ConsumableCardItemProto consume = bagInfo.baseconsumablecarditemproto(i);
			SGConsumableCard *consumeCard = new SGConsumableCard();
			SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(consume.itemid());
			
            if (consumeModel) {
                consumeCard->setCurrNum(consume.curritemnumber());
                consumeCard->setSsid(consume.ssid());
                consumeCard->setItemId(consume.itemid());
                
                consumeCard->setSell(consumeModel->getConsumePrice());
                consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
                consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
                consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
                consumeCard->setOfficerName(consumeModel->getConsumeName());
                consumeCard->setMoneyType(consumeModel->getMoneyType());
                consumeCard->setItemType(consumeModel->getConsumeType());
                consumeCard->setHeadSuffixNum(consumeModel->getIconId());
                consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
                consumeCard->setPicId(consumeModel->getConsumePicId());
                
                playerInfo->addConsumableCard(consumeCard);
            }
			consumeCard->autorelease();
		}

        //加入材料
		int materialLen = bagInfo.materialitemproto_size();
		for (int i = 0; i < materialLen; i++)
		{
#if DATA_LOADING
            if (i != 0) {
                return;
            }
#endif
			
			main::MaterialItemProto materialInfo = bagInfo.materialitemproto(i);
			SGMaterialCard *materialCard = new SGMaterialCard();
			SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(materialInfo.itemid());
			
            if (materialModel)
            {
                materialCard->setSsid(materialInfo.ssid());
                materialCard->setMaterialCurrNum(materialInfo.itemnum());
                materialCard->setItemId(materialInfo.itemid());
                
                SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(materialInfo.itemid());
                
                materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
                materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
                materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
                materialCard->setOfficerName(materialModel->getMaterialName());
                materialCard->setSell(materialModel->getMaterialSoldPrice());
                playerInfo->addMaterialCard(materialCard);
            }
			materialCard->autorelease();
		}

    
        playerInfo->getFriendList()->removeAllObjects();
        playerInfo->getApplyList()->removeAllObjects();
        playerInfo->getBlackList()->removeAllObjects();        
        
        int friendListCount = msg.friendlist_size();
        for(int i=0;i<friendListCount;i++)
        {
            main::FriendProto _friend = msg.friendlist(i);
            SGGamePlayer *player = this->getSGGamePlayer(_friend);
            playerInfo->addFriend(player);
        }
        
        int blackListCount = msg.blocklist_size();
        for(int i=0;i<blackListCount;i++)
        {
            main::FriendProto _black = msg.blocklist(i);
            SGGamePlayer *player = this->getSGGamePlayer(_black);
            playerInfo->addBlack(player);
        }
        
        int applyListCount = msg.applylist_size();
        for(int i=0;i<applyListCount;i++)
        {
            main::FriendProto _apply = msg.applylist(i);
            SGGamePlayer *player = this->getSGGamePlayer(_apply);
            playerInfo->addApply(player);
        }
        
        int triggerInfoSize = msg.triggerinfo_size();
        playerInfo->m_limitGuideTags.clear();
        for(int i=0;i<triggerInfoSize;i++)
        {
            main::TriggerInfo triggerGuideinfo = msg.triggerinfo(i);
            limitGuideData guideData(triggerGuideinfo.triggertype(),triggerGuideinfo.triggervalue(),triggerGuideinfo.id(),triggerGuideinfo.triggername());
            playerInfo->m_limitGuideTags.push_back(guideData);
        }
////////////kanata
        int qalistSize = msg.qalist_size();
        
        for(int i=0;i<qalistSize;i++)
        {
            main::QAProto qalist= msg.qalist(i);
            SGQAConcent * content=this->getSGQAContent(qalist);
            playerInfo->addQAContent(content);
////
        }
        
        
//////////kanata
        
        SGGuideManager *smgr = SGGuideManager::shareManager();
        
        int guideId = playerInfo->getGuideId();
        smgr->resetGuideId(guideId);
   
        playerInfo->checkIsSkipDayPlaying();
        playerInfo->setPowerGiftTimes(msg.powergifttimes());
        
        
        main::PowerGiftListResponse powerGiftlist = msg.powergift();
        int giftPowerCount = powerGiftlist.powergift_size();
        for(int i=0;i<giftPowerCount;i++)
        {
            main::PowerGiftProto _giftPower = powerGiftlist.powergift(i);
            SGGiftPower *giftPower = this->getSGGiftPower(_giftPower);
            playerInfo->addFriendPower(giftPower);
        }
        
        
        
        main::TeamInfo info =msg.teaminfo();
        
        int teaminfolen = info.teaminfoitem_size();
        for(int i = 0; i<teaminfolen; i++)
        {
            main::TeamInfoItem team = info.teaminfoitem(i);
            SGTeamgroup::shareTeamgroupInfo()->setteamId(i, team.teamid());
            SGTeamgroup::shareTeamgroupInfo()->setlordId(i,team.lordofficercardid());
            SGTeamgroup::shareTeamgroupInfo()->setsoldierblue(i,team.bluesoldier());
            SGTeamgroup::shareTeamgroupInfo()->setsoldiergreen(i,team.greensoldier());
            SGTeamgroup::shareTeamgroupInfo()->setsoldierred(i,team.redsoldier());
            SGTeamgroup::shareTeamgroupInfo()->setiscurrent(i,team.current());
            int length = team.officerlist_size();
            for(int j= 0 ;j<length;j++)
            {
                
                SGTeamgroup::shareTeamgroupInfo()->setEmbattleOfficers(i,j,team.officerlist(j));
            }
            
        }
        int nSize = msg.funcnoticelist_size();
        for(int i = 0 ;i < nSize;i+=4)
        {
            int nid = msg.funcnoticelist(i);
            int nTime = msg.funcnoticelist(i+1);
            int nLevel = msg.funcnoticelist(i+2);
            int nRate = msg.funcnoticelist(i+3);
            if(nid ==1&&nTime!=-1)
            {
                SGPlayerInfo::sharePlayerInfo()->setNetbattleLimit(nLevel);
                SGPlayerInfo::sharePlayerInfo()->setPvPRewardRate(nRate);
                
                CCLOG("====> start timer 天梯三倍开放提示 ===> nLevel = [%d] nRate = [%d] nTime = [%d]", nLevel, nRate, nTime);
                m_pvpDelay = (float)nTime;
                unschedule(schedule_selector(SGMainScene::OnPvpTimeOver));
                schedule(schedule_selector(SGMainScene::OnPvpTimeOver) , 1);
            }
            else if(nid ==2&&nTime!=-1)
            {
                SGPlayerInfo::sharePlayerInfo()->setSuperFubenLimit(nLevel);
                m_bossDelay = (float)nTime;
                unschedule(schedule_selector(SGMainScene::OnBossTimeOver));
                schedule(schedule_selector(SGMainScene::OnBossTimeOver) , 1);
            }
        }
        
        int nSkipCount = msg.plotharrycount();
        SGPlayerInfo::sharePlayerInfo()->setSkipFightCount(nSkipCount);
        //竞技场币(征服点)
        int nArenaCoin = msg.arenacoin();
        playerInfo->setPlayerArenaScore(nArenaCoin);
    }
    
    //updateSecond();
    //初始化完所有角色信息，说明登入了角色这里进行appflyer跟踪
    std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");

    //应该仅有台湾go2play版本才有这些广告。
#if (PLATFORM == IOS)
    OcCaller::getInstance()->appsFlyerLoginTrack(userId);
#elif (PLATFORM == ANDROID)
    AndroidSDKAdp::getInstance()->callAppsFlyerLoginTracker(userId);
#endif
}

void SGMainScene::updateSecond()
{
    SGUpdateManager* sgum = SGUpdateManager::gi();
    
    //判断大包完成的状态。
    if ( sgum->getLocalIsBigReady() )
    {
        //如果走到这里，尚未重启过，并且新手引导已完成，则必须重启一次。
        if ( !sgum->getLocalIsRelaunchDone() )
        {
            int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
            if (guideId >= 46)
            {
                //重启弹框开启之前，必须将“已完成重启”标识设置为true。
                CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
                ccud->setBoolForKey(SGUM_LOCAL_IS_RELAUNCH_DONE_KEY, true);
                ccud->flush();
                SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 68, 0, NULL);
                SGMainManager::shareMain()->showBox(cantadvanceBox);
            }
        }
    }
    else
    {
        int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        if( guideId >= 46 ) //新手引导已完成则必须强制更新。
        {
            SGUpdateManager::gi()->switchUpdateLayer(true);
        }
        else //新手引导未完成则可以继续。
        {
            ;
        }

    }
    
    //old code
//    bool isFinishUpdateAllRes = CCUserDefault::sharedUserDefault()->getBoolForKey("isFinishUpdateAllRes");
//    if(!isFinishUpdateAllRes)
//    {
//        int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
//        UpdateLayer* updateLayer =UpdateLayer::create(true);
//        updateLayer->isInMainLayer =true;
//        this->addChild(updateLayer,1000);
//        if(guideId>=46)
//        {
//            updateLayer->setUpdateLayerShow();
//        }
//        else
//        {
//            updateLayer->setVisible(false);
//        }
//        //updateLayer->retain();
//        updateLayer->DownloadResource();
//    }
    
}

//pvp快速入口定时器处理
void SGMainScene::OnPvpTimeOver(float t)
{
    m_pvpDelay -= t/SGLogicManager::gi()->getGameSpeed();
    if(m_pvpDelay > 0)
        return;
    
    unschedule(schedule_selector(SGMainScene::OnPvpTimeOver));
    CCObject * battleLayer = this->getChildByTag(sg_battleLayer);
    if(!battleLayer)
    {
        int level = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
        if(level >=SGPlayerInfo::sharePlayerInfo()->getNetbattleLimit())
        {
            CCLOG("====> start onpvptimerover level = [%d] netbattlelimit = [%d]", level, SGPlayerInfo::sharePlayerInfo()->getNetbattleLimit());
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 66, 0, NULL);
            SGMainManager::shareMain()->showBox(cantadvanceBox);
        }
    }

}
//boss快速入口定时器处理
void SGMainScene::OnBossTimeOver(float t)
{
    m_bossDelay -= t/SGLogicManager::gi()->getGameSpeed();
    if(m_bossDelay > 0)
        return;
    
    unschedule(schedule_selector(SGMainScene::OnBossTimeOver));
    CCObject * battleLayer = this->getChildByTag(sg_battleLayer);
    if(!battleLayer){
        int level = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
        if(level >=SGPlayerInfo::sharePlayerInfo()->getSuperFubenLimit()){
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 67, 0, NULL);
            SGMainManager::shareMain()->showBox(cantadvanceBox);
       }
    }

}
//生成SGGamePlayer对象
SGGamePlayer* SGMainScene::getSGGamePlayer(main::FriendProto _friend)
{
    SGGamePlayer *player = new SGGamePlayer();
    player->setFriendID(_friend.friendid());
    player->setFriendName(CCString::create(_friend.friendname()));
    player->setImage(_friend.image());
    player->setIsOnline(_friend.isonline());
    player->setLevel(_friend.level());
    player->setRoleID(_friend.roleid());
    player->setTime(_friend.time());
    player->setTypeID(_friend.typeid_());
    player->setIsGiftPower(_friend.isgiftpower());
    player->autorelease();
    return player;
    
}
//生成SGGamePlayer对象
SGGiftPower* SGMainScene::getSGGiftPower(main::PowerGiftProto _friend)
{
    SGGiftPower *player = new SGGiftPower();
    player->setRoleID(_friend.roleid());
    player->setGiftId(_friend.giftid());
    player->setFriendName(CCString::create(_friend.giftname()));
    player->setGiftTime(_friend.gifttime());
    player->setLevel(_friend.giftlevel());
    player->setImage(_friend.giftimage());
    
    player->autorelease();
    return player;
    
}
////kanata
SGQAConcent* SGMainScene::getSGQAContent(main::QAProto content)
{
    SGQAConcent* temp=new SGQAConcent();
    temp->setID(content.id());
    temp->setTitle(CCString::create(content.title()));
    temp->setType(content.type());
    
    std::string str(content.createdate());
    CCString* nn=CCString::createWithFormat("%s",str.c_str());
    temp->setcreateDate(nn);
    temp->setState(content.state());
    temp->autorelease();
    return  temp;
    
}




void SGMainScene::socketRegListener(CCObject *obj)
{
    CCLOG("socketRegListener");
    //length:13330
    SGMainManager::shareMain()->closeBox();
    SGAIManager::shareManager()->isInitPve = true;
    bool relogin =  CCUserDefault::sharedUserDefault()->getBoolForKey("pvp_retran");
    if (relogin == true) {
        CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_retran", false);
        CCUserDefault::sharedUserDefault()->flush();
        
        SGSocketRequest *sr = (SGSocketRequest *)obj;
        
        if (sr)
        {
            main::LoginResponse *lr = (main::LoginResponse *)sr->m_msg;
            if (lr->state()>0)
            {
                
               if (lr->hasrole() >0)
                {
                    main::RoleResponse msg = lr->roleresponse();
                    
                    initRoleInfo(msg);
                }
            }
        }
        
        return; // 重新连接原来的服务器，无需刷新数据
    }
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
   
    if (sr)
    {
        main::LoginResponse *lr = (main::LoginResponse *)sr->m_msg;
        if (lr->state()>0)
        {
            //发送登陆成功的消息 add by:zyc.
            SGMainManager::sendOperatorRecord(100030);
            
            if (lr->hasrole() >0)
            {
                
                main::RoleResponse msg = lr->roleresponse();
                
                initRoleInfo(msg);
                
                SGMainManager::shareMain()->showMainLayer();
                SGPlayerInfo::sharePlayerInfo()->setIsCgBattle(false);
                
            }
            else
            {
                
                //cgp gameStart
                //发送 进入游戏  第一个黑屏界面    的消息 add by:zyc.
//                SGMainManager::sendOperatorRecord(100040);
//
                //removed by cgp
//                SGPlayerInfo::sharePlayerInfo()->setCDKeyNeed(lr->cdkey());
//                SGPlayerInfo::sharePlayerInfo()->setIsCgBattle(true);
//                SGCGMovieLayer *layer = SGCGMovieLayer::create();
//                this->addShowLayer(layer);
                
//                SGCountryLayer *layer = SGCountryLayer::create();
//                this->addShowLayer(layer);
//                
//                SGNikeNameBox *nikeNameBox = SGNikeNameBox::create(temp, countryId);
//                SGMainManager::shareMain()->showBox(nikeNameBox);
                
                
                main::CreateRoleRequest *request = new main::CreateRoleRequest();
                request->set_country(2);
                
                const char * nickName = SGStaticDataManager::shareStatic()->getRandomNickName().c_str();
                if(strlen(nickName)==0)
                {
                    printf(str_NikeNameBox_str5);
                    return;
                }
               
                request->set_nickname(nickName);
                SGSocketClient::sharedSocketClient()->send(MSG_MAIN_CREATEROLE, request);
            }
        }
        else
        {
            SG_SHOW_WINDOW(lr->failmsg().c_str());
        }
        
    }
    return;
    
    //MMDEBUG: UPDATE 更新启动，现在机制下不需要了，暂时封存。
    /*
    SGUpdateManager* sgum = SGUpdateManager::gi();
    if (!sgum->getIsAlreadyTriggeredTrue())
    {
        sgum->setIsAlreadyTriggeredTrue();
        sgum->trigger();
    }
    */
}
void SGMainScene::showCountryLayer()
{
    if (getChildByTag(sg_cgMovieLayer)) {
        removeChildByTag(sg_cgMovieLayer,true);
    }
    SGPlayerInfo::sharePlayerInfo()->setIsCgBattle(false);
    SGCountryLayer *layer = SGCountryLayer::create();
    this->addShowLayer(layer);
    
    //新手引导   选择国家   发送消息。
    SGMainManager::sendOperatorRecord(100070);
}

void SGMainScene::createRoleListener(cocos2d::CCObject *obj)
{
    //新手引导   进入游戏界面  开始新手引导
    SGMainManager::sendOperatorRecord(100090);
    
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr) {
        main::CreateRoleResponse *crr = (main::CreateRoleResponse *)sr->m_msg;
        if (crr->state() == 1)
        {
            main::RoleResponse msg = crr->roleresponse();
            initRoleInfo(msg);
            SGMainManager::shareMain()->closeBox();
            SGMainManager::shareMain()->showMainLayer();

        }
        else if(crr->state() == 0)
        {
            CCString *tip = CCString::createWithFormat("%s", crr->errorstr().c_str());
            SG_SHOW_WINDOW(tip->getCString());
        }
    }
	
	//起名字完成， 新手进度
	
	SGMainManager::shareMain()->sendNewPlayerProcess(take_nickname, 0);
}


void SGMainScene::showBattleLayer(SGBaseLayer *layer)
{
    if (layer->getTag() == this->nowLayerTag)
    {
        return;
    }
    
    if (this->getChildByTag(layer->getTag()))
    {
        layer->setIsCanTouch(true);
        layer->setVisible(true);
        layer->refreshView();
    }
    else
    {
        this->addChild(layer);  //modified by
    }
    

    SGBaseLayer *baseLayer = (SGBaseLayer*)this->getChildByTag(this->nowLayerTag);
    if (baseLayer)
    {
        baseLayer->setIsCanTouch(false);
        //baseLayer->setVisible(false);
    }

    
    ResourceManager* rm = ResourceManager::sharedInstance();
    CCLOG( "addShowLayer() add=%s, before=%s", rm->getResLayerNameByTag((LayerTag)layer->getTag()), rm->getResLayerNameByTag(nowLayerTag) );
    if(
       ( this-> nowLayerTag == sg_bossbattledetailLayer
        || this->nowLayerTag == sg_bossBattleJoinInLayer
        ||this->nowLayerTag == sg_bossBattleLayer
        ||this->nowLayerTag==sg_bossRewardLayer
        )
       &&
       (
        (LayerTag)layer->getTag() != sg_bossbattledetailLayer
        && (LayerTag)layer->getTag() != sg_bossBattleLayer
        && (LayerTag)layer->getTag() != sg_bossBattleJoinInLayer
        && (LayerTag)layer->getTag() != sg_bossRewardLayer
        )
       )
    {
        MUSIC_STOP;
        MUSIC_PLAY(MUSIC_SCENE);
    }
    
    this->nowLayerTag = (LayerTag)layer->getTag();

    
}

void SGMainScene::addShowLayer(SGBaseLayer *layer, bool isBeforeDel)
{
    if (layer->getTag() == this->nowLayerTag)
    {
        return;
    }

    if (layer->getIsCocostudio())   //当前要加入的层是cocostudio
    {
        if (isBeforeDel)
        {
            if (this->nowIsCocostudio)
            {
                m_pUIGroup->removeWidget(mCocosLayers[nowLayerTag - cocostudioLayerStart]);
                mCocosLayers[nowLayerTag - cocostudioLayerStart] = NULL;
                
                //GPCCLOG("delete strengthLayer:  %p,  layer->getTag %d, total %d", SGMainManager::shareMain()->getMainScene()->mCocosLayers[cgp_strengAdvLayer - cocostudioLayerStart],  nowLayerTag, cocostudioLayerStart);
            }
            else
            {
                if (this->getChildByTag(this->nowLayerTag))
                {
                    this->removeChildByTag(this->nowLayerTag, true);
                }
            }

        }
        else
        {
            if (this->nowIsCocostudio)
            {
                mCocosLayers[nowLayerTag - cocostudioLayerStart]->setVisible(false);
                mCocosLayers[nowLayerTag - cocostudioLayerStart]->setEnabled(false);
            }
            else
            {
                SGBaseLayer *baseLayer = (SGBaseLayer*)this->getChildByTag(this->nowLayerTag);
                if (baseLayer)
                {
                    baseLayer->setIsCanTouch(false);
                    baseLayer->setVisible(false);
                }
            }

        }
        this->nowIsCocostudio = true;

    }
    
    else   //不是cocostudio
    {

        if (this->getChildByTag(layer->getTag()))
        {
            layer->setIsCanTouch(true);
            layer->setVisible(true);
            layer->refreshView();
        }
        else
        {
            this->addChild(layer);  //modified by
        }
        
        //删除前层
        if (isBeforeDel)
        {
            //前层是cocostudio
            if(this->nowIsCocostudio)
            {
                m_pUIGroup->removeWidget(mCocosLayers[nowLayerTag - cocostudioLayerStart]);
                mCocosLayers[nowLayerTag - cocostudioLayerStart] = NULL;
                
               // GPCCLOG("delete strengthLayer:  %p,  layer->getTag %d, total %d", SGMainManager::shareMain()->getMainScene()->mCocosLayers[cgp_strengAdvLayer - cocostudioLayerStart],  layer->getTag(), SG_LAYER_TAG_TOTAL_NUM);
            }
            else
            {
                if (this->getChildByTag(this->nowLayerTag))
                {
                    this->removeChildByTag(this->nowLayerTag, true);
                }
            }

        }
        else //不删除前层
        {
            //前层是cocostudio
            if(this->nowIsCocostudio)
            {
                mCocosLayers[nowLayerTag - cocostudioLayerStart]->setVisible(false);
                mCocosLayers[nowLayerTag - cocostudioLayerStart]->setEnabled(false);
            }
            else
            {
                SGBaseLayer *baseLayer = (SGBaseLayer*)this->getChildByTag(this->nowLayerTag);
                if (baseLayer)
                {
                    baseLayer->setIsCanTouch(false);
                    baseLayer->setVisible(false);
                }
            }

        }
        this->nowIsCocostudio = false;
    }
    
    ResourceManager* rm = ResourceManager::sharedInstance();
    CCLOG( "addShowLayer() add=%s, before=%s", rm->getResLayerNameByTag((LayerTag)layer->getTag()), rm->getResLayerNameByTag(nowLayerTag) );
    if(
       ( this-> nowLayerTag == sg_bossbattledetailLayer
        || this->nowLayerTag == sg_bossBattleJoinInLayer
        ||this->nowLayerTag == sg_bossBattleLayer
        ||this->nowLayerTag==sg_bossRewardLayer
        )
       &&
       (
        (LayerTag)layer->getTag() != sg_bossbattledetailLayer
        && (LayerTag)layer->getTag() != sg_bossBattleLayer
        && (LayerTag)layer->getTag() != sg_bossBattleJoinInLayer
        && (LayerTag)layer->getTag() != sg_bossRewardLayer
        )
       )
    {
        MUSIC_STOP;
        MUSIC_PLAY(MUSIC_SCENE);
    }
    
    this->nowLayerTag = (LayerTag)layer->getTag();

}




void SGMainScene::addBattleMatchLayer()
{
    SGBattleMatchLayer *battleMatchLayer = SGBattleMatchLayer::create(NULL, sg_battleMatchLayer);
    this->addChild(battleMatchLayer);
}
void SGMainScene::logCache()
{
    bool pvpLogin = CCUserDefault::sharedUserDefault()->getBoolForKey("pvp_tran");
    std::string fId = CCUserDefault::sharedUserDefault()->getStringForKey("pvp_fightId");
    std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    std::string pw = CCUserDefault::sharedUserDefault()->getStringForKey("password");
 
    short sid = (short)CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
    if(pvpLogin == false)
    {
        main::LoginRequest *lr = new main::LoginRequest();
        std::string username = CCUserDefault::sharedUserDefault()->getStringForKey("randusername");
        if(!strcmp(username.c_str(), ""))
        {
            username = CCUserDefault::sharedUserDefault()->getStringForKey("username");
        }
        std::string loginKey =CCUserDefault::sharedUserDefault()->getStringForKey("loginKey");
        lr->set_loginkey(loginKey);
        short sid = (short)CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
        lr->set_serverid(sid);
        std::string token = CCUserDefault::sharedUserDefault()->getStringForKey("deviceToken");
        lr->set_token(token);
        
        SGSocketClient::sharedSocketClient()->send(MSG_MAIN_LOGIN, lr, true, true);
    }
    else
    {
        DCLog("PVP------------------------Login");
        main::PvPLoginRequest *req = new main::PvPLoginRequest();
        std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
        int channelId = CCUserDefault::sharedUserDefault()->getIntegerForKey("channelId");
        req->set_channelid(channelId);
        req->set_accountid(userId);
        req->set_serverid(sid);
        req->set_fightid(fId);
        req->set_type(SGPlayerInfo::sharePlayerInfo()->getBattleType());
        req->set_roleid(SGPlayerInfo::sharePlayerInfo()->getPlayerRoleId());
        SGSocketClient::sharedSocketClient()->send(MSG_PVPFIGHT_LOGIN, req);
        
        SGMainManager::shareMain()->closeBox();
        
        //        SGMainScene *mainScene =  SGMainManager::shareMain()->getMainScene();
        //        mainScene->removeChildByTag(mainScene->nowLayerTag, true); // 先删掉，战斗结束后刷新
        //        mainScene->removeChildByTag(sg_mainLayer, true);
    }
    
    SGHeartbeatDetection *hd = SGHeartbeatDetection::shareHeartbeatDetection();
    hd->setIsHbdWorking(true);
}

void SGMainScene::rewardRegListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::RewardRegLotteryPush *response = (main::RewardRegLotteryPush *)sr->m_msg;
        SGPlayerInfo::sharePlayerInfo()->getrewardRegPlayers()->addObject(CCString::create(response->name()));
        SGPlayerInfo::sharePlayerInfo()->getrewardRegPlayers()->addObject(CCString::createWithFormat("%d",response->gold()));
    }
}
//void SGMainScene::taskFinishListener(cocos2d::CCObject *obj)
//{
//    SGSocketRequest *sr = (SGSocketRequest *)obj;
//    if (sr)
//    {
//        main::DayTaskFinishResponse *response = (main::DayTaskFinishResponse *)sr->m_msg;
//        if (response->state() == 1) {
//            SGMainManager::shareMain()->showMessage(CCString::createWithFormat("当日任务全部完成 获得%d元宝！",response->count())->getCString());
//            SGPlayerInfo::sharePlayerInfo()->setPlayerGold(SGPlayerInfo::sharePlayerInfo()->getPlayerGold() + response->count());
//            SGMainManager::shareMain()->updataUserMsg();
//        }
//    }
//}
void SGMainScene::shareRewardProcess(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    
    if (sr)
    {
        main::ShareRewardResponse *response = (main::ShareRewardResponse *)sr->m_msg;
        int i = (int)response->state();
        if(i ==1)
        {
            int gold = (int)response->gold();
            int coins = (int)response->coins();
            SGMainManager::shareMain()->showMessage( CCString::createWithFormat(str_SGShareBox_str7,gold,coins)->getCString());
        }
    }

    
}
void SGMainScene::stopServiceMsgProcess(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    
    if (sr)
    {
        main::ServerNoticeResponse *response = (main::ServerNoticeResponse *)sr->m_msg;
        int i = (int)response->type();
        if(i ==1)
        {
           SGMainManager::shareMain()->showMessage( CCString::create(response->content())->getCString());
        }
    }
}
void SGMainScene::announceListener(cocos2d::CCObject *obj)
{
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    CCDictionary *dic = player->getmsgstyleDic();
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (player->getAnnouns()->count()>0) {
        player->getAnnouns()->removeAllObjects();
    }
    
    if (sr)
    {
        main::ActivityMessageInfosResponse *response = (main::ActivityMessageInfosResponse *)sr->m_msg;
        int len = response->activitymessageinfolist_size();
        for (int i = 0; i<len; i++)
        {
            main::ActivityMessageInfoProto cont = response->activitymessageinfolist(i);
            SGAnnouncementDataModel *msg = new SGAnnouncementDataModel();
            msg->setTitle(CCString::create(cont.title()));
            CCLOG("cont.title()==%s",cont.title().c_str());
            msg->setmsgType(cont.typestatus());
            
            for (int a = 0; a<cont.contentlist_size(); a++)
            {
                contentproto *_cont = new contentproto();
                main::ContentProto contpro = cont.contentlist(a);
                CCLOG("contpro.content()==%s",contpro.content().c_str());
                CCLOG("contpro.style()===%s",contpro.style().c_str());
                _cont->setcontent(CCString::create(contpro.content()));
                _cont->setstyle(CCString::create(contpro.style()));
                msg->getcontentArr()->addObject(_cont);
                _cont->autorelease();
            }
            
            for (int b = 0; b<cont.styleinfolist_size(); b++)
            {
                main::StyleInfoProto pro = cont.styleinfolist(b);
                CCLOG("pro.styleksy()==%s",pro.styleksy().c_str());
                CCString *str = (CCString *)dic->objectForKey(pro.styleksy());
                if (str == NULL)
                {
                    CCLOG("pro.stylevalue()==%s",pro.stylevalue().c_str());
                    dic->setObject(CCString::create(pro.stylevalue()), CCString::create(pro.styleksy())->getCString());
                }
            }
            player->getAnnouns()->addObject(msg);
            msg->autorelease();
        }
        
    }

}

void SGMainScene::upDateBagInfoHandler(main::UpdateBagInfo *msg)
{
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    int updateItemSize = 0;
    int state = 0; //物品更新状态，0--删除、1--增加、2--更新
    int ssid = 0; //物品ID
    int oldofficerid=0;//原武将ID
	
    //材料
    updateItemSize = msg->updatematerialitemproto_size();
    
    for (int i = 0; i < updateItemSize; i++)
    {
        main::MaterialItemUpdateProto updateMaterial = msg->updatematerialitemproto(i);
        state = updateMaterial.state();
        ssid = updateMaterial.ssid();
        
        //删除
        if (state == 0)
        {
            playerInfo->removeMaterialCardById(ssid);
        }
        else if (state == 1)//增加
        {
            main::MaterialItemProto materialInfo = updateMaterial.materialitem();
            SGMaterialCard *materialCard = new SGMaterialCard();
            materialCard->autorelease();
            
            materialCard->setSsid(materialInfo.ssid());
            materialCard->setMaterialCurrNum(materialInfo.itemnum());
            materialCard->setItemId(materialInfo.itemid());
            
            SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(materialInfo.itemid());
            
            materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
            materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
            materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
            materialCard->setOfficerName(materialModel->getMaterialName());
            materialCard->setSell(materialModel->getMaterialSoldPrice());
            playerInfo->addMaterialCard(materialCard);
        }
        else if (state == 2)//修改
        {
            main::MaterialItemProto materialInfo = updateMaterial.materialitem();
            SGMaterialCard *materialCard = playerInfo->getMaterialCardById(materialInfo.ssid());
            
            if (materialCard)
            {
                materialCard->setSsid(materialInfo.ssid());
                materialCard->setMaterialCurrNum(materialInfo.itemnum());
                materialCard->setItemId(materialInfo.itemid());
                
                SGMaterialDataModel *materialModel = SGStaticDataManager::shareStatic()->getMaterialById(materialInfo.itemid());
                
                materialCard->setHeadSuffixNum(materialModel->getMaterialIconId());
                materialCard->setMaterialBorderColor(materialModel->getMaterialBorderColor());
                materialCard->setMaterialPileLimit(materialModel->getMaterialPileLimit());
                materialCard->setOfficerName(materialModel->getMaterialName());
                materialCard->setSell(materialModel->getMaterialSoldPrice());
            }
        }
        
    }
    
    
	//消耗品
	updateItemSize = msg->updateconsumablecarditemproto_size();
	for (int i = 0; i < updateItemSize; i++)
	{
		main::ConsumableCardItemUpdateProto updateConsume = msg->updateconsumablecarditemproto(i);
		state = updateConsume.state();
		ssid = updateConsume.ssid();
		if (state == 0)//删除
		{
			playerInfo->removeConsumableCardById(ssid);
		}
		else if (state == 1)//增加
		{
			main::ConsumableCardItemProto consumeInfo = updateConsume.consumablecarditem();
			SGConsumableCard *consumeCard = new SGConsumableCard();
			SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(consumeInfo.itemid());
			consumeCard->setSsid(consumeInfo.ssid());
			consumeCard->setItemId(consumeInfo.itemid());
			consumeCard->setCurrNum(consumeInfo.curritemnumber());
			
			consumeCard->setSell(consumeModel->getConsumePrice());
			consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
			consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
			consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
			consumeCard->setOfficerName(consumeModel->getConsumeName());
			consumeCard->setMoneyType(consumeModel->getMoneyType());
			consumeCard->setItemType(consumeModel->getConsumeType());
			consumeCard->setHeadSuffixNum(consumeModel->getIconId());
			consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
			consumeCard->setPicId(consumeModel->getConsumePicId());
		
			playerInfo->addConsumableCard(consumeCard);
			consumeCard->autorelease();
		}
		else if (state == 2)
		{
			main::ConsumableCardItemProto consumeInfo = updateConsume.consumablecarditem();
			SGConsumableCard *consumeCard = playerInfo->getConsumableCardById(ssid);
			if (consumeCard)
			{
				SGConsumableDataModel *consumeModel = SGStaticDataManager::shareStatic()->getConsumableById(consumeInfo.itemid());
				
				consumeCard->setSsid(consumeInfo.ssid());
				consumeCard->setItemId(consumeInfo.itemid());
				consumeCard->setCurrNum(consumeInfo.curritemnumber());
				
				consumeCard->setSell(consumeModel->getConsumePrice());
				consumeCard->setconsumableDesc(consumeModel->getConsumeDesc());
				consumeCard->setCanUseDriving(consumeModel->getCanUseDriving());
				consumeCard->setCurrStar(consumeModel->getConsumeStarLvl());
				consumeCard->setOfficerName(consumeModel->getConsumeName());
				consumeCard->setMoneyType(consumeModel->getMoneyType());
				consumeCard->setItemType(consumeModel->getConsumeType());
				consumeCard->setHeadSuffixNum(consumeModel->getIconId());
				consumeCard->setLimitAndNum(consumeModel->getLimitAndNum());
				consumeCard->setPicId(consumeModel->getConsumePicId());
				
			}
		}
		
		
	}
	
	
	//碎片
	updateItemSize = msg->updateofficerfragmentcarditemproto_size();
	for (int i = 0; i < updateItemSize; i++)
	{
		main::OfficerFragmentCardItemUpdateProto updateFragmentCard = msg->updateofficerfragmentcarditemproto(i);
		state = updateFragmentCard.state();
        ssid = updateFragmentCard.ssid();
		
		if (state  == 0)//删除
        {
			playerInfo->removePiecesCardById(ssid);
        }
        else if (state == 1)//增加
        {
			main::OfficerFragmentCardItemProto piecesInfo = updateFragmentCard.officerfragmentcarditem();
			SGPiecesCard *piecesCard = new SGPiecesCard();
			SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(piecesInfo.itemid());
			
			piecesCard->setSsid(piecesInfo.ssid());//ssid
			piecesCard->setItemId(piecesInfo.itemid());//碎片id
			piecesCard->setPiecesCurNum(piecesInfo.curritemnumber());// 碎片数量
			
			
			piecesCard->setSell(pieces->getPiecesPrice());//价格
			piecesCard->setOfficerName(pieces->getPiecesName());//名称
			piecesCard->setPiecesName(pieces->getPiecesName());//名称
			piecesCard->setItemType(pieces->getPiecesType());//类型
			piecesCard->setCurrLevel(pieces->getStarLevel());//星级
			piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
			piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
			piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
			piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
			piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
            piecesCard->setCurrStar(pieces->getStarLevel());

			//
			if (piecesInfo.itemid() == 1)//如果碎片为通用碎片
			{
				playerInfo->addPiecesCommonCard(piecesCard);
				continue;
			}
			
			playerInfo->addPiecesCard(piecesCard);
			piecesCard->autorelease();
			
        }
        else if (state == 2)//更新
        {
			main::OfficerFragmentCardItemProto piecesInfo = updateFragmentCard.officerfragmentcarditem();
			SGPiecesCard *piecesCard = playerInfo->getPiecesCardById(ssid);
			
			if (piecesCard)
			{
				SGPiecesDataModel *pieces = SGStaticDataManager::shareStatic()->getPiecesDictById(piecesInfo.itemid());
				piecesCard->setSsid(piecesInfo.ssid());//ssid
				piecesCard->setItemId(piecesInfo.itemid());//碎片id
				piecesCard->setPiecesCurNum(piecesInfo.curritemnumber());// 碎片数量
				
				
				piecesCard->setSell(pieces->getPiecesPrice());//价格
				piecesCard->setOfficerName(pieces->getPiecesName());//名称
				piecesCard->setPiecesName(pieces->getPiecesName());
				piecesCard->setItemType(pieces->getPiecesType());//类型
				piecesCard->setCurrLevel(pieces->getStarLevel());//星级
				piecesCard->setPiecesDesc(pieces->getPiecesDesc());//碎片描述
				piecesCard->setCanUseCommon(pieces->getCommonPieces());//可使用的通用碎片数量
				piecesCard->setCallCardId(pieces->getCallingCardId());//设置碎片合成后召唤的武将id
				piecesCard->setMaxNum(pieces->getMaxPiecesNum());//碎片最大数量
				piecesCard->setSummonNeed(pieces->getcallNeedPieces());//召唤所需要的碎片的数量
                piecesCard->setCurrStar(pieces->getStarLevel());

			}
        }
        else
        {
            continue;
        }
	}
	
    //武将

    updateItemSize = msg->updateofficercarditemproto_size();
    for (int i=0; i<updateItemSize; i++) {
        main::OfficerCardItemUpdateProto updateOfficerCardInfoProto = msg->updateofficercarditemproto(i);
        state = updateOfficerCardInfoProto.state();
        ssid = updateOfficerCardInfoProto.ssid();
        if (state  == 0)
        {
            playerInfo->removeOfficerCardById(ssid);
        }
        else if (state == 1)
        {
            main::OfficerCardItemProto proto = updateOfficerCardInfoProto.officeritem();
            SGOfficerCard *officerCard = new SGOfficerCard();
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
//            if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
            
            officerCard->setSsid(proto.ssid());
            officerCard->setItemId(proto.itemid());
            officerCard->setCurrExp(proto.currexp());
            officerCard->setLordId(proto.lordskillid());
            officerCard->setIsProt(proto.isprotection());
            officerCard->setCurrLevel(proto.currlevel());
            officerCard->setOfficerSkil(proto.generalsskillid());
            officerCard->setSkillExp(proto.generalsskillcurrexp());
            officerCard->setLordExp(proto.lordskillcurrexp());
            
            //武将攻防血速差异性，这几个属性改由服务器发送
            officerCard->setRealAtk(proto.atk());
            officerCard->setRealDef(proto.def());
            officerCard->setRealMorale(proto.hp());
            officerCard->setRealSpeed(proto.sp());
            
            //武将剩余潜力点值
            officerCard->setDevPoint(proto.leftdevpoint());
            //是否保存培养
            //officerCard->setHasSvaedDev(proto.saveddev());
            
            if (temp->getOfficerNextId()) {
                officerCard->setCost(temp->getOfficerCost());
                
//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
            }
            officerCard->setExpRate(temp->getOfficerExpRate());
            officerCard->setMaxStar(temp->getOfficerNextId());
            officerCard->setMaxExp(expdata->getExpValue());
            officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
            officerCard->setOfficerName(temp->getOfficerName());
            officerCard->setMaxLevel(temp->getOfficerMaxLevel());
            officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            officerCard->setRound(temp->getOfficerRound());
            officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            officerCard->setGoverning(temp->getOfficerGoverning());
            officerCard->setRace(temp->getOfficerRace());
            officerCard->setItemType(temp->getOfficerType());
            //转生最大星级
            officerCard->setUpgradestar(temp->getUpgradestar());
            //转生等级
            officerCard->setUpgradelevel(temp->getUpgradelevel());
            //武将计最大等级
            officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
            //主将计最大等级
            officerCard->setLordMaxLevel(temp->getLordMaxLevel());
            //性别和缘分
            officerCard->setGender((OfficerGender) temp->getOfficerGender());
            //转生次数
            officerCard->setAdNum(temp->getAdvNum());
            officerCard->setFateList(temp->getOfficerFateList());
            //武将原型ID
            officerCard->setProtoId(temp->getOfficerProtoId());
            //速度修正系数
            officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
			
			officerCard->setHeadSuffixNum(temp->getIconId());//12.06
            
            if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
            {
                std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                officerCard->setLordSkill(atoi(lordskill.c_str()));
            }
            
            playerInfo->addOfficerCard(officerCard);
            officerCard->autorelease();
        }
        else if (state == 2)
        {
            main::OfficerCardItemProto proto = updateOfficerCardInfoProto.officeritem();
            SGOfficerCard *officerCard = playerInfo->getOfficerCard(ssid);
            if (officerCard) {
                CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
                int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
//                if (temp->getOfficerExpType()->isEqual(CCString::create("A"))) {
//                    type = 0;
//                }
//                else if (temp->getOfficerExpType()->isEqual(CCString::create("B"))) {
//                    type = 1;
//                }
//                else {
//                    type = 2;
//                }
                SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
                
                officerCard->setSsid(proto.ssid());
                officerCard->setItemId(proto.itemid());
                officerCard->setCurrExp(proto.currexp());
                officerCard->setLordId(proto.lordskillid());
                officerCard->setIsProt(proto.isprotection());
                officerCard->setCurrLevel(proto.currlevel());
                officerCard->setOfficerSkil(proto.generalsskillid());
                officerCard->setSkillExp(proto.generalsskillcurrexp());
                officerCard->setLordExp(proto.lordskillcurrexp());
                
                //武将攻防血速差异性，这几个属性改由服务器发送
                officerCard->setRealAtk(proto.atk());
                officerCard->setRealDef(proto.def());
                officerCard->setRealMorale(proto.hp());
                officerCard->setRealSpeed(proto.sp());
                
                if (temp->getOfficerNextId()) {
                    officerCard->setCost(temp->getOfficerCost());
                    
//                    CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                    int len = arr->count();
//                    for (int j=0; j < len; j++) {
//                        officerCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                    }
                }
                officerCard->setExpRate(temp->getOfficerExpRate());
                officerCard->setMaxStar(temp->getOfficerNextId());
                officerCard->setMaxExp(expdata->getExpValue());
                officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
                officerCard->setOfficerName(temp->getOfficerName());
                officerCard->setMaxLevel(temp->getOfficerMaxLevel());
                officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                officerCard->setRound(temp->getOfficerRound());
                officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                officerCard->setGoverning(temp->getOfficerGoverning());
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
                officerCard->setFateList(temp->getOfficerFateList());
               
                //武将原型ID
                officerCard->setProtoId(temp->getOfficerProtoId());
                //速度修正系数
                officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
				
				officerCard->setHeadSuffixNum(temp->getIconId());//12.06
                
                if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
                {
                    std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                    officerCard->setLordSkill(atoi(lordskill.c_str()));
                }
            }
        }
        else
        {
            continue;
        }
    }
    //装备
    updateItemSize = msg->updateequipmentcarditemproto_size();
    for (int i=0; i<updateItemSize; i++) {
        main::EquipmentCardItemUpdateProto updateEquipmentCardItemProto = msg->updateequipmentcarditemproto(i);
        state = updateEquipmentCardItemProto.state();
        ssid = updateEquipmentCardItemProto.ssid();
        if (state  == 0)
        {
            playerInfo->removeEquipCardById(ssid);
        }
        else if (state == 1)
        {
            main::EquipmentCardItemProto cardInfo = updateEquipmentCardItemProto.equipmentitem();
            SGEquipCard *equipCard = new SGEquipCard();
            CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
            int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());
//            if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else {
//                type = 2;
//            }
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
            //转生等级
            equipCard->setUpgradelevel(temp->getUpgradelevel());
            //转生最大星级
            equipCard->setUpgradestar(temp->getUpgradestar());
            //转生次数
            equipCard->setAdNum(temp->getAdvNum());
            equipCard->setSsid(cardInfo.ssid());
            equipCard->setItemId(cardInfo.itemid());
            //装备原型ID
            equipCard->setProtoId(temp->getEquipProtoId());
            //            equipCard->setSetNum(cardInfo.setnum());
            equipCard->setCurrLevel(cardInfo.currlevel());
            //equipCard->setCurrExp(cardInfo.currexp());
            equipCard->setOfficerCardId(cardInfo.officercarditemid());
            equipCard->setIsProt(cardInfo.isprotection());
            equipCard->setSellPrice(cardInfo.cost());
            if (cardInfo.officercarditemid() >0)
            {
                SGOfficerCard *officer = playerInfo->getOfficerCard(cardInfo.officercarditemid());
                officer->addEquip(equipCard);
            }
            
            equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            equipCard->setMaxExp(expdata->getExpValue());
            equipCard->setRound(temp->getEquipRound());
            equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
            equipCard->setExpRate(temp->getEquipExpRate());
            equipCard->setCurrStar(temp->getEquipCurrStarLevel());
            equipCard->setGoverning(temp->getEquipGoverning());
            equipCard->setItemType(temp->getEquipType());
            equipCard->setMaxStar(temp->getEquipNextId());
            equipCard->setOfficerName(temp->getEquipName());
            equipCard->setMaxLevel(temp->getEquipMaxLevel());
			equipCard->setMaterialDependencyId(temp->getMaterialDependencyId());
			equipCard->setHeadSuffixNum(temp->getIconId());//12.06
            if (temp->getFateDesc())
            {
                equipCard->setFateDesc(temp->getFateDesc());
            }            if (temp->getEquipNextId()) {
                equipCard->setCost(temp->getEquipCost());
                
//                CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                int len = arr->count();
//                for (int j=0; j < len; j++) {
//                    equipCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                }
            }
            
            playerInfo->addEquipCard(equipCard);
            equipCard->autorelease();
            
        }
        else if (state == 2)
        {
            main::EquipmentCardItemProto cardInfo = updateEquipmentCardItemProto.equipmentitem();
            SGEquipCard *equipCard = playerInfo->getEquipCard(ssid);
            if (equipCard)
            {
                CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
                SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
                int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());
//                if (temp->getEquipExpType()->isEqual(CCString::create("A"))) {
//                    type = 0;
//                }
//                else if (temp->getEquipExpType()->isEqual(CCString::create("B"))) {
//                    type = 1;
//                }
//                else {
//                    type = 2;
//                }
                SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
                //转生等级
                equipCard->setUpgradelevel(temp->getUpgradelevel());
                //转生最大星级
                equipCard->setUpgradestar(temp->getUpgradestar());
                //转生次数
                equipCard->setAdNum(temp->getAdvNum());
                equipCard->setSsid(cardInfo.ssid());
                equipCard->setItemId(cardInfo.itemid());
                //装备原型ID
                equipCard->setProtoId(temp->getEquipProtoId());
                equipCard->setCurrLevel(cardInfo.currlevel());
                //equipCard->setCurrExp(cardInfo.currexp());
                equipCard->setOfficerCardId(cardInfo.officercarditemid());
                equipCard->setIsProt(cardInfo.isprotection());
                if (cardInfo.officercarditemid() >0)
                {
                    SGOfficerCard *officer = playerInfo->getOfficerCard(cardInfo.officercarditemid());
                    officer->addEquip(equipCard);
				}
                
                equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                equipCard->setMaxExp(expdata->getExpValue());
                equipCard->setRound(temp->getEquipRound());
                equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                equipCard->setExpRate(temp->getEquipExpRate());
                equipCard->setCurrStar(temp->getEquipCurrStarLevel());
                equipCard->setGoverning(temp->getEquipGoverning());
                equipCard->setItemType(temp->getEquipType());
                equipCard->setMaxStar(temp->getEquipNextId());
                equipCard->setOfficerName(temp->getEquipName());
                equipCard->setMaxLevel(temp->getEquipMaxLevel());
				equipCard->setMaterialDependencyId(temp->getMaterialDependencyId());
				equipCard->setHeadSuffixNum(temp->getIconId());//12.06
                if (temp->getFateDesc())
                {
                    equipCard->setFateDesc(temp->getFateDesc());
                }                if (temp->getEquipNextId()) {
                    equipCard->setCost(temp->getEquipCost());
//                    if (temp->getReqItems()) {
//                        CCArray *arr = SGCardAttribute::spliceStr(temp->getReqItems()->getCString());
//                        int len = arr->count();
//                        for (int j=0; j < len; j++) {
//                            equipCard->addReqitemId(((CCString *)arr->objectAtIndex(j))->intValue());
//                        }
//                    }
                }
              if (updateEquipmentCardItemProto.oldofficerid() >0)
                {
                    SGOfficerCard *officer = playerInfo->getOfficerCard(updateEquipmentCardItemProto.oldofficerid());
                    if(officer)
                    {
                        officer->removeEquip(equipCard);
                    }
                    
                }
            }
			
		
        }
        else
        {
            continue;
        }
    }
    
    //士兵
    updateItemSize = msg->updatesoldiercarditemproto_size();
    for (int i=0; i<updateItemSize; i++) {
        main::SoldierCardItemUpdateProto updateEquipmentCardItemProto = msg->updatesoldiercarditemproto(i);
        state = updateEquipmentCardItemProto.state();
        ssid = updateEquipmentCardItemProto.ssid();
        if (state  == 0)
        {
            playerInfo->removeSoldierCardById(ssid);
        }
        else if (state == 1)
        {
            main::SoldierCardItemProto proto = updateEquipmentCardItemProto.soldieritem();
            SGSoldierCard *soldierCard = new SGSoldierCard();
            CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
            SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(proto.itemid());
            SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, proto.currlevel()+1);
            
            soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
            soldierCard->setSsid(proto.ssid());
            soldierCard->setItemId(proto.itemid());
            soldierCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
            soldierCard->setMaxExp(expdata->getExpValue());
            soldierCard->setOfficerName(temp->getSoldierName());
            soldierCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
            soldierCard->setRound(temp->getSoldierRound());
            soldierCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
            soldierCard->setCurrLevel(proto.currlevel());
            soldierCard->setCurrStar(temp->getSoldierStarLevel());
            soldierCard->setCurrExp(proto.currexp());
            soldierCard->setsoldiertalentskill(temp->getTalentSkill());//士兵增加
            soldierCard->setsoldiergrowskill(temp->getGrowSkill());

            
            playerInfo->addSoldierCard(soldierCard);
            soldierCard->autorelease();
            
        }
        else if (state == 2)
        {
            main::SoldierCardItemProto proto = updateEquipmentCardItemProto.soldieritem();
            SGSoldierCard *soldierCard = playerInfo->getSoldierCard(ssid);
            if (soldierCard) {
                CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
                SGSoldierDataModel *temp = SGStaticDataManager::shareStatic()->getSoliderById(proto.itemid());
                SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(EXPTYPE_Z, proto.currlevel()+1);
                soldierCard->setMaxLevel(temp->getSoldierMaxLevel());
                soldierCard->setSsid(proto.ssid());
                soldierCard->setItemId(proto.itemid());
                soldierCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                soldierCard->setMaxExp(expdata->getExpValue());
                soldierCard->setOfficerName(temp->getSoldierName());
                soldierCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                soldierCard->setRound(temp->getSoldierRound());
                soldierCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                soldierCard->setCurrLevel(proto.currlevel());
                soldierCard->setCurrStar(temp->getSoldierStarLevel());
                soldierCard->setCurrExp(proto.currexp());
                soldierCard->setsoldiertalentskill(temp->getTalentSkill());//士兵增加
                soldierCard->setsoldiergrowskill(temp->getGrowSkill());

            }
        }
        else
        {
            continue;
        }
    }
    //props
    updateItemSize = msg->updatepropscarditemproto_size();
    for (int i=0; i<updateItemSize; i++) {
        main::PropsCardItemUpdateProto updatePropsCardItemProto = msg->updatepropscarditemproto(i);
        state = updatePropsCardItemProto.state();
        ssid = updatePropsCardItemProto.ssid();
        if (state  == 0)
        {
            playerInfo->removePropCardById(ssid);
        }
        else if (state == 1)
        {
            main::PropsCardItemProto cardInfo = updatePropsCardItemProto.propsitem();
            SGPropsCard *propCard = new SGPropsCard();
            SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
            
            propCard->setSsid(cardInfo.ssid());
            propCard->setItemId(cardInfo.itemid());
            propCard->setIsProt(cardInfo.isprotection());
            
            propCard->setSell(prop->getpropsSell());
            propCard->setValue(prop->getpropsValue());
            propCard->setRace(prop->getPropsCountry());
            propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
            propCard->setType(prop->getPropsPropsType());
            propCard->setItemType(prop->getPropsType());
            propCard->setCurrStar(prop->getPropsCurrStarLevel());
			propCard->setHeadSuffixNum(prop->getIconId());//12.05
			
            playerInfo->addPropCard(propCard);
            propCard->autorelease();
            
        }
        else if (state == 2)
        {
            main::PropsCardItemProto cardInfo = updatePropsCardItemProto.propsitem();
            SGPropsCard *propCard = playerInfo->getPropCardB(ssid);
            if (propCard) {
                SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
                
                propCard->setSsid(cardInfo.ssid());
                propCard->setItemId(cardInfo.itemid());
                propCard->setIsProt(cardInfo.isprotection());
                
                propCard->setSell(prop->getpropsSell());
                propCard->setValue(prop->getpropsValue());
                propCard->setRace(prop->getPropsCountry());
                propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
                propCard->setType(prop->getPropsPropsType());
                propCard->setItemType(prop->getPropsType());
                propCard->setCurrStar(prop->getPropsCurrStarLevel());
				propCard->setHeadSuffixNum(prop->getIconId());//12.05
            }
        }
        else
        {
            continue;
        }
    }
	
	//@warning 更新上阵数据，this is a hole ，big deep hole ^_*
    if (SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer))
    {
        ((SGEmbattleLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_embattleLayer))->updateEmbattle(1);
    }
    CCLOG("updateRoleInfo finish");
	
    //首页布阵提示
    //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
}


void SGMainScene::upDateBagInfoListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::UpdateBagInfo *msg = (main::UpdateBagInfo *)sr->m_msg;
	do
	{
		upDateBagInfoHandler(msg);
	}
	while(0);
}

void SGMainScene::backFight()
{
    BackFightBox *bfb =BackFightBox::create(this);
    bfb->btnNoHandler(nullptr);    //modified by cgp 
    
    //SGMainManager::shareMain()->showBox(bfb);
//
}
void SGMainScene::showBoxCall(cocos2d::CCObject *obj)
{
//    SGMainManager::shareMain()->closeBox();
    SGMainLayer *mainLayer = (SGMainLayer *)this->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
    SGFirstLayer * layer =  (SGFirstLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_firstLayer);
    if(layer)
    {
        layer->setIsCanTouch(true);
    }
    
    if (obj)
    {
        CCDictionary *dict = (CCDictionary *)obj;
        CCString *isback = (CCString *)dict->objectForKey("isback");
        if (isback->intValue() == 1 ) //继续上次战斗
        {
            if (mainLayer) {
                 mainLayer->requestStory();
            }
            SGBattleManager::sharedBattle()->isSecond=true;
            SGBattleManager::sharedBattle()->isGuide=false;
        }
        else //放弃上次战斗
        {
            SGBattleManager::sharedBattle()->pveContinuStart(0);
            SGPlayerInfo::sharePlayerInfo()->setHasPveFight(false);
            SGBattleManager::sharedBattle()->isSecond=false;
        }
    }

    
}
void SGMainScene::setIsCanTouch(bool isTouch)
{
    
}
void SGMainScene::setIsLock(bool isLock)
{
    
}

void SGMainScene::updateMailListener(CCObject *obj){
    SGSocketRequest *sr=(SGSocketRequest *)obj;
    if (sr) {
        
        SGPlayerInfo *playerInfo=SGPlayerInfo::sharePlayerInfo();
        main::UpdateRoleMailResponse *mail=(main::UpdateRoleMailResponse *)sr->m_msg;
       
         if (mail->mainmails().ssid()) {
             
             for(int i=0;i<playerInfo->getFriendMails()->count();i++)
             {
                 SGMailDataModel* model = (SGMailDataModel*)(playerInfo->getFriendMails()->objectAtIndex(i));
                 if (model->getMailSsid() == mail->mainmails().ssid()) {
                     playerInfo->getFriendMails()->removeObjectAtIndex(i);
                 }
             }

            playerInfo->getFriendMails()->addObject(SGMailManager::getMailDataModelByMailProto(mail->mainmails()));
             
         }
         if (mail->systemmails().ssid()) {
              playerInfo->getSystemMails()->addObject(SGMailManager::getSystemMailByProto(mail->systemmails()));
         }
            
          if (mail->gmmails().ssid()) {
             playerInfo->getGMMails()->addObject(SGMailManager::getMailDataModelByMailProto(mail->gmmails()));
          }
           
    }
    

}
void SGMainScene::updateMainTaskListener(CCObject *obj){

    SGSocketRequest *sr=(SGSocketRequest *)obj;
	CCArray *groupArray=SGPlayerInfo::sharePlayerInfo()->getTaskGroup();
    groupArray->retain();
    if (sr) {
        
//        return;
        SGPlayerInfo *playerInfo=SGPlayerInfo::sharePlayerInfo();
//        playerInfo->getMainTaskDict()->removeAllObjects();
        main::UpdateRoleQuest *mainTask=(main::UpdateRoleQuest *)sr->m_msg;
        
        int count=mainTask->updaterolequestproto_size();
        for (int i=0; i<count; i++) {
            
            SGMainTaskDataModel *datamodel=new SGMainTaskDataModel();
            main::RoleQuestProto roleQuestPro=mainTask->updaterolequestproto(i);
            
            
            datamodel->settaskId(roleQuestPro.questid());
            datamodel->settaskGroupId(roleQuestPro.groupid());
            datamodel->setallProgress(roleQuestPro.conditionvalue());
            datamodel->setcurProgress(roleQuestPro.currcontionvalue());
//            datamodel->settaskDeprision(CCString::createWithFormat("%s",roleQuestPro.conditionname().c_str()));
            datamodel->setcomplateState(roleQuestPro.queststate());
            datamodel->setshowType(roleQuestPro.conditiontype());
            datamodel->setArrivement(roleQuestPro.arrivement());
            SGMainTaskSubModal *subModal=SGStaticDataManager::shareStatic()->getMainTaskSubByTaskId(roleQuestPro.questid());
            datamodel->rewardModel=SGStaticDataManager::shareStatic()->getRewardById(subModal->getRewareId(),3);
            
            datamodel->settaskName(subModal->gettaskName());
            datamodel->settaskDeprision(subModal->gettaskDesprition());

            playerInfo->addSubTask(datamodel);
            
            SGMainTaskGroup *groupObj=(SGMainTaskGroup *)groupArray->objectAtIndex(datamodel->gettaskGroupId()-1);
            if (datamodel->getcomplateState()==2) {
                groupObj->sethasFinish(true);
            }else
            {
                
               CCArray * subTaskArray=SGPlayerInfo::sharePlayerInfo()->getSubTaskByGroupName(groupObj->gettaskGroupName()->getCString());
                
                for (int i=0; i<subTaskArray->count(); i++) {
                    SGMainTaskDataModel *datamodel=new SGMainTaskDataModel();
                    datamodel=(SGMainTaskDataModel *)subTaskArray->objectAtIndex(i);
                    if (datamodel->getcomplateState()==2) {
                        groupObj->sethasFinish(true);
                        break;
                    }
                    groupObj->sethasTask(true);
                    groupObj->sethasFinish(false);
                }
            }
            
         
            
        }
        
    }
}
void SGMainScene::upDateBossStartState(CCObject *obj)
{
    

    SGSocketRequest *req=(SGSocketRequest *)obj;
    if(req){
        main::BossStartSateResponse *rs=(main::BossStartSateResponse *)req->m_msg;
        if (rs) {
            if (rs->state()) {
                SGPlayerInfo::sharePlayerInfo()->setisBossBattleTime(true);
            }else
                SGPlayerInfo::sharePlayerInfo()->setisBossBattleTime(false);
            
        }
    }
    
}

void SGMainScene::addGiftPower(CCObject *obj)
{
    SGSocketRequest *req=(SGSocketRequest *)obj;
    if(req)
    {
        main::AcceptPowerGiftResponse *rs=(main::AcceptPowerGiftResponse *)req->m_msg;
        if (rs)
        {
            main::PowerGiftProto _giftPower = rs->powergift();
            
            SGGiftPower *giftPower = this->getSGGiftPower(_giftPower);
            SGPlayerInfo::sharePlayerInfo()->addFriendPower(giftPower);
            
            if (getChildByTag(sg_friendLayer))
            {
                ((SGFriend *)getChildByTag(sg_friendLayer))->updateData();
            }
            if (getChildByTag(sg_obtainTili))
            {
                ((SGObtainTili *)getChildByTag(sg_obtainTili))->updateData();
            }

        }
    }
    
}

// The back key clicked
void SGMainScene::keyBackClicked()
{
    CCLOG("Back Clicked");
    ExtClassOfSDK::sharedSDKInstance()->channelExitGame();
}

// The menu key clicked. only available on wophone & android
void SGMainScene::keyMenuClicked()
{
    CCLOG("Menu Clicked");
}
