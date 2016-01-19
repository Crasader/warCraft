//
//  SGLoadData.h
//  GameSanGuo
//
//  Created by lizi on 13-5-29.
//
//

#ifndef __GameSanGuo__SGLoadData__
#define __GameSanGuo__SGLoadData__

#include "cocos2d.h"
USING_NS_CC;

class SGLoadData : public CCObject
{
    /************客户端初始化数据***********/
    //角色名字
    CC_SYNTHESIZE_RETAIN(CCString *, _roleName, RoleName);
    //国家属性 //1 蜀 2 魏 3 吴
    CC_SYNTHESIZE(int, _roleType, RoleType);
    //头像ID
    CC_SYNTHESIZE(int, _roleId, RoleId);
    //主将所在位置
    CC_SYNTHESIZE(int, _mIndex, MIndex);
    //武将列表
    CC_SYNTHESIZE_RETAIN(CCArray *, _soldiers, Soldiers);
    
public:
    SGLoadData(void);
    ~SGLoadData(void);
};

#endif /* defined(__GameSanGuo__SGLoadData__) */
