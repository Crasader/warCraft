//
//  SGMyFriend.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-28.
//
//

#ifndef __GameSanGuo__SGMyFriend__
#define __GameSanGuo__SGMyFriend__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGFriendBase.h"


class SGMyFriend :public SGFriendBase{
    
private:
    
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
private:
    SGButton *buttonSend;
    SGButton *buttonBlack;
    CCArray *_arrayFriend;
    bool _model;
    
    void constructView();
    CCString* getPeopleCountOnline();
    void setSendLetterIsShow(bool isShow);

    void buttonClickSendLetter();
    void buttonClickBlack();    
    
    void checkDuringSkipDay();
    void deleteFriendListener(CCObject *obj);
    void sendTiliListener(CCObject *obj);
    
public:
    
    SGMyFriend();
    ~SGMyFriend();
    static SGMyFriend* create(CCArray *arrayFriend);
    void setEnabletouch(bool b);
    virtual void cellCallBack(SGGamePlayer *player , bool type);
    virtual void updateData();
    virtual void refreshData(int roleId);

};



#endif /* defined(__GameSanGuo__SGMyFriend__) */
