//
//  SGMyFriendCell.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#ifndef __GameSanGuo__SGMyFriendCell__
#define __GameSanGuo__SGMyFriendCell__

#include <iostream>
#include "SNSTableViewCell.h"
#include "SNSTableViewCellItem.h"
#include "SGButton.h"
#include "SGGamePlayer.h"
#include "cocos-ext.h"
#include "SGFriendCellBase.h"

USING_NS_CC_EXT;


class SGMyFriendCell :public SGFriendCellBase
{
private:
    SGButton *itemSendLetter;
    SGButton *itemDelete;
    SGButton *itemSendTili;
    bool _itemModel;
public:
    void buttonClickSendLetter();
    void buttonClickDelete();
    void buttonClickSendTili();
    void constructView();
    void setMenuEnable(bool b)
    {
        menu1->setEnabled(b);
        
    }
    
public:
    static SGMyFriendCell* create(bool itemModel , SGGamePlayer *player , SGFriendBaseDelegate *friendDelegate);
    
    void upDate(bool model , SGGamePlayer *player);
    
    SGMyFriendCell();
    ~SGMyFriendCell();

    
    
};

#endif /* defined(__GameSanGuo__SGMyFriendCell__) */
