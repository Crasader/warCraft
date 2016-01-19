//
//  SGVisitViewVO.cpp
//  GameSanGuo
//
//  Created by zenghui on 2014-2-17.
//
//

#include "SGVisitViewVO.h"

SGVisitViewVO::SGVisitViewVO(void):viistViewPic(NULL)
{
    _modelMsg = NULL;
}

SGVisitViewVO::~SGVisitViewVO(void)
{
    if (_modelMsg) {
//        CC_SAFE_DELETE(_modelMsg);
    }
}

SGVisitViewVO* SGVisitViewVO::create()
{
    SGVisitViewVO *model = new SGVisitViewVO();
    if (model) {
        model->autorelease();
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}
