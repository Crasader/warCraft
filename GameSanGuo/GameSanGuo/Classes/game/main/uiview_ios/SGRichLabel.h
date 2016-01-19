//
//  SGRichLabel.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-24.
//
//

#ifndef __GameSanGuo__SGRichLabel__
#define __GameSanGuo__SGRichLabel__

#include "cocos2d.h"
#include "PlatformAdp.h"

class SGRichLabel
{
public:
    static cocos2d::CCSprite* getFomLabel(const char* cstr, const cocos2d::CCSize &size, const char* fntName, float fontSize, cocos2d::ccColor3B color = cocos2d::ccRED, cocos2d::ccColor3B defaultInColor = cocos2d::ccWHITE,cocos2d::ccColor3B defaultOutColor = cocos2d::ccBLACK);
    static void setString(const char* cstr, cocos2d::CCSprite *sprite, cocos2d::CCNode *parent=NULL);
    static void drawOnLabel(const char* cstr, const cocos2d::CCSize &size, const char* fntName, float fontSize);
};


#endif /* defined(__GameSanGuo__SGRichLabel__) */
