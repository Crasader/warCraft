//
//  SGSuitDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-2.
//
//

#include "SGSuitDataModel.h"
SGSuitDataModel::SGSuitDataModel()
{
    suitArmsName = NULL;
    suitCount = 0;
    suitId = 0;
    suitName = NULL;
}

SGSuitDataModel::~SGSuitDataModel()
{
    CC_SAFE_RELEASE(suitName);
    CC_SAFE_RELEASE(suitArmsName);
}