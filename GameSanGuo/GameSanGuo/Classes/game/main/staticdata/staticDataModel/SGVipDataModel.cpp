//
//  SGVipDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-2.
//
//

#include "SGVipDataModel.h"
SGVipDataModel::SGVipDataModel()
{
    vipId = 0;
    vipName = NULL;
    vipmoney = 0;
    vipCount = 0;
    vipRightsList = CCArray::create();
}

SGVipDataModel::~SGVipDataModel()
{
    CC_SAFE_RELEASE(vipName);
    CC_SAFE_RELEASE(vipRightsList);
}