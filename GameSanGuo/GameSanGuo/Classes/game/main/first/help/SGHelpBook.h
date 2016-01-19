//
//  SGHelpBook.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-12.
//
//

#ifndef __GameSanGuo__SGHelpBook__
#define __GameSanGuo__SGHelpBook__

#include "SGBaseTableLayer.h"



///////////////////////
class HelpLabelItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
///////////
class HelpLabelItem : public SNSTableViewCellItem
{
public:
  
    CCMenu* menu;
    HelpLabelItem();
    HelpLabelItemDelegate *delegate;
    SNSTableView *tableview;
	virtual bool initWithSize();
	void update(CCString *str);
    void buttonClick(CCObject*obj);
	static HelpLabelItem* create(CCSize size,int num,bool isshow = false,HelpLabelItemDelegate*del=NULL,SNSTableView*table=NULL);
};
class SGHelpBook : public SGBaseTableLayer,public HelpLabelItemDelegate
{
private:
    void initView();
    void backHandler();
    void itemselect(SNSTableView* tableView, SNSIndexPath* indexPath);
    
public:
//    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void registerWithTouchDispatcher();
//     virtual void onEnter();
//    virtual void onExit();
    SGHelpBook();
    ~SGHelpBook();
    static SGHelpBook *create();
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);    
};


#endif /* defined(__GameSanGuo__SGHelpBook__) */
