//
//  SGStorySpeakerLayer.h
//  GameSanGuo
//
//  Created by lizi on 13-5-19.
//
//

#ifndef __GameSanGuo__SGStorySpeakerLayer__
#define __GameSanGuo__SGStorySpeakerLayer__

#include "SGBaseLayer.h"
#include "SGBoxDelegate.h"


class SGStorySpeakerLayer : public SGBaseLayer
{
public:
    int _wordIndex;
    CCArray *_datas;
    SGBoxDelegate *_delegateTouch;
    
    CCLabelTTF *label;
    CCSprite *speaker;
    CCSprite *bboard;
    CCSprite *spriteBlackbg;
    
public:
    SGStorySpeakerLayer(void);
    ~SGStorySpeakerLayer(void);
    
    static SGStorySpeakerLayer* create();
    void registerWithTouchDispatcher();
    void initView();
    void setData(int speakerId);
    
protected:
    // default implements are used to call script callback if exist
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};

#endif /* defined(__GameSanGuo__SGStorySpeakerLayer__) */
