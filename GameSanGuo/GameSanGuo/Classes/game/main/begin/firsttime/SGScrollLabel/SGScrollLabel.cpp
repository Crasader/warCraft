//
//  SGScrollLabel.cpp
//  TestScrollView
//
//  Created by 江南岸 on 13-2-16.
//
//

#include "SGScrollLabel.h"
#include "SGMainManager.h"
//#include "SGLoadLayer.h"

SGScrollLabel* SGScrollLabel::create(CCArray *arrayString , CCSize viewSize)
{
    SGScrollLabel *scrollLabel = new SGScrollLabel();
    scrollLabel->_arrayString = arrayString;
    scrollLabel->_arrayString->retain();
    scrollLabel->_viewSize = viewSize;
    if(scrollLabel && scrollLabel->initWithColor(ccc4(255, 255, 255, 0), viewSize.width, viewSize.height))
    {
        scrollLabel->constructView();
        scrollLabel->autorelease();
        return scrollLabel;
    }
    
    CC_SAFE_DELETE(scrollLabel);
    return NULL;
}

SGScrollLabel::SGScrollLabel()
:_arrayLabel(NULL),
_arrayString(NULL),
_viewSize(CCSizeZero)
{
    
}

SGScrollLabel::~SGScrollLabel()
{
    _arrayLabel->release();
    _arrayString->release();
}

void SGScrollLabel::constructView()
{
    _arrayLabel = CCArray::create();
    _arrayLabel->retain();
    
    for(int i=0;i<=_arrayString->count()/this->getFirstRowWordCount();i++)
    {
        std::string string;
        for(int h=i*this->getFirstRowWordCount();h<(i+1)*getFirstRowWordCount();h++)
        {
            if(h >= _arrayString->count())
            {
                break;
            }
            
            CCString *str = (CCString*)_arrayString->objectAtIndex(h);
            string.append(str->getCString());
        }
        
        CCLabelTTF *label = CCLabelTTF::create(string.c_str(), FONT_BOXINFO, 24);
        _arrayLabel->addObject(label);
        
    }
    
    CCPoint bottomCenter = ccp(_viewSize.width/2,0);
    
    for(int i=0;i<_arrayLabel->count();i++)
    {
        CCLabelTTF *label = (CCLabelTTF*)_arrayLabel->objectAtIndex(i);
        label->setPosition(ccpAdd(bottomCenter, ccp(0, - label->getContentSize().height)));
        label->setOpacity(0.0f);
        label->setHorizontalAlignment(kCCTextAlignmentLeft);
        this->addChild(label);
        //动画
        CCDelayTime *delayTime = CCDelayTime::create(i*2.0f);
        
        CCMoveTo *move2 = CCMoveTo::create(0.5f,ccpAdd(label->getPosition(), ccp(0, _viewSize.height/10)));
        CCFadeIn *fadeIn = CCFadeIn::create(0.5f);
        CCFiniteTimeAction *spawn1 = CCSpawn::create(move2,fadeIn,NULL);
        
        CCMoveTo *move = CCMoveTo::create(5.0f, ccpAdd(label->getPosition(), ccp(0, _viewSize.height)));
        
        CCMoveTo *move3 = CCMoveTo::create(0.5f, ccpAdd(label->getPosition(),ccp(0, _viewSize.height*11/10)));
        CCFadeOut *fadeOut = CCFadeOut::create(0.5f);
        CCFiniteTimeAction *spawn2 = CCSpawn::create(move3,fadeOut,NULL);
        
        CCFiniteTimeAction *sequence = CCSequence::create(delayTime,spawn1,move,spawn2,NULL);
        label->runAction(sequence);
    }
    
    CCDelayTime *delay = CCDelayTime::create(_arrayLabel->count()*2+5);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGScrollLabel::callFunction));
    CCFiniteTimeAction *sequence2 = CCSequence::create(delay,call,NULL);
    this->runAction(sequence2);
}

void SGScrollLabel::callFunction()
{
    this->stopAllActions();
//    SGMainManager::shareMain()->showMainLayer();
}

//获取单行最大可容纳字数
int SGScrollLabel::getFirstRowWordCount()
{
    CCLabelTTF *label = CCLabelTTF::create("你", FONT_BOXINFO, 24);
    CCRect rect = label->getTextureRect();
    return _viewSize.width/rect.size.width;
    
}

