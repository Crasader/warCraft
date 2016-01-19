//
//  SGFourGuideLayer.h
//  GameSanGuo
//
//  Created by lizi on 13-6-26.
//
//

#ifndef __GameSanGuo__SGFourGuideLayer__
#define __GameSanGuo__SGFourGuideLayer__

#include "SGStorySpeakerLayer.h"
#include "SGBattleLayer.h"

class SGFourGuideLayer : public SGStorySpeakerLayer
{
private:
    CCSprite *spLabel;
    SGBattleLayer *battleLayer;
    
public:
    SGFourGuideLayer(void);
    ~SGFourGuideLayer(void);
    
    static SGFourGuideLayer* create();
    void buttonAction(CCObject *sender);
    
    void setFguide();       // 提示四格武将教学
    void showJoinBtn();     // 显示参加按钮
    void showPKwords(CCString *word, SGBattleLayer *layer);     // 显示战斗PK
    
protected:
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

class SGFightDescLayer : public SGBaseLayer
{
private:
    CCSprite *bboard;
    CCLabelTTF *label;
    SGBattleLayer *battleLayer;
    
public:
    SGFightDescLayer(void);
    ~SGFightDescLayer(void);
    
    static SGFightDescLayer* create();
    void initView();
    void showPKwords(CCString *word, SGBattleLayer *layer);// 显示战斗PK


	
protected:
    virtual void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__GameSanGuo__SGFourGuideLayer__) */
