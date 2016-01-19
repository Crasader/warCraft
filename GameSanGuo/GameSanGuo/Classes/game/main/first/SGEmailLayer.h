//
//  SGEmailLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-25.
//
//

#ifndef __GameSanGuo__SGEmailLayer__
#define __GameSanGuo__SGEmailLayer__

#include "SGBaseTableLayer.h"

class SGEmailLayer : public SGBaseTableLayer
{
private:
    void initView();
    void showCards();
    void playerSet();
    void help();
    void invite();
    void advice();
    void band();
    void change();
    void backHandler();
public:
    SGEmailLayer();
    ~SGEmailLayer();
    static SGEmailLayer *create();
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
};


#endif /* defined(__GameSanGuo__SGEmailLayer__) */
