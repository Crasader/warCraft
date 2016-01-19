//
//  SGBarracksLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-26.
//
//

#ifndef __GameSanGuo__SGBarracksLayer__
#define __GameSanGuo__SGBarracksLayer__

#include "SGBaseLayer.h"
#include "SGBaseTableLayer.h"
#include "SNSTableView.h"
#include "cocos2d.h"
#include "SNSTableViewCellItem.h"
#include "SGCCLabelTTF.h"


///////////////////////
class LabelItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
/////////////////////
class SGBarracksLayer : public SGBaseTableLayer ,public LabelItemDelegate
{
private:
    CCString *str_ofr;
    CCString *str_eqp;    
    CCString *str_soul;
    CCString *str_prop;
    CCString *str_skill;
	CCString *str_pieces;//碎片
	CCString *str_consume;//道具
    CCString *str_material;//材料
    int itemNum;
private:
    void initView();
    /////////
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
//    virtual void registerWithTouchDispatcher();
//    virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
//    virtual void onEnter();
//    virtual void onExit();

    ///////
protected:
//    virtual void setIsCanTouch(bool isTouch);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 返回tableView的cell数
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    /*SNSTableViewDelegate*/
    // 定义tableView每一行或列的高度
    virtual float tableViewHeightForRow(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath){};
	// 返回tableView的列数
	virtual unsigned int tableViewColumnsOfItemsNumber(SNSTableView* tableView, int section); 

public:
    SGBarracksLayer();
    ~SGBarracksLayer();
static    SGBarracksLayer *create();

//    CCMenuItemImage *battleitem;
    void teamformationHandler();
    void generalHandler();
    void soldierHandler();
    void equipmentHandler();
    void propHandler();
    void skillHandler();
    void expandHandler();
	void consumeHandler();//道具
	void piecesHandler();//碎片
    void materialHandler();//材料
};


class LabelItem : public SNSTableViewCellItem
{
public:
    LabelItem() : label(NULL), delegate(NULL),tableview(NULL),menu(NULL),redSpot_(NULL)
    {
    }
    //本体文本，即该item标题。
    SGCCLabelTTF* title;
    //附加文本，参见背包里的“武将（20）"，即"（20）"
    SGCCLabelTTF *label;
    LabelItemDelegate *delegate;
    SNSTableView *tableview;
    CCMenu * menu;
    //条目的红点提示，通用，默认隐藏。
    CCSprite * redSpot_;
public:
    void buttonClick(CCObject*obj);
	virtual bool initWithSize(CCSize size,int num,bool isshow );
	
	static LabelItem* create(CCSize size,int num,bool isshow = false,LabelItemDelegate*del=NULL,SNSTableView*table=NULL);
    
    //设置红点的显隐。
    void setRedSpot(bool bl);

    /////
//   virtual void registerWithTouchDispatcher();
//  
//    virtual void onEnter();
};









#endif /* defined(__GameSanGuo__SGBarracksLayer__) */
