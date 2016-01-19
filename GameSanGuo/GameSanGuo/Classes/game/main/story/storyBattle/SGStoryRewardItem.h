//
//  SGStoryRewardItem.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-7.
//
//

#ifndef __GameSanGuo__SGStoryRewardItem__
#define __GameSanGuo__SGStoryRewardItem__

#include "SNSTableViewCellItem.h"
#include <iostream>
#include "cocos-ext.h"
#include "SGButton.h"
#include "SGOfficerCard.h"
#include "SGScrollViewSubview.h"
#include "SGBaseShow.h"
#include "SGBaseLayer.h"
// 掉落的道具卡View
class SGStoryRewardItem : public SNSTableViewCellItem
{
public:
    SGStoryRewardItem(void);
    ~SGStoryRewardItem(void);
    
	static SGStoryRewardItem* create(int soulNum= 0,int coin = 0, LayerTag _tag = sg_LayerNull);
    void initView();
    void setData(int iId, int iLevel, int iType, int num = 0,int index=0);
    void setDataWithArray(CCArray * array,int ntype,int ncount);
    int soulNum;
    
private:
   // CCSprite *spriteIcon;
    //CCSprite *spriteIconBg;
	//CCSprite *sprItemCount;
	
    int _itemId;
    int _itemLev;
    int _itemType;
    int m_nCoin ;
    LayerTag m_tag;
};

// 掉落的道具卡Model
class SGStoryRdItem : public CCObject
{
public:
    SGStoryRdItem(void);
    ~SGStoryRdItem(void);
    
    static SGStoryRdItem* create(int iId, int iTag, int iType);
    void initWithInfo(int iId, int iTag, int iType);
    
    CC_SYNTHESIZE_READONLY(int, _itemId, ItemId); // icon id
    CC_SYNTHESIZE_READONLY(int, _itemTag, ItemTag); // iconbg id
    CC_SYNTHESIZE_READONLY(int, _itemType, ItemType); // icon kind
    int m_nCount;//物品的计数
    void addCount(){
        m_nCount++;
    }
    void setCount(int ncount)
    {
        m_nCount = ncount;
    }
    int getCount()
    {
        return m_nCount;
    }
};


using namespace cocos2d;
using namespace cocos2d::extension;

class SGRewardFirstItem :public SNSTableViewCellItem{
public:
    SGRewardFirstItem();
    ~SGRewardFirstItem();
    static SGRewardFirstItem *create();

    
};
#endif /* defined(__GameSanGuo__SGStoryRewardItem__) */
