//
//  SGMyFriendCell.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#ifndef __GameSanGuo__SGFriendRequestCell__
#define __GameSanGuo__SGFriendRequestCell__

#include <iostream>
#include "SNSTableViewCell.h"
#include "SNSTableViewCellItem.h"
#include "SGButton.h"
#include "cocos-ext.h"
#include "SGGamePlayer.h"
#include "SGFriendRequest.h"
#include "SGFriendCellBase.h"


USING_NS_CC_EXT;


class SGFriendRequestCell :public SGFriendCellBase
{
private:
    SGButton *buttonAgree;
    SGButton *buttonRefuse;
    
    SGFriendRequest *_request;
    
    void buttonClickAgree();
    void buttonClickRefuse();
    
    void constructView();
    
    void agreeFriendListener(CCObject* obj);
    void refuseAddFriendListener(CCObject *obj);
    
public:
    static SGFriendRequestCell* create(SGGamePlayer *player,SGFriendBaseDelegate *friendRequest,SGFriendRequest *request);
    SGFriendRequestCell();
    ~SGFriendRequestCell();
    
    void upDate(SGGamePlayer *player);
    
};

#endif /* defined(__GameSanGuo__SGFriendRequestCell__) */
