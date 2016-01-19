//
//  SGScrollViewSubview.h
//  FontLabel
//
//  Created by 江南岸 on 13-3-15.
//
//


/*
 *  作为CCScrollView 的subview使用
 */


#ifndef __FontLabel__SGScrollViewSubview__
#define __FontLabel__SGScrollViewSubview__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
using namespace cocos2d;

class SGScrollViewSubview :public CCLayerColor {
    
private:
    
    CCSize _subviewSize;    //subview 的尺寸
    
    bool _isInLayer;        //点击的点是否在subview内部
    
    bool _isMove;           //是否移动了（一般情况下是滑动）
    
    CCPoint _position;      //位置
    
    
public:
    SGScrollViewSubview();
    
    ~SGScrollViewSubview();
    
    static SGScrollViewSubview *create(CCPoint position,CCSize subviewSize);
    
    bool init(CCPoint position, CCSize subviewSize);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    
    virtual void onEnter();
    
    virtual void onExit();
    
    virtual void selectSubview(){};
    
};

#endif /* defined(__FontLabel__SGScrollViewSubview__) */
