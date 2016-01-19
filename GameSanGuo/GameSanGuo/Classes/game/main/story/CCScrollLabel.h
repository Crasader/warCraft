//
//  CCScrollLabel.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-2.
//
//

#ifndef __GameSanGuo__CCScrollLabel__
#define __GameSanGuo__CCScrollLabel__

#include "cocos2d.h"

USING_NS_CC;

/**
 * 滚动字符Label，继承自CCLabelTTF, 重写draw方法
 * 目前还有问题。。。暂时不能使用
 */
class CCScrollLabel : public CCLabelTTF
{
public:
    CCScrollLabel(void);
    virtual ~CCScrollLabel(void);

    static CCScrollLabel* create(const char* pszText, const char* fontName, float fontSize, CCSize viewSize, float speed=0, CCTextAlignment hAlignment=kCCTextAlignmentCenter);
    
    void initWithSpeed(float speed, CCSize viewSize);
    void setString(const char *pszText);
    
    // rewirte CCSprite's function draw(void)
    virtual void draw(void);
    virtual const char* getString(void){return CCLabelTTF::getString();}
    
private:
    float _speed; // 字符滚动速度
    CCSize _vSize; // 显示滚动字符的窗口大小
    CCPoint _vPosition; // 显示滚动字符的窗口位置
    float _textWidth;   // 要显示的字符宽度, 横向滚动
    float _textHeigth;  // 要显示的字符高度, 纵向滚动//TODO
};

#endif /* defined(__GameSanGuo__CCScrollLabel__) */
