//
//  SGAdvanceFinishLayer.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-5-17.
//
//

#ifndef __GameSanGuo__SGAdvanceFinishLayer__
#define __GameSanGuo__SGAdvanceFinishLayer__

#include "cocos2d.h"
#include "SGBaseLayer.h"
#include "SGBaseMilitaryCard.h"
class SGAdvanceFinishLayer : public SGBaseLayer
{
private:
    CCSize s;
    float time;
    int cardType;
    bool isfromfirst;
    CCString *str_kuang;
    SGBaseMilitaryCard *_card;
    CCPoint pos;
    bool cantouch;
protected:
    virtual void onEnter();
    virtual void onExit();
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
public:
    SGAdvanceFinishLayer();
    ~SGAdvanceFinishLayer();
    static SGAdvanceFinishLayer *create(int itemid,SGBaseMilitaryCard *card_,int type,bool isfromfirst= false);
private:
    void initView(int itemid);
    void setActionTime();
    void callback(CCNode *node);
    void callBreakBall();
    void removebreakball();
    void callleihou();
    void ballbomb(CCNode *node);
    void shownewcard();
    void backHandler();
};

#endif /* defined(__GameSanGuo__SGAdvanceFinishLayer__) */
