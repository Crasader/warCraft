//
//  SGMail.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-26.
//
//

#ifndef __GameSanGuo__SGMail__
#define __GameSanGuo__SGMail__

#include <iostream>
#include "SGBaseTableLayer.h"
#include <time.h>
#include "SGMailDataModel.h"
#include "MailModule.pb.h"
#include "SGMailCell.h"

using namespace std;

class SGMail :public SGBaseTableLayer,public SGMailCellDelegate
{
private:
    //初始化界面
    void viewDidLoad();
    void buttonClickGoBack();

public:
    
    SGMail();
    ~SGMail();
    static SGMail *create();

    //tableview delegate
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
    //把一个时间戳解析成一个时间格式的字符串
    static std::string getTimeStringByTimeStamp(long long timeStamp);
    
    //解析protobuf的数据
    static SGMailDataModel *getMailDataByProto(main::MailProto proto);
    
    void recieveMailUpdate();
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
};

#endif /* defined(__GameSanGuo__SGMail__) */
