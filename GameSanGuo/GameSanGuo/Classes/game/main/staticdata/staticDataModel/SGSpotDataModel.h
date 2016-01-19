//
//  SGSpotDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-1.
//
//

#ifndef __GameSanGuo__SGSpotDataModel__
#define __GameSanGuo__SGSpotDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 关卡表 */

class SGSpotDataModel : public CCObject
{
    //id
    CC_SYNTHESIZE(int, spotId, SpotId);
    //beanId
    CC_SYNTHESIZE_RETAIN(CCString*, spotBeanId, SpotBeanId);
    //章节数
    CC_SYNTHESIZE(int, spotSectionIndex, SpotSectionIndex);
    //关卡数
    CC_SYNTHESIZE(int, spotSpotIndex, SpotSpotIndex);
    //章节名称
    CC_SYNTHESIZE_RETAIN(CCString*, spotSectionName, SpotSectionName);
    //关卡名称
    CC_SYNTHESIZE_RETAIN(CCString*, spotStoryName, SpotStoryName);
    //所需体力
    CC_SYNTHESIZE(int, spotRequestPower, SpotRequestPower);
    //关卡经验掉落基数
    CC_SYNTHESIZE(int, spotDropExp, SpotDropExp);
    //关卡金钱掉落基数
    CC_SYNTHESIZE(int, spotDropCoins, SpotDropCoins);
    //闯关最低等级限制
    CC_SYNTHESIZE(int, minLevel, MinLevel);
    
    //精英闯关每日次数
    CC_SYNTHESIZE(int , _creamDayTimes, CreamDayTimes);
public:
    
    SGSpotDataModel();
    
    ~SGSpotDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGSpotDataModel__) */
