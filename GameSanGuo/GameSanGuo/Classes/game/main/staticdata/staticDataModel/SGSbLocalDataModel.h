//
//  SGSbLocalDataModel.h
//  GameSanGuo
//
//  Created by caoguoping
//
//

#ifndef __GameSanGuo__SGSBLocalDataModel__
#define __GameSanGuo__SGSBLocalDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 士兵时间特效表 */

class SGSbLocalDataModel :public CCObject
{
    CC_SYNTHESIZE(int, fileId, FileId);
    CC_SYNTHESIZE(float, allTime, AllTime);
    CC_SYNTHESIZE(float, effTime, EffTime);
    CC_SYNTHESIZE(int, wallEffId, WallEffId);
    CC_SYNTHESIZE(int, bottomEffId, BottomEffId);
    CC_SYNTHESIZE(int, flyId, FlyId);
    CC_SYNTHESIZE(int, flyPosX, FlyPosX);
    CC_SYNTHESIZE(int, flyPosY, FlyPosY);
public:
    SGSbLocalDataModel();
    
    ~SGSbLocalDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGSbLocalDataModel__) */
