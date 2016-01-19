//
//  SGPveHelper.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-12.
//
//

#ifndef __GameSanGuo__SGPveHelper__
#define __GameSanGuo__SGPveHelper__

#include "cocos2d.h"

class SGPveHelper : public cocos2d::CCObject
{
public:
    SGPveHelper(void);
    ~SGPveHelper(void);
    
    static SGPveHelper* create(int rid, const char* rname, int rtype, int iid, const char* iname, int level, int star, int rlv, int inList);
private:
    void initWithInfo(int rid, const char* rname, int rtype, int iid, const char* iname, int level, int star, int rlv, int inList);
    
    CC_SYNTHESIZE_READONLY(int, _roleId, RoleId);
    CC_SYNTHESIZE_READONLY(int, _roleLevel, RoleLevel);
    CC_SYNTHESIZE_READONLY(int, _roleType, RoleType);
    CC_SYNTHESIZE_READONLY(int, _itemId, ItemId);
    CC_SYNTHESIZE_READONLY(int, _itemLevel, ItemLevel);
    CC_SYNTHESIZE_READONLY(int, _itemStar, ItemStar);
	CC_SYNTHESIZE_READONLY(int, _inMyFriendList, InMyFriendList);

	
    CC_SYNTHESIZE_READONLY(cocos2d::CCString *, _roleName, RoleName);
    CC_SYNTHESIZE_READONLY(cocos2d::CCString *, _itemName, ItemName);
	
};


#endif /* defined(__GameSanGuo__SGPveHelper__) */
