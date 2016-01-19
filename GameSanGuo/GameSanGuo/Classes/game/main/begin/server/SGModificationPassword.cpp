//
//  SGModificationPassword.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-1-30.
//
//

#include "SGModificationPassword.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
//#import "SdkReqCallBack.h"
#if (PLATFORM == IOS)
#include "OcCaller.h"
#else
#include "SdkManager.h"
#endif

#pragma mark - 构造函数

SGModificationPassword::SGModificationPassword():
m_pEditName(NULL),
m_pEditPasswordAgain(NULL),
m_userName(NULL),
m_pEditPassword(NULL),
labelNewPassword(NULL),
labelNewPasswordAgain(NULL),
labelOldPasswordError(NULL),
labelTip(NULL),
labelNewPasswordAgianError(NULL),
labelOldPassword(NULL)
{
    
}

SGModificationPassword::~SGModificationPassword()
{

    ResourceManager::sharedInstance()->unBindLayerTexture(sg_modifypassword);
}

SGModificationPassword* SGModificationPassword::create(SGBoxDelegate *delegate)
{
    SGModificationPassword *mPassword = new SGModificationPassword();
    if(mPassword && mPassword->init(delegate, box_modification, CCSizeMake(411, 550)))
    {
        mPassword->autorelease();
        mPassword->constructView();
        return mPassword;
    }    
    CC_SAFE_DELETE(mPassword);
    return NULL;
}

#pragma mark - 按钮点击
void SGModificationPassword::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxCloseWithOutCallBack();
}

void SGModificationPassword::buttonClickOK()
{
    EFFECT_PLAY(MUSIC_BTN);
    std::string pw = CCUserDefault::sharedUserDefault()->getStringForKey("password");
    
    //验证与原密码相同
    if(!m_pEditPassword->getText() || !pw.compare(m_pEditPassword->getText()))
    {
        labelOldPasswordError->setVisible(true);
        this->processFlag(2, 0, 0);
        return;
    }
    
    const char *newPassword = m_pEditPassword->getText();
    const char *newPasswordAgain = m_pEditPasswordAgain->getText()
    ;
    
    //检查位数
    if(sizeof(newPassword)<4 || sizeof(newPassword) >16){
        labelTip->setVisible(true);
        this->processFlag(1, 2, 0);
        return;
    }
    
    //检查是否是字母或数字
    for(int i=0;i<sizeof(newPassword);i++){
        int number = newPassword[i];
        if(number >= 48 && number <= 57){       //数字
            continue;
        }else if(number >= 65 && number <= 90){ //大写字母
            continue;
        }else if(number >=97 && number <= 122){ //小写字母
            continue;
        }else{
            labelTip->setVisible(true);
            this->processFlag(1, 2, 0);
            return;
        }
    }
    
    //检查新密码输入是否一致
    if(strcmp(newPassword, newPasswordAgain)){
        labelNewPasswordAgianError->setVisible(true);
        this->processFlag(1, 1, 2);
        return;
    }
    if(!m_pEditPassword->getText() || !pw.compare(newPassword))
    {
        SG_SHOW_WINDOW(str_ChangePassWordLayer_str5);
        return;
    }
    std::string username = "name";
    if (strcmp(m_userName->getText(), "")  != 0 ) {
        username = std::string(m_userName->getText());
    }
    else if (strcmp(m_userName->getPlaceHolder(), "")  != 0 )
    {
        username = std::string(m_userName->getPlaceHolder());
    }
	
	//使用外部调用
#if (PLATFORM == IOS)
	OcCaller::getInstance()->pyUserSDKChangePasswordReq(username.c_str(), pw.c_str(), newPassword);
#else
	//android code
	SdkManager::shareSdkManager()->modifyPassword(username, pw, newPassword);
#endif
	/*
	id  sdkReqCallBack = SGMainManager::shareMain()->getSdkdelegate();
    [[PYUserSDKLib sharedUserSDKLib] modifyPassword:[[NSString alloc] initWithUTF8String:username.c_str()]
                                           Password:[[NSString alloc] initWithUTF8String:pw.c_str()]
                                             GameId:@"1"
                                          ChannelId:[sdkReqCallBack ChannelId]
                                       SubChannelId:[sdkReqCallBack SubChannelId]
                                          OsVersion:[sdkReqCallBack OsVersion]
                                             OsType:[sdkReqCallBack OsType]
                                        NewPassword:[[NSString alloc] initWithUTF8String:newPasswordAgain]
                                   CallBackDelegate:sdkReqCallBack FinishSelector:@selector(passwordFinished:) FailSelector:@selector (requestFailed:)];
	 */
    SGHttpClient::shareHttpClient()->password(this, username.c_str(), m_pEditPassword->getText(), m_pEditPasswordAgain->getText());
    
}

#pragma mark - 构建界面

//构建界面
void SGModificationPassword::constructView()
{
    
	ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_modifypassword, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_modifypassword);

	
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    //用户帐号
    m_userName = CCEditBox::create(CCSizeMake(boxSize.width-50, 50), CCScale9Sprite::create("public_kuang_input.png"));
    m_userName->setPosition(ccpAdd(center, ccp(0, 150)));
    std::string username = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    m_userName->setPlaceHolder(username.c_str());
    m_userName->setFontColor(ccRED);
    m_userName->setMaxLength(16);
    m_userName->setReturnType(kKeyboardReturnTypeDone);
    m_userName->setInputFlag(kEditBoxInputFlagPassword);
    addChild(m_userName);

    
    //三个输入框
    m_pEditName = CCEditBox::create(CCSizeMake(boxSize.width-50, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditName->setPosition(ccpAdd(center, ccp(0, 60)));
    m_pEditName->setPlaceHolder(str_RegisterBox_str1);
    m_pEditName->setFontColor(ccWHITE);
    m_pEditName->setMaxLength(16);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditName->setInputFlag(kEditBoxInputFlagPassword);
//    m_pEditName->setDelegate(this);
    addChild(m_pEditName);
    
    m_pEditPassword = CCEditBox::create(CCSizeMake(boxSize.width-50, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditPassword->setPosition(ccpAdd(center, ccp(0, -30)));
    m_pEditPassword->setPlaceHolder(str_RegisterBox_str1);
    m_pEditPassword->setFontColor(ccWHITE);
    m_pEditPassword->setMaxLength(16);
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword->setInputMode(kEditBoxInputModeSingleLine);
//    m_pEditPassword->setDelegate(this);
    addChild(m_pEditPassword);
    
    m_pEditPasswordAgain = CCEditBox::create(CCSizeMake(boxSize.width-50, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditPasswordAgain->setPosition(ccpAdd(center, ccp(0, -120)));
    m_pEditPasswordAgain->setPlaceHolder(str_RegisterBox_str2);
    m_pEditPasswordAgain->setFontColor(ccWHITE);
    m_pEditPasswordAgain->setMaxLength(16);
    m_pEditPasswordAgain->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPasswordAgain->setInputMode(kEditBoxInputModeSingleLine);
//    m_pEditPasswordAgain->setDelegate(this);
    addChild(m_pEditPasswordAgain);
    
    //注册按钮
    SGButton *registerButton = SGButton::create("public_btn_2.png", "public_font_queding.png", this,menu_selector(SGModificationPassword::buttonClickOK), CCPointZero, false, true);
    this->addBtn(registerButton);
    registerButton->setPosition(ccpAdd(center,ccp(0, -200)));
    
    //返回按钮
    SGButton *buttonGoback = SGButton::create("public_btn_6.png", "public_fanhui.png" , this, menu_selector(SGModificationPassword::buttonClickGoBack),CCPointZero,false,true);
    this->addBtn(buttonGoback);
    buttonGoback->setPosition(ccpAdd(m_pEditName->getPosition(), ccp(-m_pEditName->getContentSize().width/2+buttonGoback->getContentSize().width/2-20, m_pEditName->getContentSize().height*3.6)));
    
    //注册游戏帐号
    CCSprite *spriteShowRegister = CCSprite::createWithSpriteFrameName("login_font_gaimima.png");
    this->addChild(spriteShowRegister);
    spriteShowRegister->setPosition(ccpAdd(center, ccp(0, 220)));
    
    // 原密碼  新密碼 再輸入一次新密碼 原密碼不正確 請輸入4—16位字母或數字 兩次密碼不一致
    //帐号 密码 再输入一次密码
    CCLabelTTF *user_name = CCLabelTTF::create(str_ModificationPassword_str1, FONT_BOXINFO, 24);
    user_name->setAnchorPoint(ccp(0, 0.5));
    user_name->setColor(ccYELLOW);
    this->addChild(user_name);
    labelOldPassword = CCLabelTTF::create(str_ModificationPassword_str2, FONT_BOXINFO, 24);
    labelNewPassword = CCLabelTTF::create(str_ChangePassWordLayer_str8, FONT_BOXINFO, 24);
    labelNewPasswordAgain = CCLabelTTF::create(str_ModificationPassword_str3, FONT_BOXINFO, 24);
    labelOldPasswordError = CCLabelTTF::create(str_ModificationPassword_str4, FONT_BOXINFO, 24);
    labelTip = CCLabelTTF::create(str_ModificationPassword_str5, FONT_BOXINFO, 24);
    labelNewPasswordAgianError = CCLabelTTF::create(str_RegisterBox_str9, FONT_BOXINFO, 24);
    /**************设置AnchorPoint*************/
    labelOldPassword->setAnchorPoint(ccp(0, 0.5));
    labelNewPassword->setAnchorPoint(ccp(0, 0.5));
    labelNewPasswordAgain->setAnchorPoint(ccp(0, 0.5));
    labelOldPasswordError->setAnchorPoint(ccp(1, 0.5));
    labelNewPasswordAgianError->setAnchorPoint(ccp(1, 0.5));
    labelTip->setAnchorPoint(ccp(1, 0.5));
    /**************设置Color*************/
    labelOldPassword->setColor(ccWHITE);
    labelNewPassword->setColor(ccWHITE);
    labelNewPasswordAgain->setColor(ccWHITE);
    labelOldPasswordError->setColor(ccRED);
    labelTip->setColor(ccRED);
    labelNewPasswordAgianError->setColor(ccRED);
    /**************设置Position*************/
    int editBox_Width_2 = m_pEditName->getContentSize().width/2;
    user_name->setPosition(ccpAdd(center, ccp(-editBox_Width_2, 190)));
    labelOldPassword->setPosition(ccpAdd(center, ccp(-editBox_Width_2, 100)));
    labelNewPassword->setPosition(ccpAdd(center, ccp(-editBox_Width_2, 10)));
    labelNewPasswordAgain->setPosition(ccpAdd(center, ccp(-editBox_Width_2, -80)));
    labelOldPasswordError->setPosition(ccpAdd(labelOldPassword->getPosition(), ccp(editBox_Width_2*2, 0)));
    labelTip->setPosition(ccpAdd(labelNewPassword->getPosition(), ccp(editBox_Width_2*2, 0)));
    labelNewPasswordAgianError->setPosition(ccpAdd(labelNewPasswordAgain->getPosition(), ccp(editBox_Width_2*2, 0)));
    /**************AddChild*************/
    this->addChild(labelOldPassword);
    this->addChild(labelNewPassword);
    this->addChild(labelNewPasswordAgain);
    this->addChild(labelOldPasswordError);
    this->addChild(labelTip);
    this->addChild(labelNewPasswordAgianError);
    
    //设为不可用
    labelOldPasswordError->setVisible(false);
    labelNewPasswordAgianError->setVisible(false);
    labelTip->setVisible(false);
    
    //对号和错号
    spriteFlag_1 = CCSprite::createWithSpriteFrameName("ok.png");
    spriteFlag_2 = CCSprite::createWithSpriteFrameName("error.png");
    spriteFlag_3 = CCSprite::createWithSpriteFrameName("ok.png");
    spriteFlag_1->setAnchorPoint(ccp(1, 0.5));
    spriteFlag_2->setAnchorPoint(ccp(1, 0.5));
    spriteFlag_3->setAnchorPoint(ccp(1, 0.5));
    spriteFlag_1->setPosition(ccpAdd(m_pEditName->getPosition(), ccp(m_pEditName->getContentSize().width/2, 0)));
    spriteFlag_2->setPosition(ccpAdd(m_pEditPassword->getPosition(), ccp(m_pEditPassword->getContentSize().width/2, 0)));
    spriteFlag_3->setPosition(ccpAdd(m_pEditPasswordAgain->getPosition(), ccp(m_pEditPasswordAgain->getContentSize().width/2, 0)));
    spriteFlag_1->setVisible(false);
    spriteFlag_2->setVisible(false);
    spriteFlag_3->setVisible(false);
    this->addChild(spriteFlag_1);
    this->addChild(spriteFlag_2);
    this->addChild(spriteFlag_3);

}

void SGModificationPassword::processFlag(int first , int secend , int third)
{
    //0不显示 1正确 2错误
    
    CCSprite *sprite1 = CCSprite::createWithSpriteFrameName("ok.png");
    CCSpriteFrame *frame1 = sprite1->displayFrame();
    CCSprite *sprite2 = CCSprite::createWithSpriteFrameName("error.png");
    CCSpriteFrame *frame2 = sprite2->displayFrame();
    
    spriteFlag_1->setVisible(first != FLAG_NODISPLAY);
    if(first == FLAG_OK)
    {
        spriteFlag_1->setDisplayFrame(frame1);
    }
    else if(first == FLAG_ERROR)
    {
        spriteFlag_1->setDisplayFrame(frame2);
    }
    
    spriteFlag_2->setVisible(secend != FLAG_NODISPLAY);
    if(secend == FLAG_OK){
        spriteFlag_2->setDisplayFrame(frame1);
    }
    else if(secend == FLAG_ERROR)
    {
        spriteFlag_2->setDisplayFrame(frame2);
    }
    
    spriteFlag_3->setVisible(third != FLAG_NODISPLAY);
    if(third == FLAG_OK)
    {
        spriteFlag_3->setDisplayFrame(frame1);
    }else if(third == FLAG_ERROR)
    {
        spriteFlag_3->setDisplayFrame(frame2);
    }
}

void SGModificationPassword::requestFinished(int msgid, cocos2d::CCObject *data)
{
    CCDictionary *dict = (CCDictionary*)data;
    CCString *isSuccess = (CCString*)dict->objectForKey("isSccess");
    CCString *flag = (CCString*)dict->objectForKey("flag");
    if(0 == isSuccess->intValue())
    {
        switch(flag->intValue())
        {
            case 30:
            {
                SG_SHOW_WINDOW(str_ChangePassWordLayer_str10);
                break;
            }
            case 31:
            {
                SG_SHOW_WINDOW(str_ChangePassWordLayer_str11);
                break;
            }
            case 32:
            {
                SG_SHOW_WINDOW(str_ChangePassWordLayer_str12);
                break;
            }
        }
    }else{
        SG_SHOW_WINDOW(str_ChangePassWordLayer_str13);
        CCUserDefault::sharedUserDefault()->setStringForKey("password", m_pEditPasswordAgain->getText());
        
        this->boxCloseWithOutCallBack();
    }
}


