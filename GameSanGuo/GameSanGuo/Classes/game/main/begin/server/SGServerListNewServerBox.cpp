//
//  SGServerListNewServerBox.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-1-25.
//
//

#include "SGServerListNewServerBox.h"
#include "SGSocketClient.h"
#include "SGHttpClient.h"
#include "SGMsgId.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
//#import "SdkReqCallBack.h"
#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "SdkManager.h"
#endif
#include "SGWelComeLayer.h"

SGServerListNewServerBox::SGServerListNewServerBox()
{
    m_serverName = NULL;
    m_pServerIp = NULL;
    m_sServerPost = 0;
    
    CCLOG("SGServerListNewServerBox()");
}

SGServerListNewServerBox::~SGServerListNewServerBox()
{
 
//
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_serverListNewServerBox);
	//10.25//goerge
//	CCTextureCache::sharedTextureCache()->removeTextureForKey("public/public0.png");
//	CCTextureCache::sharedTextureCache()->removeTextureForKey("public/publicOther.png");
//	CCTextureCache::sharedTextureCache()->removeTextureForKey("login/login0.png");
	
    m_serverName->release();
    m_pServerIp->release();
    CCLOG("~SGServerListNewServerBox()");
}

SGServerListNewServerBox* SGServerListNewServerBox::create(CCString *serverName,SGBoxDelegate *target,CCString *serverIp ,unsigned short serverPost)
{
    SGServerListNewServerBox *serverBox = new SGServerListNewServerBox();
    serverBox->m_serverName = serverName;
    serverBox->m_serverName->retain();
    serverBox->_target = target;
    serverBox->m_pServerIp = serverIp;
    serverBox->m_pServerIp->retain();
    serverBox->m_sServerPost = serverPost;

    if(serverBox && serverBox->SGBaseBox::init(target, box_registerTag, CCSizeMake(535,324)))
    {
        serverBox->initView();
        serverBox->autorelease();
        return serverBox;
    }
    
    CC_SAFE_DELETE(serverBox);
    return NULL;
}

void SGServerListNewServerBox::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_serverListNewServerBox);
    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_serverListNewServerBox);
    
    SGButton *okButton = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(SGServerListNewServerBox::buttonClickOK),CCPointZero,false,true);
    SGButton *cancelButton = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this, menu_selector(SGServerListNewServerBox::buttonClickCancel),CCPointZero,false,true);

    SGCCLabelTTF *labelContent_1 = SGCCLabelTTF::create("您之前没有玩过这个服，", FONT_BOXINFO, 32);
    SGCCLabelTTF *labelContent_2 = SGCCLabelTTF::create("要在   ", FONT_BOXINFO, 32);
	SGCCLabelTTF *labelContent_3 = SGCCLabelTTF::create(" 新建一个角色吗？", FONT_BOXINFO, 32);
    SGCCLabelTTF *labelServerName = SGCCLabelTTF::create(this->m_serverName->getCString(), FONT_BOXINFO, 32 , ccRED);
    
    SGCCLabelTTF* createRole = SGCCLabelTTF::create("新建角色", FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //createRole->setOutSideColor(ccc3(143,87,0));

    this->addChild(createRole,1);
    //modify by:zyc. merge into create.
    //labelServerName->setColor(ccRED);//ccc3(0xff, 0xf0, 0x00)
    labelServerName->setAnchorPoint(ccp(0, 0.5));
    this->addBtn(okButton);
    this->addBtn(cancelButton);
    this->addChild(labelContent_1);
    this->addChild(labelContent_2);
    this->addChild(labelServerName);
	this->addChild(labelContent_3);;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    createRole->setPosition(biao->getPosition());
    labelContent_1->setPosition(ccpAdd(center, ccp(0, 50)));
    labelContent_2->setPosition(ccpAdd(center, ccp(-140, 0)));
    labelServerName->setPosition(ccpAdd(labelContent_2->getPosition(), ccp(labelContent_2->getContentSize().width / 2, 0)));
    labelContent_3->setPosition(ccpAdd(center, ccp(-50, -50)));
	
    okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+40)));
    cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+40)));
    
}

//点击确定按钮
void SGServerListNewServerBox::buttonClickOK(SGButton *sender)
{
    EFFECT_PLAY(MUSIC_BTN);

    SGWelComeLayer* wl = dynamic_cast<SGWelComeLayer*>(SGMainManager::shareMain()->getNowShowLayer());
    if (wl)
    {
        wl->realLoginProcess();
    }
    else
    {
        CCLOG("SGServerListNewServerBox: what the hell?!");
    }
    
    return;

    //拆分之前的老逻辑，留档备案，注释太麻烦。
    //SGHttpClient::shareHttpClient()->login(this,accountId, m_sServerPost);
    
    std::string un = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    std::string pw = CCUserDefault::sharedUserDefault()->getStringForKey("password");
    //short sid = (short)CCUserDefault::sharedUserDefault()->getIntegerForKey("serverid");
	
	
    
//    id  sdkReqCallBack = SGMainManager::shareMain()->getSdkdelegate();
    if (GameConfig::isExist())
    {
		
		CCLOG("Account isExist");
        if (strcmp(un.c_str(), "") == 0 && strcmp(un.c_str(), "") == 0) {
            CCLOG("第一次进入游戏");
	
//#if (PLATFORM == IOS)
            SGMainManager::shareMain()->dealIosOKLogin();
//            
//			//OcCaller::getInstance()->okLoginSDK();
//#else
//			//android code
//			CCLOG("UserLogin000");
//			SdkManager::shareSdkManager()->oKLogin();
//#endif
			/*
            [[PYUserSDKLib sharedUserSDKLib] oKLogin:@"1"
                                       ChannelId:[sdkReqCallBack ChannelId]
                                    SubChannelId:[sdkReqCallBack SubChannelId]
                                       OsVersion:[sdkReqCallBack OsVersion]
                                          OsType:[sdkReqCallBack OsType]
                                CallBackDelegate:sdkReqCallBack
                                  FinishSelector:@selector(fastLoginFinished:) FailSelector:@selector(requestFailed:)];
			 */
        }
        else
		{
//#if (PLATFORM == IOS)
            
            SGMainManager::shareMain()->dealIosLogin(un,pw);
//            
//			//OcCaller::getInstance()->pyUserSDKLoginReq(un.c_str(), pw.c_str());
//#else
//			//android code
//			CCLOG("UserLogin----");
//			SdkManager::shareSdkManager()->userLogin(un, pw);
//#endif
			/*
            [[PYUserSDKLib sharedUserSDKLib] userLogin:[[NSString alloc] initWithUTF8String:un.c_str()]
                                          Password:[[NSString alloc] initWithUTF8String:pw.c_str()]
                                            GameId:@"1"
                                         ChannelId:[sdkReqCallBack ChannelId]
                                      SubChannelId:[sdkReqCallBack SubChannelId]
                                         OsVersion:[sdkReqCallBack OsVersion]
                                            OsType:[sdkReqCallBack OsType]
                                  CallBackDelegate:sdkReqCallBack FinishSelector:@selector(loginFinished:) FailSelector:@selector(requestFailed:)];
			 */
        }
    }
    else
    {
        CCLOG("第一次进入游戏");
#if (PLATFORM == IOS)
        SGMainManager::shareMain()->dealIosOKLogin();
        
		//OcCaller::getInstance()->okLoginSDK();
#else
		//android code
		CCLOG("UserLogin====");
		SdkManager::shareSdkManager()->oKLogin();
#endif
		/*
        [[PYUserSDKLib sharedUserSDKLib] oKLogin:@"1"
                                       ChannelId:[sdkReqCallBack ChannelId]
                                    SubChannelId:[sdkReqCallBack SubChannelId]
                                       OsVersion:[sdkReqCallBack OsVersion]
                                          OsType:[sdkReqCallBack OsType]
                                CallBackDelegate:sdkReqCallBack
                                  FinishSelector:@selector(fastLoginFinished:) FailSelector:@selector(requestFailed:)];
		 */
    }
}

//点击取消按钮
void SGServerListNewServerBox::buttonClickCancel(SGButton *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxCloseWithOutCallBack();
}

void SGServerListNewServerBox::requestFinished(int msgid, cocos2d::CCObject *data)
{
    if(msgid == MSG_HTTP_LOGIN)
    {
        //记录老帐号
//        CCDictionary *dict = (CCDictionary*)data;
//        CCString* randUserName = (CCString*)dict->objectForKey("randusername");
//        CCUserDefault::sharedUserDefault()->setStringForKey("randusername", randUserName->getCString());
        
        SGSocketClient::sharedSocketClient()->startConn(m_pServerIp->getCString(),m_sServerPost);
    }
}

