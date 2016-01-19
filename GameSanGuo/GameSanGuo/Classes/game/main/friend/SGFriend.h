//
//  SGFriend.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-27.
//
//


/*********导入系统的三种字体************
    
      字体名称     系统的fontName
 
    长城中隶繁体    TJenLei-Medium
    方正胖娃繁体    FZPWFW--GB1-0
    方正行楷繁体    FZXKFW--GB1-0
    经典细隶书繁    --unknown-1--
*************************************/

/*
 private int roleId;
 private int friendId;
 private String friendName;
 private int typeId;// 0是好友，1黑名单，2申请列表
 private int isOnline;
 private int level;
 private int time;// 上次离线距离现在的时间
 
 增加头像
 */


#define SGFRIEND_FRIEND   0     //好友
#define SGFRIEND_BLACK    1     //黑名单
#define SGFRIEND_REQUEST  2     //请求列表
#define SGFRIEND_STRANGER 3     //陌生人


#ifndef __GameSanGuo__SGFriend__
#define __GameSanGuo__SGFriend__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos-ext.h"
#include "SGGamePlayer.h"
#include "FriendModule.pb.h"
#include "SGGamePlayerInfo.h"
#include "ItemModule.pb.h"
#include "SGFriendSubLayer.h"
USING_NS_CC_EXT;

class SGFriend :public SGBaseTableLayer,public SGFriendSubLayerDelegate
{
    
private:
    
    void construct();
    void initView();
    void itemselect(SNSIndexPath* indexPath);
    
    void buttonClickBack();
    
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
//    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);

public:
    
    SGFriend();
    ~SGFriend();
    static SGFriend* create();

    static SGGamePlayer* getSGGamePlayer(main::FriendProto _friend);
    static SGGamePlayerInfo *getSGGamePlayerInfo(main::RoleInfo roleInfo);
    static int isFriendOfPlayer(int roleId);

    static bool checkFriendIsSelf(int roleId);
    
    void updateData();

};


#endif /* defined(__GameSanGuo__SGFriend__) */
