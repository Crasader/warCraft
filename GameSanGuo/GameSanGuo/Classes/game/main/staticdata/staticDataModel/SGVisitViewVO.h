//
//  SGVisitViewVO.h
//  GameSanGuo
//
//  Created by zenghui on 2014-2-17.
//
//

#ifndef __GameSanGuo__SGVisitViewVO__
#define __GameSanGuo__SGVisitViewVO__

#include "cocos2d.h"

USING_NS_CC;

class SGVisitViewVO : public CCObject
{
public:
    CCString * viistViewPic;
    CC_SYNTHESIZE_RETAIN(CCArray *, _modelMsg, ModelMsg);   // 引导对话, 可能为空
public:
    SGVisitViewVO();
    ~SGVisitViewVO();
    
    static SGVisitViewVO* create();
};
#endif /* defined(__GameSanGuo__SGVisitViewVO__) */
