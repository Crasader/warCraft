//
//  SGChangeNickName.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-15.
//
//

#include "SGChangeNickName.h"
#include "SGSocketClient.h"
#include "ShopModule.pb.h"
#include "SGMsgId.h"
#include "SGPlayerInfo.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGChangeNickName::SGChangeNickName()
:countryId(0),
label_(NULL),
m_pEditName(NULL),
label_Error(NULL)
{
}
SGChangeNickName::~SGChangeNickName()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
//    notification->removeObserver(this, MSG_STORE_ENTER);
	
	 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_changeNickName);
    
}
SGChangeNickName *SGChangeNickName::create(SGBoxDelegate *dg, int countryId_)
{
    SGChangeNickName *nikeNameBox = new SGChangeNickName();
    if (nikeNameBox && nikeNameBox->init(dg, box_changeNickName, CCSizeMake(432,385)))
    {
        nikeNameBox->countryId = countryId_;
        nikeNameBox->initView();        
        nikeNameBox->autorelease();
        return nikeNameBox;
    }
    CC_SAFE_DELETE(nikeNameBox);
    return NULL;
}
void SGChangeNickName::initView()
{
	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_box_changeNickName, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgplayerinfolayer/sgplayerinfolayer.plist", RES_TYPE_LAYER_UI, sg_box_changeNickName);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    //整合到mainlayer中, 回调相同
//    notification->addObserver(MSG_STORE_ENTER,
//                              this,
//                              callfuncO_selector(SGChangeNickName::storeEnterListener));
   
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    CCLabelTTF *label_1 = CCLabelTTF::create(str_ChangeNickName_str1, FONT_BOXINFO, 26);
    CCLabelTTF *label_2 = CCLabelTTF::create(str_ChangeNickName_str2, FONT_BOXINFO, 20);
    label_2->setDimensions(CCSizeMake(395, 70));
    label_2->setHorizontalAlignment(kCCTextAlignmentLeft);
    label_Error = CCLabelTTF::create(str_ChangeNickName_str3, FONT_BOXINFO, 26);
    label_Error->setColor(ccRED);
    label_Error->setVisible(false);
    CCSprite *spriteNike = CCSprite::createWithSpriteFrameName("font_changnike.png");
    
    this->addChild(label_1);
    this->addChild(label_2);
    this->addChild(label_Error);
    this->addChild(spriteNike);
    CCSprite* redbg = CCSprite::createWithSpriteFrameName("red_bg.png");
    redbg->setScaleX(432/redbg->getContentSize().width);
    this->addChild(redbg,-1);
    redbg->setPosition(ccpAdd(center, ccp(0, 68)));
    

    label_1->setAnchorPoint(ccp(0, 0.5));
    label_1->setPosition(ccpAdd(center, ccp(-395/2, -20)));
    label_2->setPosition(ccpAdd(center, ccp(0, -160)));
    label_Error->setAnchorPoint(ccp(0, 0.5));
    label_Error->setPosition(ccpAdd(label_1->getPosition(),ccp(label_1->getContentSize().width*1.05,0)));
    spriteNike->setPosition(ccpAdd(center,ccp(0, 143)));
    
    
    m_pEditName = CCEditBox::create(CCSizeMake(372, 55), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    m_pEditName->setFontColor(ccRED);
    m_pEditName->setMaxLength(30);
    m_pEditName->setReturnType(kKeyboardReturnTypeDone);
    m_pEditName->setDelegate(this);
    addChild(m_pEditName);
    m_pEditName->setPosition(ccpAdd(center,ccp(0, -75)));
    
    
    SGButton *ok = SGButton::create("box_btnbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGChangeNickName::confirmHandler),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
    ok->setPosition(ccpAdd(center, ccp(-100, -250)));
    
    SGButton *cancel = SGButton::create("box_btnbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGChangeNickName::boxClose),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
    cancel->setPosition(ccpAdd(center, ccp(100, -250)));
    
    if (SGPlayerInfo::sharePlayerInfo()->getPlayerLevel() <16) {
        CCLabelTTF *label1 = CCLabelTTF::create(str_ChangeNickName_str4, FONT_BOXINFO, 26);
        this->addChild(label1);
        label1->setPosition(ccpAdd(redbg->getPosition(), ccp(0, redbg->getContentSize().height*.2)));
        CCLabelTTF *label2 = CCLabelTTF::create(str_ChangeNickName_str5, FONT_BOXINFO, 26);
        label2->setAnchorPoint(ccp(0, 0.5));
        label2->setColor(ccGREEN);
        this->addChild(label2);
        label2->setPosition(ccpAdd(redbg->getPosition(), ccp(-label1->getContentSize().width/2, -redbg->getContentSize().height*.2)));
    }else
    {
            CCLabelTTF *label1 = CCLabelTTF::create(str_ChangeNickName_str6, FONT_BOXINFO, 26);
            this->addChild(label1);
            label1->setPosition(ccpAdd(redbg->getPosition(), ccp(-80, redbg->getContentSize().height*.2)));
        
            CCLabelTTF *label2 = CCLabelTTF::create(CCString::createWithFormat(str_ChangeNickName_str7,SGPlayerInfo::sharePlayerInfo()->getPlayerGold())->getCString(), FONT_BOXINFO, 26);
            label2->setAnchorPoint(ccp(0, 0.5));
            this->addChild(label2);
            label2->setPosition(ccpAdd(redbg->getPosition(), ccp(-label1->getContentSize().width/2 -80, -redbg->getContentSize().height*.2)));
        
        SGButton* chongzhi = SGButton::create("public_btn.png",
                                              "mainlayer_cz.png",
                                              this,
                                              menu_selector(SGChangeNickName::showStoreLayer),
                                              ccp(2, 0),
                                              true,
                                              true);
        this->addBtn(chongzhi);
        chongzhi->setAnchorPoint(ccp(1, 0.5));
        menu->setZOrder(-1);
        chongzhi->setPosition(ccpAdd(redbg->getPosition(), ccp(432/2, 0)));
        
        if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold() <100) {
            label_ = CCLabelTTF::create(str_ChangeNickName_str8, FONT_BOXINFO, 22);
            label_->setAnchorPoint(ccp(0, 0.5));
            label_->setColor(ccRED);
            this->addChild(label_);
            label_->setPosition(ccpAdd(label_1->getPosition(), ccp(label_1->getContentSize().width*1.07, 0)));
        }       

    }
    
}
void SGChangeNickName::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if(!m_pEditName->getText())
    {
        if (label_) {
            label_->setVisible(false);
        }
        label_Error->setVisible(true);
        return;
    }
    
    char bufferStr[1024];
    const char *buffer = m_pEditName->getText();
    strcpy(bufferStr, buffer);
    
    //神一样的检查字数
    if(characterCount(bufferStr) < 2 || characterCount(bufferStr) > 8)
    {
        if (label_) {
            label_->setVisible(false);
        }
        label_Error->setVisible(true);
        return;
    }
    
//    main::CreateRoleRequest *request = new main::CreateRoleRequest();
//    request->set_country(countryId);
//    request->set_nickname(m_pEditName->getText());
//    CCLOG("nkiename:%s",m_pEditName->getText());
//    CCLOG("countryId:%d",countryId);
//    SGSocketClient::sharedSocketClient()->send(MSG_MAIN_CREATEROLE, request);
    boxClose();
}
void SGChangeNickName::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    label_Error->setVisible(false);
    if (label_) {
       label_->setVisible(true);
    }
    CCLog("editBox %p DidBegin !", editBox);
}

void SGChangeNickName::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void SGChangeNickName::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void SGChangeNickName::editBoxReturn(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p was returned !");
    
}

/**
 *  计算字符个数
 *  type: 0、计算字符个数 ，1、返回字节Id ，2、返回字符id ，3、是否为标点
 */

//计算字符串长度
int SGChangeNickName::characterCount(char *str)
{
    int chineseLength = 0;  //中文字符个数
    int englishLength = 0;  //英文字符个数
    int otherLength = 0;    //符号个数
    int index = 0;          //计数器
    
    while (str[index] != '\0') {
        
        //各种特殊符号
        if(str[index] == ' ' || str[index] == '\t' ||
           str[index] == '\r' || str[index] == '\n')
        {
            return -1;
        }
        
        if(('A' <= str[index] && 'Z' >= str[index]) || ('a' <= str[index] && 'z' >= str[index])){
            englishLength++;
        }else if(str[index] < 0){//这个判断暂定 ascii
            chineseLength++;
        }else{
            otherLength++;
        }
        
        index ++;
    }
    
    int length = englishLength + chineseLength/3 + otherLength;
    
    return length;
}
void SGChangeNickName::showStoreLayer()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (SGMainManager::shareMain()->getMainLayer()) {
        SGMainManager::shareMain()->getMainLayer()->requestStoreLayer();
    }
}



