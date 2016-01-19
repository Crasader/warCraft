//
//  SGMailCell.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-26.
//
//



#define MAIL_FRIEND     0   //好友邮件
#define MAIL_SYSTEM     1   //系统通知
#define MAIL_GM         2   //联系GM 

#ifndef __GameSanGuo__SGMailCell__
#define __GameSanGuo__SGMailCell__

#include <iostream>
#include "SNSTableViewCellItem.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SNSTableView.h"
USING_NS_CC;
USING_NS_CC_EXT;
////
class SGMailCellDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};



class SGMailCell : public SNSTableViewCellItem
{
private:
    int _index;
    SGMailCellDelegate *delegate;
    SNSTableView *tableview;
    CCSprite *spriteRequestBG;          //消息数量的背景
    CCLabelAtlas *labelRequestCount;    //消息的数量
    void buttonClick(CCObject*obj);
public:
    SGMailCell();
    
    ~SGMailCell();
    
    static SGMailCell *create(int index,SGMailCellDelegate*del,SNSTableView*table);
        
    //界面初始化
    void viewDidLoad();

};


#endif /* defined(__GameSanGuo__SGMailCell__) */
