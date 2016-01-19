//
//  SGConnectTipBox.h
//  GameSanGuo
//
//  Created by wwl on 13-12-18.
//
//

#ifndef __GameSanGuo__SGConnectTipBox__
#define __GameSanGuo__SGConnectTipBox__

#include <iostream>
#include "cocos2d.h"
#include "SGBaseBox.h"

USING_NS_CC;

class SGConnectTipBox: public SGBaseBox{
    
public:
    SGConnectTipBox();
    ~SGConnectTipBox();
    static SGConnectTipBox *create(SGBoxDelegate *target, SGNetworkErrorType tp);
    virtual void initMsg();
    const char* getTipName(SGNetworkErrorType tp);
    
    void buttonOk();
    void buttonClose();
    
    void registerWithTouchDispatcher();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
public:
    SGNetworkErrorType tipType;
};

#endif /* defined(__GameSanGuo__SGConnectTipBox__) */
