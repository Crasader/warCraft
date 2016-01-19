//
//  SGShowJingZhouRankPlayerLayer.cpp
//  GameSanGuo
//
//  Created by kanata on 13-11-18.
//
//

#include "SGShowJingZhouRankPlayerLayer.h"
#include <string.h>
#include <stdlib.h>

USING_NS_CC;

SGShowJingZhouRankPlayerLayer::SGShowJingZhouRankPlayerLayer()
{
    _name = NULL;
    _rank = 0;
    _wave = 0;
    _damage = 0;
	_goldNum = 0;

}
SGShowJingZhouRankPlayerLayer::~SGShowJingZhouRankPlayerLayer()
{
    CC_SAFE_DELETE(_name);
}

SGShowJingZhouRankPlayerLayer* SGShowJingZhouRankPlayerLayer::create(int id, const char *name, int rank, int wav, int damage,int head, int goldNum)
{
    SGShowJingZhouRankPlayerLayer *player = new SGShowJingZhouRankPlayerLayer();
    if (player&&player->initWithInfo(id, name, rank, wav, damage,head, goldNum)) {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return NULL;
}

bool SGShowJingZhouRankPlayerLayer::initWithInfo(int id, const char *name, int rank, int wav, int damage,int head, int goldNum)
{
    _playerId = id;
    _name = CCString::create(name);
    _name->retain();
    _headId = head;
    _rank = rank;
    _wave = wav;
    _damage = damage;
	_goldNum = goldNum;
    return true;
}