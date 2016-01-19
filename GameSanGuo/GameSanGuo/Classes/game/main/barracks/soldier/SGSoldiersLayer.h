//
//  SGSoldiersLayer.h
//  GameSanGuo
// 士兵界面
//  Created by 陈雪龙 on 13-1-9.
//
//

#ifndef __GameSanGuo__SGSoldiersLayer__
#define __GameSanGuo__SGSoldiersLayer__


#include "SGBaseTableLayer.h"
#include "SGSoldierCard.h"
#include "SGCardItem.h"
class SGSoldiersLayer : public SGBaseLayer, public SNSScrollViewPageDelegate ,public SGCardItemDelegate
{
private:
    int x_;
    CCArray *soldiers;
    SGCCLabelTTF *pageLabel;
private:
    void backHandler();    
    void initView();
    void initData();
    void strengHandler(SGSoldierCard *card);

protected:
//    // 根据indexPath返回tableView的cell
//    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);    
//    // 每个item的点击事件
//    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
//    // 设置翻页
//    virtual void refreshPage(int type);
    
protected:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
public:
    SGSoldiersLayer();
    ~SGSoldiersLayer();
    static SGSoldiersLayer *create(int x);
    virtual void initMsg();
    virtual void setIsCanTouch(bool isTouch);
};

#endif /* defined(__GameSanGuo__SGSoldiersLayer__) */
