//
//  SGChooseSkillCardLayer.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#ifndef __GameSanGuo__SGChooseSkillCardLayer__
#define __GameSanGuo__SGChooseSkillCardLayer__

#include "SGBaseTableLayer.h"
#include "SGSkillItem.h"
#include "SGSortBox.h"


class SGChooseSkillCardLayer : public SGBaseTableLayer ,public SGSortLayerDelegate,public SGSkillItemDelegate
{
    
private:
    void backHandler();
    SGButton *range;
protected:
    void sortHandler();
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void sortLayerCallBack(CCArray *cardArray);
public:
    SGChooseSkillCardLayer();
    ~SGChooseSkillCardLayer();
    static SGChooseSkillCardLayer *create();
    void initView();
    void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
};

#endif /* defined(__GameSanGuo__SGChooseSkillCardLayer__) */
