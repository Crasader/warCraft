//
//  SGSpotDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-1.
//
//

#include "SGSpotDataModel.h"
SGSpotDataModel::SGSpotDataModel()
{
    spotBeanId = NULL;
    spotId = 0;
    spotRequestPower = 0;
    spotSectionIndex = 0;
    spotSectionName = NULL;
    spotSpotIndex = 0;
    spotStoryName = NULL;
    spotDropExp = 0;
    spotDropCoins = 0;
    minLevel = 0 ;
    mapId = 1;
}

SGSpotDataModel::~SGSpotDataModel()
{
//    CC_SAFE_RELEASE(spotBeanId);
//    CC_SAFE_RELEASE(spotSectionName);
//    CC_SAFE_RELEASE(spotStoryName);
}