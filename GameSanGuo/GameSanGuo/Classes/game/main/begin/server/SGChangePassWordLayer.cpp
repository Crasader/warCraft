//
//  SGChangePassWordLayer.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-4.
//
//

#include "SGChangePassWordLayer.h"
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

SGChangePassWordLayer::SGChangePassWordLayer():
m_userName(NULL),
m_pEditName(NULL),
m_pEditPassword(NULL),
m_pEditPasswordAgain(NULL)
{
    
}

SGChangePassWordLayer::~SGChangePassWordLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_changepassword);
}

SGChangePassWordLayer* SGChangePassWordLayer::create()
{
    SGChangePassWordLayer *mPassword = new SGChangePassWordLayer();
    if(mPassword && mPassword->init(NULL, sg_changepassword))
    {
        mPassword->autorelease();
        mPassword->constructView();
        return mPassword;
    }
    CC_SAFE_DELETE(mPassword);
    return NULL;
}

#pragma mark - 按钮点击
void SGChangePassWordLayer::buttonClickGoBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showloginLayer();
}

void SGChangePassWordLayer::buttonClickOK()
{
    EFFECT_PLAY(MUSIC_BTN);
    std::string pw = CCUserDefault::sharedUserDefault()->getStringForKey("password");
    
    const char *newPassword = m_pEditPassword->getText();
    const char *newPasswordAgain = m_pEditPasswordAgain->getText()
    ;
    
    //检查位数
    if (m_pEditName->getText() == NULL ||newPassword == NULL || newPassword==NULL) {
        SG_SHOW_WINDOW(str_SHOWWINDOW_1002);
        return;
    }
    
    if(strlen(newPassword) < 6 || strlen(newPassword) > 16)
    {
        //        labelTip->setVisible(true);
        //        this->processFlag(1, 2, 0);
        SG_SHOW_WINDOW(str_SHOWWINDOW_1003);
        return;
    }
    
    //检查是否是字母或数字
    for(int i=0;i<sizeof(newPassword)-1;i++){
        int number = newPassword[i];
        if(number >= 48 && number <= 57){       //数字
            continue;
        }else if(number >= 65 && number <= 90){ //大写字母
            continue;
        }else if(number >=97 && number <= 122){ //小写字母
            continue;
        }else{
            SG_SHOW_WINDOW(str_SHOWWINDOW_1004);
            return;
        }
    }
    
    //检查新密码输入是否一致
    if(strcmp(newPassword, newPasswordAgain)){
        //        labelNewPasswordAgianError->setVisible(true);
        //        this->processFlag(1, 1, 2);
        SG_SHOW_WINDOW(str_SHOWWINDOW_1005);
        return;
    }
    
    //验证与原密码相同
    //    if(!m_pEditPassword->getText() || !pw.compare(m_pEditPassword->getText()))
    if(!m_pEditName->getText() || pw.compare(m_pEditName->getText()))
    {
        //        labelOldPasswordError->setVisible(true);
        //        this->processFlag(2, 0, 0);
        
        SG_SHOW_WINDOW(str_SHOWWINDOW_1006);
        return;
    }
    
    if(!m_pEditPassword->getText() || !pw.compare(m_pEditPassword->getText()))
    {
        //        labelOldPasswordError->setVisible(true);
        //        this->processFlag(2, 0, 0);
        
        SG_SHOW_WINDOW(str_SHOWWINDOW_1007);
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
    
	
	//下面的调用改为外围OcCaller调用
#if (PLATFORM == IOS)
	OcCaller::getInstance()->pyUserSDKChangePasswordReq(username.c_str(), pw.c_str(), newPassword);
#else
	//android code here
	SdkManager::shareSdkManager()->modifyPassword(username.c_str(), pw.c_str(), newPassword);
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
                                           NewPassword:[[NSString alloc] initWithUTF8String:newPassword]
                                           CallBackDelegate:sdkReqCallBack FinishSelector:@selector(passwordFinished:) FailSelector:@selector (requestFailed:)];
    */

    
}
#pragma mark - 构建界面

//构建界面
void SGChangePassWordLayer::constructView()
{
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist",RES_TYPE_LAYER_UI ,sg_changepassword);
    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist",RES_TYPE_LAYER_UI ,sg_changepassword);

    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
//顶端部分
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-1);
    
//    CCSprite *title = CCSprite::createWithSpriteFrameName("login_font_changepswd.png");
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_ChangePassWordLayer_str6, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccp(s.width/2, title_bg->getPosition().y - title_bg->getContentSize().height*.52));
    this->addChild(title);
    
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
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGChangePassWordLayer::buttonClickGoBack),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGChangePassWordLayer::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55 - 3, - backBtn->getContentSize().height*.55)));
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_changepassword);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("storyBg.png");
    
    //CCRect r = CCRectMake(0, 1136/2 - (s.height - title_bg->getContentSize().height )/2, s.width, s.height - title_bg->getContentSize().height);
    //bg->setTextureRect(r);
    bg->setScaleY(s.height/2 / bg->getContentSize().height);
    bg->setScaleX(s.width / bg->getContentSize().width);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(SGLayout::getPoint(kBottomCenter));
    
    
//输入框
    CCSprite *accountbg = CCSprite::createWithSpriteFrameName("change_diban.png");
    this->addChild(accountbg);
    accountbg->setPosition(ccpAdd(center, ccp(0, 73)));
    
    SGCCLabelTTF *registertitle = NULL;
    registertitle = SGCCLabelTTF::create(str_RegisterBox_str16, FONT_XINGKAI, 32);
    registertitle->cocos2d::CCNode::setPosition(20 + registertitle->getContentSize().width/2, accountbg->getContentSize().height - accountbg->getContentSize().height*0.16);
    accountbg->addChild(registertitle);

    registertitle = SGCCLabelTTF::create(str_ChangePassWordLayer_str7, FONT_XINGKAI, 32);
    registertitle->cocos2d::CCNode::setPosition(20 + registertitle->getContentSize().width/2,
                                                accountbg->getContentSize().height/2 + accountbg->getContentSize().height/8);
    accountbg->addChild(registertitle);
    registertitle = SGCCLabelTTF::create(str_ChangePassWordLayer_str8, FONT_XINGKAI, 32);
    registertitle->cocos2d::CCNode::setPosition(20 + registertitle->getContentSize().width/2,
                                                accountbg->getContentSize().height/2 - accountbg->getContentSize().height/8);
    accountbg->addChild(registertitle);
    registertitle = SGCCLabelTTF::create(str_ChangePassWordLayer_str9, FONT_XINGKAI, 32);
    registertitle->cocos2d::CCNode::setPosition(20 + registertitle->getContentSize().width/2, accountbg->getContentSize().height*0.14);
    accountbg->addChild(registertitle);
    
    
    CCSprite *font = CCSprite::createWithSpriteFrameName("change_font.png");
    this->addChild(font);
    font->setPosition(ccpAdd(accountbg->getPosition(), ccp(-accountbg->getContentSize().width*.5 + font->getContentSize().width*.5, 180)));
    
    //登陆部分
    CCSprite *bgl = CCSprite::createWithSpriteFrameName("login_dik_l.png");
    this->addChild(bgl,1);
    bgl->setAnchorPoint(ccp(0, 0.5));
    bgl->setPosition(ccpAdd(accountbg->getPosition(), ccp(-accountbg->getContentSize().width*.5, -235)));
    
    CCSprite *bgr = CCSprite::createWithSpriteFrameName("login_dik_r.png");
    this->addChild(bgr,1);
    bgr->setAnchorPoint(ccp(1, 0.5));
    bgr->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width,0)));
    
    CCSprite *bgm = CCSprite::createWithSpriteFrameName("login_dik_m.png");
    this->addChild(bgm);
    bgm->setScaleX(((accountbg->getContentSize().width - bgr->getContentSize().width*2)+5.5)/bgm->getContentSize().width);
    bgm->setPosition(ccpAdd(bgl->getPosition(), ccp(accountbg->getContentSize().width*.5, 1)));

//提交按钮
    
    SGButton *registerButton =  SGButton::createFromLocal("loginbtnbg.png", str_LiuyanLayer_str2, this, menu_selector(SGChangePassWordLayer::buttonClickOK),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(registerButton);
    registerButton->setPosition(bgm->getPosition());
    
    //用户帐号
    m_userName = CCEditBox::create(CCSizeMake(320, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    std::string username = CCUserDefault::sharedUserDefault()->getStringForKey("username");
    m_userName->setPlaceHolder(username.c_str());
    m_userName->setFontColor(ccRED);
    m_userName->setMaxLength(30);
    m_userName->setReturnType(kKeyboardReturnTypeDone);
 //   m_userName->setInputFlag(kEditBoxInputFlagPassword);
    m_userName->setInputMode(kEditBoxInputModeEmailAddr);
    addChild(m_userName);
    
    
    //三个输入框
    m_pEditName = CCEditBox::create(CCSizeMake(320, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditName->setPlaceHolder(str_RegisterBox_str1);
    m_pEditName->setFontColor(ccWHITE);
    m_pEditName->setMaxLength(30);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditName->setInputFlag(kEditBoxInputFlagPassword);
    //    m_pEditName->setDelegate(this);
    addChild(m_pEditName);
    
    m_pEditPassword = CCEditBox::create(CCSizeMake(320, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditPassword->setPlaceHolder(str_RegisterBox_str1);
    m_pEditPassword->setFontColor(ccWHITE);
    m_pEditPassword->setMaxLength(16);
    m_pEditPassword->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPassword->setInputMode(kEditBoxInputModeSingleLine);
    //    m_pEditPassword->setDelegate(this);
    addChild(m_pEditPassword);
    
    m_pEditPasswordAgain = CCEditBox::create(CCSizeMake(320, 50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditPasswordAgain->setPlaceHolder(str_RegisterBox_str2);
    m_pEditPasswordAgain->setFontColor(ccWHITE);
    m_pEditPasswordAgain->setMaxLength(16);
    m_pEditPasswordAgain->setInputFlag(kEditBoxInputFlagPassword);
    m_pEditPasswordAgain->setInputMode(kEditBoxInputModeSingleLine);
    //    m_pEditPasswordAgain->setDelegate(this);
    addChild(m_pEditPasswordAgain);
    
    m_userName->setPosition(ccpAdd(accountbg->getPosition(), ccp(82, 103)));
    m_pEditName->setPosition(ccpAdd(accountbg->getPosition(), ccp(82, 33)));
    m_pEditPassword->setPosition(ccpAdd(accountbg->getPosition(), ccp(82, -35)));
    m_pEditPasswordAgain->setPosition(ccpAdd(accountbg->getPosition(), ccp(82, -105)));
    
    
//    // 原密碼  新密碼 再輸入一次新密碼 原密碼不正確 請輸入4—16位字母或數字 兩次密碼不一致
//    //帐号 密码 再输入一次密码
//    CCLabelTTF *user_name = CCLabelTTF::create("帐号", FONT_BOXINFO, 24);
//    user_name->setAnchorPoint(ccp(0, 0.5));
//    user_name->setColor(ccYELLOW);
//    this->addChild(user_name);
//    labelOldPassword = CCLabelTTF::create("原密码", FONT_BOXINFO, 24);
//    labelNewPassword = CCLabelTTF::create("新密码", FONT_BOXINFO, 24);
//    labelNewPasswordAgain = CCLabelTTF::create("再输入一次新密码", FONT_BOXINFO, 24);
//    labelOldPasswordError = CCLabelTTF::create("原密码不正确", FONT_BOXINFO, 24);
//    labelTip = CCLabelTTF::create("请输入4—16位字母或数字", FONT_BOXINFO, 24);
//    labelNewPasswordAgianError = CCLabelTTF::create("两次密码不一致", FONT_BOXINFO, 24);
//    /**************设置AnchorPoint*************/
//    labelOldPassword->setAnchorPoint(ccp(0, 0.5));
//    labelNewPassword->setAnchorPoint(ccp(0, 0.5));
//    labelNewPasswordAgain->setAnchorPoint(ccp(0, 0.5));
//    labelOldPasswordError->setAnchorPoint(ccp(1, 0.5));
//    labelNewPasswordAgianError->setAnchorPoint(ccp(1, 0.5));
//    labelTip->setAnchorPoint(ccp(1, 0.5));
//    /**************设置Color*************/
//    labelOldPassword->setColor(ccYELLOW);
//    labelNewPassword->setColor(ccYELLOW);
//    labelNewPasswordAgain->setColor(ccYELLOW);
//    labelOldPasswordError->setColor(ccRED);
//    labelTip->setColor(ccRED);
//    labelNewPasswordAgianError->setColor(ccRED);
//    /**************设置Position*************/
//    int editBox_Width_2 = m_pEditName->getContentSize().width/2;
//    user_name->setPosition(ccpAdd(center, ccp(-editBox_Width_2, 190)));
//    labelOldPassword->setPosition(ccpAdd(center, ccp(-editBox_Width_2, 100)));
//    labelNewPassword->setPosition(ccpAdd(center, ccp(-editBox_Width_2, 10)));
//    labelNewPasswordAgain->setPosition(ccpAdd(center, ccp(-editBox_Width_2, -80)));
//    labelOldPasswordError->setPosition(ccpAdd(labelOldPassword->getPosition(), ccp(editBox_Width_2*2, 0)));
//    labelTip->setPosition(ccpAdd(labelNewPassword->getPosition(), ccp(editBox_Width_2*2, 0)));
//    labelNewPasswordAgianError->setPosition(ccpAdd(labelNewPasswordAgain->getPosition(), ccp(editBox_Width_2*2, 0)));
//    /**************AddChild*************/
//    this->addChild(labelOldPassword);
//    this->addChild(labelNewPassword);
//    this->addChild(labelNewPasswordAgain);
//    this->addChild(labelOldPasswordError);
//    this->addChild(labelTip);
//    this->addChild(labelNewPasswordAgianError);
//    
//    //设为不可用
//    labelOldPasswordError->setVisible(false);
//    labelNewPasswordAgianError->setVisible(false);
//    labelTip->setVisible(false);
    
//    //对号和错号
//    spriteFlag_1 = CCSprite::create("public/ok.png");
//    spriteFlag_2 = CCSprite::create("public/error.png");
//    spriteFlag_3 = CCSprite::create("public/ok.png");
//    spriteFlag_1->setAnchorPoint(ccp(1, 0.5));
//    spriteFlag_2->setAnchorPoint(ccp(1, 0.5));
//    spriteFlag_3->setAnchorPoint(ccp(1, 0.5));
//    spriteFlag_1->setPosition(ccpAdd(m_pEditName->getPosition(), ccp(m_pEditName->getContentSize().width/2, 0)));
//    spriteFlag_2->setPosition(ccpAdd(m_pEditPassword->getPosition(), ccp(m_pEditPassword->getContentSize().width/2, 0)));
//    spriteFlag_3->setPosition(ccpAdd(m_pEditPasswordAgain->getPosition(), ccp(m_pEditPasswordAgain->getContentSize().width/2, 0)));
//    spriteFlag_1->setVisible(false);
//    spriteFlag_2->setVisible(false);
//    spriteFlag_3->setVisible(false);
//    this->addChild(spriteFlag_1);
//    this->addChild(spriteFlag_2);
//    this->addChild(spriteFlag_3);
    
}

//void SGChangePassWordLayer::processFlag(int first , int secend , int third)
//{
//    //0不显示 1正确 2错误
//    
//    CCSprite *sprite1 = CCSprite::create("public/ok.png");
//    CCSpriteFrame *frame1 = sprite1->displayFrame();
//    CCSprite *sprite2 = CCSprite::create("public/error.png");
//    CCSpriteFrame *frame2 = sprite2->displayFrame();
//    
//    spriteFlag_1->setVisible(first != FLAG_NODISPLAY);
//    if(first == FLAG_OK)
//    {
//        spriteFlag_1->setDisplayFrame(frame1);
//    }
//    else if(first == FLAG_ERROR)
//    {
//        spriteFlag_1->setDisplayFrame(frame2);
//    }
//    
//    spriteFlag_2->setVisible(secend != FLAG_NODISPLAY);
//    if(secend == FLAG_OK){
//        spriteFlag_2->setDisplayFrame(frame1);
//    }
//    else if(secend == FLAG_ERROR)
//    {
//        spriteFlag_2->setDisplayFrame(frame2);
//    }
//    
//    spriteFlag_3->setVisible(third != FLAG_NODISPLAY);
//    if(third == FLAG_OK)
//    {
//        spriteFlag_3->setDisplayFrame(frame1);
//    }else if(third == FLAG_ERROR)
//    {
//        spriteFlag_3->setDisplayFrame(frame2);
//    }
//}

void SGChangePassWordLayer::requestFinished(int msgid, cocos2d::CCObject *data)
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
                SG_SHOW_WINDOW(str_SHOWWINDOW_1008);
                break;
            }
            case 31:
            {
                SG_SHOW_WINDOW(str_SHOWWINDOW_1009);
                break;
            }
            case 32:
            {
                SG_SHOW_WINDOW(str_SHOWWINDOW_1010);
                break;
            }
        }
    }else{
        SG_SHOW_WINDOW(str_SHOWWINDOW_1011);
        CCUserDefault::sharedUserDefault()->setStringForKey("password", m_pEditPasswordAgain->getText());
        CCUserDefault::sharedUserDefault()->flush();
        SGMainManager::shareMain()->showloginLayer();
    }
}
