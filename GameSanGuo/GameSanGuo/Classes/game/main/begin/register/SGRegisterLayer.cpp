//
//  SGRegisterLayer.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-3.
//
//


#include "SGRegisterLayer.h"
#include "SGMsgId.h"
#include "SGRegisterSuccess.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include <regex.h>
#include "SGWelComeLayer.h"
#include "SGStringConfig.h"
#include "SGCantAdvanceBox.h"
//#import "SdkReqCallBack.h"
#if (PLATFORM == IOS)
	#include "OcCaller.h"
#else
	#include "SdkManager.h"
    #include "AndroidSDKAdp.h"
#endif
#include "AppMacros.h"
#include "SdkController.h"

SGRegisterLayer::SGRegisterLayer()
:m_pEditName(NULL),
m_pEditPassword(NULL),
m_pEditPasswordAgain(NULL),
EnterType(0),
_target(NULL)
{
}
SGRegisterLayer::~SGRegisterLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_registerLayer);
    
    if(EnterType != 1 && EnterType != 2 && EnterType != 5)
    {
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        if (mainLayer)
        {
            mainLayer->setPortraitMenuCanTouch(true);
        }
        if(EnterType == 4 )
        {
            if (mainLayer)
            {
                mainLayer->setIsCanTouch(true);
            }
        }
    }
}
/*entertype：
 *5  新端 注册登入
 *3  新端 有游客信息 然后点击游客。       todo:请知情人士补充完整……
 *
 */
SGRegisterLayer *SGRegisterLayer::create(int type)
{
    SGRegisterLayer *registerBox = new SGRegisterLayer();
    registerBox->EnterType = type;
 
        if (registerBox && registerBox->init(NULL,sg_registerLayer))
        {
            registerBox->constructView();
            registerBox->autorelease();
            return registerBox;
        }
    CC_SAFE_DELETE(registerBox);
    return NULL;
}

void SGRegisterLayer::constructView()
{
    
    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist",RES_TYPE_LAYER_UI ,sg_registerLayer);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist",RES_TYPE_LAYER_UI ,sg_registerLayer);
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
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

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGRegisterLayer::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55 - 3, - backBtn->getContentSize().height*.55)));

    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_registerLayer);

#if (PLATFORM == ANDROID)
    GameConfig::gcForAndroid();
#endif

    CCSprite *bg = CCSprite::createWithSpriteFrameName("storyBg.png");
    //CCRect r = CCRectMake(0, 1136/2 - (s.height - title_bg->getContentSize().height )/2, s.width, s.height - title_bg->getContentSize().height);
    //bg->setTextureRect(r);
//    bg->setScaleX(s.width / bg->getContentSize().width);
//    bg->setScaleY(s.height / bg->getContentSize().height);
    bg->setScale(std::max(s.width / bg->getContentSize().width, s.height / bg->getContentSize().height));
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(SGLayout::getPoint(kBottomCenter));
    
// 输入框
    
    CCSprite *accountbg = CCSprite::createWithSpriteFrameName("register_diban.png");
    this->addChild(accountbg);
    accountbg->setPosition(ccpAdd(center, ccp(0, 103)));
    
    SGCCLabelTTF *registertitle = NULL;
    registertitle = SGCCLabelTTF::create(str_RegisterBox_str16, FONT_XINGKAI, 32);
    registertitle->setPosition(ccp(24 + registertitle->getContentSize().width/2, accountbg->getContentSize().height - accountbg->getContentSize().height/5) );
    accountbg->addChild(registertitle);
    registertitle = SGCCLabelTTF::create(str_RegisterBox_str17, FONT_XINGKAI, 32);
    registertitle->setPosition(ccp(24 + registertitle->getContentSize().width/2, accountbg->getContentSize().height/2) );
    accountbg->addChild(registertitle);
    registertitle = SGCCLabelTTF::create(str_RegisterBox_str18, FONT_XINGKAI, 32);
    registertitle->setPosition(ccp(24 + registertitle->getContentSize().width/2, accountbg->getContentSize().height*0.18) );
    accountbg->addChild(registertitle);
    
    CCSprite *font = CCSprite::createWithSpriteFrameName("login_font_emailandpw.png");
    this->addChild(font);
    font->setPosition(ccpAdd(accountbg->getPosition(), ccp(-accountbg->getContentSize().width*.5 + font->getContentSize().width*.5, 145)));
    
//登陆部分
    CCSprite *bgl = CCSprite::createWithSpriteFrameName("login_dik_l.png");
    this->addChild(bgl,1);
    bgl->setAnchorPoint(ccp(0, 0.5));
    bgl->setPosition(ccpAdd(accountbg->getPosition(), ccp(-accountbg->getContentSize().width*.5, -208)));
    
    CCSprite *bgr = CCSprite::createWithSpriteFrameName("login_dik_r.png");
    this->addChild(bgr,1);
    bgr->setAnchorPoint(ccp(1, 0.5));
    bgr->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width,0)));
    
    CCSprite *bgm = CCSprite::createWithSpriteFrameName("login_dik_m.png");
    this->addChild(bgm);
    bgm->setScaleX(((accountbg->getContentSize().width - bgr->getContentSize().width*2)+5.5)/bgm->getContentSize().width);
    bgm->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width*.5, 1)));
    
//注册按钮
//标题
    SGCCLabelTTF *title = NULL;
    if(EnterType == 1 || EnterType == 5)
    {
        title = SGCCLabelTTF::create(str_RegisterBox_str19, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
       
        SGButton *registerButton = SGButton::create("loginbtnbg.png", "public_font_queding.png", this, menu_selector(SGRegisterLayer::registerHandler),CCPointZero,false,true);
        this->addBtn(registerButton);
        registerButton->setPosition(bgm->getPosition());
        
        
        
        
        CCSprite *bgl = CCSprite::createWithSpriteFrameName("login_dik_l.png");
        this->addChild(bgl,1);
        bgl->setScaleY(0.8);
        bgl->setAnchorPoint(ccp(0, 0.5));
        bgl->setPosition(ccpAdd(accountbg->getPosition(), ccp(-accountbg->getContentSize().width*.5, -488)));
        
        CCSprite *bgr = CCSprite::createWithSpriteFrameName("login_dik_r.png");
        this->addChild(bgr,1);
        bgr->setScaleY(0.8);
        bgr->setAnchorPoint(ccp(1, 0.5));
        bgr->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width,0)));
        
        CCSprite *bgm = CCSprite::createWithSpriteFrameName("login_dik_m.png");
        this->addChild(bgm);
        bgm->setScaleY(0.8);
        bgm->setScaleX(((accountbg->getContentSize().width - bgr->getContentSize().width*2)+5.5)/bgm->getContentSize().width);
        bgm->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width*.5, 1)));
        
        
        //原有帐号登陆提示
        SGCCLabelTTF *oldAccountLogin = SGCCLabelTTF::create(str_RegisterLayer_login, FONT_PANGWA, 32);
        oldAccountLogin->setTag(9);
        this->addChild(oldAccountLogin, 1);
        //oldAccountLogin->setPosition(ccpAdd(bgm->getPosition(), ccp(-170, 0)));
        oldAccountLogin->setPosition(ccp(24 + oldAccountLogin->getContentSize().width/2 + accountbg->getPositionX() - accountbg->getContentSize().width * 0.5, bgm->getPositionY()  ) );
        
        //右侧箭头
        CCSprite *jiantou1 = CCSprite::createWithSpriteFrameName("login_jiantou.png");
        this->addChild(jiantou1, 2);
        jiantou1->setPosition(ccp( accountbg->getPositionX() + accountbg->getContentSize().width * 0.5 - jiantou1->getContentSize().width * 1.3 , oldAccountLogin->getPosition().y));
        

    }
    else//绑定账号
    {
        ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_registerLayer);
        title = SGCCLabelTTF::create(str_RegisterBox_str20, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
        SGButton *registerButton = SGButton::create("loginbtnbg.png", "reward_font_bind.png", this, menu_selector(SGRegisterLayer::registerHandler),CCPointZero,false,true);
        this->addBtn(registerButton);
        registerButton->setPosition(bgm->getPosition());
        CCLabelTTF *label = CCLabelTTF::create(str_RegisterBox_str21, FONT_BOXINFO, 36);
        label->setColor(ccc3(0xff,0xa2,0));
        label->setPosition(ccpAdd(bgm->getPosition(),ccp(0,70)));
        this->addChild(label);
        
        
        
        //新添功能：如果是游客 给予放弃游客资料的机会
        CCSprite *bgl = CCSprite::createWithSpriteFrameName("login_dik_l.png");
        this->addChild(bgl,1);
        bgl->setScaleY(0.8);
        bgl->setAnchorPoint(ccp(0, 0.5));
        bgl->setPosition(ccpAdd(accountbg->getPosition(), ccp(-accountbg->getContentSize().width*.5, -488)));
        
        CCSprite *bgr = CCSprite::createWithSpriteFrameName("login_dik_r.png");
        this->addChild(bgr,1);
        bgr->setScaleY(0.8);
        bgr->setAnchorPoint(ccp(1, 0.5));
        bgr->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width,0)));
        
        CCSprite *bgm = CCSprite::createWithSpriteFrameName("login_dik_m.png");
        this->addChild(bgm);
        bgm->setScaleY(0.8);
        bgm->setScaleX(((accountbg->getContentSize().width - bgr->getContentSize().width*2)+5.5)/bgm->getContentSize().width);
        bgm->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width*.5, 1)));
        
        
        //原有帐号登陆提示 请注意tag
        SGCCLabelTTF *oldAccountLogin = SGCCLabelTTF::create(str_RegisterLayer_login_old, FONT_PANGWA, 32);
        oldAccountLogin->setTag(10);
        this->addChild(oldAccountLogin, 1);
        //oldAccountLogin->setPosition(ccpAdd(bgm->getPosition(), ccp(-170, 0)));
        oldAccountLogin->setPosition(ccp(24 + oldAccountLogin->getContentSize().width/2 + accountbg->getPositionX() - accountbg->getContentSize().width * 0.5, bgm->getPositionY()  ) );
        
        //右侧箭头
        CCSprite *jiantou1 = CCSprite::createWithSpriteFrameName("login_jiantou.png");
        this->addChild(jiantou1, 2);
        jiantou1->setPosition(ccp( accountbg->getPositionX() + accountbg->getContentSize().width * 0.5 - jiantou1->getContentSize().width * 1.3 , oldAccountLogin->getPosition().y));
        
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
        if (mainLayer)
        {
            mainLayer->setPortraitMenuCanTouch(false);
        }

    }
    
    title->setPosition(ccp(s.width/2, title_bg->getPosition().y - title_bg->getContentSize().height*.5));
    this->addChild(title);

    
    
    m_pEditName = CCEditBox::create(CCSizeMake(320, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditName->setPlaceHolder(str_RegisterBox_str1);
    m_pEditName->setMaxLength(30);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditName->setInputMode(kEditBoxInputModeEmailAddr);
    m_pEditName->setDelegate(this);
    addChild(m_pEditName);
    
    m_pEditPassword = CCEditBox::create(CCSizeMake(320, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditPassword->setPlaceHolder(str_RegisterBox_str1);
    m_pEditPassword->setMaxLength(16);
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword->setInputMode(kEditBoxInputModeSingleLine);
    m_pEditPassword->setDelegate(this);
    addChild(m_pEditPassword);
    
    m_pEditPasswordAgain = CCEditBox::create(CCSizeMake(320, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditPasswordAgain->setPlaceHolder(str_RegisterBox_str2);
    m_pEditPasswordAgain->setMaxLength(16);
    m_pEditPasswordAgain->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPasswordAgain->setInputMode(kEditBoxInputModeSingleLine);
    m_pEditPasswordAgain->setDelegate(this);
    addChild(m_pEditPasswordAgain);
    
    m_pEditName->setPosition(ccpAdd(accountbg->getPosition(), ccp(67, 67)));
    m_pEditPassword->setPosition(ccpAdd(accountbg->getPosition(), ccp(67, -2)));
    m_pEditPasswordAgain->setPosition(ccpAdd(accountbg->getPosition(), ccp(67, -72)));
    
    
    this->setTouchEnabled(true);
}
void SGRegisterLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool SGRegisterLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    bool ret = true;
    if (EnterType != 3 && EnterType != 2 && EnterType !=4 )
    {
        CCPoint pt = pTouch->getLocationInView();
        pt = CCDirector::sharedDirector()->convertToGL(pt);
        SGCCLabelTTF *a = (SGCCLabelTTF *)this->getChildByTag(9);
        CCRect rect = CCRectMake(a->getPosition().x,a->getPosition().y-a->getContentSize().height*.5f, a->getContentSize().width +456, a->getContentSize().height);
        if(rect.containsPoint(pt))
        {
            showLoginLayer(NULL);
            ret = true;
        }
    }
    //在游客绑定账号页添加 使用老账号登陆功能
    else if(3 == EnterType)
    {
        CCPoint pt = pTouch->getLocationInView();
        pt = CCDirector::sharedDirector()->convertToGL(pt);
        SGCCLabelTTF *a = (SGCCLabelTTF *)this->getChildByTag(10);
        CCRect rect = CCRectMake(a->getPosition().x,a->getPosition().y-a->getContentSize().height*.5f, a->getContentSize().width +456, a->getContentSize().height);
        if(rect.containsPoint(pt))
        {
            SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(NULL, NULL, 219,0 ,0  ,this,0,str_RegisterLayer_login_old_verify) ;
            SGMainManager::shareMain()->showBox(cantadvanceBox);
        }

    }
    return ret;
}



#pragma mark button clike

void SGRegisterLayer::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (EnterType == 3 || EnterType == 5)
    {
        SGMainManager::shareMain()->showwelcomelayer();
    }
    else if (EnterType == 4)
    {
        if ( SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bindLayer))
        {
            SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bindLayer)->setVisible(true);
        }
        SGMainManager::shareMain()->getMainScene()->addShowLayer((SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bindLayer));
    }
    else
    {
        SGMainManager::shareMain()->showloginLayer();
    }
//    this->boxClose();
}

//void SGRegisterLayer::processErrorState(int firstFlag , int scendFlag ,int thirdFlag)
//{
//    CCSprite *sprite1 = CCSprite::create("public/ok.png");
//    CCSpriteFrame *frame1 = sprite1->displayFrame();
//    CCSprite *sprite2 = CCSprite::create("public/error.png");
//    CCSpriteFrame *frame2 = sprite2->displayFrame();
//    
//    // 0 不显示 1 正确 2 错误
//    
//    spriteEditIsError_1->setVisible(firstFlag != 0);
//    if(firstFlag == 1){
//        spriteEditIsError_1->setDisplayFrame(frame1);
//    }else if(firstFlag == 2){
//        spriteEditIsError_1->setDisplayFrame(frame2);
//    }
//    
//    spriteEditIsError_2->setVisible(scendFlag != 0);
//    if(scendFlag == 1){
//        spriteEditIsError_2->setDisplayFrame(frame1);
//    }else if(scendFlag == 2){
//        spriteEditIsError_2->setDisplayFrame(frame2);
//    }
//    
//    spriteEditIsError_3->setVisible(thirdFlag != 0);
//    if(thirdFlag == 1){
//        spriteEditIsError_3->setDisplayFrame(frame1);
//    }else if(thirdFlag == 2){
//        spriteEditIsError_3->setDisplayFrame(frame2);
//    }
//}


void SGRegisterLayer::showLoginLayer(CCObject *obj)
{
    SGMainManager::shareMain()->showloginLayer();
}

void SGRegisterLayer::registerHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    bool bool_editName = true;//this->is_email_valid(m_pEditName->getText());
    if(!bool_editName)
    {
        SG_SHOW_WINDOW(str_RegisterBox_str22);
        return;
    }
    
    bool bool_password = this->checkInputStringChar(m_pEditPassword->getText());
    if(!bool_password)
    {
        SG_SHOW_WINDOW(str_RegisterBox_str8);
        return;
    }
    
    bool bool_passwordagain = this->checkInputStringChar(m_pEditPasswordAgain->getText());
    if(!bool_passwordagain)
    {
        SG_SHOW_WINDOW(str_RegisterBox_str8);
        return;
    }
    
    int iseque = strcmp(m_pEditPassword->getText(), m_pEditPasswordAgain->getText());
    if (iseque != 0)
    {
        SG_SHOW_WINDOW(str_RegisterBox_str9);
        return;
    }

    
//不同渠道登陆，这里宏区分
#if (GAME_ACCOUNT_TYPE == GCAT_XDYOU)
    
    if(EnterType == 1 || EnterType == 5)
    {
#if (PLATFORM == IOS)
		OcCaller::getInstance()->registerUserSDK(m_pEditName->getText(), m_pEditPassword->getText());
#else
		//android code
		SdkManager::shareSdkManager()->userRegister(m_pEditName->getText(), m_pEditPassword->getText());
#endif

    }
    else
    {

#if (PLATFORM == IOS)
		OcCaller::getInstance()->registerUserSDK(m_pEditName->getText(), m_pEditPassword->getText());
#else
		//android code
		SdkManager::shareSdkManager()->userRegister(m_pEditName->getText(), m_pEditPassword->getText());
#endif
        //注册替换bind
        // SGHttpClient::shareHttpClient()->bind(this, m_pEditName->getText(), m_pEditPassword->getText(), m_pEditPasswordAgain->getText());
    }
    //其他所有非xdyou无SDK自带界面的渠道
#else
    
    UserAndExtInfo uei;
    uei.userName = m_pEditName->getText();
    uei.userPwd = m_pEditPassword->getText();
    
    SdkController::gi()->getSdkImp()->sdkRegister(uei);
    
#endif

    
    //发送进入注册界面的消息 add by:zyc.
    SGMainManager::sendOperatorRecord(100021);
}

bool SGRegisterLayer::checkInputStringChar(const char *inputString)
{
    //检查位数
    if(!inputString || sizeof(inputString)<4 || sizeof(inputString) >17){
        return false;
    }
    
    //检查是否是字母或数字
    for(int i=0;i<sizeof(inputString);i++)
    {
        int number = inputString[i];
        if(number >= 48 && number <= 57){       //数字
            continue;
        }else if(number >= 65 && number <= 90){ //大写字母
            continue;
        }else if(number >=97 && number <= 122){ //小写字母
            continue;
        }else{
            return false;
        }
    }
    
    return true;
}

#pragma mark editBox delegate

void SGRegisterLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void SGRegisterLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void SGRegisterLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void SGRegisterLayer::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    
}

#pragma mark http

void SGRegisterLayer::requestFinished(int msgid, CCObject *data)
{
    if(msgid == MSG_HTTP_REGISTER)
    {
        CCString *backString = (CCString*)data;
        if(backString->intValue() == 1)
        {
            CCUserDefault::sharedUserDefault()->setStringForKey("username", m_pEditName->getText());
            CCUserDefault::sharedUserDefault()->setStringForKey("password", m_pEditPassword->getText());
            CCUserDefault::sharedUserDefault()->setBoolForKey("isExist", true);
            CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount", true);
            CCUserDefault::sharedUserDefault()->flush();
            
//            SGRegisterSuccess *su = SGRegisterSuccess::create(this);
////            this->boxCloseWithOutCallBack();
//            SGMainManager::shareMain()->showBox(su);
            SG_SHOW_WINDOW(str_RegisterBox_str23);
            
			
            //id  sdkReqCallBack = SGMainManager::shareMain()->getSdkdelegate();
            std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
            
#if (PLATFORM == IOS)
			OcCaller::getInstance()->setAccountSDK(userId, m_pEditName->getText());
#else
			//android code
            AndroidSDKAdp::getInstance()->setAccountSDK(userId, m_pEditName->getText());
#endif
			/*
			[sdkReqCallBack setAccount:[NSString stringWithFormat:@"%d",userId]
                           accountName:[NSString stringWithUTF8String:m_pEditName->getText()]
                           accountType:kAccountRegistered];
            */
            SGMainManager::shareMain()->showwelcomelayer();
        }
    }else if(msgid == MSG_HTTP_BING)
    {
        CCDictionary *dict = (CCDictionary*)data;
        
        CCString *string1 = (CCString*)dict->objectForKey("string1");
        CCString *string2 = (CCString*)dict->objectForKey("string2");
               
        if(1 == string1->intValue())
        {
            //绑定成功
            SG_SHOW_WINDOW(str_RegisterBox_str11);
            CCUserDefault::sharedUserDefault()->setStringForKey("username", m_pEditName->getText());
            CCUserDefault::sharedUserDefault()->setStringForKey("password", m_pEditPassword->getText());
            CCUserDefault::sharedUserDefault()->setBoolForKey("isAccount", true);
            CCUserDefault::sharedUserDefault()->flush();
            SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
            SGWelComeLayer *layer = SGWelComeLayer::create();
            layer->loginstart();
            mainScene->addShowLayer(layer);
            layer->showbox();
            
            if (EnterType == 4)
            {
                if ( SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bindLayer))
                {
                    SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bindLayer)->removeFromParent();
                }
            }
        }else
        {
            if(string2->intValue() == 1)
            {
                //绑定失败
                SG_SHOW_WINDOW(str_RegisterBox_str12);
            }
            else if(2 == string2->intValue())
            {
                SG_SHOW_WINDOW(str_RegisterBox_str13);
            }else if(3 == string2->intValue())
            {
                SG_SHOW_WINDOW(str_RegisterBox_str14);
            }else if(4 == string2->intValue())
            {
                SG_SHOW_WINDOW(str_RegisterBox_str24);
            }
        }
    }
}

void SGRegisterLayer::requestFailed()
{
    CCLOG("注册接收数据失败");
    SG_SHOW_WINDOW(str_RegisterBox_str15);
    
}
bool SGRegisterLayer::is_email_valid(const char* email)
{
    if (email == NULL)
    {
        return false;
    }
    //    const std::regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    //    return std::regex_match(email, pattern);
    const char *eset = "[A-Z0-9a-z._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,4}";
    regmatch_t   subs[10];
    regex_t stuRT;
    
    regcomp(&stuRT, eset, REG_EXTENDED);
    int err = regexec(&stuRT, email, (size_t)10, subs, 0);
    
    if (err) {
        
        regfree   (&stuRT);
        return false;
    }
    else
    {
        regfree   (&stuRT);
        return true;
    }
}