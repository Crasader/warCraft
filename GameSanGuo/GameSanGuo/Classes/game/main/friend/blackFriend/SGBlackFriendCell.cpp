//
//  SGBlackFriendCell.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#include "SGBlackFriendCell.h"
#include "SGLayout.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "ResourceManager.h"

SGBlackFriendCell::SGBlackFriendCell() : itemDelete(NULL)
{
    
}

SGBlackFriendCell::~SGBlackFriendCell()
{

}

SGBlackFriendCell* SGBlackFriendCell::create(SGGamePlayer *player ,  SGFriendBaseDelegate *friendDelegate)
{
    SGBlackFriendCell *cell = new SGBlackFriendCell();
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

void SGBlackFriendCell::constructView()
{

    itemDelete = SGButton::create("friend_redButton.png","friend_remove.png",this, menu_selector(SGBlackFriendCell::buttonClickDelete),CCPointZero,false,true);
    ///////////
   SGButton* itemDelete1 = SGButton::create("friend_redButton.png","friend_remove.png",this, NULL,CCPointZero,false,true);
    ///////////
    itemDelete->setPosition(ccp(200, 0));
    itemDelete1->setPosition(itemDelete->getPosition());
//    this->addBtn(itemDelete);
    menu->addChild(itemDelete,-1);
     menu->addChild(itemDelete1,1);
    this->updateCellBySGamePlayer(_player);
}

void SGBlackFriendCell::buttonClickDelete()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::DeleteBlockRequest *deleteBlack = new main::DeleteBlockRequest();
    deleteBlack->set_friendroleid(_player->getFriendID());
    SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_DELETEBLACK, deleteBlack);
    
    if (_parentLayer != NULL) {
        _parentLayer->_selectRoleId = _player->getFriendID();
    }
}
