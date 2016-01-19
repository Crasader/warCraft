//
//  SGFriendCellBase.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-7.
//
//

#include "SGFriendCellBase.h"
#include "SGLayout.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "ResourceManager.h"
#include "SGMyFriendCell.h"
#include "SGStringConfig.h"


SGFriendCellBase::SGFriendCellBase()
: itemHeadPic(NULL)
, labelName(NULL)
, labelLoginTime(NULL)
, labelLevel(NULL)
, buttonAgree(NULL)
, buttonRefuse(NULL)
, _player(NULL)
, menu(NULL)
, _parentLayer(NULL)
, _friendDelegate(NULL)
, menu1(NULL)
{
    
}

SGFriendCellBase::~SGFriendCellBase()
{
    
}
bool SGFriendCellBase::viewDidLoad()
{
    if(!SNSTableViewCellItem::init())
    {
        return false;
    }
    
    this->constructView();
    return true;
}

void SGFriendCellBase::constructView()
{
    itemHeadPic = NULL;
    CCSprite *spriteBorder = CCSprite::createWithSpriteFrameName("item_smallbg.png");
    spriteBorder->setScaleY(1.1);
    this->addChild(spriteBorder);
   // spriteBorder->setVisible(false);
//    bgbgbg = NULL;
//   SGButton* spriteBorder1=SGButton::create("item_smallbg.png", NULL, this, menu_selector(SGFriendCellBase::clickbutnone),CCPointZero,false,true);
//    spriteBorder1->setScaleY(1.1);
//    this->addChild(spriteBorder1,-10);
   //addBtn(spriteBorder);
//    this->addChild(spriteBorder);
  //  this->addChild(spriteBorder,-5);
    //    addBtn(backBtn);
   // spriteBorder->setContentSize(CCSizeMake(spriteBorder->getContentSize().width*0.9, spriteBorder->getContentSize().height));
  //  addBtn(spriteBorder);
    //////////////
    ////////  bigBGBtn->setContentSize(CCSizeMake(bigBGBtn->getContentSize().width- backBtn->getContentSize().width*1.2, bigBGBtn->getContentSize().height));
    
    menu = CCMenu::create();
    itemHeadPic = SGButton::create("countryIcon_1.png","jinkuang.png" , this, menu_selector(SGFriendCellBase::buttonClickHeadPic),CCPointZero,false,true);
    itemHeadPic->setAnchorPoint(ccp(1, 0.5));
    ///
    this->addChild(itemHeadPic);
    
    ////////////////
    menu1 = CCMenu::create();
	menu1->setTouchPriority(128);
    menu1->setPosition(CCPointZero);
    menu1->setTag(99999);
    this->addChild(menu1);
    
    
    /////////////////

   SNSTableViewCellItem::addBtn(itemHeadPic);
//////////////////
  ///////////////////
    menu->setPosition(CCPointZero);
    this->addChild(menu);

    spriteBorder->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(itemHeadPic->getContentSize().width/2, 0)));
    itemHeadPic->setPosition(ccpAdd(spriteBorder->getPosition(), ccp(-spriteBorder->getContentSize().width*0.50, 2)));
    
    labelLoginTime = SGCCLabelTTF::create(str_FriendBase_str14, FONT_BOXINFO, 24 , ccGREEN);
    labelLevel = SGCCLabelTTF::create(str_FriendBase_str15, FONT_BOXINFO, 24);
    labelName = SGCCLabelTTF::create(str_FriendBase_str16, FONT_BOXINFO, 24);
    labelLoginTime->setPosition(ccp(-150, 30));
    //modify by:zyc.  merge into create.
    //labelLoginTime->setInsideColor(ccGREEN);
    labelLoginTime->setAnchorPoint(ccp(0, 0.5));
    labelName->setAnchorPoint(ccp(0, 0.5));

    labelName->setPosition(ccpAdd(itemHeadPic->getPosition(), ccp(itemHeadPic->getContentSize().width*0.01, itemHeadPic->getContentSize().height*0.15)));
    labelLoginTime->setPosition(ccpAdd(labelName->getPosition(), ccp(0,-itemHeadPic->getContentSize().height*0.35)));
    labelLevel->setPosition(ccpAdd(itemHeadPic->getPosition(), ccp(-itemHeadPic->getContentSize().width/2,-itemHeadPic->getContentSize().height/2+16)));
    this->addChild(labelLoginTime,9);
    this->addChild(labelLevel,9);
    this->addChild(labelName,9);
    _parentLayer = NULL;
}

void SGFriendCellBase::setParentLayer(SGFriendBase *layer)
{
    _parentLayer = layer;
}
void SGFriendCellBase::updateCellBySGamePlayer(SGGamePlayer *player)
{
    _player = player;
    CCString *fileName = CCString::createWithFormat("countryIcon_%d.png",player->getImage());
    CCSprite *sprite = CCSprite::createWithSpriteFrameName(fileName->getCString());
    if(NULL == sprite)
    {
        fileName->setString("countryIcon_3.png");
    }

    itemHeadPic->setImage(fileName->getCString());
    itemHeadPic->setFontImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("country_%d.png",player->getImage())->getCString())->displayFrame());
    itemHeadPic->setFontImageScale(1.08);
    labelLevel->setString(CCString::createWithFormat(str_Format_ji,player->getLevel())->getCString());;
    if(player->getTime() == 0/*player->getIsOnline()*/)
    {
        labelLoginTime->setInsideColor(ccGREEN);
        labelLoginTime->setString(str_FriendBase_str14);
    }else{
        labelLoginTime->setInsideColor(ccRED);
        if (player->getTime()>0 && player->getTime() < 3600000) {
            labelLoginTime->setString(CCString::createWithFormat(str_FriendBase_str17,player->getTime()/600000)->getCString());
        }
        else if (player->getTime()>= 3600000 && player->getTime() < 1000*60*60*24) {
            labelLoginTime->setString(CCString::createWithFormat(str_FriendBase_str18,player->getTime()/3600000)->getCString());
        }
        else if (player->getTime()>= 1000*60*60*24) {
            labelLoginTime->setString(CCString::createWithFormat(str_FriendBase_str19,player->getTime()/(1000*60*60*24))->getCString());
        }else{
            labelLoginTime->setString(CCString::createWithFormat(str_FriendBase_str20,player->getTime()/(1000*60*60*24*30))->getCString());
        }
    }
    labelName->setString(player->getFriendName()->getCString());
}

void SGFriendCellBase::buttonClickHeadPic()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    ////////////
    
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    int playerid=player->getplayerID();
    
    
    /////////////
    if(!_player)
    {
        SG_SHOW_WINDOW(str_FriendBase_str21);
        return;
    }
    
    main::FriendInfoRequest *info = new main::FriendInfoRequest();
    if(_player->getFriendID() == 0)
    {
        //这个人是搜出来的，friendId没有，为0
        info->set_friendid(_player->getRoleID());
        /////////////
        player->setplayerID(_player->getRoleID());
    }else{
        //这个人属于你的好友黑名单或申请列表中，friendId就是他们自己
        info->set_friendid(_player->getFriendID());
        ///////////
        player->setplayerID(_player->getFriendID());
    }
    CCLOG("玩家信息：SGFriendCellBase");
    
    info->set_type(0);
   
    
    
    
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, info);
}

void SGFriendCellBase::addBtn(SGButton *button)
{
    menu->addChild(button);
}

void SGFriendCellBase::setIsCanTouch(bool isTouch)
{
    menu->setTouchEnabled(isTouch);
}

/////
void   SGFriendCellBase::clickbutnone()
{
    
}
void SGFriendCellBase::setHeadTouchEnable(bool touchEnable)
{
    if (itemHeadPic) {
        itemHeadPic->setEnabled(touchEnable);
        menu1->setEnabled(false);
    }
    
}
void SGFriendCellBase::setPlayer(SGGamePlayer *player)
{
    _player = player;
}
