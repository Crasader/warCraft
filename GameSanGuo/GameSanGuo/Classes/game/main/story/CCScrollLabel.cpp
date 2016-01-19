//
//  CCScrollLabel.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-2.
//
//

#include "CCScrollLabel.h"

CCScrollLabel::CCScrollLabel(void)
{
    _speed = 0.0f;
    _textWidth = 0.0f;
    _textHeigth = 0.0f;
}

CCScrollLabel::~CCScrollLabel(void)
{
    
}

CCScrollLabel* CCScrollLabel::create(const char* pszText, const char* fontName, float fontSize, CCSize viewSize, float speed, CCTextAlignment hAlignment)
{
    CCScrollLabel *pRet = new CCScrollLabel();
    if (pRet && pRet->initWithString(pszText, fontName, fontSize, CCSizeMake(0, viewSize.height), hAlignment)) {
        pRet->initWithSpeed(speed, viewSize);
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

void CCScrollLabel::initWithSpeed(float speed, CCSize viewSize)
{
    _speed = speed;
    _speed < 0 && (_speed = 0);
    _vSize = viewSize;
    _vPosition = CCPointZero;
    
    _textWidth = this->getContentSize().width;
    _textHeigth = this->getContentSize().height;
    this->setContentSize(CCSizeMake(0, _textHeigth));
}
                                          
void CCScrollLabel::setString(const char *pszText)
{
    CCLabelTTF::setString(pszText);
}

void CCScrollLabel::draw(void)
{
    if (_vPosition.x == 0 && _vPosition.y == 0) {
        _vPosition = ccp(this->getPosition().x, this->getPosition().y);
    }
    
    static int index = -1;
    float scale = this->getScale();
    CCPoint screenPos = this->getParent()->convertToWorldSpace(_vPosition);
    this->setContentSize(CCSizeMake(_speed*(++index), _textHeigth));
    float posX = (screenPos.x-_vSize.width/2)*scale;
    float posY = (screenPos.y-_vSize.height/2)*scale;
    
    if (this->getContentSize().width > (_textWidth*2+_vSize.width)) {
        index = -1;
        this->setContentSize(CCSizeMake(0, _textHeigth));
    }
    
    glEnable(GL_SCISSOR_TEST);
    CCEGLView::sharedOpenGLView()->setScissorInPoints(posX, posY, _vSize.width*scale, _vSize.height*scale);
    
    CCLabelTTF::draw();
    
#define cc_drawRect 1
#ifdef cc_drawRect
    CCSize s = this->getTextureRect().size;
    CCPoint offsetPix = this->getOffsetPosition();
    CCPoint vertices[4] = {
        ccp(offsetPix.x,offsetPix.y), ccp(offsetPix.x+s.width,offsetPix.y),
        ccp(offsetPix.x+s.width,offsetPix.y+s.height), ccp(offsetPix.x,offsetPix.y+s.height)
    };
    ccDrawPoly(vertices, 4, true);
#endif
}

