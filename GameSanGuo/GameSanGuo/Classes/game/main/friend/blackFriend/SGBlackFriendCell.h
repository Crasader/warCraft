//
//  SGBlackFriendCell.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#ifndef __GameSanGuo__SGBlackFriendCell__
#define __GameSanGuo__SGBlackFriendCell__

#include <iostream>
#include "SGButton.h"
#include "SNSTableViewCellItem.h"
#include "cocos-ext.h"
#include "SGGamePlayer.h"
#include "SGFriendCellBase.h"
USING_NS_CC_EXT;

class SGBlackFriendCell :public SGFriendCellBase
{
private:
    SGButton *itemDelete;
    
    void buttonClickDelete();
    
    void constructView();

public:
    static SGBlackFriendCell* create(SGGamePlayer *player , SGFriendBaseDelegate *friendDelegate);
    SGBlackFriendCell();
    ~SGBlackFriendCell();
    
};


#endif /* defined(__GameSanGuo__SGBlackFriendCell__) */
