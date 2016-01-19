/*
 
 此 class已无人使用 废弃。
 
 
 



*/


//
//  SGAdvancingLayer.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-22.
//
//

#ifndef __GameSanGuo__SGAdvancingLayer__
#define __GameSanGuo__SGAdvancingLayer__

#include "SGBaseStuffLayer.h"
#include "SGBaseMilitaryCard.h"
#include "cocos2d.h"
//class SGInfoLayer: public SGBaseStuffLayer
//{
//public:
//    CREATE_FUNC(SGInfoLayer);
//    virtual bool init();
//};

class SGAdvancingLayer : public SGBaseStuffLayer
{
private:
    bool isbreakdown;
    bool isfromfirst;
    int cardType;
    SGBaseMilitaryCard *_card;
    CCSprite *jinbian;
    int beforeitemid;
    int beforestar;
//    SGInfoLayer *infolayer;
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
public:
    SGAdvancingLayer();
    ~SGAdvancingLayer();
    static SGAdvancingLayer *create(int itemid,SGBaseMilitaryCard *card_,int type,int star,bool isfromfirst= false);
private:
    void initView(int itemid);
    void backHandler();
    void showinfo();
    void showequipinfo();
    void action1();
    void action2();
    void action3();
    void action4();
    void action5();
     void action6();
};


#endif /* defined(__GameSanGuo__SGAdvancingLayer__) */
