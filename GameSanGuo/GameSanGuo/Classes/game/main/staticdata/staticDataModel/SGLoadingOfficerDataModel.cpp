//
//  SGLoadingOfficerDataModel.cpp
//  GameSanGuo
//
//  Created by caoguoping
//
//

#include "SGLoadingOfficerDataModel.h"
SGLoadingOfficerDataModel::SGLoadingOfficerDataModel()
{
    fileId = 0;
    randomId = 0;
    bigIcon = 0;
    name = NULL;
    description = NULL;

}

SGLoadingOfficerDataModel::~SGLoadingOfficerDataModel()
{
    CC_SAFE_RELEASE(name);
    CC_SAFE_RELEASE(description);

}