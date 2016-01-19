//
//  SGRankPlayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-8.
//
//

#include "SGRankPlayer.h"
#include <string.h>
#include <stdlib.h>

USING_NS_CC;

SGRankPlayer::SGRankPlayer(void)
{
    _name = NULL;
    _headId = 0;
    _rank = 0;
    _level = 0;
    _bcount = 0;
    _winRate = 0;
    _serverid = 0;
    _servername = NULL;
    _roleid = 0;
    _nation =0;
}
SGRankPlayer::~SGRankPlayer(void)
{
    CC_SAFE_DELETE(_name);
    CC_SAFE_DELETE(_servername);
}

SGRankPlayer* SGRankPlayer::create(const char *name, int hid, int rank, int levl, int ftime, float rwin,int serverid,const char* servername,int roleid,int nation)
{
    SGRankPlayer *player = new SGRankPlayer();
    if (player&&player->initWithInfo(name, hid, rank, levl, ftime, rwin,serverid,servername,roleid,nation)) {
        player->autorelease();
        return player;
    }
    CC_SAFE_DELETE(player);
    return NULL;
}

bool SGRankPlayer::initWithInfo(const char *name, int hid, int rank, int levl, int ftime, float rwin,int serverid,const char* servername,int roleid,int nation)
{
    _name = CCString::create(name);
    _name->retain();
    
    _headId = hid;
    _rank = rank;
    _level = levl;
    _bcount = ftime;
    _winRate = rwin;
    _serverid = serverid;
    _servername = CCString::create(servername);
    _servername->retain();
    _roleid = roleid;
    _nation = nation;
    return true;
}