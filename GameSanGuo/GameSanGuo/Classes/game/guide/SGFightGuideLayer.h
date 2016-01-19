//
//  SGFightGuideLayer.h
//  GameSanGuo
//
//  Created by geng on 13-5-9.
//
//

#ifndef __GameSanGuo__SGFightGuideLayer__
#define __GameSanGuo__SGFightGuideLayer__

#include "cocos2d.h"
#include "SGBattleLayer.h"
#include "SGRichLabel.h"
USING_NS_CC;
class SGFightGuideLayer : public CCLayer
{
public:
    SGFightGuideLayer() : step(0), battleLayer(NULL), label(NULL), bboard(NULL), showNum(0), msgArrs(NULL), actSprite(NULL), spTouch(NULL), spDirect(NULL), guidePopLeft(NULL), guidePopWid(0), tuodong(NULL), blink(false), stopBlink(false), isShowDemo(false), isShowDemoTouch(false)
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
    CCSprite * guidePopLeft; // 文字气泡
    float guidePopWid;
    SGCCLabelTTF * tuodong;//拖动文字

private:
    void nextMsg(bool showResultSprite = true);
    void nextDemoMsg();
    void dealPopVisible( bool popVisible);
protected:
    // default implements are used to call script callback if exist
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void registerWithTouchDispatcher();
public:
    ~SGFightGuideLayer();
    static SGFightGuideLayer *create(SGBattleLayer *battleLayer);
    virtual bool init(SGBattleLayer *bl);
//    void startGuide();
	void setTouch();
	void setNoTouch();
	
    void  initMsg(CCArray *arr,bool showResultSprite = true);
    void initTips(std::vector<int> tips);
    SGBattleLayer *getBattleLayer();
    void setPlace(int place);
    void setBattleLayer(SGBattleLayer *bl);
    
	void creatTouchTipScaling(CCPoint pos);
	
    void creatTouchTipMoving(CCPoint startPos,CCPoint endPos,bool showBoard =false);
    void showTouchTip(int fromId);
    void showDirectTip(int fromId);

    void closeTips( bool selfTouch = false);
    void showTipLine();
    void deleteLine();
    void showFreeTip();
    void deleteFreeTip();
    virtual void draw();
    
    bool blink ;
    bool stopBlink ;
    void updateBlink();
    
    bool isShowDemo;
    bool isShowDemoTouch;
    void creatWujiangDemo(CCArray *arr,bool iswujiangdemo = true);
};
#endif /* defined(__GameSanGuo__SGFightGuideLayer__) */
