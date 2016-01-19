//
//  SGActivityMainLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGActivityMainLayer__
#define __GameSanGuo__SGActivityMainLayer__

#include "SGBaseTableLayer.h"

class SGActivityMainCellDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
class SGActivityMainLayer : public SGBaseTableLayer,public SGActivityMainCellDelegate
{
public:
    SGActivityMainLayer(void);
    ~SGActivityMainLayer(void);
    
    static SGActivityMainLayer *create(CCArray *array);
    
private:
    int selectIndex;
    
private:
    void initView();
    void initDatas(CCArray *array);
    void backHandler(CCObject *sender);
    void sceneHandler(CCObject *sender);

public:
     void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
};


#endif /* defined(__GameSanGuo__SGActivityMainLayer__) */
