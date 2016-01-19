//
//  SGFriendMailLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#ifndef __GameSanGuo__SGFriendMailLayer__
#define __GameSanGuo__SGFriendMailLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "MailModule.pb.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGNotificationCenter.h"
#include "SGMailManager.h"

USING_NS_CC;
USING_NS_CC_EXT;

class SGFriendMailLayer :public SGBaseTableLayer
{
private:
    SGButton *buttonModel;
    //删除模式 阅读模式
    bool _isDeleteModel;
    
    //存储数据的数组
    CCArray *_mailArray;
    
    //没有任何好友消息
    SGCCLabelTTF *m_pNoMailLabel;
    
    void buttonClickGoBack();
    
    void buttonClickModelChange();
    
    //初始化界面
    void viewDidLoad();
    
    void deleteMailListener(CCObject *obj);
    

    
public:
    
    SGFriendMailLayer();
    
    ~SGFriendMailLayer();
    
    static SGFriendMailLayer *create(CCArray *mailArray);
    
    /* 待删除邮件ID */
    static int _waitBeDeletedMailId;
    
    //tableview delegate
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);

    void recieveReloadUpdate(SGMailDataModel* model=NULL);

};

#endif /* defined(__GameSanGuo__SGFriendMailLayer__) */
