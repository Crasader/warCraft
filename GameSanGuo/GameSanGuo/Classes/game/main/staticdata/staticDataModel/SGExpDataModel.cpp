//
//  SGExpDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-7.
//
//

#include "SGExpDataModel.h"
SGExpDataModel::SGExpDataModel()
{
    expId = NULL;
    expLevel = 0;
    expReGold = 0;
    expValue = 0;
    expAll = 0;
    equipCost = 0;
}

SGExpDataModel::~SGExpDataModel()
{
    CC_SAFE_RELEASE(expId);
}