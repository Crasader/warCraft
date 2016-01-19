//
//  SGMonthlySignLayer.h
//  GameSanGuo
//
//  Created by Fygo Woo on 12/16/14.
//
//

#ifndef __GameSanGuo__SGMonthlySignLayer__
#define __GameSanGuo__SGMonthlySignLayer__

#include "SGBaseTableLayer.h"

class SigninDetailResponse;

class SGMonthlySignLayer : public SGBaseTableLayer
{
//function
public:
    //ctor
    SGMonthlySignLayer();
    
    //dtor
    ~SGMonthlySignLayer();
    
    //static create method
    static SGMonthlySignLayer* create(main::SigninDetailResponse* sdrData);
    
    //view
    void initView();
    
    //根据indexPath返回tableView的cellItem
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    
    //返回tableView的列数
    virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section);
    
    //返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
    //点击某个item。
    void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
//    //点击某个item。
//    void itemSelect();
    
private:
    //领取奖励resp
    void getRewardResponse(CCObject *sender);
    
    //返回按钮
    void backHandler();

//variable
private:
    //总览数据
    main::SigninDetailResponse *sdrData_;
    
    //点击了哪个。
    int selectedIdx_;
    
    //统计文本标签，动态。
    SGCCLabelTTF* statLabel_;
    
};

#endif /* defined(__GameSanGuo__SGMonthlySignLayer__) */

