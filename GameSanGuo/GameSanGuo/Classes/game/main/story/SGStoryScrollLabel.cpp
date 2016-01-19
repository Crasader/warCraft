//
//  SGStoryScrollLabel.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-29.
//
//

#include "SGStoryScrollLabel.h"

SGStoryScrollLabel::SGStoryScrollLabel(void) : m_pScrollView(NULL), m_pLabel(NULL), _speed(0), _canScrolling(false)
{
    ;
}

SGStoryScrollLabel::~SGStoryScrollLabel(void)
{
    CC_SAFE_RELEASE_NULL(m_pScrollView);
    CC_SAFE_RELEASE_NULL(m_pLabel);
}

SGStoryScrollLabel* SGStoryScrollLabel::create(const char *pszText, const char *fontName, float fontSize, CCSize viewSize, float speed, CCTextAlignment hAlignment)
{
    SGStoryScrollLabel *scrollLabel = new SGStoryScrollLabel();
    if (scrollLabel && scrollLabel->init(pszText, fontName, fontSize, viewSize, speed, hAlignment)) {
        scrollLabel->autorelease();
        return scrollLabel;
    }
    CC_SAFE_DELETE(scrollLabel);
    return NULL;
}

bool SGStoryScrollLabel::init(const char *pszText, const char *fontName, float fontSize, CCSize viewSize, float speed, CCTextAlignment hAlignment)
{
    m_pLabel = CCLabelTTF::create(pszText, fontName, fontSize, CCSizeMake(0, viewSize.height), hAlignment);
    m_pLabel->retain();
    
    _speed = speed;
    this->setContentSize(viewSize);
    CCSize textSize = m_pLabel->getTexture()->getContentSize();
    
    m_pScrollView = CCScrollView::create(viewSize);
    m_pScrollView->retain();
    m_pScrollView->setDelegate(this);
    m_pScrollView->setDirection(kCCScrollViewDirectionBoth);
    
    if (textSize.width > viewSize.width) {
        _canScrolling = true;
        m_pScrollView->setContentSize(textSize);
        m_pScrollView->setContentOffset(ccp(0, 0));
    }else {
        _canScrolling = false;
        m_pScrollView->setContentSize(viewSize);
    }
    
    m_pScrollView->addChild(m_pLabel);
    m_pLabel->ignoreAnchorPointForPosition(false);
    m_pLabel->setAnchorPoint(ccp(0, 1));
    m_pLabel->setPosition(ccp(0, m_pScrollView->getContentSize().height));
    this->addChild(m_pScrollView);
    
    this->scheduleUpdate();
    return true;
}

void SGStoryScrollLabel::update(float dt)
{
    if (!_canScrolling) return;
    
    static float xOffset = 3.0f;
    xOffset -= _speed;
    m_pScrollView->setContentOffset(ccp(xOffset, 0));
    
    if ((m_pScrollView->getContentOffset().x+m_pScrollView->getContentSize().width) <= 0) {
        xOffset = m_pScrollView->getViewSize().width;
        m_pScrollView->setContentOffset(ccp(xOffset, 0));
    }
}

void SGStoryScrollLabel::setString(const char *pszText)
{
    if (m_pLabel) {
        m_pLabel->setString(pszText);
        
        CCSize textSize = m_pLabel->getTexture()->getContentSize();
        
        if (textSize.width > m_pScrollView->getContentSize().width) {
            _canScrolling = true;
            m_pScrollView->setContentSize(textSize);
            m_pScrollView->setContentOffset(ccp(0, 0));
        }else {
            _canScrolling = false;
            m_pScrollView->setContentSize(m_pScrollView->getViewSize());
        }
    }
}

#pragma mark -
#pragma mark - ---------scrollview delegate----------
void SGStoryScrollLabel::scrollViewDidScroll(CCScrollView* view)
{
    
}

void SGStoryScrollLabel::scrollViewDidZoom(CCScrollView* view)
{
    
}