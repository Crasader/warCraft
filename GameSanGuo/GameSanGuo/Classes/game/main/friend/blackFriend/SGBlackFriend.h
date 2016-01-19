//
//  SGBlackFriend.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#ifndef __GameSanGuo__SGBlackFriend__
#define __GameSanGuo__SGBlackFriend__

#include <iostream>
#include "SGBaseLayer.h"
#include "SGBaseTableLayer.h"
#include "SGFriendBase.h"

class SGBlackFriend : public SGFriendBase{
    
private:
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
private:
    CCArray *_arrayBlackFriend;
    SGButton *buttonSearch;
    
    void constructView();
    CCString* getPeopleCountOnline();
    
    void buttonClickSearchBlack();
    void deleteBlackListener(CCObject *object);
    
public:
    SGBlackFriend();
    ~SGBlackFriend();
    static SGBlackFriend* create(CCArray *arrayBlackFriend);
    virtual void cellCallBack(SGGamePlayer *player , bool type);
    virtual void updateData();
    
    virtual void refreshData(int roleId);
};


#endif /* defined(__GameSanGuo__SGBlackFriend__) */
