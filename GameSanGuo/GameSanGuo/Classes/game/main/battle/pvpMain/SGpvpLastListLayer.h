//
//  SGpvpLastListLayer.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//

#ifndef __GameSanGuo__SGpvpLastListLayer__
#define __GameSanGuo__SGpvpLastListLayer__

#include "SGBaseLayer.h"
#include "SNSTableView.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGCCLabelTTF.h"
#include "SGBaseTableLayer.h"
#include "SGJunbuff.h"
#include "SGRankAllPlayer.h"


///////////////////////
//class SeasonListItemDelegate
//{
//public:
//    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
//};
/////////////////////
class SGpvpLastListLayer : public SGBaseTableLayer
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
    virtual void initView(int count);
    virtual void initMsg();
    /////////
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
    void backHandler();
    void showSeasonMainLayer(CCObject *sender);
    void showEmbattleInfoLayer(CCObject *sender);
//   void receivedetail(CCObject *sender);
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
    SGpvpLastListLayer();
    ~SGpvpLastListLayer();
static    SGpvpLastListLayer *create(int count,CCArray *ary);

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


class LastlistItem : public SNSTableViewCellItem
{
private:
//    SeasonListItemDelegate *delegate;
//   CCMenu * menu;
    CCSprite *countryicon;
    CCSprite *kuang;
    CCSprite *jiangzhang;
     CCSprite *namebg;
    CCLabelTTF *name;
    CCLabelTTF *number;
    CCLabelTTF *servicename;
    CCSize bgSize;
    SGButton *btn;
    int state;
    CCMenu *menu;
    SGRankPlayer *_player;
    
public:
//    void buttonClick(CCObject*obj);
    LastlistItem();
	virtual bool initWithSize();
	void updateitem(SGRankPlayer* jun);
	static LastlistItem* create(SGRankPlayer *player);
    void btnclick(SGButton *btn);
    /////
//   virtual void registerWithTouchDispatcher();
//  
//    virtual void onEnter();
};









#endif /* defined(__GameSanGuo__SGpvpLastListLayer__) */
