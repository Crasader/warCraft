//
//  SGFont.h
//  GameSanGuo
//  描边字体
//  Created by 陈雪龙 on 13-1-23.
//
//

#ifndef __GameSanGuo__SGFont__
#define __GameSanGuo__SGFont__

#include "cocos2d.h"
USING_NS_CC;
class SGFont : public CCLabelTTF
{
public:    
    static CCRenderTexture* createStroke(cocos2d::CCLabelTTF *label, float size, cocos2d::ccColor3B color);
    static CCLabelTTF *createmyfont(const char *string, const char *fontName, float fontSize);
};

#endif /* defined(__GameSanGuo__SGFont__) */
