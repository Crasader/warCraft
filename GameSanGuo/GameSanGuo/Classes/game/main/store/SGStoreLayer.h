//
//  SGStoreLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-15.
//
//

#ifndef __GameSanGuo__SGStoreLayer__
#define __GameSanGuo__SGStoreLayer__

#include "SGBaseTableLayer.h"
#include "SGVipGifts.h"
#include "SGStoreItem.h"
class SGStoreLayer : public SGBaseTableLayer,public SGStoreItemDelegate
{
private:
    int enterType;
    CCSprite *fontvip;
    CCProgressTimer *expprog;
    int vipLevel;
    SGCCLabelTTF *label1;
    SGCCLabelTTF *label2;
    CCLabelAtlas *currvip;
    SGCCLabelTTF *label_exp;
    SGButton *special;
    CCSprite *redbg;
    CCArray *_vipGifts;
    //GameIapStore *iap;	外围调用
    CCArray *_item;
    CCArray *_strary;
    std::string userId;
    long _time;
    int ranNum;
private:
    void showLayerByTag(LayerTag layerTag);
    void initView(int curr,int next,int currrmb,int maxrmb,CCArray *ary);
    void backHandler();
    void buyGoldListener(CCObject *obj);
    void exchangGoldListener(CCObject *obj);
    void showExchangeLayer();
    void showGiftsLayer();
//    void showVipInfo();
    void vipGiftBagListener(cocos2d::CCObject *obj);
    void addGifts(SGVipGift *gift);
	void freshMonthCard(int days);
    ////////
    virtual void itemselect(SNSTableView* tableView, SNSIndexPath* indexPath);
    ////////
protected:
    void setIsCanTouch(bool isTouch);
    
public:
    SGStoreLayer();
    ~SGStoreLayer();
    static SGStoreLayer *create(int curr,int next,int currrmb,int maxrmb,CCArray *a,int type,CCArray *strary);
    void reSetData(CCArray *a,CCArray *strary);//重设数据刷新条目
    
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
};
#endif /* defined(__GameSanGuo__SGStoreLayer__) */
