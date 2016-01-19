//
//  SGMenu.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-29.
//
//

#ifndef __GameSanGuo__SGMenu__
#define __GameSanGuo__SGMenu__
#include "cocos2d.h"
USING_NS_CC;
class SGMenu : public CCMenu
{
public:
    SGMenu() : _rect(CCRectZero), _canTouch(false)
    {};
    
private:
    CCRect _rect;
    bool _canTouch;
    
public:
        
    static SGMenu* create(CCRect rect);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void ccTouchEnded(CCTouch *touch, CCEvent* event);
    
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
    
};

#endif /* defined(__GameSanGuo__SGMenu__) */
