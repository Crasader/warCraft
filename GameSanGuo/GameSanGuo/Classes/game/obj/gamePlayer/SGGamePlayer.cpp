//
//  SGGamePlayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-5.
//
//

#include "SGGamePlayer.h"
SGGamePlayer::SGGamePlayer()
{
    roleID = 0;
    friendID = 0;
    typeID = 0;
    isOnline = false;
    time = 0;
    image = 0;
    friendName = NULL;
    level = 0;
    isGiftPower = 0;
}

SGGamePlayer::~SGGamePlayer()
{
    CC_SAFE_RELEASE(friendName);
}