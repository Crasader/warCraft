//
//  SGLandingReward.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-8.
//
//

#ifndef __GameSanGuo__SGLandingReward__
#define __GameSanGuo__SGLandingReward__

#include "SGBaseLayer.h"
#include "SGBoxDelegate.h"

class SGLandingReward : public SGBaseLayer
{
private:
    int state;
    SGButton *uncard;
    SGButton *enterGame;
    int currChance;
    int btntag;
    int chanceTag[9];
    int tempTag;
    int logindays;
    int chances;
    SGCCLabelTTF *label1;
    CCArray *tagArray;
    
    SGButton *tempBtn;
    CCSprite *tempBg;
    
    SGButton *AllBtn;

    void testFlipCardCallFunc1();
    void testFlipCardCallFunc2();
    
    void testFlipCardCallFunc3(CCNode *node);
    void testFlipCardCallFunc4(CCNode *node);
    void testeffect(CCNode *node);
    void sortLandingButton();
     
protected:
    SGBoxDelegate *deletage;
protected:
    void initView(SGBoxDelegate *dg);
    void confirmHandler(CCNode *sender);
    void lotteryListener(CCObject *obj);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();

public:
    SGLandingReward();
    ~SGLandingReward();
    static SGLandingReward *create(SGBoxDelegate *dg);
    void showCard();
    void boxCloseWithOutCallBack();
    
};


#endif /* defined(__GameSanGuo__SGLandingReward__) */
