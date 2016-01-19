//
//  SGInviteFriend.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-1.
//
//

#include "SGInviteFriend.h"
#include "SGFriend.h"
#include "SGMainManager.h"
#include "SGCCLabelTTF.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGInviteFriend::SGInviteFriend() : editSNS(NULL) , editMessage(NULL)
{
    
}

SGInviteFriend::~SGInviteFriend()
{
    
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_inviteFriend);
}

SGInviteFriend* SGInviteFriend::create()
{
    SGInviteFriend *search = new SGInviteFriend();
    if(search && search->init(NULL, sg_inviteFriend))
    {
        
        search->constructView();
        search->autorelease();
        return search;
    }
    
    CC_SAFE_DELETE(search);
    return NULL;
}

void SGInviteFriend::constructView()
{
    CCPoint leftCenter = SGLayout::getPoint(kMiddleCenter);
    CCPoint upLeft = SGLayout::getPoint(kUpLeft);
    CCPoint bottomCenter = SGLayout::getPoint(kBottomCenter);
    CCPoint bottomLeft = SGLayout::getPoint(kBottomLeft);
    CCPoint upCenter = SGLayout::getPoint(kUpCenter);
    CCPoint upRight = SGLayout::getPoint(kUpRight);
    
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist", RES_TYPE_LAYER_UI, sg_inviteFriend);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_inviteFriend);
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_inviteFriend, LIM_PNG_AS_PNG);
    //星形背景
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_inviteFriend);

    CCSprite *spriteBackground = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(768/2-SGLayout::getSkewing(160), 47+150+72, SGLayout::getSkewing(320), SGLayout::getSkewingY(480)-47-150-72-56-101);
    spriteBackground->setTextureRect(r);
    spriteBackground->setAnchorPoint(ccp(0.5, 1));
    spriteBackground->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72)));
    this->addChild(spriteBackground);
    
    //我的好友
    CCSprite *spriteTitle =  CCSprite::createWithSpriteFrameName("friend_title_send.png");
    spriteTitle->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72/2)));
    this->addChild(spriteTitle);
    
    //返回按钮
//    SGButton *buttonBack = SGButton::create("chatbtn.png", "public_fanhui.png",this, menu_selector(SGInviteFriend::buttonClickGoBack),CCPointZero,false,true);
	SGButton *buttonBack = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGInviteFriend::buttonClickGoBack),CCPointZero,FONT_PANGWA,ccWHITE,32);
	buttonBack->setScale(1.05);
    buttonBack->setPosition(ccpAdd(upLeft, ccp(0, -47-150-72/2)));
    buttonBack->setAnchorPoint(ccp(0, 0.5));
    this->addBtn(buttonBack);
    
    CCSprite *spriteBackground1 = CCSprite::createWithSpriteFrameName("server_BlueBG.png");
    CCSprite *spriteBackground2 = CCSprite::createWithSpriteFrameName("login/server_GreenBG.png");
    spriteBackground1->setScaleX(SGLayout::getSkewing(280)/spriteBackground1->getContentSize().width);
    spriteBackground2->setScaleX(SGLayout::getSkewing(280)/spriteBackground2->getContentSize().width);
    spriteBackground1->setScaleY(200/spriteBackground1->getContentSize().height);
    spriteBackground2->setScaleY(200/spriteBackground2->getContentSize().height);
    spriteBackground1->setPosition(ccpAdd(upCenter, ccp(0, -47-150-72-120)));
    spriteBackground2->setPosition(ccpAdd(spriteBackground1->getPosition(), ccp(0, -250)));
    this->addChild(spriteBackground1);
    this->addChild(spriteBackground2);
    
    CCScale9Sprite *spriteBorder1 = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    CCScale9Sprite *spriteBorder2 = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    spriteBorder1->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), 200));
    spriteBorder2->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), 200));
    spriteBorder1->setPosition(spriteBackground1->getPosition());
    spriteBorder2->setPosition(spriteBackground2->getPosition());
    this->addChild(spriteBorder1);
    this->addChild(spriteBorder2);
    
    SGCCLabelTTF *labelNike = SGCCLabelTTF::create(str_FriendCountFull_str7, FONT_BOXINFO, 24);
    SGCCLabelTTF *labelID = SGCCLabelTTF::create(str_FriendCountFull_str8, FONT_BOXINFO, 24);
    labelNike->setAnchorPoint(ccp(0, 1));
    labelID->setAnchorPoint(ccp(0, 1));
    labelNike->setPosition(ccpAdd(spriteBackground1->getPosition(), ccp(-spriteBackground1->getContentSize().width/2+20, spriteBackground1->getContentSize().height/2+10)));
    labelID->setPosition(ccpAdd(spriteBackground2->getPosition(), ccp(-spriteBackground2->getContentSize().width/2+20, spriteBackground2->getContentSize().height/2+10)));
    this->addChild(labelNike);
    this->addChild(labelID);
    
    
    
    
    editMessage = CCEditBox::create(CCSizeMake(SGLayout::getSkewing(250),50),  CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    editSNS = CCEditBox::create(CCSizeMake(SGLayout::getSkewing(250),50),  CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    editMessage->setAnchorPoint(ccp(0.5, 0.5));
    editSNS->setAnchorPoint(ccp(0.5, 0.5));
    editSNS->setPosition(ccpAdd(spriteBackground1->getPosition(),ccp(0, 10)));
    editMessage->setPosition(ccpAdd(spriteBackground2->getPosition(),ccp(0, 10)));
    editSNS->setFontColor(ccBLACK);
    editMessage->setFontColor(ccBLACK);
    
    editSNS->setEnabled(false);
    editMessage->setEnabled(false);
    
    this->addChild(editSNS);
    this->addChild(editMessage);
    
    SGButton *buttonSearch1 = SGButton::create("public_btn_1.png", "friend_invite_send.png", this, menu_selector(SGInviteFriend::buttonClickSendSNS),CCPointZero,false,true);
    SGButton *buttonSearch2 = SGButton::create("public_btn_1.png", "friend_invite_send.png", this, menu_selector(SGInviteFriend::buttonClickMessage),CCPointZero,false,true);
    buttonSearch1->setAnchorPoint(ccp(0.5, 1));
    buttonSearch2->setAnchorPoint(ccp(0.5, 1));
    buttonSearch1->setPosition(ccpAdd(spriteBackground1->getPosition(), ccp(0, -40)));
    buttonSearch2->setPosition(ccpAdd(spriteBackground2->getPosition(), ccp(0, -40)));
    this->addBtn(buttonSearch1);
    this->addBtn(buttonSearch2);
}

void SGInviteFriend::buttonClickSendSNS()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCLOG("发送到社交平台");
    SGMainManager::shareMain()->showMessage(str_FriendCountFull_str9);
}

void SGInviteFriend::buttonClickMessage()
{
    EFFECT_PLAY(MUSIC_BTN);
    CCLOG("发送到手机短信");
    SGMainManager::shareMain()->showMessage(str_FriendCountFull_str9);
    
}

void SGInviteFriend::buttonClickGoBack()
{
    SGFriend *friends = SGFriend::create();
    SGMainManager::shareMain()->showLayer(friends);
    EFFECT_PLAY(MUSIC_BTN);
    
}
