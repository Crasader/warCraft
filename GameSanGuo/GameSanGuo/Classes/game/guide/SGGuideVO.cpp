//
//  SGGuideVO.cpp
//  GameSanGuo
//
//  Created by geng on 13-5-6.
//
//

#include "SGGuideVO.h"

SGGuideVO::SGGuideVO(void)
{
    _modelId = 0;
    _modelTag = 0;
    _modelType = 0;
    _modelMsg = NULL;
}

SGGuideVO::~SGGuideVO(void)
{
    if (_modelMsg) {
//        CC_SAFE_DELETE(_modelMsg);
    }
}

SGGuideVO* SGGuideVO::create(int mid, int mtype, int mtag)
{
    SGGuideVO *model = new SGGuideVO();
    if (model) {
        model->autorelease();
        model->initWithInfo(mid, mtype, mtag);
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}

void SGGuideVO::initWithInfo(int mid, int mtype, int mtag)
{
    _modelId = mid;
    _modelTag = mtag;
    _modelType = mtype;
    
    _modelMsg = NULL;
}