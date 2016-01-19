//
//  SGVerticalSlider.h
//  TestScrollView
//
//  Created by 江南岸 on 13-1-31.
//
//


/******         SGVerticalSlider
 
 *  为 cocos2d-x 提供了一个显示竖排滑动进度与数量的滑动框
 
 *  调用create函数，传入相应的参数后即可初始化 SGVerticalSlider
 
 *  通过调用 setSliderValue 方法可以改变滑动块的位置
 
 *****/


#ifndef __TestScrollView__SGVerticalSlider__
#define __TestScrollView__SGVerticalSlider__


#include <iostream>
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class SGVerticalSlider: public CCLayerColor
{
private:
    CCScale9Sprite *slider;       //滑标
    CCSprite *background;   //背景
    float maxSlider;        //最大值
    float minSlider;        //最小值
    float sliderValue;      //当前slider的值
    float sliderProportion; //slider的比例尺
    float sliderScaleHeight;//新的slider的高度
    
    void constructView();
    void scrollToPosition(float newValue , float oldValue);

    
public:
    
    SGVerticalSlider();
    ~SGVerticalSlider();
    
    
    static SGVerticalSlider *create(const char *sliderFileName , const char *backgroundFileName , float maxSlider ,float minSlider ,float sliderProportion);
    static SGVerticalSlider *create(CCScale9Sprite *slider ,CCSprite *background , float maxSlider ,float minSlider ,float sliderProportion);
    
    void initWithSliderFileNameAndBackgroundFileName(const char *sliderFileName , const char *backgroundFileName);
    void initWithSliderAndBackround(CCScale9Sprite *slider ,CCSprite *background);

    //最大值 
    float getMaxSlider(float max);
    //最小值 
    float getMinSlider(float min);
    //slider值 setter getter 方法
    void setSliderValue(float value);
    float getSliderValue();
    //比例尺 setter getter 方法
    float getSliderProportion();    
    void scaleSpriteByCCSize(CCScale9Sprite *sprite , float newHeight);

    
};

#endif /* defined(__TestScrollView__SGVerticalSlider__) */
