//
//  SGLimitGuideVO.h
//  GameSanGuo
//
//  Created by geng on 13-5-6.
//
//

#ifndef __GameSanGuo__SGLimitGuideVO__
#define __GameSanGuo__SGLimitGuideVO__

#include "cocos2d.h"

USING_NS_CC;

class SGLimitGuideVO : public CCObject
{
    CC_SYNTHESIZE_READONLY(int, _modelId, ModelId);         // 引导ID
    CC_SYNTHESIZE_READONLY(int, _layerTag, LayerTag);       // 跳转页面Tag
    CC_SYNTHESIZE_READONLY(int, _modelType, ModelType);     // 引导类型 : 1 对话类型 2 按钮高亮 3 区域高亮
    CC_SYNTHESIZE_RETAIN(CCArray *, _modelMsg, ModelMsg);   // 引导对话, 可能为空
    CC_SYNTHESIZE(int, _parentTag, ParentTag);              // 引导高亮的父tag
    CC_SYNTHESIZE(int, _highlightTag, HighlightTag);        // 引导高亮tag
 
public:
    SGLimitGuideVO();
    ~SGLimitGuideVO();
    
    static SGLimitGuideVO* create(int mid, int mtype, int layerTag,int parentTag,int highlightTag);
    void initWithInfo(int mid, int mtype, int layerTag,int parentTag,int highlightTag);
};
#endif /* defined(__GameSanGuo__SGLimitGuideVO__) */
