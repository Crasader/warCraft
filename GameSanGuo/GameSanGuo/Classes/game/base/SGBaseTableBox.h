//
//  SGBasetableBox.h
//  GameSanGuo
// 具有列表功能的弹框
//  Created by gengshengcai on 13-1-11.
//
//

#ifndef __GameSanGuo__SGBasetableBox__
#define __GameSanGuo__SGBasetableBox__

#include "SGBaseBox.h"
#include "SNSTableView.h"

class SGBaseTableBox : public SGBaseBox, public SNSTableViewDataSource, public SNSTableViewDelegate
{
protected:
    SNSTableView *tableView;
    CCArray *datas;
    
    int tableViewHeight;
    int tableViewColumns;
    
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
	// 返回tableView的列数
	virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section);
    
public:
    SGBaseTableBox();
    ~SGBaseTableBox();
    
    virtual bool init(SGBoxDelegate *dg, BoxTag bt, CCSize size, bool isHaveBottom = true, bool isHaveBg = true,bool useDefaultBg = true);
};
#endif /* defined(__GameSanGuo__SGBasetableBox__) */
