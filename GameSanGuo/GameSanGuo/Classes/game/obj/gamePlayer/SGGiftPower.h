//
//  SGGiftPower.h
//  GameSanGuo
//
//  Created by zenghui 2014-6-5.
//
//

#ifndef __GameSanGuo__SGGiftPower__
#define __GameSanGuo__SGGiftPower__


#include "cocos2d.h"
USING_NS_CC;

class SGGiftPower : public CCObject
{
public:

    CC_SYNTHESIZE(int, roleID, RoleID);
    CC_SYNTHESIZE(int, giftId, GiftId);
    CC_SYNTHESIZE_RETAIN(CCString*, friendName, FriendName);
    CC_SYNTHESIZE(long long, giftTime, GiftTime);
    CC_SYNTHESIZE(int, level, Level);
    CC_SYNTHESIZE(int , image, Image);
    
    SGGiftPower();
    ~SGGiftPower();
};



#endif /* defined(__GameSanGuo__SGGiftPower__) */
