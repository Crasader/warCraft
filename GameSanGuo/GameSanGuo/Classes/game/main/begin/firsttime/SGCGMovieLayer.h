//
//  SGCGMovieLayer.h
//  GameSanGuo
// 选择国家界面
//  Created by zenghui on 13-12-9.
//
//

#ifndef __GameSanGuo__SGCGMovieLayer__
#define __GameSanGuo__SGCGMovieLayer__

#include "SGBaseLayer.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class SGCGMovieLayer : public SGBaseLayer
{
public:
    SGCGMovieLayer();
    ~SGCGMovieLayer();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
    static SGCGMovieLayer *create();
private:
    void constructView();
    void cgSkipToBattle();
    void moveCgMsgUp();
    void cgBattleListener(cocos2d::CCObject *obj);
    void cdkListener(cocos2d::CCObject *obj);
    void btnclick(SGButton *sender);
    void btnclickcancel(SGButton *sender);

    SNSScrollView *cgScrollView;
    CCRect scrollRect;
    float posY;
    int moveCgMsgDis;
    CCSize s;
    CCSprite *finaleSprite;
    CCEditBox *m_cdk;
    CCSprite *box;
    CCSprite *title;
    SGButton *btn;
    SGButton *btn2;
};
#endif /* defined(__GameSanGuo__SGCGMovieLayer__) */
