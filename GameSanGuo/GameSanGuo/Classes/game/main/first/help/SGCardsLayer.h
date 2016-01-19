//
//  SGCardsLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-10.
//
//

#ifndef __GameSanGuo__SGCardsLayer__
#define __GameSanGuo__SGCardsLayer__
#include "SGBaseTableLayer.h"
class SGCardsLayer : public SGBaseTableLayer
{
private:
    int enterType;
    CCArray *_array;
private:
    void initView(CCArray *array);
    void backHandler();
    void sortArray();
public:
    SGCardsLayer();
    ~SGCardsLayer();
    static SGCardsLayer *create(CCArray *array,int type);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    
};

#endif /* defined(__GameSanGuo__SGCardsLayer__) */
