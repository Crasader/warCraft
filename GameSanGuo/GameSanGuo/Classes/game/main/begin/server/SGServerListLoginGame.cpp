//
//  SGServerListLoginGame.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-1-28.
//
//

#include "SGServerListLoginGame.h"
#include "SGMainManager.h"
#include "SGHttpClient.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGRegisterBox.h"
#include "SGModificationPassword.h"
#include "SGSkillInfoBox.h"
#include "SGCantAdvanceBox.h"
#include "SGShowString.h"
#include "ResourceManager.h"
//#import "SdkReqCallBack.h"
#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "SdkManager.h"
#endif

#include "ExtClass.h"
#include "GlobalConfig.h"
#include "SdkController.h"

#pragma mark - 构造函数

SGServerListLoginGame::SGServerListLoginGame()
:isAccount(false),
editBoxAccount(NULL),
editBoxPassword(NULL)
{
}

SGServerListLoginGame::~SGServerListLoginGame()
{
    CCLOG("~SGServerListLoginGame");

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_listLoginLayer);
}

SGServerListLoginGame* SGServerListLoginGame::create()
{
    SGServerListLoginGame *loginGame = new SGServerListLoginGame();
     if(loginGame && loginGame->init(NULL, sg_listLoginLayer))
    {
        loginGame->constructView();
        loginGame->autorelease();
        return loginGame;
    }
    
    CC_SAFE_DELETE(loginGame);
    return NULL;
}

void SGServerListLoginGame::constructView()
{

    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI, sg_listLoginLayer);
    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_listLoginLayer);

    
    this->setTouchEnabled(true);
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    isAccount = CCUserDefault::sharedUserDefault()->getBoolForKey("isAccount");
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
//顶端部分
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titlecenter,100);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_l,100);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_r,100);
    
//返回按钮
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGServerListLoginGame::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55 - 3, - backBtn->getContentSize().height*.55)));

    
//标题 背景 logo

    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ServerItem_str12, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc . merge into create func.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccp(s.width/2, title_bg->getPosition().y - title_bg->getContentSize().height*.52));
    this->addChild(title);
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_listLoginLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("storyBg.png");
    
    //CCRect r = CCRectMake(0, 1136/2 - (s.height - title_bg->getContentSize().height )/2, bg->getContentSize().width, s.height - title_bg->getContentSize().height);
    //bg->cocos2d::CCNode::setScale(r.size.width / bg->getContentSize().width, r.size.height / bg->getContentSize().height);
    //bg->setTextureRect(r);
//    bg->setScaleY(s.height/2 / bg->getContentSize().height);
//    bg->setScaleX(s.width / bg->getContentSize().width);
    bg->setScale(std::max(s.width / bg->getContentSize().width, s.height / bg->getContentSize().height));
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 0)));
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/logo.plist", RES_TYPE_LAYER_UI, sg_listLoginLayer);

    CCSprite *logo = CCSprite::createWithSpriteFrameName("logo.png");
    logo->setScale(.72);
    this->addChild(logo);
    logo->setVisible(false);
    logo->setPosition(ccpAdd(center, ccp(0, 250)));
    
//输入框 帐号  密码
    CCSprite *accountbg = CCSprite::createWithSpriteFrameName("login_register_dik.png");
    this->addChild(accountbg);
    accountbg->setPosition(ccpAdd(center, ccp(0, 30)));
    
//    CCSprite *spr_account = CCSprite::createWithSpriteFrameName("login_font_email.png");
    SGCCLabelTTF *spr_account = SGCCLabelTTF::create(str_RegisterBox_str16, FONT_XINGKAI, 30);
    this->addChild(spr_account);
    spr_account->setAnchorPoint(ccp(0, 0.5));
    spr_account->setPosition(ccpAdd(accountbg->getPosition(), ccp(-240, 30)));//-220
    
//    CCSprite *spr_password = CCSprite::createWithSpriteFrameName("login_font_password.png");
    SGCCLabelTTF *spr_password = SGCCLabelTTF::create(str_RegisterBox_str17, FONT_XINGKAI, 30);
    this->addChild(spr_password);
    spr_password->setAnchorPoint(ccp(0, 0.5));
    spr_password->setPosition(ccpAdd(accountbg->getPosition(), ccp(-220, -40)));
        
    editBoxAccount = CCEditBox::create(CCSizeMake(360, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    editBoxAccount->setInputMode(kEditBoxInputModeEmailAddr);
//    editBoxAccount->setFontColor(ccRED);
    editBoxAccount->setMaxLength(30);
    editBoxAccount->setPosition(ccpAdd(accountbg->getPosition(), ccp(39, 30)))
    ;
    this->addChild(editBoxAccount);
    editBoxAccount->setDelegate(this);
    
    editBoxPassword = CCEditBox::create(CCSizeMake(360, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    editBoxPassword->setInputFlag(kEditBoxInputFlagPassword);
    editBoxPassword->setInputMode(kEditBoxInputModeSingleLine);
    editBoxPassword->setMaxLength(16);
    editBoxPassword->setPosition(ccpAdd(accountbg->getPosition(), ccp(39, -40)));
	
	editBoxPassword->setReturnType(kKeyboardReturnTypeDone);
	editBoxAccount->setReturnType(kKeyboardReturnTypeDone);
    this->addChild(editBoxPassword);
    editBoxPassword->setDelegate(this);
    
//登陆部分
    CCSprite *bgl = CCSprite::createWithSpriteFrameName("login_dik_l.png");
    this->addChild(bgl,1);
    bgl->setAnchorPoint(ccp(0, 0.5));
    bgl->setPosition(ccpAdd(accountbg->getPosition(), ccp(-accountbg->getContentSize().width*.5, -150)));
    
    CCSprite *bgr = CCSprite::createWithSpriteFrameName("login_dik_r.png");
    this->addChild(bgr,1);
    bgr->setAnchorPoint(ccp(1, 0.5));
    bgr->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width,0)));
    
    CCSprite *bgm = CCSprite::createWithSpriteFrameName("login_dik_m.png");
    this->addChild(bgm);
    bgm->setScaleX(((accountbg->getContentSize().width - bgr->getContentSize().width*2)+5.5)/bgm->getContentSize().width);
    bgm->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width*.5, 1)));

//登陆按钮
    SGButton *okButton = SGButton::createFromLocal("loginbtnbg.png", str_ServerItem_str18, this, menu_selector(SGServerListLoginGame::buttonClickLoginGame),CCPointZero,FONT_PANGWA,ccWHITE,32);
    
    this->addBtn(okButton);
    okButton->setPosition(bgm->getPosition());
    
//注册 修改密码 升级帐号
    CCSprite *downbg = CCSprite::createWithSpriteFrameName("login_register_dik.png");
    this->addChild(downbg);
    downbg->setPosition(ccpAdd(center, ccp(0, -362)));
    
//    CCMenuItemSprite *creatNewAccount = CCMenuItemSprite::create(CCSprite::create("login/login_font_createNewAccount.png"), CCSprite::create("login/login_font_createNewAccount.png"), this, menu_selector(SGServerListLoginGame::registerHandler));
//    this->addBtn(creatNewAccount);
    CCSprite *creatNewAccount = CCSprite::createWithSpriteFrameName("login_font_createNewAccount.png");
    addChild(creatNewAccount);
    creatNewAccount->setTag(9);
    creatNewAccount->setAnchorPoint(ccp(0, 0.5));
    creatNewAccount->setPosition(ccpAdd(downbg->getPosition(), ccp(-220, 30)));
    
    CCSprite *jiantou1 = CCSprite::createWithSpriteFrameName("login_jiantou.png");
    this->addChild(jiantou1);
    jiantou1->setPosition(ccpAdd(creatNewAccount->getPosition(), ccp(440, 0)));
    
    CCSprite *upAccount = NULL;
    if (GameConfig::isExist())
    {
        if (isAccount)
        {
            std::string username = CCUserDefault::sharedUserDefault()->getStringForKey("username");
            std::string password = CCUserDefault::sharedUserDefault()->getStringForKey("password");
            
            //MMDEBUG: FBADR fb不应该显示其真实的id和pw。
            if (strcmp(username.c_str(), "") == 0 || username.find("XD_THIRDPARTY_FB_") != std::string::npos)
            {
                editBoxAccount->setPlaceHolder(str_RegisterBox_str1);
            }
            else
            {
                editBoxAccount->setText( username.c_str());
            }
            
            if (strcmp(password.c_str(), "") == 0 || username.find("XD_THIRDPARTY_FB_") != std::string::npos)
            {
                editBoxPassword->setPlaceHolder(str_RegisterBox_str1);
            }
            else
            {
                editBoxPassword->setText( password.c_str());
            }
            
//            editBoxAccount->setText( username.c_str());
//            editBoxPassword->setText(password.c_str());
            
            upAccount = CCSprite::createWithSpriteFrameName("login_font_xgmm.png");
            
        }
        if (!isAccount)
        {
            editBoxAccount->setPlaceHolder(str_RegisterBox_str1);
            editBoxPassword->setPlaceHolder(str_RegisterBox_str1);
            upAccount = CCSprite::createWithSpriteFrameName("login_font_accountup.png");
        }
    }
    else
    {
        editBoxAccount->setPlaceHolder(str_RegisterBox_str1);
        editBoxPassword->setPlaceHolder(str_RegisterBox_str1);
        upAccount = CCSprite::createWithSpriteFrameName("login_font_accountup.png");
    }
    this->addChild(upAccount);
    upAccount->setTag(10);
    upAccount->setAnchorPoint(ccp(0, 0.5));
    upAccount->setPosition(ccpAdd(downbg->getPosition(), ccp(-220, -40)));
    CCSprite *jiantou2 = CCSprite::createWithSpriteFrameName("login_jiantou.png");
    this->addChild(jiantou2);
    jiantou2->setPosition(ccpAdd(upAccount->getPosition(), ccp(440, 0)));
    
    //如果不是xdyou的渠道，不显示修改密码
    if (GAME_ACCOUNT_TYPE != GCAT_XDYOU)
    {
        upAccount->setVisible(false);
        jiantou2->setVisible(false);
    }
    
}

//void SGServerListLoginGame::processFlag(int first , int secend)
//{
//    // 0 不可见 1 正确 2错误
//    CCSprite *sprite1 = CCSprite::create("public/ok.png");
//    CCSpriteFrame *frame1 = sprite1->displayFrame();
//    CCSprite *sprite2 = CCSprite::create("public/error.png");
//    CCSpriteFrame *frame2 = sprite2->displayFrame();
//    
//    spriteFlag_1->setVisible(first != 0);
//    if(first == 1)
//    {
//        spriteFlag_1->setDisplayFrame(frame1);
//    }
//    else if(first == 2)
//    {
//        spriteFlag_1->setDisplayFrame(frame2);
//    }
//    
//    spriteFlag_2->setVisible(secend != 0);
//    if(secend == 1)
//    {
//        spriteFlag_2->setDisplayFrame(frame1);
//    }
//    else if(secend == 2)
//    {
//        spriteFlag_2->setDisplayFrame(frame2);
//    }
//}

void SGServerListLoginGame::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
//    SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_listLoginLayer, true);
//    ((SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_welComeLayer))->setIsCanTouch(true);
//    this->boxCloseWithOutCallBack();
    SGMainManager::shareMain()->showwelcomelayer();
}
void SGServerListLoginGame::setIsNewAccount(bool newAccount)
{
    isAccount = newAccount;
    CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount",newAccount);
    if (!newAccount) {
       editBoxAccount->setText("");
       editBoxPassword->setText("");
    }
    
}
void SGServerListLoginGame::buttonClickLoginGame()
{
    EFFECT_PLAY(MUSIC_BTN);
    if ( strcmp(editBoxAccount->getText(), "")  == 0 || strcmp(editBoxPassword->getText(), "")  == 0) {
        SG_SHOW_WINDOW(str_SHOWWINDOW_1012);
        return;
    }

#if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
    if (GameConfig::isExist())
    {
        isAccount = CCUserDefault::sharedUserDefault()->getBoolForKey("isAccount");
        CCLOG("go2play -- isAccount == %d", isAccount);
//        if (isAccount)
        //2014-10-31 注册流程更改 在游客绑定页面 新增使用老账号登陆 谨：此处不再判断游客。
		{
            const char *userAccount = editBoxAccount->getText();
            const char *userPassword = editBoxPassword->getText();
            //SGHttpClient::shareHttpClient()->login(this, userAccount , userPassword,serverId);
			
            std::string un(userAccount);
            std::string pw(userPassword);
            SGMainManager::shareMain()->dealIosLogin(un,pw);
		}

    }
    else
    {
        CCLOG("go2play -- isn't exists!");

        if (strcmp(editBoxAccount->getText(), "")  != 0)
		{
            const char *userAccount = editBoxAccount->getText();
            const char *userPassword = editBoxPassword->getText();
            //SGHttpClient::shareHttpClient()->login(this, userAccount , userPassword,serverId);
            std::string un(userAccount);
            std::string pw(userPassword);
            SGMainManager::shareMain()->dealIosLogin(un,pw);
        }
        else
        {
            //SGMainManager::shareMain()->showwelcomelayer();
            this->setTouchEnabled(false);
            editBoxAccount->setVisible(false);
            editBoxPassword->setVisible(false);
            SGCantAdvanceBox *box = SGCantAdvanceBox::create(this, NULL, 21, NULL);
            SGMainManager::shareMain()->showBox(box);
        }
    }
#else
    //其他所有非xdyou无SDK自带界面的渠道
    UserAndExtInfo uei;
    uei.userName = editBoxAccount->getText();
    uei.userPwd = editBoxPassword->getText();
    
    SdkController::gi()->getSdkImp()->sdkLogin(uei);

#endif

}

#pragma mark http

void SGServerListLoginGame::requestFinished(int msgid, cocos2d::CCObject *data)
{
    CCDictionary *dict = (CCDictionary *)data;
    if(msgid == MSG_HTTP_LOGIN)
    {
        //记录老帐号
      //  CCDictionary *dict = (CCDictionary*)data;
//        CCString* randUserName = (CCString*)dict->objectForKey("randUserName");
//        CCUserDefault::sharedUserDefault()->setStringForKey("randusername", randUserName->getCString());
        
		//检测当前的渠道ID
		if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
		{
			CCUserDefault::sharedUserDefault()->setStringForKey("username",editBoxAccount->getText());
			CCUserDefault::sharedUserDefault()->setStringForKey("password",editBoxPassword->getText());
			CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount", true);
			CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", true);
		}
		else
		{
			std::string nickName = CCUserDefault::sharedUserDefault()->getStringForKey("nickName");
			CCUserDefault::sharedUserDefault()->setStringForKey("username", nickName);
			//CCUserDefault::sharedUserDefault()->setStringForKey( "password",editBoxPassword->getText());
			CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount", true);
			CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", true);
		}
		

        
        CCUserDefault::sharedUserDefault()->flush();
        
        std::string serverip = CCUserDefault::sharedUserDefault()->getStringForKey("serverip");
      //  short serverpost = CCUserDefault::sharedUserDefault()->getIntegerForKey("serverpost");
        //SGSocketClient::sharedSocketClient()->startConn(serverip.c_str(), serverpost);
        SGMainManager::shareMain()->showwelcomelayer();
    }else if(msgid == MSG_HTTP_LOGIN_ERROR)
    {
        CCString *string = (CCString*)dict->objectForKey("error");
        
        if(string->intValue() == 1)
        {
//            labelAccountError->setVisible(true);
//            labelPasswordError->setVisible(false);
//            this->processFlag(2, 0);
            SG_SHOW_WINDOW(str_SHOWWINDOW_1013);
            
        }else{
//            labelAccountError->setVisible(false);
//            labelPasswordError->setVisible(true);
//            this->processFlag(1,2);
            SG_SHOW_WINDOW(str_SHOWWINDOW_1014);

        }
        
    }else if(msgid == MSg_HTTP_SERVERLIST)
    {
        CCUserDefault::sharedUserDefault()->setStringForKey("username", editBoxAccount->getText());
        CCUserDefault::sharedUserDefault()->setStringForKey("password", editBoxPassword->getText());
        CCUserDefault::sharedUserDefault()->flush();        
//        this->boxClose();
    }else if(msgid == MSG_HTTP_FAST_LOGIN)
    {    
                
                CCDictionary *dict = (CCDictionary*)data;
                
                //用户名
                CCString *username_ = (CCString *)dict->objectForKey("username");
                CCUserDefault::sharedUserDefault()->setStringForKey("username", username_->getCString());
                
                //密码
                CCString *password_ = (CCString*)dict->objectForKey("password");
                CCUserDefault::sharedUserDefault()->setStringForKey("password", password_->getCString());
                
                //ip地址和端口,id
                CCString *serverIp = (CCString*)dict->objectForKey("serverip");
                CCUserDefault::sharedUserDefault()->setStringForKey("serverip", serverIp->getCString());
                
                CCString *serverpost = (CCString *)dict->objectForKey("serverpost");
                CCUserDefault::sharedUserDefault()->setIntegerForKey("serverpost", serverpost->intValue());
                
                CCLog("快速登陆返回的ip地址 %s",serverIp->getCString());
                CCLog("快速登陆返回的端口 %s",serverpost->getCString());
                
                CCString *serverid = (CCString *)dict->objectForKey("serverid");
                CCUserDefault::sharedUserDefault()->setIntegerForKey("serverid", serverid->intValue());
                
                //记录服务器ip和端口号
                SGMainManager::shareMain()->setServerInfo((CCString *)dict->objectForKey("serverip"), (short)serverpost->intValue());
                
                //是否有帐号
                CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", true);
                CCUserDefault::sharedUserDefault()->flush();
                
                SGSocketClient::sharedSocketClient()->startConn(serverIp->getCString(),(short)serverpost->intValue());
    }    
}

void SGServerListLoginGame::requestFailed()
{
    CCLog("登陆连接错误");
}
void SGServerListLoginGame::registerHandler()
{
    if (CCUserDefault::sharedUserDefault()->getBoolForKey("isExist") && !CCUserDefault::sharedUserDefault()->getBoolForKey("isAccount")) {
        CCLOG("need bind first  ");
        SGCantAdvanceBox *Box = SGCantAdvanceBox::create(this, NULL, 22, NULL);
        SGMainManager::shareMain()->showBox(Box);
    }
    else
    {
       SGMainManager::shareMain()->showregisterLayer(1);
    }
    
}
void SGServerListLoginGame::passwordHandler()
{
     SGMainManager::shareMain()->showChangePassWord();
}
void SGServerListLoginGame::bindHandler()
{
    
     SGMainManager::shareMain()->showregisterLayer(2);
//    if (GameConfig::isExist())
//    {
////        this->boxCloseWithOutCallBack();
////        SGRegisterBox *registerBox = SGRegisterBox::create(this,2);
////        SGMainManager::shareMain()->showBox(registerBox);
//       
//    }else
//    {
//        SGMainManager::shareMain()->showMessage("您还未进入过游戏，不能升级帐号");
//    }
}
void SGServerListLoginGame::editBoxEditingDidBegin(cocos2d::extension::CCEditBox *editBox)
{
//    SGButton *a = SGButton::create("login/login_dele.png", "login/login_dele.png", this, menu_selector(SGServerListLoginGame::cleanEditText),CCPointZero,false,false);
//    this->addBtn(a);
//    a->setPosition(ccpAdd(editBox->getPosition(), ccp(0, 0)));
}
void SGServerListLoginGame::editBoxEditingDidEnd(cocos2d::extension::CCEditBox *editBox)
{
}
void SGServerListLoginGame::editBoxTextChanged(CCEditBox* editBox, const std::string& text)
{
}
void SGServerListLoginGame::editBoxReturn(CCEditBox* editBox)
{
}
void SGServerListLoginGame::cleanEditText(cocos2d::extension::CCEditBox *editBox)
{
}
//void SGServerListLoginGame::onEnter()
//{
//   // CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, false);
//}
//
//void SGServerListLoginGame::onExit()
//{
//    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
//}
//
//
//
void SGServerListLoginGame::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool SGServerListLoginGame::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint pt = pTouch->getLocationInView();
    pt = CCDirector::sharedDirector()->convertToGL(pt);
    CCSprite *a = (CCSprite*)this->getChildByTag(9);
    CCRect rect = CCRectMake(a->getPosition().x,a->getPosition().y-a->getContentSize().height*.5f, a->getContentSize().width +456, a->getContentSize().height);
    if(rect.containsPoint(pt))
    {
        this->setTouchEnabled(false);
        editBoxAccount->setVisible(false);
        editBoxPassword->setVisible(false);
        registerHandler();
        return true;
    }
    
    CCSprite *b = (CCSprite*)this->getChildByTag(10);
    CCRect rectb = CCRectMake(b->getPosition().x,b->getPosition().y-b->getContentSize().height*.5f, b->getContentSize().width +456, b->getContentSize().height);
    if(rectb.containsPoint(pt))
    {
        if (GameConfig::isExist())
        {
            if (isAccount)
            {
                //是xdyou才显示修改密码
                if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
                {
                    passwordHandler();
                }
            }
            if (!isAccount)
            {
                bindHandler();
            }
        }else
        {
            bindHandler();
        }

        return true;
    }
    return true;
}

void SGServerListLoginGame::showBoxCall(cocos2d::CCObject *obj)
{
    bool needEnable = true;
    if (obj)
    {
        int dataValue = ((CCString*)obj)->intValue();
        if (dataValue == -1)
        {
            needEnable = false;
        }
    }
    this->setIsCanTouch(needEnable);
    this->setTouchEnabled(true);
}

void SGServerListLoginGame::setIsCanTouch(bool isTouch)
{
    menu->setEnabled(isTouch);
    editBoxAccount->setTouchEnabled(isTouch);
    editBoxPassword->setTouchEnabled(isTouch);
    editBoxAccount->setVisible(isTouch);
    editBoxPassword->setVisible(isTouch);
}

