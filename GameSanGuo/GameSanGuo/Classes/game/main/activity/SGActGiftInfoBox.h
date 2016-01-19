//
//  SGActGiftInfoBox.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//

#ifndef __GameSanGuo__SGActGiftInfoBox__
#define __GameSanGuo__SGActGiftInfoBox__

#include "SGBaseTableBox.h"
#include "SGGift.h"
#include "SGVipGifts.h"
#define   FIRSTCHARGESHOWTAG 111
class SGActGiftInfoBox : public SGBaseTableBox
{
private:
    CCArray *_array;
    SGVipGift *_buyGift;
    SGBoxDelegate *_delegate;
    int _type;
public:
    SGActGiftInfoBox();
    ~SGActGiftInfoBox();
    static SGActGiftInfoBox *create(SGBoxDelegate *delegate,SGVipGift *buyGift,int type,CCString *name , CCString*info);
    void initView(SGVipGift *buyGift,CCString *name , CCString*info);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    virtual void boxCloseWithOutCallBack();
    void buyHandler();
    virtual void onEnter();
    void registerWithTouchDispatcher();
};
#pragma mark Sell_Item

class actGift_Item : public SNSTableViewCellItem
{
private:
    SGGift *_gift;
    CCSprite *spritePlayerHead;
    CCLabelTTF *labelName;
    CCLabelTTF *num;
    CCArray *arrayStar;
    CCSprite * kuang;
    

private:
    void showPlayerStar();
    
public:
    actGift_Item();
    ~actGift_Item();
    
public:
	void initView();
	static actGift_Item* create(SGGift *gift);
    void updategift(SGGift *gift,int index);
};
#endif /* defined(__GameSanGuo__SGActGiftInfoBox__) */
