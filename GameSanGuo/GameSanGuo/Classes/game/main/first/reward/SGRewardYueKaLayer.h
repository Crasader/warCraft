//
//  SGRewardYueKaLayer.h
//  GameSanGuo
//
//  Created by zenghui on 14-2-11.
//
//

#ifndef __GameSanGuo__SGRewardYueKaLayer__
#define __GameSanGuo__SGRewardYueKaLayer__

#include "SGBaseTableLayer.h"
class SGYueKaDelegate;
class YueKa_Item : public SNSTableViewCellItem
{
private:
    SGYueKaDelegate *delegate;       
public:    
    int state; 
    int gold;
    int type;
    int days;
    CCSprite *item;
    SGButton *btn;
private:
   
public:
    YueKa_Item();
    ~YueKa_Item();
public:
     void getGoldHandler(CCNode *sender);
	void setView(int station);
	static YueKa_Item* create(SGYueKaDelegate *dlg,int gold,int station,int type,int days);
    void updateitem(int station);
};

class SGYueKaDelegate
{
public:
    virtual void getGoldHandler(YueKa_Item *LvlGold_Item){};
};

class SGRewardYueKaLayer : public SGBaseLayer ,public SGYueKaDelegate
{
private:
    YueKa_Item *lvlupItem;
    CCArray *_array;
    void initView();
    void backHandler();
    void yueKaRewardListener(cocos2d::CCObject *obj);
public:
    virtual void getGoldHandler(YueKa_Item *LvlGold_Item);
    SGRewardYueKaLayer();
    ~SGRewardYueKaLayer();
    static SGRewardYueKaLayer *create(CCArray *array);
};

#endif /* defined(__GameSanGuo__SGRewardYueKaLayer__) */
