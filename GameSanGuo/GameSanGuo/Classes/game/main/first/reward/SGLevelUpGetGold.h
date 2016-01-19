//
//  SGLevelUpGetGold.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-15.
//
//

#ifndef __GameSanGuo__SGLevelUpGetGold__
#define __GameSanGuo__SGLevelUpGetGold__

#include "SGBaseTableLayer.h"
class SGLevelUpDelegate;
class LvlGold_Item : public SNSTableViewCellItem
{
private:
    SGLevelUpDelegate *delegate;       
public:    
    int state; 
    int lvl;
    CCSprite *item;
    SGButton *btn;

    void getGoldHandler(CCNode *sender);
public:
    LvlGold_Item();
    ~LvlGold_Item();
public:
	void setView(int num,int station);
	static LvlGold_Item* create(SGLevelUpDelegate *dlg,int num,int station);
    void updateitem(int num,int station);
};

class SGLevelUpDelegate
{
public:
    virtual void getGoldHandler(LvlGold_Item *LvlGold_Item){};
};

class SGLevelUpGetGold : public SGBaseTableLayer ,public SGLevelUpDelegate
{
private:
    LvlGold_Item *lvlupItem;
    CCArray *_array;
    void initView();
    void backHandler();
    void showGetGold();
    void showGetEverydayGold();
    void showWine();
    void showHope();
    void showliandan();
    void showcaishen();
    void lvlUpEnterListener(cocos2d::CCObject *obj);
public:
    virtual void getGoldHandler(LvlGold_Item *LvlGold_Item);
    SGLevelUpGetGold();
    ~SGLevelUpGetGold();
    static SGLevelUpGetGold *create(CCArray *array);
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
};

#endif /* defined(__GameSanGuo__SGLevelUpGetGold__) */
