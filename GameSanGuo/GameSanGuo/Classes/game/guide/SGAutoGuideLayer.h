//
//  SGAutoGuideLayer.h
//  GameSanGuo
//
//  Created by pengyou on 13-9-18.
//
//

#ifndef __GameSanGuo__SGAutoGuideLayer__
#define __GameSanGuo__SGAutoGuideLayer__

#include "cocos2d.h"
#include "SGBattleLayer.h"
#include "SGRichLabel.h"
USING_NS_CC;
class SGAutoGuideLayer : public CCLayer
{
public:
    SGAutoGuideLayer() : step(0), battleLayer(NULL), label(NULL), bboard(NULL), showNum(0), msgArrs(NULL), actSprite(NULL), spTouch(NULL), spDirect(NULL), labelHight(0), cgScrollView(NULL), posY(0), moveCgMsgDis(0), size(CCSizeZero), finaleUpSprite(NULL)
    {};
private:
    int step;
    SGBattleLayer *battleLayer;
    CCSprite *label;
    CCSprite *bboard;
    int showNum;
    CCArray *msgArrs;
    CCSprite *actSprite;
    std::vector<int> msgTips;
    
    CCSpriterX *spTouch;  //按钮指引
    CCSprite *spDirect; //箭头指向
    float labelHight;
    SNSScrollView *cgScrollView;
    float posY;
    int moveCgMsgDis;
    CCSize size;
    CCSprite *finaleUpSprite;
private:
    void nextMsg();
    void showFinalMsg();
    void moveCgMsgUp();
    void autoGuideToCountry();
protected:
    // default implements are used to call script callback if exist
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
public:
    ~SGAutoGuideLayer();
    static SGAutoGuideLayer *create(SGBattleLayer *battleLayer);
    virtual bool init(SGBattleLayer *bl);
    //    void startGuide();
    
    void  initMsg(CCArray *arr);
    void initTips(std::vector<int> tips);
    SGBattleLayer *getBattleLayer();
    void setPlace(int place);
    void setBattleLayer(SGBattleLayer *bl);
    void recordStepListener();
    void showTouchTip(int fromId);
    void showDirectTip(int fromId);
    
    void closeTips();
    void initFinalMsg(CCArray *arr);
    
    void autoAttackSkill(SGBattleMap *bm);
};

#endif /* defined(__GameSanGuo__SGAutoGuideLayer__) */
