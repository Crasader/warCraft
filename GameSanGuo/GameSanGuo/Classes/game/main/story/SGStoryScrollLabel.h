//
//  SGStoryScrollLabel.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-29.
//
//

#ifndef __GameSanGuo__SGStoryScrollLabel__
#define __GameSanGuo__SGStoryScrollLabel__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

/**
 * 滚动字符Label，使用CCScrollView + CCLabelTTF实现
 **/
class SGStoryScrollLabel : public CCNode, public CCScrollViewDelegate
{
public:
    SGStoryScrollLabel(void);
    ~SGStoryScrollLabel(void);

    static SGStoryScrollLabel* create(const char* pszText, const char* fontName, float fontSize, CCSize viewSize, float speed=0, CCTextAlignment hAlignment=kCCTextAlignmentCenter);
    
    bool init(const char* pszText, const char* fontName, float fontSize, CCSize viewSize, float speed=0, CCTextAlignment hAlignment=kCCTextAlignmentCenter);
    
    void setTextColor(const ccColor3B& color3);
    void setString(const char *pszText);
    void update(float dt);

public: //scrollview delegate
    virtual void scrollViewDidScroll(CCScrollView* view);
    virtual void scrollViewDidZoom(CCScrollView* view);

private:
    CCScrollView *m_pScrollView;
    CCLabelTTF *m_pLabel;
    float _speed; // 字符滚动速度 默认为0.0f
    bool _canScrolling; // 是否需要滚动
};

#endif /* defined(__GameSanGuo__SGStoryScrollLabel__) */
