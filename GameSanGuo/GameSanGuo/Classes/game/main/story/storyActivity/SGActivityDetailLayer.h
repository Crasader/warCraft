//
//  SGActivityDetailLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-3.
//
//

#ifndef __GameSanGuo__SGActivityDetailLayer__
#define __GameSanGuo__SGActivityDetailLayer__

#include "SGBaseTableLayer.h"

class SGActivityDetailLayer : public SGBaseTableLayer
{
public:
    SGActivityDetailLayer(void);
    ~SGActivityDetailLayer(void);
    
    static SGActivityDetailLayer *create(int aid = 0);
    
private:
    void initView(int aid);
    void backHandler(CCObject *sender);
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
private:
    int _activityId; // 当前活动ID
};

#endif /* defined(__GameSanGuo__SGActivityDetailLayer__) */
