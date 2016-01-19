//
//  SGpvpEverydayLayer.h
//  GameSanGuo
//
//  Created by dongboqian on 14-2-12.
//
//

#ifndef __GameSanGuo__SGpvpEverydayLayer__
#define __GameSanGuo__SGpvpEverydayLayer__

#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "CCMenu.h"

using namespace cocos2d::extension;

class SGpvpEverydayLayer : public SGBaseLayer{
private:
    void backHandler();
    void btnclick(SGButton *btn);
    void setgiftsbtns();
    void setView();
    void showSeasonMainLayer(CCObject *sender);
    void aftergetgift(CCObject *sender);
    virtual bool init(const char *fileName_, int tag_);

public:
    SGpvpEverydayLayer();
    ~SGpvpEverydayLayer();
    void setIsCanTouch(bool isTouch);
    static SGpvpEverydayLayer *create(int count,CCArray* arrary);
    virtual void initView(CCArray *ary);
    void getHandle(SGVipGift* gift);
private:
    CCProgressTimer* progress;
    CCSprite * icon1;
    CCSprite * icon2;
    CCSprite *icon3;
    SGButton *btn1;
    SGButton *btn2;
    SGButton *btn3;
    CCArray *_gifts;
    CCMenu *mm;
    CCSprite * bgbox;
    int _count;

   };
#endif /* defined(__GameSanGuo__SGpvpEverydayLayer__) */
