//
//  SGGiftInfoBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-14.
//
//

#ifndef __GameSanGuo__SGGiftInfoBox__
#define __GameSanGuo__SGGiftInfoBox__

#include "SGBaseTableBox.h"
#include "SGGift.h"
#include "SGVipGifts.h"

class SGGiftInfoBox : public SGBaseTableBox
{
private:
    CCArray *_array;
    SGVipGift *_buyGift;
    SGBoxDelegate *_delegate;
   public:
    SGGiftInfoBox();
    ~SGGiftInfoBox();
    static SGGiftInfoBox *create(SGBoxDelegate *delegate,SGVipGift *buyGift);
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

class Gift_Item : public SNSTableViewCellItem
{
private:
    SGGift *_gift;
    CCSprite *spritePlayerHead;
    CCLabelTTF *labelName;
    CCLabelTTF *num;
//    CCArray *arrayStar;
    CCSprite * kuang;
    CCSprite *advnacenum;
    CCSprite *bj;
    LayerTag m_tag;

private:
    void showPlayerStar();
    
public:
    Gift_Item();
    ~Gift_Item();
    
public:
	void initView();
	static Gift_Item* create(SGGift *gift, LayerTag _tag);
    void updategift(SGGift *gift,int index);
};
#endif /* defined(__GameSanGuo__SGGiftInfoBox__) */
