//
//  SGGraySprite.h
//  Test
//
//  Created by 江南岸 on 13-4-3.
//
//

#ifndef __Test__SGGraySprite__
#define __Test__SGGraySprite__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 绘制灰白色纹理 */

class SGGraySprite :public CCSprite
{
    
public:
    SGGraySprite();
    virtual ~SGGraySprite();
    static SGGraySprite* create(const char* pszFileName);
    bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
    virtual void draw();
    
    
};


    

#endif /* defined(__Test__SGGraySprite__) */
