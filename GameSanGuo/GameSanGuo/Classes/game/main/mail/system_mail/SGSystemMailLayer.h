//
//  SGSystemMailLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#ifndef __GameSanGuo__SGSystemMailLayer__
#define __GameSanGuo__SGSystemMailLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "MailModule.pb.h"
#include "SGNotificationCenter.h"
#include "SGMailManager.h"

class SGSystemMailLayer :public SGBaseTableLayer
{
private:
    SGButton *buttonModel;  //删除模式 阅读模式
    bool _isDeleteModel;
    //没有任何邮件
    SGCCLabelTTF *m_pNoMailLabel;

    //存储数据的数组
    CCArray *_mailArray;
    void buttonClickGoBack();
    void buttonClickModelChange();
    
    //初始化界面
    void viewDidLoad();    
    //删除监听
    void deleteMailListener(CCObject *obj);
    
    CCArray * sortBytime(CCArray *mailData);
    CCArray * searchByRead(CCArray *mailData,bool isRead);
    CCArray * sortByTimeAndRead(CCArray *mailData);
public:
    
    SGSystemMailLayer();
    ~SGSystemMailLayer();
    static SGSystemMailLayer *create(CCArray *mailArray);
    
    //tableview delegate
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    void recieveReloadUpdate(SGMailSystemDataModel* model=NULL);
};

#endif /* defined(__GameSanGuo__SGSystemMailLayer__) */
