//
//  SGSellBox.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-27.
//
//

#ifndef __GameSanGuo__SGSellBox__
#define __GameSanGuo__SGSellBox__

#include "SGBaseMilitaryCard.h"
#include "SGBaseBox.h"
#include "SNSTableViewCellItem.h"
class SGSellBox : public SGBaseBox
{
private:
    int totlesell;
    SGBaseMilitaryCard *_card;
    int enterType;
    CCArray *datas;
private:
    void confirmHandler(); 
    void showGenerInfo();
    void sellListener(CCObject *obj);
    void boxClose();
    
public:
    SGSellBox();
    ~SGSellBox();
    static SGSellBox *create(SGBoxDelegate *delegate,SGBaseMilitaryCard *card,int type);
    void initView(SGBoxDelegate *delegate);
};
#pragma mark Sell_Item

class Sell_Item : public SNSTableViewCellItem
{
private:
    SGBaseMilitaryCard *_card;
    CCSprite *spritePlayerHead;
    int sellCoins;
private:
    void showPlayerStar();    
    
public:
    Sell_Item();
    ~Sell_Item();
    
public:
	void initView();
	static Sell_Item* create(SGBaseMilitaryCard *card);
};

#endif /* defined(__GameSanGuo__SGSellBox__) */
