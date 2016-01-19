//
//  SGJunxianLayer.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//

#ifndef __GameSanGuo__SGJunxianLayer__
#define __GameSanGuo__SGJunxianLayer__

#include "SGBaseLayer.h"
#include "SNSTableView.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGCCLabelTTF.h"
#include "SGBaseTableLayer.h"
#include "SGJunbuff.h"


///////////////////////
//class SeasonListItemDelegate
//{
//public:
//    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
//};
/////////////////////
class SGJunxianLayer : public SGBaseTableLayer
{
private:   
//    SNSTableView *m_tableView;    
 private:
    virtual void initView();
    virtual void initMsg();
    /////////
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
    void backHandler();
    void showSeasonMainLayer(CCObject *sender);
//    virtual void registerWithTouchDispatcher();
//    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void onEnter();
//    virtual void onExit();

    ///////
protected:
    virtual void setIsCanTouch(bool isTouch);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    /*SNSTableViewDelegate*/
    // 定义tableView每一行或列的高度
//    virtual float tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
	// 返回tableView的列数
//	virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section); 

public:
    SGJunxianLayer();
    ~SGJunxianLayer();
static    SGJunxianLayer *create();

//    CCMenuItemImage *battleitem;
//    void teamformationHandler();
//    void generalHandler();
//    void soldierHandler();
//    void equipmentHandler();
//    void propHandler();
//    void skillHandler();
//    void expandHandler();
//	
//	void piecesHandler();//碎片
};


class JunxianItem : public SNSTableViewCellItem
{
public:
    SGCCLabelTTF *label;
//    SeasonListItemDelegate *delegate;
    SNSTableView *tableview;
    CCSize bgSize;
    CCSprite *spriteBg;
    CCSprite *icon;
    CCSprite *name;
    CCLabelTTF *condition;
    CCLabelTTF *buff;
//    CCMenu * menu;
public:
    JunxianItem();
    void buttonClick(CCObject*obj);
	virtual bool initWithSize();
	void updateitem(SGJunbuff* jun);
	static JunxianItem* create();
    /////
//   virtual void registerWithTouchDispatcher();
//  
//    virtual void onEnter();
};









#endif /* defined(__GameSanGuo__SGJunxianLayer__) */
