//
//  SGGuideVO.h
//  GameSanGuo
//
//  Created by geng on 13-5-6.
//
//

#ifndef __GameSanGuo__SGGuideVO__
#define __GameSanGuo__SGGuideVO__

#include "cocos2d.h"

USING_NS_CC;

class SGGuideVO : public CCObject
{
    CC_SYNTHESIZE_READONLY(int, _modelId, ModelId);         // 引导ID
    CC_SYNTHESIZE_READONLY(int, _modelType, ModelType);     // 引导类型 : 1 对话类型 2 按钮高亮 3 区域高亮
    CC_SYNTHESIZE_RETAIN(CCArray *, _modelMsg, ModelMsg);   // 引导对话, 可能为空
    CC_SYNTHESIZE(int, _modelTag, ModelTag);                // 引导高亮区域 Node tag
 
public:
    SGGuideVO(void);
    ~SGGuideVO(void);
    
    static SGGuideVO* create(int mid, int mtype, int mtag);
    void initWithInfo(int mid, int mtype, int mtag);
};
#endif /* defined(__GameSanGuo__SGGuideVO__) */
