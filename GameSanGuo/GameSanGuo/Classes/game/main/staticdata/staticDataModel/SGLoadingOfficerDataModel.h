//
//  SGLoadingOfficerDataModel.h
//  GameSanGuo
//
//  Created by caoguoping
//
//

#ifndef __GameSanGuo__SGLoadingOfficerDataModel__
#define __GameSanGuo__SGLoadingOfficerDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 战斗loading表 */

class SGLoadingOfficerDataModel :public CCObject
{
    CC_SYNTHESIZE(int, fileId, FileId);
    CC_SYNTHESIZE(int, bigIcon, BigIcon);
    CC_SYNTHESIZE_RETAIN(CCString*, name, Name);
    CC_SYNTHESIZE_RETAIN(CCString*, description, Description);
    CC_SYNTHESIZE(int, randomId, RandomId);    //随机数1开始，连续
public:
    SGLoadingOfficerDataModel();
    
    ~SGLoadingOfficerDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGLoadingOfficerDataModel__) */
