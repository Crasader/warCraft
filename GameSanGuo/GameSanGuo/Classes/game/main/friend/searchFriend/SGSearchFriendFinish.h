//
//  SGSearchFriendFinish.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-1.
//
//

#ifndef __GameSanGuo__SGSearchFriendFinish__
#define __GameSanGuo__SGSearchFriendFinish__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos-ext.h"
#include "SGSearchFriendCell.h"
#include "SGFriend.h"
#include "SGMainManager.h"
#include "SGFriendBase.h"
#include "SGGamePlayer.h"

USING_NS_CC_EXT;

class SGSearchFriendFinish : public SGFriendBase
{
private:
    
    CCArray *_arraySearch;
    bool _isFriend;
    SGButton *buttonAddFriend;
    SGButton *buttonAddBlack;
    
    int _pageCount;
    bool _canSearch;
    CCString *_nickName;
    
    void constructView();
    
    void buttonClickAddFriend();
    void buttonClickAddBlack();
    
    void searchFriendByPageListener(CCObject *obj);
    
    bool getIsCanPage();
    
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
public:
    SGSearchFriendFinish();
    ~SGSearchFriendFinish();
    
    void addFriendListener(CCObject *sender);   // 好友
    void addBlackListener(CCObject *sender);    // 黑名单
    
    static SGSearchFriendFinish* create(CCString *nickName,CCArray *arraySearch, bool isFriend);
    
    virtual void cellCallBack(SGGamePlayer *player , bool type);

    virtual bool init(const char *fileName_, LayerTag tag_, bool isSpriteFrame = true);
    virtual void updateData(){};

};



#endif /* defined(__GameSanGuo__SGSearchFriendFinish__) */
