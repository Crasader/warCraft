//
//  SGLoginLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-27.
//
//


#include "SGWelComeLayer.h"
#include "SGMainManager.h"
#include "SGHttpClient.h"
#include "SGMsgId.h"
#include "SGServerListLayer.h"
#include "LoginModule.pb.h"
#include "SGSocketClient.h"
#include "SGNotificationCenter.h"
#include "SGButton.h"
#include "DataBase64.h"
#include "SGRegisterBox.h"
#include "SGCountryLayer.h"
#include "SGRegisterSuccess.h"
#include "SGServerListLoginGame.h"
#include "SGSortLayer.h"
#include "SGGamePlayerInfo.h"
#include "SGOtherPlayerInfo.h"
#include "SGBattleSpeedLayer.h"
#include "SGBattleChangeRoundLayer.h"
#include "SGServerListNewServerBox.h"
#include "GameMusicmanage.h"
#include "SGBindSuccessBox.h"
#include "SGAnnouncementDataModel.h"
#include "SGAnnounceMsg.h"
#include"SGStringConfig.h"
//#import "SdkReqCallBack.h"
//测试资源管理
#include "ResourceManager.h"
#include "CCSpriterX.h"
#include "ExtClass.h"
#include "SGSkillInfoBox.h"
#include "SGUpdateManager.h"
#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "SdkManager.h" // for android
#include "AndroidSDKAdp.h"
#endif
#include "SGShareBox.h"
#include "SGUpdateClientBox.h"
#include "CCLuaEngine.h"
#include "SGDrawCards.h"
#include "GlobalConfig.h"
#include "SdkController.h"

#if GAME_CHANNEL_TYPE == GCCT_CHINA_UC_ADR_THIRD
#include "auxiliary/UCSdk.h"
#include "auxiliary/UCSdkCallback.h"
#include "android/jni/CUCGameSdk.h"
using namespace ucgamesdk;
#endif

#if (GCCT_CHINA_KUAIYONG_IOS_THIRD == GAME_CHANNEL_TYPE)
#include "KuaiYongIosImp.h"
#endif

//是否开启一些简单的测试，省的进入游戏。
#define SIMPLE_TEST 0

#define NMAELIMIT 15
SGWelComeLayer::SGWelComeLayer()
:isstart(true)
,facebookBt(NULL)
,facebookLogOut(NULL)
,guestBidBtn(NULL),
m_pServerDict(NULL),
changeServerbtn(NULL),
canStart(false)
,startBtn(NULL)
{
    m_pServerDict = NULL;
    registerBtn = NULL;
    changeServerbtn = NULL;
    logo = NULL;
    
}
SGWelComeLayer::~SGWelComeLayer()
{
    CC_SAFE_DELETE(m_pServerDict);
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_welComeLayer);
	
}

SGWelComeLayer *SGWelComeLayer::create()
{
    SGWelComeLayer *welComeLayer = new SGWelComeLayer();
    
    if (welComeLayer && welComeLayer->init(NULL, sg_welComeLayer))
    {
        welComeLayer->autorelease();
        return welComeLayer;
    }
    CC_SAFE_DELETE(welComeLayer);
    return NULL;
}

void SGWelComeLayer::initMsg()
{

    //MMDEBUG: BEGIN
#if SIMPLE_TEST

    ResourceManager* rm = ResourceManager::sharedInstance();
    rm->bindTexture("sgserverlistlayer/sgserverlistlayer.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    rm->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    rm->bindTexture("sgfirstactivitylayer/sgfirstactivitylayer.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    rm->bindTexture("animationFile/short.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    rm->bindTexture("sanguobigpic/barrack_bg.plist",RES_TYPE_LAYER_UI ,sg_luaactivity);
    
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    pEngine->executeScriptFile("qingming.lua"); //这里更换为自己的xxx.lua，lua文件请直接放在ipad文件夹下。
    return;
#endif
    //MMDEBUG: END
    
    //	CCFileUtils::sharedFileUtils()->fullPathForFilename("hello.lua");
	ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_welComeLayer);
	ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_welComeLayer);
    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
    std::string localResVer = ccud->getStringForKey(SGUM_LOCAL_RES_VER_KEY);

    float fontSize = 8.0f;
    CCLabelTTF *version = CCLabelTTF::create(CCString::createWithFormat("V%s.%s", BUNDLE_VERSION,localResVer.c_str())->getCString(), FONT_PENGYOU, FONTSIZE(fontSize));
    version->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp( -version->getContentSize().width * 0.6f, version->getContentSize().height * 0.5)));
    version->setColor(ccGREEN);
    this->addChild(version, 99999);
	
    //this->setTouchEnabled(true);   //removed by cgp

    CCPoint center = SGLayout::getPoint(kMiddleCenter);

    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/logo.plist", RES_TYPE_LAYER_UI, sg_welComeLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/login_ip5.plist", RES_TYPE_LAYER_UI, sg_welComeLayer);
    
    //removed by cgp for welcomelayer
//    logo = CCSprite::createWithSpriteFrameName("logo.png");
//    this->addChild(logo,10);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("login_ip5.png");
    bg->setScaleY(CCDirector::sharedDirector()->getWinSize().height / bg->getContentSize().height);
    bg->setScaleX(CCDirector::sharedDirector()->getWinSize().width / bg->getContentSize().width);
    this->addChild(bg,-1);
    bg->setPosition(center);
    
//logo->setPosition(ccpAdd(center, ccp(0, bg->getContentSize().height * 0.11)));
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //m_blackColor = CCLayerColor::create(ccc4(0, 0, 0, 0), winSize.width, winSize.height);
    //m_blackColor = CCLayerColor::create(ccc4(0, 0, 0, 180), winSize.width, winSize.height);
   // m_blackColor->setTouchEnabled(false);
   // m_blackColor->setPosition(0,0);
    
//    SGButton *login_start = SGButton::createFromLocal("login_start.png", "", this, NULL, CCPointZero, FONT_BOXINFO, ccRED, 26, false, true);
//    this->addChild(login_start , 21);
//    login_start->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, screenheight * 0.2)));
//    login_start->setTag(1);
//    login_start->runAction(CCRepeatForever::create(CCBlink::create(1.5, 1)));
//    EFFECT_PLAY(MUSIC_BTN);

    CCUserDefault::sharedUserDefault()->setBoolForKey("pvp_tran",false);//每次登入时取消pvp登入状态
    
     SGStaticDataManager::shareStatic()->getOfficerById(1001);    //added by cgp for battle of officer table
     loginstart();
    
}

void SGWelComeLayer::showFBBindBox()
{
    this->setIsCanTouch(false);
    SGSkillInfoBox *cantstrengBox = SGSkillInfoBox::create(this,NULL,616);
    SGMainManager::shareMain()->showBox(cantstrengBox);
}



void SGWelComeLayer::doFacebookBind()
{
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
#if (PLATFORM == IOS)
    OcCaller::getInstance()->showFBlogin();
#else
    AndroidSDKAdp::getInstance()->fbLogin();
#endif
#endif
    //发送进入Facebook登陆界面的消息 add by:zyc.
    SGMainManager::sendOperatorRecord(100011);
}

void SGWelComeLayer::startFacebookLogin()
{
#if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
    bool isAccount = CCUserDefault::sharedUserDefault()->getBoolForKey("isAccount");
    if (GameConfig::isExist())
    {
        if (!isAccount && SGMainManager::shareMain()->SGMainManager::shareMain()->getIsFBLogin() != 1)
        {
			//如果是兄弟游戏
			if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
			{
				registerBtn->setFont(str_WdelcomeLayer_str4);
			}
            
            showFBBindBox();
            
            return;
        }
    }
    this->doFacebookBind();

#elif (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY)
    UserAndExtInfo uei;
    SdkController::gi()->getSdkImp()->sdkFBLogin(uei);
#endif

}

void SGWelComeLayer::bindGuestHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showregisterLayer(3);
}
void SGWelComeLayer::resetCanStart()
{
    canStart = true;
    unschedule(schedule_selector(SGWelComeLayer::resetCanStart));
}
#pragma mark 进入游戏
//点击进入游戏按钮的回调   现需要 修改
//当true==GameConfig::isExist() 时发送20号消息  更新当前选中的服务器状态
void SGWelComeLayer::fastStartHandler()
{

    
    //cgp test
//    CCSize size =  CCDirector::sharedDirector()->getVisibleSize();
//    
//    //左边屏幕
//    CCRenderTexture* leftTexture = CCRenderTexture::create(size.width/2, size.height);
//    
//    leftTexture->getSprite()->setAnchorPoint(CCPoint(1,1));
//    leftTexture->getSprite()->setPosition(CCPoint(size.width/2,0));
//    
//    leftTexture->begin();
//    CCDirector::sharedDirector()->getRunningScene()->visit();
//    leftTexture->end();
//    
//    leftTexture->getSprite()->setTextureRect(CCRect(0,0,size.width / 2,size.height));
//    
//    //右边屏幕
//    CCRenderTexture* rightTexture = CCRenderTexture::create(size.width, size.height);
//    
//    rightTexture->getSprite()->setAnchorPoint(CCPoint(0,1));
//    rightTexture->getSprite()->setPosition(CCPoint(size.width/2,0));
//    
//    rightTexture->begin();
//    CCDirector::sharedDirector()->getRunningScene()->visit();
//    rightTexture->end();
//    
//    rightTexture->getSprite()->setTextureRect(CCRect(size.width/2,0,size.width / 2,size.height));
//    
//    this->addChild(rightTexture);
//    
//    this->addChild(leftTexture);
//    
//    CCRotateTo* cwAction = CCRotateTo::create(2, 180);
//    
//    CCRotateTo* ccwAction = CCRotateTo::create(2, -180);
//    
//    
//    leftTexture->getSprite()->runAction(ccwAction);
//    
//    rightTexture->runAction(cwAction);
//    return;
  
    EFFECT_PLAY(MUSIC_BTN);
    if (GAME_ACCOUNT_TYPE == GCAT_XDYOU) //xdyou账号体系，不做额外处理。
    {
        ;
    }
    else //其他账号体系，必须先登陆。
    {
        if (!ExtClassOfSDK::sharedSDKInstance()->isLoginSDK())
        {
            ExtClassOfSDK::sharedSDKInstance()->channelLogin(0);  //cgp come
            return;
        }
    }
    
    //封装登陆业务。
    loginGameProcess();
    
}

//点击“登陆”按钮之后的逻辑，封装后可用于SDK统一登陆。
void SGWelComeLayer::loginGameProcess()
{
    //防止多次快速点击。
    if (canStart)
    {
        canStart = false;
        schedule(schedule_selector(SGWelComeLayer::resetCanStart), 1);
    }
    else
    {
        return;
    }
    
    //账号系统（SDK/自家）处理完成后，走这个统一的登陆逻辑。
    CCLOG("go2play -- isExist == %d", GameConfig::isExist());
    if (GameConfig::isExist())
    {
        int serverId = 0 ;
        if (m_pServerDict)//点击服务器列表
        {
            serverId = ( (CCString * )m_pServerDict->objectForKey("serverId"))->intValue();
        }
        else//默认无修改   走上次登陆的服务器
        {
            serverId = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid") ;
        }
        
        SGHttpClient::shareHttpClient()->updateSelectServer( this , (short)serverId );
    }
    else //理论上，非XDYOU的账号系统，不会走到这一步，一定会有isExist。
    {
        SGMainManager::shareMain()->getMainScene()->plat->showConnection();
        CCLOG("第一次进入游戏");
        SGMainManager::shareMain()->dealIosOKLogin();
    }
}

//設置registerBtn的文本，以走順註冊/登陸邏輯。
void SGWelComeLayer::setRegisterBtnText(const std::string& text)
{
    this->registerBtn->setFont(text.c_str());
}

#pragma mark 获得服务器列表
void SGWelComeLayer::changeIDHandler()
{
    EFFECT_PLAY(MUSIC_BTN);

    std::string userId = "";

    if (SGMainManager::shareMain()->getIsFBLogin() == 1)
    {
         userId =  CCUserDefault::sharedUserDefault()->getStringForKey("FBaccountId");
    }
    else
    {
        userId =  CCUserDefault::sharedUserDefault()->getStringForKey("userId");
    }
    
    int chanleId = CCUserDefault::sharedUserDefault()->getIntegerForKey("channelId");

    SGHttpClient::shareHttpClient()->getServerList(this, userId,chanleId);
}
#pragma mark 注册帐号
void SGWelComeLayer::registerHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGMainManager::shareMain()->doCloseFBlogin(0);
    CCUserDefault::sharedUserDefault()->setBoolForKey("XdFBLoginState",false);

    if (GAME_ACCOUNT_TYPE == GCAT_XDYOU) //xdyou账号体系的默认处理流程。
    {
        //如果是显示点击登入，直接进入注册界面
        if (strcmp(registerBtn->getFont(), str_ServerItem_str17) == 0 || strcmp(registerBtn->getFont(), str_WdelcomeLayer_str5) == 0)
        {
            SGMainManager::shareMain()->showregisterLayer(5);
        }
        //如果是游客，进入绑定界面
        else if (strcmp(registerBtn->getFont(), str_WdelcomeLayer_str4) == 0)
        {
            SGMainManager::shareMain()->showregisterLayer(3);
        }
        else
        {
            SGMainManager::shareMain()->showloginLayer();
            CCLog("帐号注册");
        }
    }
    else //非xdyou账号体系的处理。
    {
#if (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY)
        //如果是显示点击登入，直接进入注册界面
        if (strcmp(registerBtn->getFont(), str_ServerItem_str17) == 0 || strcmp(registerBtn->getFont(), str_WdelcomeLayer_str5) == 0)
        {
            ExtClassOfSDK::sharedSDKInstance()->channelRegister(5);
        }
        //如果是游客，进入绑定界面
        else if (strcmp(registerBtn->getFont(), str_WdelcomeLayer_str4) == 0)
        {
            ExtClassOfSDK::sharedSDKInstance()->channelRegister(3);
        }
        else
        {
            SGMainManager::shareMain()->showloginLayer();
            CCLog("帐号注册");
        }
#else
        ExtClassOfSDK::sharedSDKInstance()->channelRegister();
#endif
    }
    //发送进入登陆界面的消息 add by:zyc.
    SGMainManager::sendOperatorRecord(100020);
}
//同步推其他渠道切换帐号
void SGWelComeLayer::exchangeHandler()
{
	ExtClassOfSDK::sharedSDKInstance()->exchangeAccount();
}

void SGWelComeLayer::requestFinished(int msgid, CCObject *data)
{
    CCLOG("收到数据");
    switch (msgid)
    {
        case MSG_HTTP_FAST_LOGIN:
        {
            std::string serverip = CCUserDefault::sharedUserDefault()->getStringForKey("serverip");
            short serverpost = (short)CCUserDefault::sharedUserDefault()->getIntegerForKey("serverpost");
            
            //记录服务器ip和端口号
            SGMainManager::shareMain()->setServerInfo(CCString::create(serverip.c_str()) , serverpost);
            
            //连接socket
            SGSocketClient::sharedSocketClient()->startConn(serverip.c_str() , serverpost);
            
        }
            break;
        case MSG_HTTP_LOGIN:
        {
            int isLogin = SGMainManager::shareMain()->getIsFBLogin();
            if (isLogin != 1 || !SGMainManager::shareMain()->getIsFbAutologin())
            {
                std::string serverip = CCUserDefault::sharedUserDefault()->getStringForKey("serverip");
                int serverpost = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverpost");
                SGSocketClient::sharedSocketClient()->startConn(serverip.c_str() ,(short)serverpost);
            }
            else
            {
                SGMainManager::shareMain()->setIsFbAutologin(false);
            }
        }
            break;
        case MSg_HTTP_SERVERLIST:
        {
            SGServerListLayer *box = SGServerListLayer::create(this, (CCArray *)data, SGServerListLayerComeFromFirst);
            SGMainManager::shareMain()->showBox(box,false);

            
        }
            break;
        case MSG_HTTP_UPDATE_SERVER_STATE:
        {
            CCDictionary *dict = (CCDictionary*)data;
            CCString *serverIp = (CCString*)dict->objectForKey("serverIp");
            CCString *serverPost = (CCString*)dict->objectForKey("serverPost");
            CCString *serverId = (CCString*)dict->objectForKey("serverId");
            CCString *serverShowId = (CCString*)dict->objectForKey("serverShowId");
            CCString* serverName = (CCString*)dict->objectForKey("serverName");
            //保存即将登陆的服务器的描述
            serverDes = ((CCString * )dict->objectForKey("serverDes") )->m_sString;
            //save ip and post of server
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverid",serverId->intValue());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverShowId", serverShowId->intValue());
            CCUserDefault::sharedUserDefault()->setStringForKey("serverip", serverIp->getCString());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverpost", serverPost->intValue());
            CCUserDefault::sharedUserDefault()->setStringForKey("servername", serverName->getCString());
            CCUserDefault::sharedUserDefault()->flush();
            
            
            CCLog("最新服务器 ip=%s\n post=%s\n id=%s\n name=%s",serverIp->getCString(),serverPost->getCString(),serverId->getCString(),serverName->getCString());
            
            changeServerbtn->setFont(serverName->getCString());
            CCUserDefault::sharedUserDefault()->setStringForKey("servername", serverName->getCString());
            
            //如果是维护中 则不再登陆。
            short serverState = (short)(  ( (CCString *)dict->objectForKey("serverState") )->intValue() );
            if (1 != serverState)
            {
                ;
            }
            //提示不能登陆的详细信息。
            else
            {
                SGMainManager::shareMain()->showMessage(serverDes.c_str());
            }
            //MM: 即使维护中，也尝试登录，101用户会自动放过。
            this->realLogin();
        }
            break;

        case MSg_HTTP_LASTESTSERVER:
        {
            CCDictionary *dict = (CCDictionary*)data;
            CCString *serverIp = (CCString*)dict->objectForKey("serverIp");
            CCString *serverPost = (CCString*)dict->objectForKey("serverPost");
            CCString *serverId = (CCString*)dict->objectForKey("serverId");
            CCString* serverName = (CCString*)dict->objectForKey("serverName");
            //保存即将登陆的服务器的描述
            serverDes = ((CCString * )dict->objectForKey("serverDes") )->m_sString;
            //save ip and post of server
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverid",serverId->intValue());
            CCUserDefault::sharedUserDefault()->setStringForKey("serverip", serverIp->getCString());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverpost", serverPost->intValue());
            CCUserDefault::sharedUserDefault()->setStringForKey("servername", serverName->getCString());
            CCUserDefault::sharedUserDefault()->flush();
            
            
            CCLog("最新服务器 ip=%s\n post=%s\n id=%s\n name=%s",serverIp->getCString(),serverPost->getCString(),serverId->getCString(),serverName->getCString());
            
            changeServerbtn->setFont(serverName->getCString());
            CCUserDefault::sharedUserDefault()->setStringForKey("servername", serverName->getCString());
            
        }
            break;
        case MSG_HTTP_GET_CHANNEL_USER_ID:
        {
            CCDictionary* dict = (CCDictionary*) data;
            CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
            CCString* actId = (CCString*) dict->objectForKey("sdkAccountId");
            ccud->setStringForKey("userId", actId->m_sString);
            ccud->setStringForKey("username", actId->m_sString);
            ccud->setStringForKey("password", actId->m_sString);
            ccud->setIntegerForKey("channelId", GlobalConfig::gi()->getRealChannelId());
#if (GCCT_CHINA_UC_ADR_THIRD == GAME_CHANNEL_TYPE) //uc adr
            //MM: 将本次s_sid作为loginkey，登入使用。
            ccud->setStringForKey("loginKey", UCSdk::s_sid);
#elif (GCCT_CHINA_KUAIYONG_IOS_THIRD == GAME_CHANNEL_TYPE)
            //MM: 将快用的tokenKey作为loginkey，登入使用。
            ccud->setStringForKey("loginKey", ( (KuaiYongIosImp*) SdkController::gi()->getSdkImp() )->kyToken );
#else
            
#endif
            CCString* nickName = (CCString*) dict->objectForKey("sdkNickName");
            ccud->setStringForKey("sdk_user_nick_name", nickName->m_sString);
            //还有一个"sdkCreator"字段，但是未起作用，所以没有在这里添加。
            registerBtn->setFont(nickName->getCString()); //将按钮文本变为其uc账号昵称。

            ccud->setBoolForKey("isExist", true);
            ccud->setBoolForKey("isAccount", true);
            ccud->flush();
            //MMD: SDKIF SDK登陆成功！仅当21号http消息完成后，才设置SDK登陆成功标志，其他情况一概无视。
            ExtClassOfSDK::sharedSDKInstance()->setIsLoginSDK(true);
            //添加的针对非注册体系下的td数据设置，目前有uc在使用。
            ExtClassOfSDK::sharedSDKInstance()->setTdData(actId->m_sString, nickName->m_sString);
        }
            break;
        default:
            break;
    }
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
}

void SGWelComeLayer::requestFailed()
{
  
    SGMainManager::shareMain()->showConnectTip(SGNET_CANNOT_REACH_SERVER);
}

void SGWelComeLayer::alertViewClickedButtonAtIndex(int buttonIndex)
{
    SGMainManager::shareMain()->getMainScene()->plat->removeConnection();
    SGMainManager::shareMain()->closeBox();
}

void SGWelComeLayer::showBoxCall(CCObject *obj)
{
    if (obj)
    {
        CCDictionary *dict = (CCDictionary *)obj;
        CCString *key =  (CCString*)dict->objectForKey("key");
        
        if(key->isEqual(CCString::create("RegisterSuccess"))
           || key->isEqual(CCString::create("BingSuccess")))
        {
            std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
            std::string account = CCUserDefault::sharedUserDefault()->getStringForKey("username");
            changeServerbtn->setFont(str.c_str());
            if (account.size() > NMAELIMIT) {
                account = account.substr(0,NMAELIMIT-3);
                account.append("...");
            }
            //只有不是facebook登陆的账号，才显示。
            if (account.find("XD_THIRDPAR") == std::string::npos)
            {
                registerBtn->setFont(account.c_str());
            }
        }
        else if(key->isEqual(CCString::create("serverList")))
        {
            CCString *serverName = (CCString*)dict->objectForKey("serverName");
            CCString *serverIp = ((CCString*)dict->objectForKey("serverIp"));
            CCString *serverId = (CCString*)dict->objectForKey("serverId");
            CCString *serverPost = (CCString*)dict->objectForKey("serverPost");
            serverDes = ( (CCString * )dict->objectForKey("serverDes"))->m_sString;
            CCLOG("选中服务器名字：  %s",serverName->getCString());
            CCLOG("选中服务器Ip：   %s",serverIp->getCString());
            CCLOG("选中服务器Post： %s",serverPost->getCString());
            CCLOG("选中服务器Id：   %s",serverId->getCString());
            
            
            CCUserDefault::sharedUserDefault()->setStringForKey("serverip", serverIp->getCString());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverpost", (short)serverPost->intValue());
            CCUserDefault::sharedUserDefault()->setIntegerForKey("serverid", (short)serverId->intValue());
            CCUserDefault::sharedUserDefault()->setStringForKey("servername", serverName->getCString());
            CCUserDefault::sharedUserDefault()->flush();
            
            //设置服务器名称
            changeServerbtn->setFont(serverName->getCString());
            
            //选一个服务器
            if(m_pServerDict)
            {
                m_pServerDict->release();
            }
            m_pServerDict = dict;
            m_pServerDict->retain();
        }
    }
}

//服务器列表界面回调函数
void SGWelComeLayer::serverListCallBack(CCDictionary *serverDict)
{
    
}

void SGWelComeLayer::refreshRegisterBtn()
{
    bool isAccount = CCUserDefault::sharedUserDefault()->getBoolForKey("isAccount");
    if (GameConfig::isExist())
    {
        if (isAccount)
        {
            std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
            
            if (un.size() > NMAELIMIT) {
                un = un.substr(0,NMAELIMIT-3);
                un.append("...");
            }
			if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
			{
                if (strcmp(un.c_str(), "") == 0 )
                {
                    registerBtn->setFont(str_ServerItem_str17);
                }
                else
                {
                    
                    bool savedFbState = CCUserDefault::sharedUserDefault()->getBoolForKey("XdFBLoginState");
                    if (savedFbState)
                    {
                        //已用facebook登入
                        registerBtn->setFont(str_ServerItem_str17);
                    }
                    else
                    {
                        //只有不是facebook登陆的账号，才显示。
                        if (un.find("XD_THIRDPAR") == std::string::npos)
                        {
                            registerBtn->setFont(un.c_str());
                        }
                    }
                }
				
			}
			else if (GAME_ACCOUNT_TYPE == GCAT_TONGBUTUI)
			{
				registerBtn->setFont(str_WdelcomeLayer_str3);
			}
            
            
        }
        else
        {
			//如果是兄弟游戏
			if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
			{
				registerBtn->setFont(str_WdelcomeLayer_str4);
			}
        }
    }
    else
    {
		if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
		{
			registerBtn->setFont(str_WdelcomeLayer_str5);
		}
    }
    
    if (guestBidBtn)
    {
        guestBidBtn->setVisible(!isAccount);
        guestBidBtn->setEnabled(!isAccount);
    }
}


void SGWelComeLayer::loginstart()
{
    isstart= false;
    
    testSomething(); //MM: 保留
    
    //发送进入欢迎界面的消息 add by:zyc.
    SGMainManager::sendOperatorRecord(100010);
    
//    ((SGButton *)this->getChildByTag(1))->stopAllActions();
//    this->removeChildByTag(1, true);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCLayerColor *layer = CCLayerColor::create(ccc4(0, 0, 0, 50));
    this->addChild(layer, -1);

    std::string sbPng = "";
    
    if ( GAME_ACCOUNT_TYPE == GCAT_XDYOU ) //MM: xdyou支持快速登入功能。
    {
        if (GameConfig::isExist())
        {
            sbPng = "login_ksksbtn.png";
        }
        else
        {
            sbPng = "login_ksksOncebtn.png";
        }
    }
    else if ( GAME_ACCOUNT_TYPE == GCAT_GO2PLAY ) //MM: go2play的快速登陆比较坑爹，需要单独标志位标记。//cgp come here
    {
        CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
        bool isFast = ccud->getBoolForKey("isFastLogin");
        if (GameConfig::isExist())
        {
            if (isFast)
            {
                sbPng = "login_ksksOncebtn.png";
            }
            else
            {
                sbPng = "login_ksksbtn.png";
            }
        }
        else
        {
            sbPng = "login_ksksOncebtn.png";
        }
    }
    else //MM: 非xdyou和go2play的账号体系，不管是否有过登陆历史，一概无视，重新登陆，即图片为“进入游戏”，而不是“快速开始”。
    {
        sbPng = "login_ksksbtn.png";
    }
    startBtn = SGButton::create(sbPng.c_str(), NULL, this, menu_selector(SGWelComeLayer::fastStartHandler),CCPointZero,false,true);
    
    this->addBtn(startBtn);
    
    startBtn->setScale(0.8);
    canStart = true;
    
	//不同渠道显示不同文本。
	CCString *no = NULL;
	if ( (GAME_ACCOUNT_TYPE == GCAT_XDYOU) || (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY) )
	{
		no = CCString::create(str_WdelcomeLayer_str1);
	}
	else
	{
		no = CCString::create(str_WdelcomeLayer_str2);
	}
    
    SGCCLabelTTF *notice = SGCCLabelTTF::create(no->getCString(), FONT_BOXINFO,FONTSIZE(13));
    this->addChild(notice,100);
    
    CCSprite *noticeBg = CCSprite::createWithSpriteFrameName("loginMsgbg.png");
    this->addChild(noticeBg,90);
    noticeBg->setScale(0.8);
    
    changeServerbtn = SGButton::createFromLocal("loginbtnText.png", "", this, menu_selector(SGWelComeLayer::changeIDHandler), CCPointZero, FONT_BOXINFO, ccWHITE, 26, false, true);
    changeServerbtn->setFontColor(ccWHITE);
    changeServerbtn->setScale(0.8);
    this->addBtn(changeServerbtn);
    
    //帐号注册
	if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
	{
		registerBtn = SGButton::createFromLocal("loginbtnText.png",
												" ",
												this,
												menu_selector(SGWelComeLayer::registerHandler), CCPointZero, FONT_BOXINFO, ccWHITE, 26, false, true);
	}
	//如果渠道SDK不是朋游，帐号部分不可见
	else if (GAME_ACCOUNT_TYPE == GCAT_TONGBUTUI)
	{
		//registerBtn->setVisible(false);
		registerBtn = SGButton::createFromLocal("loginbtnText.png", str_WdelcomeLayer_str3, this, menu_selector(SGWelComeLayer::exchangeHandler), CCPointZero, FONT_BOXINFO, ccWHITE, 26, false, true);
	}
    else if   (GAME_ACCOUNT_TYPE == GCAT_PP)//下边部分使用registerBtn太多，暂时初始化，设为不可见
	{
        
		registerBtn = SGButton::createFromLocal("loginbtnText.png", str_WdelcomeLayer_str3, this, menu_selector(SGWelComeLayer::exchangeHandler), CCPointZero, FONT_BOXINFO, ccWHITE, 26, false, true);
	}
    else
    {//cgp come
		registerBtn = SGButton::createFromLocal("loginbtnText.png",
												" ",
												this,
												menu_selector(SGWelComeLayer::registerHandler), CCPointZero, FONT_BOXINFO, ccRED, 26, false, true);
    }
    registerBtn->setScale(0.8);
    
	//如果渠道
    bool isAccount = CCUserDefault::sharedUserDefault()->getBoolForKey("isAccount");
    if (GameConfig::isExist())  //cgp come here
    {
        std::string str = CCUserDefault::sharedUserDefault()->getStringForKey("servername");
        changeServerbtn->setFont(str.c_str());
        
        if (isAccount)
        {
            std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
            
            if (un.size() > NMAELIMIT) {
                un = un.substr(0,NMAELIMIT-3);
                un.append("...");
            }
			if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
			{
                if (strcmp(un.c_str(), "") == 0 )
                {
                    registerBtn->setFont(str_ServerItem_str17);
                }
                else
                {
                    bool savedFbState = CCUserDefault::sharedUserDefault()->getBoolForKey("XdFBLoginState");
                    if (savedFbState)
                    {
                        //已用facebook登入
                        registerBtn->setFont(str_ServerItem_str17);
                    }
                    else
                    {
                        //只有不是facebook登陆的账号，才显示。
                        if (un.find("XD_THIRDPAR") == std::string::npos)
                        {
                            registerBtn->setFont(un.c_str());
                        }
                    }
                }
				
			}
			else if (GAME_ACCOUNT_TYPE == GCAT_TONGBUTUI)
			{
				registerBtn->setFont(str_WdelcomeLayer_str3);
			}
            
            
        }
        else
        {
			//如果是兄弟游戏
			if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
			{
				registerBtn->setFont(str_WdelcomeLayer_str4);
			}
            
            guestBidBtn = SGButton::create("binding_account.png",
                                           NULL,
                                           this,
                                           menu_selector(SGWelComeLayer::bindGuestHandler), CCPointZero,false, true);
			
			//如果是渠道SDK, 不显示绑定
			if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
			{
				guestBidBtn->setVisible(false);
			}
			
            this->addBtn(guestBidBtn);
        }
    }
    else
    {
		if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
		{
			registerBtn->setFont(str_WdelcomeLayer_str5);
		}
        //获取最新服务器
        SGHttpClient::shareHttpClient()->getlastestServer(this);
    }
    this->addBtn(registerBtn);
    
    //MMD: SDKIF 截断处理，只要不是XDYOU账号体系，一概显示为“点击登入”。
    if (GAME_ACCOUNT_TYPE != GCAT_XDYOU)
    {
        //有时候会被提掉线，但是SDK账号还登录着的，所以必须让其显示正确的username。
        if (ExtClassOfSDK::sharedSDKInstance()->isLoginSDK())
        {
#if (GCCT_CHINA_UC_ADR_THIRD == GAME_CHANNEL_TYPE || GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD) //uc和快用特殊，如果可能，显示nickname。
            registerBtn->setFont(CCUserDefault::sharedUserDefault()->getStringForKey("sdk_user_nick_name", "sanguo user").c_str());
#else
            registerBtn->setFont(CCUserDefault::sharedUserDefault()->getStringForKey("username", "sdk_user").c_str());
#endif
        }
        else
        {
#if (GCCT_CHINA_UC_ADR_THIRD == GAME_CHANNEL_TYPE || GAME_CHANNEL_TYPE == GCCT_CHINA_KUAIYONG_IOS_THIRD) //uc和快用特殊，只显示点击登入，由自动逻辑登入。
            registerBtn->setFont(str_ServerItem_str17);
#else
            if (GameConfig::isExist())  //cgp come here
            {
                registerBtn->setFont(CCUserDefault::sharedUserDefault()->getStringForKey("username", "sdk_user").c_str());
            }
            else
            {
                registerBtn->setFont(str_ServerItem_str17);
            }
#endif
        }
    }
    
    registerBtn->setFontColor(ccWHITE);
    
    const int nSpace = 15;
    if (s.height == 960)
    {
        registerBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 379-nSpace-nSpace)));
        changeServerbtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 285-1-nSpace)));
        startBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 180.5-1)));
        notice->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 90)));
    }
    else //if(s.height == 1136)
    {
//        registerBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 380+100-nSpace-nSpace)));
//        changeServerbtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 285+100-nSpace)));
//        startBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 180.5+100)));
//        notice->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 90+100)));
#define cgpLoginSpace -50
        registerBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 300 + cgpLoginSpace-nSpace-nSpace)));
        changeServerbtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 240 + cgpLoginSpace-nSpace)));
        startBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 150 + cgpLoginSpace)));
        notice->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 60 + cgpLoginSpace)));
        registerBtn->setScale(0.8);
        changeServerbtn->setScale(0.8);
        startBtn->setScale(0.6);
        notice->setScale(1.0);
        
        
    }
//    else
//    {
//        registerBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 380 +39.2-nSpace-nSpace)));
//        //fermEffect->setPosition(ccp(s.width/2, 145+40));
//        changeServerbtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 285+39-nSpace)));
//        startBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 180.5+40)));
//        notice->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 90+40)));
//    }
    
    if (noticeBg)
    {
        noticeBg->setPosition(notice->getPosition());
        notice->setVisible(false);
        noticeBg->setVisible(false);
    }
    if (guestBidBtn != NULL) {
        guestBidBtn->setPosition(ccp(registerBtn->getPositionX() - registerBtn->getContentSize().width/2 -36 ,
                                     registerBtn->getPositionY() + guestBidBtn->getContentSize().height / 16));
        guestBidBtn->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.6,0.8),CCScaleTo::create(0.6,1.0),NULL)));
    }
    
#if (FACEBOOK_OPEN_STATUS == FACEBOOK_ON)
    
    facebookBt = SGButton::create("sharefacebook-3.png",
                                         NULL,
                                         this,
                                         menu_selector(SGWelComeLayer::startFacebookLogin),
                                         ccp(0, 0),
                                         false,
                                         true);
    
    facebookBt->setPosition(ccp(registerBtn->getPositionX() + registerBtn->getContentSize().width/2 + facebookBt->getContentSize().width/2 , registerBtn->getPositionY() + facebookBt->getContentSize().height / 8));
    this->addBtn(facebookBt);
    facebookBt->setScale(0.8);
    
    facebookLogOut = SGButton::createFromLocal("zhuxiaoFB.png", "", this, menu_selector(SGWelComeLayer::logOutFB), ccp(-45,0), FONT_BOXINFO, ccWHITE, 26, false, true);
    
    facebookLogOut->setPosition(registerBtn->getPosition());
    this->addBtn(facebookLogOut);
    facebookLogOut->setVisible(false);
#if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
    updateViewForFB();
#elif (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY)
    CCUserDefault* ccud = CCUserDefault::sharedUserDefault();
    bool shallFastFb = ccud->getBoolForKey("isSdkFBLogin", false);
    if (shallFastFb)
    {
        UserAndExtInfo uei;
        SdkController::gi()->getSdkImp()->sdkFBLogin(uei);
    }
#endif
#endif
    
    //针对uc渠道的特殊处理，启动完成后，直接试图登陆账号。
#if (GCCT_CHINA_UC_ADR_THIRD == GAME_CHANNEL_TYPE)
    if (GameConfig::isExist())
    {
        if (!UCSdk::s_logined)
        {
            ExtClassOfSDK::sharedSDKInstance()->channelRegister();
        }
    }
#endif
}

bool SGWelComeLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (isstart)
    {
        //m_blackColor->setVisible(false);
        loginstart();
        return true;
    }
    return false;
}

void SGWelComeLayer::onEnter()
{
    CCLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 100, true);
}

void SGWelComeLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void SGWelComeLayer::showbox()
{
    SGBindSuccessBox *success = SGBindSuccessBox::create(this);
    SGMainManager::shareMain()->showBox(success);
}

//sdk的facebook界面更新
void SGWelComeLayer::updateViewFBForSdk(bool isLogin)
{
    //isLogin == true, fb在登陆， isLogin == false ,fb在注销
    
    if (registerBtn)
    {
        registerBtn->setVisible(!isLogin);
        registerBtn->setEnabled(!isLogin);
        //如果是注销
        if(!isLogin)
        {
            registerBtn->setFont(str_ServerItem_str17);
        }
    }
    
    //facebook登陆按钮存在，根据isSdkFbLogin确定是否显示
    if (facebookBt)
    {
        facebookBt->setVisible(!isLogin);
        facebookBt->setEnabled(!isLogin);
    }
    
    //如果登出按钮存在
    if (facebookLogOut)
    {
        facebookLogOut->setVisible(isLogin);
        facebookLogOut->setEnabled(isLogin);
        //如果登入
        if (isLogin)
        {
            //如果有渠道登入则注销
            std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
            //显示userName的内容，facebook对应的是用户名称
            if (un.size() > NMAELIMIT)
            {
                un = un.substr(0,NMAELIMIT-3);
                un.append("...");
            }
            facebookLogOut->setFont(un.c_str());
            CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount",true);
            CCUserDefault::sharedUserDefault()->setBoolForKey("isExist",true);
            
            if (startBtn)
            {
                startBtn->init("login_ksksbtn.png", NULL, this, menu_selector(SGWelComeLayer::fastStartHandler),CCPointZero,false,true);
            }
        }
    }

}



void SGWelComeLayer::updateViewForFB()
{
    int isLogin = SGMainManager::shareMain()->getIsFBLogin();
    CCLOG("go2play -- isLogin == %d", isLogin);
    CCLOG("go2play -- registerBtn == %p", registerBtn);
    
    
    if (registerBtn)
    {
        registerBtn->setVisible(!(isLogin == 1));
        registerBtn->setEnabled(!(isLogin == 1));
        if(isLogin == -1 &&
           CCUserDefault::sharedUserDefault()->getBoolForKey("XdFBLoginState"))
        {
           
            //if (facebookLogOut && facebookLogOut->isVisible())
            {
                //facebook注销
                registerBtn->setFont(str_ServerItem_str17);
            }
        }
    }
    
    if (guestBidBtn)
    {
        bool isvis = (!(isLogin == 1)) && (!CCUserDefault::sharedUserDefault()->getBoolForKey("isAccount"));
        guestBidBtn->setVisible(isvis);
        guestBidBtn->setEnabled(isvis);
    }
    
    if (facebookBt) {
        facebookBt->setVisible(!(isLogin == 1));
        facebookBt->setEnabled(!(isLogin == 1));
    }
    
    CCLOG("go2play -- facebookLogout == %p", facebookLogOut);
    if (facebookLogOut)
    {
        facebookLogOut->setVisible((isLogin == 1));
        facebookLogOut->setEnabled((isLogin == 1));
        if (isLogin == 1)
        {
            //如果有渠道登入则注销

            std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
            if (un.size() > NMAELIMIT) {
                un = un.substr(0,NMAELIMIT-3);
                un.append("...");
            }
            facebookLogOut->setFont(un.c_str());
            CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount",true);
            CCUserDefault::sharedUserDefault()->setBoolForKey("isExist",true);
            if (startBtn)
            {
                startBtn->init("login_ksksbtn.png", NULL, this, menu_selector(SGWelComeLayer::fastStartHandler),CCPointZero,false,true);
            }
            
            SGMainManager::shareMain()->setIsFbAutologin(true);
            SGMainManager::shareMain()->doFBLogin();//修改
            
            
        }
    }
}
void SGWelComeLayer::logOutFB()
{
#if (GAME_ACCOUNT_TYPE == GCAT_XDYOU) //MM: 這是原來的邏輯。
    #if (PLATFORM == IOS)
        startFacebookLogin();
    #else
        AndroidSDKAdp::getInstance()->fbLogout();
    #endif
#elif (GAME_ACCOUNT_TYPE == GCAT_GO2PLAY) //MM: 这是新增的逻辑。
    SdkController::gi()->getSdkImp()->sdkFBLogout();
#endif
}
void SGWelComeLayer::clearGust()
{
    if (registerBtn)
    {
        registerBtn->setFont(str_ServerItem_str17);
    }
}

//MM: 已选过服务器，且更新过该服务器信息后，则会走到这里，是登陆流程的一部分。参考HTTP消息MSG_HTTP_UPDATE_SERVER_STATE。
void SGWelComeLayer::realLogin()
{
    
    realLoginProcess();
}

//统一执行逻辑，主要因为SGServerListNewServerBox对登陆的流程与本UI不一样。
void SGWelComeLayer::realLoginProcess()
{
    std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    std::string pw = CCUserDefault::sharedUserDefault()->getStringForKey("password");
    
    bool isAccount = CCUserDefault::sharedUserDefault()->getBoolForKey("isAccount");
    if (strcmp(un.c_str(), "") == 0 && strcmp(un.c_str(), "") == 0 && !isAccount)
    {
        CCLOG("第一次进入游戏");
        SGMainManager::shareMain()->dealIosOKLogin();
    }
    else
    {
        if ((strcmp(registerBtn->getFont(), str_ServerItem_str17) == 0 ) && isAccount && SGMainManager::shareMain()->getIsFBLogin() != 1)
        {
            SG_SHOW_WINDOW(str_NeedInput);
            return;
        }
        SGMainManager::shareMain()->getMainScene()->plat->showConnection();
        //加入渠道判定，如果是朋游的SDK则进入朋游的SDK处理
        if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
        {
            SGMainManager::shareMain()->dealIosLogin(un,pw);
        }
        //非xdyou的SDK，进入其他的SDK统一处理
        else //MMD: ACCOUNT 非XDYOU的账号记得登陆是在这里处理的噢！
        {
            if (ExtClassOfSDK::sharedSDKInstance()->isLoginSDK())
            {
                CCLOG("sdk已经登入！请等待！");
                std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
                std::string loginKey = CCUserDefault::sharedUserDefault()->getStringForKey("loginKey");
                int channelId = CCUserDefault::sharedUserDefault()->getIntegerForKey("channelId");
                int subChannelId = CCUserDefault::sharedUserDefault()->getIntegerForKey("subChannelId");
                std::string nickName = CCUserDefault::sharedUserDefault()->getStringForKey("nickName");
                //开始登陆
                SGMainManager::shareMain()->doLogin(userId,loginKey.c_str(), channelId, subChannelId, nickName.c_str());
            }
            else
            {
                CCLOG("sdk没有登入！请研究！");
            }
        }
    }
}

void SGWelComeLayer::doFbBindFail()
{
    int isLogin = SGMainManager::shareMain()->getIsFBLogin();
    if (isLogin == 1)
    {
        CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount",false);
        logOutFB();
        registerBtn->setFont(str_WdelcomeLayer_str4);
        if(guestBidBtn)
        {
            guestBidBtn->setVisible(true);
            guestBidBtn->setEnabled(true);
        }
        else
        {
            
            guestBidBtn = SGButton::create("binding_account.png",
                                           NULL,
                                           this,
                                           menu_selector(SGWelComeLayer::bindGuestHandler), CCPointZero,false, true);
        
            guestBidBtn->setPosition(ccp(registerBtn->getPositionX() - registerBtn->getContentSize().width/2 -36 ,
                                         registerBtn->getPositionY() + guestBidBtn->getContentSize().height / 16));
            guestBidBtn->runAction(CCRepeatForever::create(CCSequence::create(CCScaleTo::create(0.6,0.8),CCScaleTo::create(0.6,1.0),NULL)));
            this->addBtn(guestBidBtn);
        
        }
        
    }
}

//属于本layer的测试代码。
void SGWelComeLayer::testSomething()
{
    /*
     //MMDBG: BGN 测试统一绘制ICON和按钮。
     
     //gold
     CCSprite* spt = SGDrawCards::createNormalSprite(9999, BIT_GOLD, 1000, sg_welComeLayer);
     CCSize wSize = CCDirector::sharedDirector()->getWinSize();
     spt->setPosition(ccp(wSize.width * 0.2, wSize.height * 0.8));
     this->addChild(spt, 1000);
     
     CCSprite* spt2 = SGDrawCards::createComplexSprite(9999, BIT_GOLD, 1000, sg_welComeLayer);
     spt2->setPosition(ccp(wSize.width * 0.4, wSize.height * 0.8));
     this->addChild(spt2, 1000);
     
     CCSprite* spt3 = SGDrawCards::createDetailedSprite(9999, BIT_GOLD, 1000, sg_welComeLayer);
     spt3->setPosition(ccp(wSize.width * 0.6, wSize.height * 0.8));
     this->addChild(spt3, 1000);
     
     //other
     CCSprite* oicon1 = SGDrawCards::createNormalSprite(1114, BIT_OFFICER, 5, sg_welComeLayer);
     oicon1->setPosition(ccp(wSize.width * 0.2, wSize.height * 0.4));
     this->addChild(oicon1, 1000);
     
     CCSprite* oicon2 = SGDrawCards::createComplexSprite(1114, BIT_OFFICER, 6, sg_welComeLayer);
     oicon2->setPosition(ccp(wSize.width * 0.4, wSize.height * 0.4));
     this->addChild(oicon2, 1000);
     
     CCSprite* oicon3 = SGDrawCards::createDetailedSprite(1114, BIT_OFFICER, 7, sg_welComeLayer);
     oicon3->setPosition(ccp(wSize.width * 0.6, wSize.height * 0.4));
     this->addChild(oicon3, 1000);
     
     //btn
     SGButton* btn1 = SGDrawCards::createNormalButton(1114, BIT_OFFICER, 6, sg_welComeLayer, this, menu_selector(SGWelComeLayer::showFBBindBox));
     btn1->setPosition(ccp(wSize.width * 0.2, wSize.height * 0.9));
     btn1->setZOrder(200);
     this->addBtn(btn1);
     
     SGButton* btn2 = SGDrawCards::createDetailedButton(1114, BIT_OFFICER, 6, sg_welComeLayer, this, menu_selector(SGWelComeLayer::showFBBindBox));
     btn2->setPosition(ccp(wSize.width * 0.4, wSize.height * 0.9));
     btn2->setZOrder(200);
     this->addBtn(btn2);
     
     //MMDBG: END
     */
}

