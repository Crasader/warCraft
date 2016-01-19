//
//  SGPlayerDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-3.
//
//

#ifndef __GameSanGuo__SGPlayerDataModel__
#define __GameSanGuo__SGPlayerDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 角色数据表 */

class SGPlayerDataModel : public CCObject
{
    //beanId
    CC_SYNTHESIZE_RETAIN(CCString*, playerBeanId, PlayerBeanId);
    
    //等级
    CC_SYNTHESIZE(int, playerLevel, PlayerLevel);
    
    //玩家体力
    CC_SYNTHESIZE(int, playerPower, PlayerPower);
    
    //原始体力值，不必关注
    CC_SYNTHESIZE(int, playerPowerOld, PlayerPowerOld);
    
    //统御力
    CC_SYNTHESIZE(int, playerGoverning, PlayerGoverning);
    
    //经验值
    CC_SYNTHESIZE(int, playerExp, PlayerExp);
    
    //好友个数
    CC_SYNTHESIZE(int, playerBaseFriendSize, PlayerBaseFriendSize);
    
    //领工资
    CC_SYNTHESIZE(int, playerSalary, PlayerSalary);
    
    //武将上阵上限
    CC_SYNTHESIZE(int, playerOfficerMax, PlayerOfficerMax);
    
    //解锁提示
    CC_SYNTHESIZE_RETAIN(CCString*, playerUnlockTip, PlayerUnlockTip);
    
    //解锁等级
    CC_SYNTHESIZE_RETAIN(CCString *, playerUnlockLevel, PlayerUnlockLevel);
    
    //解锁等级
    CC_SYNTHESIZE(int,grantPower, GrantPower);
public:
    
    SGPlayerDataModel();
    
    ~SGPlayerDataModel();

    
};

#endif /* defined(__GameSanGuo__SGPlayerDataModel__) */
