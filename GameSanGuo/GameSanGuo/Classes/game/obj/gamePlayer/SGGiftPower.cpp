//
//  SGGiftPower.cpp
//  GameSanGuo
//
//  Created by zenghui 2014-6-5.
//
//

#include "SGGiftPower.h"
SGGiftPower::SGGiftPower()
: giftId(0)
, giftTime(0)
, level(0)
{
    roleID = 0;
    image = 0;
    friendName = NULL;
}

SGGiftPower::~SGGiftPower()
{
    CC_SAFE_RELEASE(friendName);
}