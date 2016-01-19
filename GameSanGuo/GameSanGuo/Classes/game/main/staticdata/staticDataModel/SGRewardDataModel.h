//
//  SGSpotDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-1.
//
//

#ifndef __GameSanGuo__SGRewardDataModel__
#define __GameSanGuo__SGRewardDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 关卡表 */

class SGRewardDataModel : public CCObject
{
public:
    //id
    CC_SYNTHESIZE(int, rewardId, RewardId);
    //类型
    CC_SYNTHESIZE(int, rewardType, RewardType);
    
    CCArray *rewardArray;

    
    SGRewardDataModel();
    
    ~SGRewardDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGSpotDataModel__) */
