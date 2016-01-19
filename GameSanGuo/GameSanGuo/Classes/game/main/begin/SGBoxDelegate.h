//
//  SGBoxDelegate.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-4.
//
//

#ifndef __GameSanGuo__SGBoxDelegate__
#define __GameSanGuo__SGBoxDelegate__
#include "cocos2d.h"
USING_NS_CC;
class SGBoxDelegate
{
public:
    bool isLock;
public:
    virtual void showBoxCall(CCObject *obj){};
    virtual void setIsCanTouch(bool isTouch){};
    virtual void setIsLock(bool isLock){};
};

#endif /* defined(__GameSanGuo__SGBoxDeletage__) */
