//
//  SGSelLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-11.
//
//

#ifndef __GameSanGuo__SGSelLayer__
#define __GameSanGuo__SGSelLayer__

#include "SGBaseTableLayer.h"

class SGSetLayer : public SGBaseTableLayer
{

private:
    void initView();
    void backHandler();
public:
    SGSetLayer();
    ~SGSetLayer();
    static SGSetLayer *create();
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
};


#endif /* defined(__GameSanGuo__SGSelLayer__) */
