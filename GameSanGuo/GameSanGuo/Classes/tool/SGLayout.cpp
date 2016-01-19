//
//  SGLayout.cpp
//  TestSG
//
//  Created by gengshengcai on 12-12-1.
//
//

#include "SGLayout.h"


CCPoint SGLayout::getPoint(sgLayoutType layout)
{
    switch (layout) {
        case kUpLeft:
        {
            return ccp(0, screenheight);
        }
            break;
        case kUpCenter:
        {
            return ccp(screenwidth * .5, screenheight);
        }
            break;
        case kUpRight:
        {
            return ccp(screenwidth, screenheight);
        }
            break;
        case kMiddleLeft:
        {
            return ccp(0, screenheight * .5);
        }
            break;
        case kMiddleCenter:
        {
            return ccp(screenwidth * .5, screenheight * .5);
        }
            break;
        case KMiddleRight:
        {
            return ccp(screenwidth, screenheight *.5);
        }
            break;
        case kBottomLeft:
        {
            return ccp(0, 0);
        }
            break;
        case kBottomCenter:
        {
            return ccp(screenwidth *.5, 0);
        }
            break;
        case kBottomRight:
        {
            return ccp(screenwidth, 0);
        }
        default:
            break;
    }
}
float SGLayout::getSkewing(float x)
{
    float skewing = cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width/320.0f;    
    return skewing * x;
}

float SGLayout::getSkewingY(float y)
{
    float skewing = cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height/480.0f;
//    CCLOG("skewing:%f",skewing);
    
    return skewing * y;

}

float SGLayout::getProportion()
{
    float skewing = cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height/480.0f;
    return skewing;

}
float SGLayout::getProportionX()
{
    float skewing = cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height/380.0f;
    return skewing;
    
}
