//
//  SGCCLabelTTF.cpp
//  Test
//
//  Created by 江南岸 on 13-4-9.
//
//
#include "SGCCLabelTTF.h"

////BYMM: 针对字体描边的主观修正值，可以按平台不同来写值。
//const static float OffsetXFix = 0.0;
//const static float OffsetYFix = 0.0;

SGCCLabelTTF::SGCCLabelTTF() : m_mainLabel(NULL), m_outsideLabel(NULL) , _offset(0),m_time(0),m_time1(0)
{
//    m_downLabel = NULL;
//    m_upLabel = NULL;
//    m_leftLabel = NULL;
//    m_rightLabel = NULL;
}

SGCCLabelTTF::~SGCCLabelTTF()
{
    ;
}

SGCCLabelTTF* SGCCLabelTTF::create(const char *str,const char*fileFont,int fontSize, const cocos2d::CCSize &dimensions, const cocos2d::ccColor3B &inColor, const cocos2d::ccColor3B &outColor /*= ccBLACK*/, const float outSize /*= 2.0*/, bool isOutline /*= true*/)
{
    return SGCCLabelTTF::create(str, fileFont, fontSize, dimensions, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop, inColor, outColor, outSize, isOutline);
}

SGCCLabelTTF* SGCCLabelTTF::create(const char *str,const char*fileFont,int fontSize, const cocos2d::ccColor3B &inColor, const cocos2d::ccColor3B &outColor /*= ccBLACK*/, const float outSize /*= 2.0*/, bool isOutline /*= true*/)
{
    return SGCCLabelTTF::create(str, fileFont, fontSize, CCSizeZero, kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop, inColor, outColor, outSize, isOutline);
}

SGCCLabelTTF* SGCCLabelTTF::create(const char *str, const char *fileFont, int fontSize, const cocos2d::CCSize &dimensions /*= CCSizeZero*/, CCTextAlignment hAlignment /*= kCCTextAlignmentLeft*/, CCVerticalTextAlignment vAlignment /*= kCCVerticalTextAlignmentTop*/, const cocos2d::ccColor3B &inColor /*= ccWHITE*/, const cocos2d::ccColor3B &outColor /*= ccBLACK*/, const float outSize /*= 2.0*/, bool isOutline /*= true*/)
{
    SGCCLabelTTF *label = new SGCCLabelTTF();
    if(label)
    {
        label->viewDidLoad(str, fileFont, fontSize, dimensions, hAlignment, vAlignment, inColor, outColor, outSize, isOutline);
        label->autorelease();
        return label;
    }
    
    CC_SAFE_DELETE(label);
    return label;
}

//SGCCLabelTTF* SGCCLabelTTF::create(const char *str, const char *fileFont, int fontSize, const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment)
//{
//    SGCCLabelTTF *label = new SGCCLabelTTF();
//    if(label)
//    {
//        label->viewDidLoad(str, fileFont, fontSize);
//        //        label->setContentSize(dimensions);
//        label->setDimensions (CCSizeMake(dimensions.width, dimensions.height));
//        label->setHorizontalAlignment(hAlignment);
//        label->setVerticalAlignment(vAlignment);
//        label->autorelease();
//        return label;
//    }
//    
//    CC_SAFE_DELETE(label);
//    return label;
//}
//
//SGCCLabelTTF* SGCCLabelTTF::create(const char *str, const char *fileFont, int fontSize /*, const cocos2d::CCSize &dimensions = CCSizeZero */)
//{
//    SGCCLabelTTF *label = new SGCCLabelTTF();
//    if(label)
//    {
//        label->viewDidLoad(str, fileFont, fontSize);
//        label->autorelease();
//        return label;
//    }
//    
//    CC_SAFE_DELETE(label);
//    return label;
//}

void SGCCLabelTTF::viewDidLoad(const char *str, const char *fileFont, int fontSize, const cocos2d::CCSize &dimensions, CCTextAlignment hAlignment,CCVerticalTextAlignment vAlignment, const cocos2d::ccColor3B &inColor, const cocos2d::ccColor3B &outColor, const float outSize, bool isOutline)
{
//    m_leftLabel = CCLabelTTF::create(str, fileFont, fontSize);
//    m_downLabel = CCLabelTTF::create(str, fileFont, fontSize);
//    m_upLabel = CCLabelTTF::create(str, fileFont, fontSize);
//    m_rightLabel = CCLabelTTF::create(str, fileFont, fontSize);
    

    
    //实体字，颜色用白色，方便修改。
    ccFontDefinition mainDef;
    mainDef.m_fontName = fileFont;
    mainDef.m_fontSize = fontSize;
    mainDef.m_alignment = hAlignment;
    mainDef.m_vertAlignment = vAlignment;
    mainDef.m_dimensions = dimensions;
    
    mainDef.m_fontFillColor = inColor;
    
    mainDef.m_stroke.m_strokeEnabled = isOutline;
    mainDef.m_stroke.m_strokeColor = outColor;
    mainDef.m_stroke.m_strokeSize = outSize;
    
    m_colorInside = inColor;
    m_colorOutSide = outColor;
    _offset = outSize;

    m_mainLabel = CCLabelTTF::createWithFontDefinition(str, mainDef);
//临时解决方案
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    if ((inColor.r > 128 || inColor.g > 250 ))
//        m_mainLabel->setColor(inColor);
//#endif
    
    addChild(m_mainLabel);
    
    //MMDEBUG: BGN
//    for (int k = 0; k < 4; ++k)
//    {
//        
//        //实体字，颜色用白色，方便修改。
//        ccFontDefinition mainDef;
//        mainDef.m_fontName = fileFont;
//        mainDef.m_fontSize = fontSize;
//        mainDef.m_alignment = hAlignment;
//        mainDef.m_vertAlignment = vAlignment;
//        mainDef.m_dimensions = dimensions;
//        
//        mainDef.m_fontFillColor = ccBLACK;
//        
//        CCLabelTTF *label = CCLabelTTF::createWithFontDefinition(str, mainDef);
////        setInsideColor(inColor);
//        addChild(label, -5, 1000 + k);
//        
//        label->setAnchorPoint(CCPointZero);
//        switch (k) {
//            case 0:
//                label->setPosition(ccp(-2, 0));
//                break;
//            case 1:
//                label->setPosition(ccp(0, -2));
//                break;
//            case 2:
//                label->setPosition(ccp(2, 0));
//                break;
//            case 3:
//                label->setPosition(ccp(0, 2));
//                break;
//        }
//    }

//    if (false)
//    {
//        //描边字，颜色用白色，方便修改。
//        ccFontDefinition outsideDef;
//        outsideDef.m_fontName = fileFont;
//        outsideDef.m_fontSize = fontSize;
//        outsideDef.m_alignment = hAlignment;
//        outsideDef.m_vertAlignment = vAlignment;
//        outsideDef.m_dimensions = dimensions;
//        outsideDef.m_stroke.m_strokeEnabled = true;
//        outsideDef.m_stroke.m_strokeColor = ccWHITE;
//        outsideDef.m_stroke.m_strokeSize = 1;
//        
//        m_outsideLabel = CCLabelTTF::createWithFontDefinition(str, outsideDef);
//        setOutSideColor(outColor);
//        addChild(m_outsideLabel, -1);
//        
//        m_outsideLabel->setAnchorPoint(CCPointZero);
//        m_outsideLabel->setPosition(CCPointZero);
//    }
    //MMDEBUG: END
    
    CCLayerColor::initWithColor(ccc4(0, 0, 0, 0), m_mainLabel->getContentSize().width, m_mainLabel->getContentSize().height);

//    addChild(m_leftLabel,-1);
//    addChild(m_rightLabel,-1);
//    addChild(m_upLabel,-1);
//    addChild(m_downLabel,-1);
}

void SGCCLabelTTF::setPosition(const CCPoint &position)
{
    m_mainLabel->setPosition(position);
    
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->setPosition(ccp(position.x + OffsetXFix, position.y - OffsetYFix));
//    }
    
//    m_leftLabel->setPosition(ccpAdd(m_mainLabel->getPosition(), ccp(-_offset, 0)));
//    m_downLabel->setPosition(ccpAdd(m_mainLabel->getPosition(), ccp(0, -_offset)));
//    m_upLabel->setPosition(ccpAdd(m_mainLabel->getPosition(), ccp(0, _offset)));
//    m_rightLabel->setPosition(ccpAdd(m_mainLabel->getPosition(), ccp(_offset, 0)));
}

void SGCCLabelTTF::setString(const char *label)
{
    m_mainLabel->setString(label);
    
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->setString(label);
//    }
    
//    m_upLabel->setString(label);
//    m_downLabel->setString(label);
//    m_leftLabel->setString(label);
//    m_rightLabel->setString(label);

}

const char* SGCCLabelTTF::getString()
{
    return m_mainLabel->getString();
}

void SGCCLabelTTF::setOutSideColor(const ccColor3B &outSideColor)
{
    m_colorOutSide = outSideColor;
    m_mainLabel->enableStroke(m_colorOutSide, _offset);
    
//    if (m_outsideLabel)
//    {
//        m_colorOutSide = outSideColor;
//        m_outsideLabel->setColor(outSideColor);
//    }
    
//    m_upLabel->setColor(outSideColor);
//    m_downLabel->setColor(outSideColor);
//    m_rightLabel->setColor(outSideColor);
//    m_leftLabel->setColor(outSideColor);
    
}

void SGCCLabelTTF::setInsideColor(const ccColor3B &insideColor)
{
    m_colorInside = insideColor;

//临时解决方案
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//if ((insideColor.r > 128 || insideColor.g > 250 ))
//{
//    m_mainLabel->setColor(m_colorInside);
//}
//#endif

    m_mainLabel->setFontFillColor(m_colorInside);
    
//    m_mainLabel->setColor(insideColor);
}

void SGCCLabelTTF::setAnchorPoint(const CCPoint& anchor)
{
    m_mainLabel->setAnchorPoint(anchor);
    
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->setAnchorPoint(anchor);
//    }
    
//    m_upLabel->setAnchorPoint(anchor);
//    m_downLabel->setAnchorPoint(anchor);
//    m_rightLabel->setAnchorPoint(anchor);
//    m_leftLabel->setAnchorPoint(anchor);
}

/*
void SGCCLabelTTF::setOffset(float offset)
{
    _offset = offset;
    
    setPosition(m_mainLabel->getPosition());
}
*/

//设置旋转
void SGCCLabelTTF::setRotation(float fRotation)
{
    m_mainLabel->setRotation(fRotation);
}

const CCSize &SGCCLabelTTF::getContentSize()
{
    return m_mainLabel->getContentSize();
}

const CCPoint& SGCCLabelTTF::getPosition()
{
    return m_mainLabel->getPosition();
}

void SGCCLabelTTF::setColor(const ccColor3B& color)
{
    this->setInsideColor(color);
}

void SGCCLabelTTF::runactionWithScale(float duration, float s)
{
//    CCScaleTo *scale = CCScaleTo::create(duration, s);
//    m_upLabel->runAction(scale);
//    CCScaleTo *scale1 = CCScaleTo::create(duration, s);
//    m_downLabel->runAction(scale1);
//    CCScaleTo *scale2 = CCScaleTo::create(duration, s);
//    m_leftLabel->runAction(scale2);
//    CCScaleTo *scale3 = CCScaleTo::create(duration, s);
//    m_rightLabel->runAction(scale3);
    CCScaleTo *scale4 = CCScaleTo::create(duration, s);
    m_mainLabel->runAction(scale4);
    
//    if (m_outsideLabel)
//    {
//        CCScaleTo *scale5 = CCScaleTo::create(duration, s);
//        m_outsideLabel->runAction(scale5);
//    }

}
void SGCCLabelTTF::runactionWithFlicker()
{
    CCFadeIn *fadeIn_1 = CCFadeIn::create(m_time);
    m_mainLabel->runAction(fadeIn_1);
    
//    if (m_outsideLabel)
//    {
//        CCFadeIn *fadeIn_6 = CCFadeIn::create(m_time);
//        m_outsideLabel->runAction(fadeIn_6);
//    }
    
//    CCFadeIn *fadeIn_2 = CCFadeIn::create(m_time);
//    CCFadeIn *fadeIn_3 = CCFadeIn::create(m_time);
//    CCFadeIn *fadeIn_4 = CCFadeIn::create(m_time);
//    CCFadeIn *fadeIn_5 = CCFadeIn::create(m_time);
//    m_upLabel->runAction(fadeIn_2);
//    m_downLabel->runAction(fadeIn_3);
//    m_leftLabel->runAction(fadeIn_4);
//    m_rightLabel->runAction(fadeIn_5);
}
void SGCCLabelTTF::runactionWithFlicker1()
{
    CCFadeOut *fadeIn_11 = CCFadeOut::create(m_time1);
    m_mainLabel->runAction(fadeIn_11);
    
//    if (m_outsideLabel)
//    {
//        CCFadeOut *fadeIn_66 = CCFadeOut::create(m_time1);
//        m_outsideLabel->runAction(fadeIn_66);
//    }
    
//    m_upLabel->runAction(fadeIn_22);
//    m_downLabel->runAction(fadeIn_33);
//    m_leftLabel->runAction(fadeIn_44);
//    m_rightLabel->runAction(fadeIn_55);
//    CCFadeOut *fadeIn_22 = CCFadeOut::create(m_time1);
//    CCFadeOut *fadeIn_33 = CCFadeOut::create(m_time1);
//    CCFadeOut *fadeIn_44 = CCFadeOut::create(m_time1);
//    CCFadeOut *fadeIn_55 = CCFadeOut::create(m_time1);
}

void SGCCLabelTTF::callactionback(cocos2d::CCNode *node,int change)
{
    ;
}

void SGCCLabelTTF::labelstopact()
{
//    m_upLabel->stopAllActions();
//    m_downLabel->stopAllActions();
//    m_leftLabel->stopAllActions();
//    m_rightLabel->stopAllActions();
    m_mainLabel->stopAllActions();
    
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->stopAllActions();
//    }
}

CCAction* SGCCLabelTTF::fadin(float time)
{
    m_time = time;
    CCAction *a = CCCallFuncN::create(this, callfuncN_selector(SGCCLabelTTF::runactionWithFlicker));
    return a;
}
CCAction* SGCCLabelTTF::fadout(float time)
{
    m_time1 = time;
    CCAction *a = CCCallFuncN::create(this, callfuncN_selector(SGCCLabelTTF::runactionWithFlicker1));
    return a;
}
void SGCCLabelTTF::setop(int a)
{
//    m_upLabel->setOpacity(a);
//    m_downLabel->setOpacity(a);
//    m_leftLabel->setOpacity(a);
//    m_rightLabel->setOpacity(a);
    m_mainLabel->setOpacity(a);
    if (m_outsideLabel)
    {
        m_outsideLabel->setOpacity(a);
    }
}

void SGCCLabelTTF::runBlink(float gapTime)
{
    //由于文本合一，这里跳转到统一闪烁的函数。
    this->runAllBlink(gapTime);
    
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
//    }
    
//    m_upLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
//    m_downLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
//    m_leftLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
//    m_rightLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
}

void SGCCLabelTTF::runAllBlink(float gapTime)
{
    m_mainLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
    
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
//    }
    
//    m_upLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
//    m_downLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
//    m_leftLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
//    m_rightLabel->runAction(CCRepeatForever::create(CCSequence::create(CCFadeOut::create(gapTime),CCFadeIn::create(gapTime),NULL)));
}

void SGCCLabelTTF::setHorizontalAlignment(CCTextAlignment alignment)
{
    m_mainLabel->setHorizontalAlignment(alignment);
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->setHorizontalAlignment(alignment);
//    }
    //    m_upLabel->setHorizontalAlignment(alignment);
    //    m_downLabel->setHorizontalAlignment(alignment);
    //    m_leftLabel->setHorizontalAlignment(alignment);
    //    m_rightLabel->setHorizontalAlignment(alignment);
}

void SGCCLabelTTF::setVerticalAlignment(CCVerticalTextAlignment vAlignment)
{
    m_mainLabel->setVerticalAlignment(vAlignment);
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->setVerticalAlignment(vAlignment);
//    }
    //    m_upLabel->setVerticalAlignment(vAlignment);
    //    m_downLabel->setVerticalAlignment(vAlignment);
    //    m_leftLabel->setVerticalAlignment(vAlignment);
    //    m_rightLabel->setVerticalAlignment(vAlignment);
}

//设置文本域
void SGCCLabelTTF::setDimensions(const CCSize &dim)
{
    m_mainLabel->setDimensions(dim);
//    if (m_outsideLabel)
//    {
//        m_outsideLabel->setDimensions(dim);
//    }
    //    m_upLabel->setDimensions(dim);
    //    m_downLabel->setDimensions(dim);
    //    m_leftLabel->setDimensions(dim);
    //    m_rightLabel->setDimensions(dim);
}

//设置flipx
void SGCCLabelTTF::setFlipX(bool b)
{
    m_mainLabel->setFlipX(b);
}

//设置flipy
void SGCCLabelTTF::setFlipY(bool b)
{
    m_mainLabel->setFlipY(b);
}
