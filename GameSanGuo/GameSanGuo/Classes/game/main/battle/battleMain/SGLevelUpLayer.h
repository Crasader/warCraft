//
//  SGLevelUpLayer.h
//  GameSanGuo
//
//  Created by lizi on 13-5-22.
//
//

#ifndef __GameSanGuo__SGUpdateLayer__
#define __GameSanGuo__SGUpdateLayer__

#include "SGBaseLayer.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

class SGLevelUpLayer : public SGBaseLayer
{
public:
    int _type;
    int _index;
    bool isFirstTouch;
	CCSprite *bgLevel;
	CCSprite *eff;
	CCScale9Sprite *frame;
	CCSprite *spCon;
    
public:
    SGLevelUpLayer(void);
    ~SGLevelUpLayer(void);
    
    static SGLevelUpLayer* create();
    
private:
    void initView();
    void confirmAction();   // 确定Action
    void showCreamSceneLayer(CCObject *sender);
	
	void setLevel(CCNode *n);//mm从屏幕左侧移入
	void setEff(CCNode *n);
	void setFrame(CCNode *n);
	void spConRunaction(CCNode *n);
	void getFriendOfficerListener(CCObject *sender);
    void showSceneLayer(CCObject * sender);
protected:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__GameSanGuo__SGUpdateLayer__) */
