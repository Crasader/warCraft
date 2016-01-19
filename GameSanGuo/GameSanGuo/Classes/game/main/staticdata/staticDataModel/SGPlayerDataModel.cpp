//
//  SGPlayerDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-3.
//
//

#include "SGPlayerDataModel.h"
SGPlayerDataModel::SGPlayerDataModel()
{
    playerBaseFriendSize = 0;
    playerBeanId = NULL;
    playerExp = 0;
    playerGoverning = 0;
    playerLevel = 0;
    playerPower = 0;
    playerPowerOld = 0;
    playerSalary = 0;
    playerOfficerMax = 0;
    playerUnlockTip = NULL;
    playerUnlockLevel=NULL;
    grantPower = 0;
}

SGPlayerDataModel::~SGPlayerDataModel()
{
    CC_SAFE_RELEASE(playerBeanId);
    CC_SAFE_RELEASE(playerUnlockTip);
}