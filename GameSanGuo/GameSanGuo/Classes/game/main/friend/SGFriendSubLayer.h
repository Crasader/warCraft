//
//  SGFriendSubLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-27.
//
//

#ifndef __GameSanGuo__SGFriendSubLayer__
#define __GameSanGuo__SGFriendSubLayer__

#include <iostream>
#include "cocos-ext.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SNSTableView.h"
USING_NS_CC;
USING_NS_CC_EXT;

#define SGFriendTypeMyFriend      0
#define SGFriendTypeSendTiLi      1
#define SGFriendTypeFriendRequest 2
#define SGFriendTypeSearchFriend  3
#define SGFriendTypeBlackFriend   4
//#define SGFriendTypeInviteFriend  4
#define SGFriendTypeExpandFriend  5

//#define SGFriendTypeCount  6  删除功能：好友位置扩充
#define SGFriendTypeCount  5
class SGFriendSubLayerDelegate
{
public:
    virtual void itemselect(SNSIndexPath* indexpath){};
};

class SGFriendSubLayer : public SNSTableViewCellItem
{
    
private:
    int _friendType;
    SGFriendSubLayerDelegate *delegate;
    void constrctView();
    void buttonClick(CCObject*obj);
public:
    SGFriendSubLayer();
    ~SGFriendSubLayer();
    static SGFriendSubLayer *create(int friendType,SGFriendSubLayerDelegate*del);
    
};


#endif /* defined(__GameSanGuo__SGFriendSubLayer__) */
