//
//  SGPvpMainLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-9.
//
//

#ifndef __GameSanGuo__SGPvpMainLayer__
#define __GameSanGuo__SGPvpMainLayer__

#include "SGBaseTableLayer.h"
#include "SGPvpMainCell.h"
class SGPvpMainLayer : public SGBaseTableLayer,public SGPvpMainCellDelegate
{
public:
    SGPvpMainLayer(void);
    ~SGPvpMainLayer(void);
    
    static SGPvpMainLayer *create();
    
private:
    void initView();
    void initData();
//    void gotoMatchRequest();
    
    void itemselect(SNSTableView* tableView,SNSIndexPath* indexPath);
    
public:
    // 天梯赛
//    void showSeasonMainLayer(CCObject *sender);
    void showFieldLayer();
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
};

#endif /* defined(__GameSanGuo__SGPvpMainLayer__) */
