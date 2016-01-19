//
//  SGLimitGuideVO.cpp
//  GameSanGuo
//
//  Created by geng on 13-5-6.
//
//

#include "SGLimitGuideVO.h"

SGLimitGuideVO::SGLimitGuideVO(void) : _layerTag(0), _parentTag(0), _highlightTag(0)
{
    _modelId = 0;
    _modelType = 0;
    _modelMsg = NULL;

}

SGLimitGuideVO::~SGLimitGuideVO(void)
{
    if (_modelMsg) {
//        CC_SAFE_DELETE(_modelMsg);
    }
}

SGLimitGuideVO* SGLimitGuideVO::create(int mid, int mtype, int layerTag,int parentTag,int highlightTag)
{
    SGLimitGuideVO *model = new SGLimitGuideVO();
    if (model) {
        model->autorelease();
        model->initWithInfo(mid,mtype,layerTag,parentTag,highlightTag);
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}

void SGLimitGuideVO::initWithInfo(int mid, int mtype, int layerTag,int parentTag,int highlightTag)
{
    _modelId = mid;
    _modelType = mtype;
    _layerTag = layerTag;       // 跳转页面Tag
    _parentTag = parentTag;              // 引导高亮的父tag
    _highlightTag = highlightTag;        // 引导高亮tag
    
    _modelMsg = NULL;
}