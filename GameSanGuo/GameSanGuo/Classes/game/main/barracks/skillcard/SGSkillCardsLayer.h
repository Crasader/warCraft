//
//  SGStrengSkillCardLayer.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#ifndef __GameSanGuo__SGStrengSkillCardLayer__
#define __GameSanGuo__SGStrengSkillCardLayer__

#include "SGBaseTableLayer.h"
#include "SGCardItem.h"
class SGSkillCardsLayer : public SGBaseTableLayer, public SNSScrollViewPageDelegate
{

private:
    void backHandler();
    SGCCLabelTTF *pageLabel;
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 设置翻页
    virtual void refreshPage(int type);
public:
    SGSkillCardsLayer();
    ~SGSkillCardsLayer();
    static SGSkillCardsLayer *create(int type);
    void initView(int type);
	void batchSell();
};

#endif /* defined(__GameSanGuo__SGStrengSkillCardLayer__) */
