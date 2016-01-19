//
//  SGVipGift.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-13.
//
//

#include "SGVipGifts.h"
SGVipGift::SGVipGift()
:m_cost(0),
m_discount(0),
m_vipLvl(0),
m_id(0),
str_name(NULL),
str_info(NULL),
list(NULL),
m_Coin(0),
m_Speaker(0),
m_Award(0),
m_liandan(0),
m_pvpgifttype(0),
m_activityid(0),
m_btnid(0),
m_haveBuy(0),
str_condition(NULL)
{
}
SGVipGift::~SGVipGift()
{
    CC_SAFE_RELEASE(str_info);
    CC_SAFE_RELEASE(str_condition);
    CC_SAFE_RELEASE(str_name);
    CC_SAFE_RELEASE(list);
}
void SGVipGift::addGiftList(SGGift *gift)
{
    if (!list) {
        list = CCArray::create();
        list->retain();
    }
    list->addObject(gift);
}
CCArray* SGVipGift::getGiftList()
{
    return list;
}