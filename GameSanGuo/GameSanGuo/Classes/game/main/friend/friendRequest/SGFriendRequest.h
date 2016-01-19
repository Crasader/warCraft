//
//  SGMyFriend.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#ifndef __GameSanGuo__SGFriendRequest__
#define __GameSanGuo__SGFriendRequest__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGGamePlayer.h"
#include "SGFriendBase.h"


class SGFriendRequest :public SGFriendBase,public SGFriendCellBaseDelegate
{
    
private:
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);

private:
    CCArray *_arrayFriendRequest;
    
    void constructView();
    
public:
    SGFriendRequest();
    ~SGFriendRequest();
    static SGFriendRequest* create(CCArray *arrayFriendRequest);

    virtual void cellCallBack(SGGamePlayer *player , bool type);
    
    virtual void updateData();
    virtual void refreshData(int roleId);

};



#endif /* defined(__GameSanGuo__SGFriendRequest__) */
