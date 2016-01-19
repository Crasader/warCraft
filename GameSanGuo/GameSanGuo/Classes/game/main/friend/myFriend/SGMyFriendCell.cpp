//
//  SGMyFriendCell.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#include "SGMyFriendCell.h"
#include "SGLayout.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGFriendMailContent.h"
#include "SGMailDataModel.h"
#include "ResourceManager.h"
#include "SGShowString.h"
#define SEND_TAG  5

SGMyFriendCell::SGMyFriendCell() : itemSendLetter(NULL) , itemDelete(NULL) , itemSendTili(NULL)
{
    _itemModel = true;

}

SGMyFriendCell::~SGMyFriendCell()
{
//   ResourceManager::sharedInstance()->unBindLayerTexture(sg_countrysInfo);
}

SGMyFriendCell* SGMyFriendCell::create(bool itemModel ,SGGamePlayer *player,SGFriendBaseDelegate *friendDelegate)
{
    SGMyFriendCell *cell = new SGMyFriendCell();
    cell->_itemModel = itemModel;
    cell->_player = player;
    cell->_friendDelegate = friendDelegate;

    if(cell && cell->viewDidLoad())
    {
        cell->constructView();
        cell->autorelease();
        return cell;
    }
    
    CC_SAFE_DELETE(cell);
    return NULL;
}



void SGMyFriendCell::constructView()
{
    ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist",RES_TYPE_LAYER_UI ,sg_myFriend);
    itemDelete = SGButton::create("friend_redButton.png","friend_delete.png",this, menu_selector(SGMyFriendCell::buttonClickDelete),CCPointZero,false,true);
    itemSendLetter = SGButton::create("friendChating.png",NULL,this, menu_selector(SGMyFriendCell::buttonClickSendLetter),CCPointZero,false,true);
    
    if (_player->getIsGiftPower() == 1)
    {
        //已送过
       itemSendTili  = SGButton::create("tiliHui.png","tiliGotten.png",this, menu_selector(SGMyFriendCell::buttonClickSendTili),ccp(0,-18),false,true);
       itemSendTili->setEnabled(false);
    }
    else
    {
        itemSendTili  = SGButton::create("tiliNormal.png","tiliSending.png",this, menu_selector(SGMyFriendCell::buttonClickSendTili),ccp(0,-18),false,true);
    }
    
    itemDelete->setPosition(ccp(200, 0));
    itemSendLetter->setPosition(ccp(190 - itemSendLetter->getContentSize().width, 0));
    itemSendTili->setPosition(ccp(190 + (int)itemSendLetter->getContentSize().width*0.4, 0));

    //menu1 = CCMenu::create();
    //this->addChild(menu1);
    menu->addChild(itemDelete);
    menu->addChild(itemSendLetter);
    menu->addChild(itemSendTili);
    
    itemDelete->setVisible(!_itemModel);
    itemSendLetter->setVisible(_itemModel);
    itemSendTili->setVisible(_itemModel);
    this->updateCellBySGamePlayer(_player);

}

void SGMyFriendCell::upDate(bool model , SGGamePlayer *player)
{
    _player = player;
    this->updateCellBySGamePlayer(_player);    
    if(_itemModel == model)
    {
        return;
    }
    
    _itemModel = model;
    itemDelete->setVisible(!_itemModel);
    itemSendLetter->setVisible(_itemModel);
    itemSendTili->setVisible(_itemModel);
}

void SGMyFriendCell::buttonClickDelete()
{
    
    EFFECT_PLAY(MUSIC_BTN);
    main::DeleteFriendRequest *deleteFriend = new main::DeleteFriendRequest();
    deleteFriend->set_friendroleid(_player->getFriendID());
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_DELETEFRIEND, deleteFriend);
    
    if (_parentLayer != NULL) {
        _parentLayer->_selectRoleId = _player->getFriendID();
    }
}

void SGMyFriendCell::buttonClickSendLetter()
{
    
    EFFECT_PLAY(MUSIC_BTN);

       SGMailDataModel *data = NULL;

       bool haveHe = false;
       CCObject *object = NULL;
       CCARRAY_FOREACH(SGPlayerInfo::sharePlayerInfo()->getFriendMails(), object)
         {
            SGMailDataModel *mailModel = (SGMailDataModel*)object;
            if(mailModel->getMailOtherId() == _player->getFriendID())
              {
                haveHe = true;
                data = mailModel;
                break;
              }
          }
    
    if(!haveHe)
    {
        //构造一个假的主体邮件，先作为发送基础，等服务器发来真正的主体时在替换
        data = new SGMailDataModel();
        data->setMailSsid(-1);
        data->setMailOtherId(_player->getFriendID());
        data->setMailOtherNickname(_player->getFriendName());
        data->setMailOneSelfId(_player->getRoleID());
        data->setMailItemId(_player->getImage());
        data->autorelease();

    }
    
    SGFriendMailContent *friendMail = SGFriendMailContent::create(data);
    SGMainManager::shareMain()->showLayer(friendMail);
}

void SGMyFriendCell::buttonClickSendTili()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_player->getIsGiftPower() == 1)
    {
        //已送过
        SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str5);
    }
    else
    {
        _player->setIsGiftPower(1);
        
        itemSendTili->init("tiliHui.png","tiliGotten.png",this, menu_selector(SGMyFriendCell::buttonClickSendTili),ccp(0,-18),false,true);
        itemSendTili->setPosition(ccp(190 + (int)itemSendLetter->getContentSize().width*0.4, 0));
        itemSendTili->setEnabled(false);
        
        main::PowerGiftRequest *presentFriend = new main::PowerGiftRequest();
        presentFriend->set_friendid(_player->getFriendID());
        SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PRESENTPOW, presentFriend);
        
        SGMainManager::shareMain()->showMessage(str_FriendSendTiLi_str2);
    }
}
