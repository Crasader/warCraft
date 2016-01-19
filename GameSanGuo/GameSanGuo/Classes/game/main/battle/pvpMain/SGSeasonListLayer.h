//
//  SGSeasonListLayer.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//

#ifndef __GameSanGuo__SGSeasonListLayer__
#define __GameSanGuo__SGSeasonListLayer__

#include "SGBaseLayer.h"
#include "SNSTableView.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGCCLabelTTF.h"
#include "SGBaseTableLayer.h"
#include "SGJunbuff.h"
//#include "SGMenu.h"


///////////////////////
class SeasonListItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
/////////////////////
class SGSeasonListLayer : public SGBaseTableLayer,public SeasonListItemDelegate
{
private:   
//    SNSTableView *m_tableView;    
    CCString *str_ofr;
    CCString *str_eqp;    
    CCString *str_soul;
    CCString *str_prop;
    CCString *str_skill;
	CCString *str_pieces;//碎片
private:
    virtual void initView(int count,CCArray *ary);
    virtual void initMsg();
    /////////
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
    void backHandler();
     void showSeasonMainLayer(CCObject *sender);
    void aftergetgiftseason(CCObject *sender);
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
    SGSeasonListLayer();
    ~SGSeasonListLayer();
static    SGSeasonListLayer *create(int count,CCArray *ary);
    void getHandler(SGVipGift* gift);

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


class SeasonItem : public SNSTableViewCellItem
{
private:
    SeasonListItemDelegate *delegate;
//   CCMenu * menu;
    CCSprite *gift;
    CCSprite *kuang;
    CCSprite *ylq;
    SGCCLabelTTF *giftname;
    SGCCLabelTTF *number;
    SGCCLabelTTF * condition;
    CCSize bgSize;
    SGButton *btn;
    int state;
    int giftType;
//    CCMenu *menu;
    
    
public:
    void buttonClick(CCObject*obj);
	virtual bool initWithSize();
	void updateitem(SGVipGift* gift);
	static SeasonItem* create(SeasonListItemDelegate *delegate,int type);
    void btnclick(SGButton *btn);

    
};









#endif /* defined(__GameSanGuo__SGSeasonListLayer__) */
