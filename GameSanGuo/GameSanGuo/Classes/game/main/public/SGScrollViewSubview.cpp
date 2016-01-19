//
//  SGScrollViewSubview.cpp
//  FontLabel
//
//  Created by 江南岸 on 13-3-15.
//
//

#include "SGScrollViewSubview.h"
SGScrollViewSubview::SGScrollViewSubview()
{
    _isInLayer = false;
    _isMove = false;
}

SGScrollViewSubview::~SGScrollViewSubview()
{
    
}
SGScrollViewSubview *SGScrollViewSubview::create(CCPoint position,CCSize subviewSize)
{
    SGScrollViewSubview *subview = new SGScrollViewSubview();
    
    if(subview && subview->init(position, subviewSize))
    {
        subview->autorelease();
        return subview;
    }
    
    CC_SAFE_DELETE(subview);
    return NULL;
}

//初始化方法
bool SGScrollViewSubview::init(CCPoint position, CCSize subviewSize)
{
 
    _subviewSize = subviewSize;
    
    _position = position;
    
    if(!CCLayerColor::initWithColor(ccc4(255, 255, 255, 0), subviewSize.width , subviewSize.height))
    {
        return false;
    }
    
    return true;
    
}

bool SGScrollViewSubview::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    //从别的地方找来的算法，挺好用，计算坐标点
    
    CCPoint sLocalPos = convertToNodeSpace(pTouch->getLocation());
    
    CCRect sRC = CCRect(_position.x - getContentSize().width * getAnchorPoint().x,
                        _position.y - getContentSize().height * getAnchorPoint().y,
                        _subviewSize.width, _subviewSize.height);

    
    sRC.origin = CCPointZero;
    
    //判断点击点是否在subview内部
    
    bool isTouched = sRC.containsPoint(sLocalPos);
    
    _isInLayer = isTouched;
    
    _isMove = false;
    
    return true;

}

void SGScrollViewSubview::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    _isMove = true;     //如果滑动了，把_isMove 设为true
}

void SGScrollViewSubview::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(_isInLayer)
    {
        if(!_isMove)
        {
            //调用
            
            printf("点击 \n");
            
            selectSubview();
            
        }
    }
}

void SGScrollViewSubview::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX+1, false);
    CCLayerColor::onEnter();
}

void SGScrollViewSubview::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCLayerColor::onExit();
}

