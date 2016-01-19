//
//  SGVipGiftLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-13.
//
//

#ifndef __GameSanGuo__SGVipGiftLayer__
#define __GameSanGuo__SGVipGiftLayer__

#include "SGBaseTableLayer.h"
#include "SGVipGifts.h"
#include "SGGift.h"
#include "SGCCLabelTTF.h"
#include "PlatformAdp.h"

class SGBuyDelegate;
class SGVipGiftItem : public SNSTableViewCellItem
{
private:
    SGButton *store_k;
    SGVipGift *_gift;
    CCMenu *menu1;
    SGCCLabelTTF *_yuan;
    SGCCLabelTTF *_te;
    
    SGBuyDelegate *delegate;
    CCSprite *vipLevel;
    CCSprite* itembg;
#if (PLATFORM == IOS)
    //CCSprite *info;
    CCSprite *vip1;
#else
    //SGCCLabelTTF *info;
    SGCCLabelTTF *vip1;
#endif
    CCSprite *vipTiao;
    int _playerLev;
    bool _haveGot;
private:
    void showGiftInfo();
    void buyHandler();
    CCPoint infoPos;
    void setVipBuyTag(int num,int need);
    void setInfoLabel(int num);
    float getSGLabelHeight(CCSprite *sgRichLabel);
    void createSubLine();
public:
    void updateInfo(SGVipGift *gift,int index,int num,int playerlvl,bool haveGot,int need);
    SGVipGiftItem();
    ~SGVipGiftItem();
    SGVipGift *getGift();
	virtual bool initWithSize(int num,SGVipGift *gift,int need);
	static SGVipGiftItem* create(SGBuyDelegate *delegate, int num,SGVipGift *gift,int playerlvl,bool haveGot,int need);
};
class SGBuyDelegate
{
public:
    virtual void buyHandler(SGVipGift *giftItem){};
    virtual void showInfoHandler(SGVipGiftItem *giftItem){};
};

class SGVipGiftLayer : public SGBaseTableLayer,public SGBuyDelegate
{
private:
    int currVipLvl;
    SGVipGift *_buyGift;
    CCArray *vipGifts;
    CCArray *giftArray;
    bool _touch;
    CCProgressTimer *expprog;
    SGCCLabelTTF *label1;
    CCLabelTTF *label2;
    SGCCLabelTTF *label_exp;
    CCSprite *fontvip;
    CCLabelAtlas *currvip;
    int _need;
    int _curr;
    int _max;
private:
    void addGift(SGGift *gift);
    void initView();
    void backHandler();    
    void giftBuyListener(CCObject *obj);
    void showVipInfo();
    //整合到mainLayer中
//    void storeEnterListener(CCObject *obj);
public:
    SGVipGiftLayer();
    ~SGVipGiftLayer();
    virtual void showInfoHandler(SGVipGiftItem *giftItem);
    virtual void buyHandler(SGVipGift *giftItem);
    
    static SGVipGiftLayer *create(int lvl, CCArray *_array,int curr,int max );
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    virtual void setIsCanTouch(bool isTouch);
};



#endif /* defined(__GameSanGuo__SGVipGiftLayer__) */
