//
//  SGSearchFriendCell.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-1.
//
//

#ifndef __GameSanGuo__SGSearchFriendCell__
#define __GameSanGuo__SGSearchFriendCell__

#include <iostream>
#include "SNSTableViewCellItem.h"
#include "cocos-ext.h"
#include "SGLayout.h"
#include "SGGamePlayer.h"
#include "SGButton.h"
#include "SGFriendCellBase.h"
#include "SGSearchFriendFinish.h"

USING_NS_CC_EXT;

class SGSearchFriendFinish;

class SGSearchFriendCell : public SGFriendCellBase {
private:
    
    SGButton *buttonControl;
    
    bool _isFriend;

    void constructView();
    
    void buttonClickControl();
    
    CCString* getButtonImageName();
    void addBlackListener(CCObject *obj);


    
public:
    SGSearchFriendCell();
    ~SGSearchFriendCell();
    static SGSearchFriendCell* create(SGGamePlayer *player , bool isFriend , SGFriendBaseDelegate *friendDelegate);
    void update(SGGamePlayer *player , bool isFriend);
    
};

#endif /* defined(__GameSanGuo__SGSearchFriendCell__) */
