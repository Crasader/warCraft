//
//  SGSearchFriend.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#ifndef __GameSanGuo__SGSearchFriend__
#define __GameSanGuo__SGSearchFriend__

#include <iostream>
#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "SGGamePlayer.h"
#include "FriendModule.pb.h"
USING_NS_CC_EXT;

class SGSearchFriend : public SGBaseLayer
{
    
private:
    CCEditBox *editNikeName;
    CCEditBox *editID;
    bool _isFriend;
    CCString* _nickName;
    
    void constructView();
    
    void searchFriendByNikeName();
    void searchFriendByID();
    void buttonClickGoBack();
   
    void searchFriendByNikeNameListener(CCObject *obj);
    void searchFriendByIdListener(CCObject *obj);

public:
    
    SGSearchFriend();
    ~SGSearchFriend();
    static SGSearchFriend* create(bool isFriend);

};


#endif /* defined(__GameSanGuo__SGSearchFriend__) */
