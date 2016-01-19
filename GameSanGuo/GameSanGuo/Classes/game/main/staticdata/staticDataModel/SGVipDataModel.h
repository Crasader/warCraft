//
//  SGVipDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-2.
//
//

#ifndef __GameSanGuo__SGVipDataModel__
#define __GameSanGuo__SGVipDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/*  vip表 */

class SGVipDataModel : public CCObject
{
    //vip id
    CC_SYNTHESIZE(int, vipId, VipId);
    
    //vip 等级
    CC_SYNTHESIZE_RETAIN(CCString*, vipName, VipName);
    
    //vip 特权数量
    CC_SYNTHESIZE(int, vipCount, VipCount);
    
    CC_SYNTHESIZE_RETAIN(CCArray*, vipRightsList, VipRightsList);
    
    //需要RMB
    CC_SYNTHESIZE(int, vipmoney, needRMB);
    
public:
    SGVipDataModel();
    
    ~SGVipDataModel();
    
};

#endif /* defined(__GameSanGuo__SGVipDataModel__) */
