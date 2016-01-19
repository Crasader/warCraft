//
//  SGStrengSoldierLayer.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-19.
//
//

#ifndef __GameSanGuo__SGStrengSoldierLayer__
#define __GameSanGuo__SGStrengSoldierLayer__

#include "SGBaseLayer.h"
#include "SGSoldierCard.h"
#include "SGBaseTableLayer.h"
#include "SGCardItem.h"
class SGStrengSoldierLayer: /*public SGBaseLayer*/ public SGBaseTableLayer,public SGCardItemDelegate
{
private:
    CCArray *soldiercards;
private:
    void confirmHandler(SGSoldierCard*card);
    void backHandler();
    void strengHandler(SGSoldierCard *card);
    virtual  void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
protected:
//    virtual void registerWithTouchDispatcher();
//    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
//    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

public:
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
    static SGStrengSoldierLayer* create();
    void initView();
    SGStrengSoldierLayer();
    ~SGStrengSoldierLayer();
};
#endif /* defined(__GameSanGuo__SGStrengSoldierLayer__) */
