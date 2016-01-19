//
//  SGFriendCountFull.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-4.
//
//

#include "SGFriendCountFull.h"
#include "SGMyFriend.h"
#include "SGMainManager.h"
#include "SGExpandFriend.h"
#include "SGCCLabelTTF.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGFriendCountFull::SGFriendCountFull()
{
    
}

SGFriendCountFull::~SGFriendCountFull()
{
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_friendCountFull);
}

SGFriendCountFull *SGFriendCountFull::create(SGBoxDelegate *boxDelegate)
{
    SGFriendCountFull *full = new SGFriendCountFull();
    
    if(full && full->init(boxDelegate, box_friendCountFull, CCSizeMake(450,350)))
    {
        
        full->constructView();
        full->autorelease();
        return full;
    }
    
    CC_SAFE_DELETE(full);
    return NULL;
    
}

void SGFriendCountFull::constructView()
{
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    
    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_box_friendCountFull);
 
    CCSprite *spriteFull = CCSprite::createWithSpriteFrameName("friend_friendFull.png");
    SGCCLabelTTF *label_1 = SGCCLabelTTF::create(str_FriendCountFull_str1, FONT_PANGWA, 26);
    SGCCLabelTTF *label_2 = SGCCLabelTTF::create(str_FriendCountFull_str2, FONT_PANGWA, 26);
    
    spriteFull->setPosition(ccpAdd(center, ccp(0,120)));
    label_1->setPosition(ccpAdd(center, ccp(0,25)));
    label_2->setPosition(ccpAdd(center, ccp(0,-35)));
    
    
    this->addChild(spriteFull);
    this->addChild(label_1);
    this->addChild(label_2);
    
    
    //SGButton *buttonExpand = SGButton::create("public_btn_5.png", "friend_expand.png", this, menu_selector(SGFriendCountFull::buttonClickExpand),CCPointZero,false,true);
    //SGButton *buttonManage = SGButton::create("public_btn_5.png", "friend_manage.png", this, menu_selector(SGFriendCountFull::buttonClickManage),CCPointZero,false,true);
    SGButton *buttonCancel = SGButton::create("box_btnbg.png", "public_font_queding.png", this, menu_selector(SGFriendCountFull::buttonClickCancel),CCPointZero,false,true);
    
    //buttonExpand->setPosition(ccpAdd(center, ccp(0, -150)));
//    buttonManage->setPosition(ccpAdd(buttonExpand->getPosition(), ccp(-180, 0)));
//    buttonCancel->setPosition(ccpAdd(buttonExpand->getPosition(), ccp(180, 0)));
    
    //buttonManage->setPosition(ccpAdd(center, ccp(-120, -120)));
    buttonCancel->setPosition(ccpAdd(center, ccp(0, -120)));
    
    //this->addBtn(buttonExpand);
    //this->addBtn(buttonManage);
    this->addBtn(buttonCancel);
    
}

void SGFriendCountFull::buttonClickExpand()
{
    CCLOG("位置扩充");
    
    SGExpandFriend *expand = SGExpandFriend::create();
    SGMainManager::shareMain()->showLayer(expand);
    this->boxClose();
}

void SGFriendCountFull::buttonClickManage()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMyFriend *fr = SGMyFriend::create(SGPlayerInfo::sharePlayerInfo()->getFriendList());
    SGMainManager::shareMain()->showLayer(fr);
    this->boxClose();
}

void SGFriendCountFull::buttonClickCancel()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
}



