//
//  SGShopLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-13.
//

/*
 *注：此class只负责显示商店列表条目，具体商店内容除vip礼包和储值外 皆在malllayer class中。
 *
 *
 */

#ifndef __GameSanGuo__SGShopLayer__
#define __GameSanGuo__SGShopLayer__

#include "SGBaseTableLayer.h"
#include "SGVipGifts.h"
#include "SGMallLayer.h"
///////////////////////
class SGShopItemItemDelegate
{
public:
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath){};
};
/////////////////////
class SGShopLayer : public SGBaseTableLayer,public SGShopItemItemDelegate
{
private:
    void initView();
    CCArray *_vipGifts;
    int vipLvl;
    int nGuiShopingVipLimit;//鬼市vip等级开启限制
private:
    void addGifts(SGVipGift *gift);
    void vipGiftBagListener(CCObject *obj);
    //整合到mainLayer中
//    void storeEnterListener(CCObject *obj);
    void exchangeEnterListener(CCObject *obj);
    void showChgScoreLayer(CCObject *obj);
    /////
    virtual void itemselect(SNSTableView* tableView,SNSIndexPath* indexpath);
   // void entryGoldShopListener(CCObject *sender);
public:
    SGShopLayer();
    ~SGShopLayer();
    static SGShopLayer *create();
    void showStoreLayer();
    void showGiftsLayer();
    void showExchangeLayer();
	void showTodyspPriceLayer();//今日特价界面

protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    
    
    
    //by:zyc 调整商城入口 BEGIN.
public:
    //云游商人  。。。拼音   怎么起名字。。。
    void onYunYouShangRenClicked();
    
    //洛阳鬼市 名字。。。拼音
    void onLuoYangGuiShiClicked();
    
    //天梯商城
    void onPvpClicked();
    
    //荆州军需处
    void onJinZhouClicked();
    
    //幻境商城
    void onFairylandClicked();
    
    //乱世商人（竞技场）
    void onArenaClicked();
    
    
    //点击具体按钮后 发送消息
    void sendShopMsg(ShopGUID shopId);
    
    
    //by:zyc 调整商城入口 end

};

class SGShopItem : public SNSTableViewCellItem
{
public:
    SGShopItem();
    ~SGShopItem();
    ////////////
    SGShopItemItemDelegate *delegate;
    SNSTableView *tableview;
    void buttonClick(CCObject*obj);
    CCMenu* menu;
    //////////////////////
	static SGShopItem* create(CCSize size,int num,bool isshow = false,SGShopItemItemDelegate*del=NULL,SNSTableView*table=NULL);
    
	virtual bool initWithSize(int num);
	//static SGShopItem* create(int num);
};

#endif /* defined(__GameSanGuo__SGShopLayer__) */
