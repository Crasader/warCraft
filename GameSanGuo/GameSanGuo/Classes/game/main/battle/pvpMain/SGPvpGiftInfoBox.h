//
//  SGPvpGiftInfoBox.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-11.
//
//

#ifndef __GameSanGuo__SGPvpGiftInfoBox__
#define __GameSanGuo__SGPvpGiftInfoBox__

#include "SGBaseTableBox.h"
#include "SGGift.h"
#include "SGVipGifts.h"

class SGPvpGiftInfoBox : public SGBaseTableBox
{
private:
    CCArray *_array;
    SGVipGift *_buyGift;
    SGBoxDelegate *_delegate;
    int _type;
public:
    SGPvpGiftInfoBox();
    ~SGPvpGiftInfoBox();
    static SGPvpGiftInfoBox *create(SGBoxDelegate *delegate,SGVipGift *buyGift,int type);
    void initView(SGVipGift *buyGift);
protected:
    // 根据indexPath返回tableView的cell
    virtual SNSTableViewCellItem* tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
    // 每个item的点击事件
    virtual void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
    virtual void boxCloseWithOutCallBack();
    void buyHandler();
};
#pragma mark Sell_Item

class pvpGift_Item : public SNSTableViewCellItem
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
    pvpGift_Item();
    ~pvpGift_Item();
    
public:
	void initView();
	static pvpGift_Item* create(SGGift *gift);
    void updategift(SGGift *gift,int index);
};
#endif /* defined(__GameSanGuo__SGPvpGiftInfoBox__) */
