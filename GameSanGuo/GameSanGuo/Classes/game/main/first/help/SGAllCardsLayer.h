//
//  SGAllCardsLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-11.
//
//

#ifndef __GameSanGuo__SGAllCardsLayer__
#define __GameSanGuo__SGAllCardsLayer__

#include "SGBaseTableLayer.h"
#include "SGBarracksLayer.h"

class SGAllCardsLayer : public SGBaseTableLayer, public LabelItemDelegate
{
private:
    CCArray *diction;
    int weiOfficerCount;
    int shuOfficerCount;
    int wuOfficerCount;
    int qunOfficerCount;
    int equipCount;
    
    CCArray *weidatas;
    CCArray *shudatas;
    CCArray *wudatas;
    CCArray *qundatas;
    CCArray *equipdatas;
private:
    void initView(CCArray *dict); 
    void backHandler();
    void itemselect(SNSTableView* tableView, SNSIndexPath* indexPath);
public:
    SGAllCardsLayer();
    ~SGAllCardsLayer();
    static SGAllCardsLayer *create(CCArray *dict);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
};


#endif /* defined(__GameSanGuo__SGAllCardsLayer__) */
