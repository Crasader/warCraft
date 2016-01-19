//
//  SGDynamicModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-8.
//
//

#include "SGDynamicModel.h"
SGDynamicModel::SGDynamicModel()
{
    dyArray = CCArray::create();
    dyArray->retain();
    dyId = 0;
}

SGDynamicModel::~SGDynamicModel()
{
    CC_SAFE_RELEASE(dyArray);
}

CCArray* SGDynamicModel::getDyArray()
{
    return dyArray;
}