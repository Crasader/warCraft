//
//  SGPveHelper.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-12.
//
//

#include "SGPveHelper.h"

USING_NS_CC;

SGPveHelper::SGPveHelper(void): _roleId(0)
, _roleLevel(0)
, _roleType(0)
, _itemId(0)
, _itemLevel(0)
, _itemStar(0)
, _inMyFriendList(0)
, _roleName(NULL)
, _itemName(NULL)
{

}
SGPveHelper::~SGPveHelper(void)
{
    CC_SAFE_DELETE(_roleName);
    CC_SAFE_DELETE(_itemName);
}

SGPveHelper* SGPveHelper::create(int rid, const char* rname, int rtype, int iid, const char* iname, int level, int star, int rlv, int inList)
{
    SGPveHelper *helper = new SGPveHelper();
    if (helper) {
        helper->initWithInfo(rid, rname, rtype, iid, iname, level, star, rlv, inList);
        helper->autorelease();
        return helper;
    }
    return NULL;
}

void SGPveHelper::initWithInfo(int rid, const char *rname, int rtype, int iid, const char *iname, int level, int star, int rlv, int inList)
{
    _roleId = rid;
    _roleType = rtype;
    _roleLevel = rlv;
    _itemId = iid;
    _itemLevel = level;
    _itemStar = star;
	
	_inMyFriendList = inList;
	
    if (rname == NULL) {
        rname = "";
    }
    if (iname == NULL) {
        iname = "";
    }
    _itemName = CCString::create(iname);
    _itemName->retain();
    _roleName = CCString::create(rname);
    _roleName->retain();
}