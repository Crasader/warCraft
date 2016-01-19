//
//  SGSpotDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-1.
//
//

#include "SGRewardDataModel.h"
SGRewardDataModel::SGRewardDataModel() : rewardArray(0) , rewardId(0),rewardType(0)
{
    rewardArray = CCArray::create();
    rewardArray->retain();
}

SGRewardDataModel::~SGRewardDataModel()
{
    rewardArray->release();
}