//
//  SGBaseTableLayer.h
//  GameSanGuo
// 具有tableview公能的界面
//  Created by gengshengcai on 13-1-11.
//
//

#ifndef __GameSanGuo__SGBaseTableLayer__
#define __GameSanGuo__SGBaseTableLayer__

#include "SGBattleLayer.h"
#include "SNSTableView.h"

class SGBaseTableLayer : public SGBaseLayer, public SNSTableViewDataSource, public SNSTableViewDelegate
{
protected:
//    SNSTableView *tableView;
    CCArray *datas;
    
    int tableViewColumns;
    
    CCRect boxRect;
public:
    virtual void setIsCanTouch(bool isTouch);
    
protected: /*SNSTableViewDataSource*/
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    /*SNSTableViewDelegate*/
    // 定义tableView每一行或列的高度
    virtual float tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath, CCPoint position);
	// 返回tableView的列数
	virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section);
    
public:
    SNSTableView *tableView;
    SGBaseTableLayer();
    virtual ~SGBaseTableLayer();
    
    virtual bool init(const char *fileName_, LayerTag tag_, bool isSpriteFrame = true);
    virtual void initView();
    inline SNSTableView *getSNSTavleView(){return tableView;};
    void hideScrollItemSingleMove();
    void hideScrollItemScale();
    void hideScrollItemMultiMove();
    
public:
    int tableViewHeight;
};
#endif /* defined(__GameSanGuo__SGBaseTableLayer__) */
