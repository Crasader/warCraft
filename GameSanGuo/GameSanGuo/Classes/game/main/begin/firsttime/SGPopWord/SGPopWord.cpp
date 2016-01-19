//
//  SGPopWord.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-4.
//
//

#include "SGPopWord.h"
#include "SGLayout.h"
#include "ResourceManager.h"
SGPopWord::SGPopWord()
:_stringContent(NULL),   //数组，存放所有的文字内容
labelContent(NULL),   //标签
_wordIndex(NULL),         //文字索引
_viewSize(CCSizeZero),         //Layer大小
_contentCount(0),     //字符总数
_timerDuration(0),     //单字弹出间隔时间
_maxRowWordCount(0),      //单行最大字数
_clockTick(0),        //计时器
smallAngle(NULL)       //右下角小三角
{
        
}

SGPopWord::~SGPopWord()
{

//    CCSpriteFrameCache::sharedSpriteFrameCache() -> removeSpriteFramesFromFile("login/selectCountry/selectCountry0.plist");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_popWord);
    this->_stringContent->release();
}

SGPopWord* SGPopWord::create(CCArray *arrayContent , CCSize viewSize , int contentCount , float timerDuration)
{
    SGPopWord *pop = new SGPopWord();
    pop->_viewSize = viewSize;
    pop->_contentCount = contentCount;
    pop->_timerDuration = timerDuration;
    pop->_stringContent = arrayContent;
    pop->_stringContent->retain();
    
    if(pop && pop->initWithColor(ccc4(255, 255, 255, 0), viewSize.width, viewSize.height))
    {
        pop->constructView();
        pop->autorelease();
        return pop;
        
    }
    
    CC_SAFE_DELETE(pop);
    return NULL;
}

//把一个char 类型的二维数组，转换为CCArray
//  char str[4][10] = {"你","好","啊","啊"};

CCArray* SGPopWord::convertCharToArray(char string[][10] , int length)
{
    CCArray *array = CCArray::create();
    for(int i=0;i<length;i++)
    {
        CCString *str = CCString::createWithFormat("%s",string[i]);
        array->addObject(str);
    }
    return array;
}

void SGPopWord::constructView()
{
    //CCSpriteFrameCache::sharedSpriteFrameCache() -> addSpriteFramesWithFile("login/selectCountry/selectCountry0.plist");
    
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer.plist", RES_TYPE_LAYER_UI, sg_popWord);

    labelContent = CCLabelTTF::create("", FONT_BOXINFO, 24);
    labelContent->setDimensions(CCSizeMake(_viewSize.width, _viewSize.height-20));
    labelContent->setHorizontalAlignment(kCCTextAlignmentLeft);
    this->addChild(labelContent);
    
    smallAngle = CCSprite::createWithSpriteFrameName("country_sanjiao.png");
    smallAngle->setAnchorPoint(ccp(1, 0));
    smallAngle->setPosition(ccp(200, 20));
    this->addChild(smallAngle);
    
    this->setContentSize(_viewSize);
    this->setAnchorPoint(CCPointZero);
    this->schedule(schedule_selector(SGPopWord::timerStep), 0.05f);
    this->_maxRowWordCount = this->getFirstRowWordCount();
    this->setTouchEnabled(true);
    
}

void SGPopWord::timerStep(CCTime dt)
{
    //tick...小秒针
    _clockTick += 0.05;
    if(_clockTick >= _timerDuration)
    {
        _clockTick = 0.0f;
    }else{
        return;
    }
    
    
    _wordIndex ++;
    if(_wordIndex >= _contentCount)
    {
        smallAngle->setVisible(false);
        return;
    }
    
    int startIndex = 0;
    if(_wordIndex >= _maxRowWordCount*3)
    {
        startIndex = _wordIndex-_wordIndex%_maxRowWordCount-_maxRowWordCount*2;
    }
    
    std::string string1;
    
    for(int i=startIndex;i<=_wordIndex;i++)
    {
        CCString* str = (CCString*)_stringContent->objectAtIndex(i);
        string1.append(str->getCString());
    }
    
    labelContent->setString(string1.c_str());
    
}

//获取单行最大可容纳字数
int SGPopWord::getFirstRowWordCount()
{
    CCLabelTTF *label = CCLabelTTF::create("你", FONT_BOXINFO, 24);
    CCRect rect = label->getTextureRect();
    return _viewSize.width/rect.size.width;
}

void SGPopWord::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
    CCLOG("SGPopWord Click .....");
    //点击改变打字速度
    _timerDuration = 0.05f;
}

