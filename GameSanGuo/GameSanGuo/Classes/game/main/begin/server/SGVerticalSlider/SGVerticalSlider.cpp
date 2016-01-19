//
//  SGVerticalSlider.cpp
//  TestScrollView
//
//  Created by 江南岸 on 13-1-31.
//
//

#include "SGVerticalSlider.h"
SGVerticalSlider::SGVerticalSlider()
: slider(NULL),    //滑标
background(NULL),   //背景
maxSlider(0.0f),        //最大值
minSlider(0.0f),        //最小值
sliderValue(0.0f),      //当前slider的值
sliderProportion(0.0f), //slider的比例尺
sliderScaleHeight(0.0f)//新的slider的高度
{
    this->sliderProportion = 0;
    this->sliderValue = 0;
    this->maxSlider = 0;
    this->minSlider = 0;
}

SGVerticalSlider::~SGVerticalSlider()
{
    
}

SGVerticalSlider* SGVerticalSlider::create(const char *sliderFileName, const char *backgroundFileName , float maxSlider ,float minSlider ,float sliderProportion)
{
    SGVerticalSlider *vs = new SGVerticalSlider();
    vs->maxSlider = maxSlider;
    vs->minSlider = minSlider;
    vs->sliderProportion = sliderProportion;
    
    if(sliderFileName && backgroundFileName && vs->initWithColor(ccc4(ccYELLOW.r, ccYELLOW.g, ccYELLOW.b, 0), 0, 0))
    {
        vs->initWithSliderFileNameAndBackgroundFileName(sliderFileName, backgroundFileName);
        vs->autorelease();
        return vs;
        
    }
    
    CC_SAFE_DELETE(vs);
    return NULL;
}


void SGVerticalSlider::initWithSliderFileNameAndBackgroundFileName(const char *sliderFileName, const char *backgroundFileName)
{
    CCSprite *ss = CCSprite::create(sliderFileName);
    CCSize size = ss->getContentSize();
    this->slider = CCScale9Sprite::create(sliderFileName,CCRectMake(0, 0, size.width, size.height),CCRectMake(size.width*0.25,size.height*0.1, size.width*0.5, size.height*0.8));
    this->background = CCSprite::create(backgroundFileName);
    
    this->constructView();
}

SGVerticalSlider* SGVerticalSlider::create(CCScale9Sprite *slider, CCSprite *background , float maxSlider ,float minSlider ,float sliderProportion)
{
    SGVerticalSlider *vs = new SGVerticalSlider();
    vs->maxSlider = maxSlider;
    vs->minSlider = minSlider;
    vs->sliderProportion = sliderProportion;
    
    if(slider && background && vs->initWithColor(ccc4(ccYELLOW.r, ccYELLOW.g, ccYELLOW.b, 0), 0, 0))
    {
        vs->initWithSliderAndBackround(slider, background);
        vs->autorelease();
        return vs;
        
    }
    
    CC_SAFE_DELETE(vs);
    return NULL;

}

void SGVerticalSlider::initWithSliderAndBackround(CCScale9Sprite *slider, CCSprite *background)
{
    this->slider = slider;
    this->background = background;
    
    this->constructView();
}

//构建布局
void SGVerticalSlider::constructView()
{
    this->addChild(background);
    this->addChild(slider);
    this->setContentSize(background->getContentSize());
    background->setAnchorPoint(CCPointZero);
    background->setPosition(this->getPosition());
    slider->setAnchorPoint(CCPointMake(0, 0.5));
    
    
    /*******************/
    
    //background 高度相对于 slider 的倍数
    float proportion = this->sliderProportion/this->maxSlider;
    //拉伸后的slider的高度值
    sliderScaleHeight = this->background->getContentSize().height * proportion;
    //scale sprite
    this->scaleSpriteByCCSize(slider, sliderScaleHeight);
    //设置slider在background的中心位置
    slider->setPosition(ccpAdd(background->getPosition(), ccp(0, background->getContentSize().height/2)));
}

//slider 变形函数
void SGVerticalSlider::scaleSpriteByCCSize(CCScale9Sprite *sprite, float newHeight)
{
    CCSize spriteSize = sprite->getContentSize();
    sprite->setPreferredSize(CCSizeMake(spriteSize.width, newHeight));
}

//获取最小值
float SGVerticalSlider::getMaxSlider(float max)
{
    return maxSlider;
}

//获取最大值
float SGVerticalSlider::getMinSlider(float min)
{
    return minSlider;
}

//设置Slider Value
void SGVerticalSlider::setSliderValue(float value)
{
    float oldValue = sliderValue;

    if(value >= maxSlider){
        sliderValue = maxSlider;
        
    }else if(value <= minSlider){
        sliderValue = minSlider;
        
    }else{
        sliderValue = value;
        
    }
    
     //call function
    this->scrollToPosition(value, oldValue);
}

//获得sliderValue 
float SGVerticalSlider::getSliderValue()
{
    return sliderValue;    
}

//获取比例尺
float SGVerticalSlider::getSliderProportion()
{
    return sliderProportion;
}

//移动slider
void SGVerticalSlider::scrollToPosition(float newValue , float oldValue)
{
    float h = (newValue - oldValue)/maxSlider*this->background->getContentSize().height;
    CCPoint newPt = newPt = ccpAdd(slider->getPosition(), ccp(0, h));
    float sub = newPt.y - background->getPosition().y;

    if(sub >= background->getContentSize().height - sliderScaleHeight*0.5)
    {
        newPt = ccpAdd(background->getPosition(), ccp(0, background->getContentSize().height - sliderScaleHeight*0.5));
        
    }else if(sub <= sliderScaleHeight*0.5)
    {
        newPt = ccpAdd(background->getPosition(), ccp(0, sliderScaleHeight*0.5));
    }
    
    CCMoveTo *move = CCMoveTo::create(0.01f, newPt);
    slider->runAction(move);
}




















