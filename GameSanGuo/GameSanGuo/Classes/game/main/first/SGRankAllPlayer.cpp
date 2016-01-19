//
//  SGRankAllPlayer.cpp
//  GameSanGuo
//
//  Created by kanata on 14-2-11.
//
//

#include "SGRankAllPlayer.h"
#include <string.h>
#include <stdlib.h>

USING_NS_CC;

SGRankAllPlayer::SGRankAllPlayer()
{
    _name = NULL;
    _servername =NULL;
    _servernum=0;
    _playerId=0;
    _headId=0;
    _rank=0;
    _ranktype=0;
    
}
SGRankAllPlayer::~SGRankAllPlayer()
{
    CC_SAFE_DELETE(_name);
}

SGRankAllPlayer* SGRankAllPlayer::create(const char *name,const char *servername,int headid,int rank,int playerId,int ranktype, int servernum)
{
    SGRankAllPlayer *player = new SGRankAllPlayer();
    if (player&&player->initWithInfo(name, servername, headid, rank, playerId,ranktype,servernum)) {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return NULL;
}

bool SGRankAllPlayer::initWithInfo(const char *name,const char *servername,int headid,int rank,int playerId,int ranktype, int servernum)
{
    
    _name = CCString::create(name);
    _name->retain();
    _servername =CCString::create(servername);
    _servername->retain();
    _servernum=servernum;
    _playerId=playerId;
    _headId=headid;
    _rank=rank;
    _ranktype=ranktype;

    
    
    return true;
}