//
//  SGExpandLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-28.
//
//

#ifndef __GameSanGuo__SGExpandLayer__
#define __GameSanGuo__SGExpandLayer__

#include "SGBaseTableLayer.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGCCLabelTTF.h"


////////////////
class TabelItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
////////////////
class SGExpandLayer : public SGBaseTableLayer,public TabelItemDelegate
{
private:
    int bagsize;
    int needGold;
    //    卡牌数量
    SGCCLabelTTF *cardcount;
    SGCCLabelTTF *notice;
private:
    void expandListener(CCObject *obj);
    void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
//    virtual void initMsg();
protected:
    virtual void setIsCanTouch(bool isTouch);
protected: 
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);   
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
public:
    SGExpandLayer();
    ~SGExpandLayer();
    void backHandler();
    static    SGExpandLayer *create();
    virtual void initView();

  
};

class TabelItem : public SNSTableViewCellItem
{
public:
    SGCCLabelTTF *label;
    SGCCLabelTTF* label2;
    TabelItemDelegate *delegate;
    SNSTableView *tableview;
public:
    TabelItem();
	virtual bool initWithSize(CCSize size,int num);
	
	static TabelItem* create(CCSize size,int num,TabelItemDelegate*del,SNSTableView*table);
    void buttonClick(CCObject*obj);
};


#endif /* defined(__GameSanGuo__SGExpandLayer__) */
