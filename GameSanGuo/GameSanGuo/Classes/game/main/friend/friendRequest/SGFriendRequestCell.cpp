//
//  SGFriendRequestCell.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#include "SGFriendRequestCell.h"
#include "SGLayout.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGFriendCountFull.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGFriendRequestCell::SGFriendRequestCell()
: buttonAgree(NULL)
, buttonRefuse(NULL)
, _request(NULL)
{
    
}

SGFriendRequestCell::~SGFriendRequestCell()
{

}

SGFriendRequestCell* SGFriendRequestCell::create(SGGamePlayer *player,SGFriendBaseDelegate *friendRequest , SGFriendRequest *request)
{
    SGFriendRequestCell *cell = new SGFriendRequestCell();
    cell->_player = player;
    cell->_friendDelegate = friendRequest;
    cell->_request = request;
    
    if(cell && cell->viewDidLoad())
    {
        cell->constructView();
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return NULL;
}

void SGFriendRequestCell::constructView()
{

    ResourceManager::sharedInstance()->bindTexture("sgfriendmaillayer/sgfriendmaillayer.plist", RES_TYPE_LAYER_UI, sg_friendRequest);
    buttonAgree = SGButton::create("friend_blueButton.png","friend_btn_agree.png",this, menu_selector(SGFriendRequestCell::buttonClickAgree),CCPointZero,false,true);
    buttonRefuse = SGButton::create("friend_blueButton.png","friend_btn_refuse.png",this, menu_selector(SGFriendRequestCell::buttonClickRefuse),CCPointZero,false,true);
    buttonAgree->setPosition(ccp(130, 0));
    buttonRefuse->setPosition(ccp(230, 0));

//    this->addBtn(buttonAgree);
//    this->addBtn(buttonRefuse);
    menu1->addChild(buttonAgree);
    menu1->addChild(buttonRefuse);
    
    this->updateCellBySGamePlayer(_player);
    
}

void SGFriendRequestCell::agreeFriendListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("agreeFriendListener");
        return;
    }
    
    main::AgreeFriendResponse *agree = (main::AgreeFriendResponse*)request->m_msg;
    
    //0; 异常失败
    //1; 不在自己的申请列表里
    //2; 自己的好友列表满了
    //3; 对方的好友满了
    //4; 已经在对方申请列表中，清除对方申请并添加好友
    //5; 正常添加

    switch(agree->state())
    {
        case 0:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str9);
            break;
        }
        case 1:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str10);
            break;
        }
        case 2:
        {
            SGFriendCountFull *full = SGFriendCountFull::create(_request);
            SGMainManager::shareMain()->showBox(full);
            break;
        }
        case 3:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str3);
            break;
        }
        case 4:
        case 5:
        {
            SGMainManager::shareMain()->showMessage(str_FriendBase_str11);
            _friendDelegate->cellCallBack(_player,true);
            break;
        }
    }
}

void SGFriendRequestCell::refuseAddFriendListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest*)obj;
    if(!request)
    {
        CCLOG("refuseAddFriendListener");
        return;
    }
    
    main::UnagreeFriendResponse *response = (main::UnagreeFriendResponse*)request->m_msg;
    if(0 == response->state())
    {
        SG_SHOW_WINDOW(str_FriendBase_str12);
    }else if(1 == response->state()){
        SG_SHOW_WINDOW(str_FriendBase_str13);
    }
    
    _friendDelegate->cellCallBack(_player,false);
}

void SGFriendRequestCell::upDate(SGGamePlayer *player)
{
    this->_player = player;
    this->updateCellBySGamePlayer(_player);
}

void SGFriendRequestCell::buttonClickAgree()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::AgreeFriendRequest *agree = new main::AgreeFriendRequest();
    agree->set_friendroleid(_player->getFriendID());
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_AGREEFRIEND, agree);
    if (_parentLayer != NULL) {
        _parentLayer->_selectRoleId = _player->getFriendID();
    }
}

void SGFriendRequestCell::buttonClickRefuse()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::UnagreeFriendRequest *unagree = new main::UnagreeFriendRequest();
    unagree->set_friendroleid(_player->getFriendID());
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_UNAGREEADDFRIEND, unagree);
    if (_parentLayer != NULL) {
        _parentLayer->_selectRoleId = _player->getFriendID();
    }
}
