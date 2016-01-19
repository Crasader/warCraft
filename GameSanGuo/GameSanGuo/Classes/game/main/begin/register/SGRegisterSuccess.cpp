//
//  SGRegisterSuccess.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-17.
//
//

#include "SGRegisterSuccess.h"
#include "SGHttpClient.h"
#include "SGMsgId.h"
#include "SGServerListLayer.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"


SGRegisterSuccess::SGRegisterSuccess()
{
    
}

SGRegisterSuccess::~SGRegisterSuccess()
{
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_registerSuccess);
}

SGRegisterSuccess* SGRegisterSuccess::create(SGBoxDelegate *delegate)
{
    SGRegisterSuccess *success = new SGRegisterSuccess();
    if(success && success->init(delegate,box_registerTag,CCSizeMake(411, 250)))
    {
        success->constructView();
        success->autorelease();
        return success;
    }
    
    CC_SAFE_DELETE(success);
    return NULL;
        
}

void SGRegisterSuccess::constructView()
{
//    register_font_5.png 注册成功
//    register_font_6.png快速开始游戏
//    public_btn_2.png按钮
//    public/public_font_queding.png确定
    
 
      ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist",RES_TYPE_LAYER_UI ,sg_registerSuccess);
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    CCSprite *spriteRegister = CCSprite::createWithSpriteFrameName("register_font_5.png");
    spriteRegister->setPosition(ccpAdd(center, ccp(0, 70)));
    this->addChild(spriteRegister);
    
    CCLabelTTF *label1 = CCLabelTTF::create(str_RegisterSuccess_str1, FONT_BOXINFO, 24);
    label1->setAnchorPoint(ccp(1, 0.5));
    label1->setPosition(ccpAdd(center, ccp(0, 20)));
    this->addChild(label1);
    
    CCSprite *spriteButton = CCSprite::createWithSpriteFrameName("register_font_6.png");
    spriteButton->setAnchorPoint(ccp(0, 0.5));
    spriteButton->setPosition(label1->getPosition());
    this->addChild(spriteButton);
    
    CCLabelTTF *label2 = CCLabelTTF::create(str_RegisterSuccess_str2, FONT_BOXINFO, 24);
    label2->setPosition(ccpAdd(center, ccp(0, -30)));
    this->addChild(label2);
    
    SGButton *button = SGButton::create("public_btn_2.png", "public_font_queding.png", this, menu_selector(SGRegisterSuccess::buttonClick),CCPointZero,true,true);
    button->setPosition(ccpAdd(center, ccp(0, -190)));
    this->addBtn(button);
    
}

void SGRegisterSuccess::buttonClick()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("RegisterSuccess"), "key");
    data = dict;
    this->boxClose();

}

#pragma mark http
void SGRegisterSuccess::requestFinished(int msgid, cocos2d::CCObject *data)
{
    if(msgid == MSg_HTTP_SERVERLIST)
    {
//        SGServerListLayer *serverListLayer = SGServerListLayer::create((CCArray *)data,SGServerListLayerComeFromFirst);
//        SGMainManager::shareMain()->showLayer(serverListLayer);
        
        this->boxClose();
    }
}
void SGRegisterSuccess::requestFailed()
{
    CCLOG("注册成功网络错误");
}


