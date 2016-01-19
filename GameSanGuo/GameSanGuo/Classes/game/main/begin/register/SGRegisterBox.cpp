//
//  SGRegisterBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-6.
//
//

#include "SGRegisterBox.h"
#include "SGButton.h"
#include "SGMsgId.h"
#include "SGRegisterSuccess.h"
#include "SGMainManager.h"
#include "SGBindSuccessBox.h"
#include "SGWelComeLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
//#import "SdkReqCallBack.h"

#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "SdkManager.h"
#include "AndroidSDKAdp.h"
#endif

SGRegisterBox::SGRegisterBox()
:m_pEditName(NULL),
m_pEditPassword(NULL),
m_pEditPasswordAgain(NULL),
labelAccountError(NULL),
labelPasswordError(NULL),
labelPasswordAgainError(NULL),
spriteEditIsError_1(NULL),
spriteEditIsError_2(NULL),
spriteEditIsError_3(NULL),
EnterType(0),
_target(NULL)
{

}
SGRegisterBox::~SGRegisterBox()
{
    //modified by cgp
    CCTexture2D *texture = CCTextureCache::sharedTextureCache()->textureForKey("login/server.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTexture(texture);
    CCTexture2D *texture1 = CCTextureCache::sharedTextureCache()->textureForKey("public/public.pvr.ccz");
    CCTextureCache::sharedTextureCache()->removeTexture(texture1);
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_registerBox);
}

SGRegisterBox *SGRegisterBox::create(SGBoxDelegate *dg, int type)
{
    SGRegisterBox *registerBox = new SGRegisterBox();
    registerBox->_target = dg;
    registerBox->EnterType = type;
    if (type == 1) {
        if (registerBox && registerBox->init(dg,box_registerTag,CCSizeMake(460, 600)))
        {
            registerBox->constructView();
            registerBox->autorelease();
            return registerBox;
        }

    }else{
        if (registerBox && registerBox->init(dg,box_registerTag,CCSizeMake(460, 750)))
        {
            registerBox->constructView();
            registerBox->autorelease();
            return registerBox;
        }
    }
       CC_SAFE_DELETE(registerBox);
    return NULL;
}

void SGRegisterBox::constructView()
{
 
     ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist", RES_TYPE_LAYER_UI,sg_registerBox);
     ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI,sg_registerBox, LIM_PNG_AS_PNG);
     ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI,sg_registerBox);
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    m_pEditName = CCEditBox::create(CCSizeMake(boxSize.width-50, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
   
    m_pEditName->setPlaceHolder(str_RegisterBox_str1);
    m_pEditName->setFontColor(ccWHITE);
    m_pEditName->setMaxLength(30);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditName->setDelegate(this);
    
    
    m_pEditPassword = CCEditBox::create(CCSizeMake(boxSize.width-50, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    
    m_pEditPassword->setPlaceHolder(str_RegisterBox_str1);
    m_pEditPassword->setFontColor(ccWHITE);
    m_pEditPassword->setMaxLength(16);
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword->setInputMode(kEditBoxInputModeSingleLine);
    m_pEditPassword->setDelegate(this);
    
    
    m_pEditPasswordAgain = CCEditBox::create(CCSizeMake(boxSize.width-50, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    
    m_pEditPasswordAgain->setPlaceHolder(str_RegisterBox_str2);
    m_pEditPasswordAgain->setFontColor(ccWHITE);
    m_pEditPasswordAgain->setMaxLength(16);
    m_pEditPasswordAgain->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPasswordAgain->setInputMode(kEditBoxInputModeSingleLine);
    m_pEditPasswordAgain->setDelegate(this);
   
    
    if (EnterType == 1) {
        m_pEditName->setPosition(ccpAdd(center, ccp(0, 150)));
        m_pEditPassword->setPosition(ccpAdd(center, ccp(0, 50)));
        m_pEditPasswordAgain->setPosition(ccpAdd(center, ccp(0, -50)));
  
    }else{
        m_pEditName->setPosition(ccpAdd(center, ccp(0, 100+75+150)));
        m_pEditPassword->setPosition(ccpAdd(center, ccp(0, -100+75+150)));
        m_pEditPasswordAgain->setPosition(ccpAdd(center, ccp(0, -200+75 +150)));
    }
    addChild(m_pEditName);
    addChild(m_pEditPassword);
    addChild(m_pEditPasswordAgain);
    
    
    //注册按钮
    SGButton *registerButton = SGButton::create("public_btn_2.png", "login_font_register.png", this,menu_selector(SGRegisterBox::registerHandler), CCPointZero, false, true);
    this->addBtn(registerButton);
        
    //返回按钮
    SGButton *buttonGoback = SGButton::createFromLocal("public_btn_6.png",str_Back_ , this, menu_selector(SGRegisterBox::buttonClickGoBack), CCPointZero, FONT_PANGWA,ccWHITE,28,false, true);
    this->addBtn(buttonGoback);
    buttonGoback->setAnchorPoint(ccp(0, 0.5));
    
    
    //注册游戏帐号
    SGCCLabelTTF *spriteShowRegister = NULL;
    if(EnterType == 1)
    {
        spriteShowRegister = SGCCLabelTTF::create(str_RegisterBox_str3,  FONT_PANGWA, 32);

    }else{
        spriteShowRegister = SGCCLabelTTF::create(str_RegisterBox_str4,  FONT_PANGWA, 32);
    }
    
    this->addChild(spriteShowRegister);
    
    //对号或者错号
    spriteEditIsError_1 = CCSprite::createWithSpriteFrameName("ok.png");
    spriteEditIsError_2 = CCSprite::createWithSpriteFrameName("ok.png");
    spriteEditIsError_3 = CCSprite::createWithSpriteFrameName("ok.png");
    this->addChild(spriteEditIsError_1);
    this->addChild(spriteEditIsError_2);
    this->addChild(spriteEditIsError_3);
    spriteEditIsError_1->setAnchorPoint(ccp(1, 0.5));
    spriteEditIsError_2->setAnchorPoint(ccp(1, 0.5));
    spriteEditIsError_3->setAnchorPoint(ccp(1, 0.5));
    
    spriteEditIsError_1->setVisible(false);
    spriteEditIsError_2->setVisible(false);
    spriteEditIsError_3->setVisible(false);
    
//    //三方帐号
//    CCSprite *spriteOtherRegister = CCSprite::createWithSpriteFrameName("sanfangzhanghao.png");
//    this->addChild(spriteOtherRegister);
    
    if (EnterType == 1) {
        registerButton->setPosition(ccpAdd(center,ccp(0, -140)));
        buttonGoback->setPosition(ccpAdd(center, ccp(-202, 270)));
        spriteShowRegister->setPosition(ccpAdd(center, ccp(0, 245)));
    }else{
        registerButton->setPosition(ccpAdd(center,ccp(0, -290+75 - 55)));
        buttonGoback->setPosition(ccpAdd(center, ccp(-202, 120+75+150)));
        spriteShowRegister->setPosition(ccpAdd(center, ccp(0, 95+75+150)));
        
        CCLabelTTF *label1 = CCLabelTTF::create(str_RegisterBox_str5, FONT_BOXINFO, FONTSIZE(12));
        CCLabelTTF *label2 = CCLabelTTF::create(str_RegisterBox_str6, FONT_BOXINFO, FONTSIZE(12));
        CCLabelTTF *label3 = CCLabelTTF::create(str_RegisterBox_str7, FONT_BOXINFO, FONTSIZE(12));
        label1->setColor(ccBLACK);
        label2->setColor(ccBLACK);
        label3->setColor(ccBLACK);
        this->addChild(label1);
        this->addChild(label2);
        this->addChild(label3);
        label1->setPosition(ccpAdd(center, ccp(0,265 - 225- 130)));
        label2->setPosition(ccpAdd(center, ccp(label2->getContentSize().width/2-label1->getContentSize().width/2, 175 +45- 225- 130)));
        label3->setPosition(ccpAdd(center, ccp(label3->getContentSize().width/2-label1->getContentSize().width/2, 175- 225- 130)));
    }    
}

#pragma mark button clike

void SGRegisterBox::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
}

void SGRegisterBox::processErrorState(int firstFlag , int scendFlag ,int thirdFlag)
{
    CCSprite *sprite1 = CCSprite::createWithSpriteFrameName("ok.png");
    CCSpriteFrame *frame1 = sprite1->displayFrame();
    CCSprite *sprite2 = CCSprite::createWithSpriteFrameName("error.png");
    CCSpriteFrame *frame2 = sprite2->displayFrame();
    
    // 0 不显示 1 正确 2 错误
    
    spriteEditIsError_1->setVisible(firstFlag != 0);
    if(firstFlag == 1){
        spriteEditIsError_1->setDisplayFrame(frame1);
    }else if(firstFlag == 2){
        spriteEditIsError_1->setDisplayFrame(frame2);
    }
    
    spriteEditIsError_2->setVisible(scendFlag != 0);
    if(scendFlag == 1){
        spriteEditIsError_2->setDisplayFrame(frame1);
    }else if(scendFlag == 2){
        spriteEditIsError_2->setDisplayFrame(frame2);
    }

    spriteEditIsError_3->setVisible(thirdFlag != 0);
    if(thirdFlag == 1){
        spriteEditIsError_3->setDisplayFrame(frame1);
    }else if(thirdFlag == 2){
        spriteEditIsError_3->setDisplayFrame(frame2);
    }
}

void SGRegisterBox::registerHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    bool bool_editName = this->checkInputStringChar(m_pEditName->getText());
    if(!bool_editName)
    {
        labelAccountError->setVisible(true);
        this->processErrorState(FLAG_ERROR, FLAG_NODISPLAY, FLAG_NODISPLAY);
        labelAccountError->setString(str_RegisterBox_str8);
        return;
    }else{
        labelAccountError->setVisible(false);
    }
    
    bool bool_password = this->checkInputStringChar(m_pEditPassword->getText());
    if(!bool_password)
    {
        labelPasswordError->setVisible(true);
        spriteEditIsError_1->setVisible(true);
        this->processErrorState(FLAG_OK, FLAG_ERROR, FLAG_NODISPLAY);
        labelAccountError->setString(str_RegisterBox_str8);
        return;
    }else{
        labelPasswordError->setVisible(false);
    }
    
    bool bool_passwordagain = this->checkInputStringChar(m_pEditPasswordAgain->getText());
    if(!bool_passwordagain)
    {
        labelPasswordAgainError->setVisible(true);
        this->processErrorState(FLAG_OK, FLAG_OK, FLAG_ERROR);
        labelPasswordAgainError->setString(str_RegisterBox_str8);
        return;
    }else{
        labelPasswordAgainError->setVisible(false);
        this->processErrorState(FLAG_OK, FLAG_OK, FLAG_OK);
    }
    
    int iseque = strcmp(m_pEditPassword->getText(), m_pEditPasswordAgain->getText());
    if (iseque != 0)
    {
        labelPasswordAgainError->setVisible(true);
        this->processErrorState(FLAG_OK, FLAG_OK, FLAG_ERROR);
        labelPasswordAgainError->setString(str_RegisterBox_str9);
        return;
    }else
    {
        labelPasswordAgainError->setVisible(false);
        this->processErrorState(FLAG_OK, FLAG_OK, FLAG_OK);
    }
    
    if(EnterType == 1)
    {
#if (PLATFORM == IOS)
		OcCaller::getInstance()->registerUserSDK(m_pEditName->getText(), m_pEditPassword->getText());
#else
		//android code
		SdkManager::shareSdkManager()->userRegister(m_pEditName->getText(), m_pEditPassword->getText());
#endif
		/*
        id  sdkReqCallBack = SGMainManager::shareMain()->getSdkdelegate();
        [[PYUserSDKLib sharedUserSDKLib] userRegister:[[NSString alloc] initWithUTF8String:m_pEditName->getText()]
                                             Password:[[NSString alloc] initWithUTF8String:m_pEditPassword->getText()]
                                             GameId:@"1"
                                             ChannelId:[sdkReqCallBack ChannelId]
                                             SubChannelId:[sdkReqCallBack SubChannelId]
                                             OsVersion:[sdkReqCallBack OsVersion]
                                             OsType:[sdkReqCallBack OsType]
                                             PhoneNum:@"123"
                                     CallBackDelegate:sdkReqCallBack FinishSelector:@selector(registFinished:) FailSelector:@selector(requestFailed:)];
		 */
    }else{
		
#if (PLATFORM == IOS)
		OcCaller::getInstance()->registerUserSDK(m_pEditName->getText(), m_pEditPassword->getText());
#else
		//android code
		SdkManager::shareSdkManager()->userRegister(m_pEditName->getText(), m_pEditPassword->getText());
#endif
		/*
        id  sdkReqCallBack = SGMainManager::shareMain()->getSdkdelegate();
        [[PYUserSDKLib sharedUserSDKLib] userRegister:[[NSString alloc] initWithUTF8String:m_pEditName->getText()]
                                             Password:[[NSString alloc] initWithUTF8String:m_pEditPassword->getText()]
                                               GameId:@"1"
                                            ChannelId:[sdkReqCallBack ChannelId]
                                         SubChannelId:[sdkReqCallBack SubChannelId]
                                            OsVersion:[sdkReqCallBack OsVersion]
                                               OsType:[sdkReqCallBack OsType]
                                             PhoneNum:@"123"
                                     CallBackDelegate:sdkReqCallBack FinishSelector:@selector(registFinished:) FailSelector:@selector(requestFailed:)];
        */
		
        //注册替换bind
        //SGHttpClient::shareHttpClient()->bind(this, m_pEditName->getText(), m_pEditPassword->getText(), m_pEditPasswordAgain->getText());
    }
}

bool SGRegisterBox::checkInputStringChar(const char *inputString)
{
    //检查位数
    if(!inputString || sizeof(inputString)<4 || sizeof(inputString) >16){
        return false;
    }
    
    //检查是否是字母或数字
    for(int i=0;i<sizeof(inputString);i++){
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

void SGRegisterBox::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void SGRegisterBox::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void SGRegisterBox::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void SGRegisterBox::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{

}

#pragma mark http

void SGRegisterBox::requestFinished(int msgid, CCObject *data)
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
            
            
            std::string userId = CCUserDefault::sharedUserDefault()->getStringForKey("userId");
			
			//外部适配类调用
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
            
            SGRegisterSuccess *su = SGRegisterSuccess::create(deletage);
            this->boxCloseWithOutCallBack();
            SGMainManager::shareMain()->showBox(su);
            
        }else{
            
            labelAccountError->setVisible(true);
            labelAccountError->setString(str_RegisterBox_str10);
            this->processErrorState(FLAG_ERROR, FLAG_NODISPLAY, FLAG_NODISPLAY);
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
            
            SGBindSuccessBox *success = SGBindSuccessBox::create(deletage);
            this->boxCloseWithOutCallBack();
            SGMainManager::shareMain()->showBox(success);    
            
        }else
        {
            if(string2->intValue() == 1)
            {
                //绑定失败
                SG_SHOW_WINDOW(str_RegisterBox_str12);
            }
            else if(2 == string2->intValue())
            {
                SG_SHOW_WINDOW(str_RegisterBox_str1301);
            }else if(3 == string2->intValue())
            {
                SG_SHOW_WINDOW(str_RegisterBox_str14);
            }
        }
    }
}

void SGRegisterBox::requestFailed()
{
    CCLOG("注册接收数据失败");
    SG_SHOW_WINDOW(str_RegisterBox_str15);

}