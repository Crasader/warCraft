//
//  SGLoadData.cpp
//  GameSanGuo
//
//  Created by lizi on 13-5-29.
//
//

#include "SGLoadData.h"

SGLoadData::SGLoadData(void)
{
    _roleType = 0;
    _roleId = 0;
    _mIndex = 0;
    _roleName = NULL;
    _soldiers = NULL;
}
SGLoadData::~SGLoadData(void)
{
    CC_SAFE_DELETE(_roleName);
    CC_SAFE_DELETE(_soldiers);
}