//
//  SGPlayerInfo.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#ifndef __GameSanGuo__SGTeamInfo__
#define __GameSanGuo__SGTeamInfo__
#include "cocos2d.h"
#include "SGOfficerCard.h"
#include "SGSoldierCard.h"
#include "SGEquipCard.h"
#include "SGPropsCard.h"
#include "SGGamePlayer.h"
#include "SGVipGifts.h"
#include "SGGamePlayerInfo.h"
#include "SGRankPlayer.h"
#include "SGLottery9ViewItem.h"
#include "SGPlayerInfo.h"

//#define TEAMID 3//阵容数
#define POSITION 10//每阵容最多武将数


USING_NS_CC;

class SGTeamInfo : public CCObject
{
public:

//上阵武将数组
    int arry[POSITION];
    //阵容id
    CC_SYNTHESIZE(int, teamId, TeamId);
    //主将id
    CC_SYNTHESIZE(int, lordId, LordId);
    //是否设置
    CC_SYNTHESIZE(int, iscurrent, Iscurrent);
    //红兵id
    CC_SYNTHESIZE(int, soldierred, Soldierred);
    //蓝兵id
    CC_SYNTHESIZE(int, soldierblue, Soldierblue);
    //绿兵id
    CC_SYNTHESIZE(int, soldiergreen, Soldiergreen);
    SGTeamInfo();
    ~SGTeamInfo();
    
public:
    static SGTeamInfo* shareTeamInfo(void);
    //设置武将数组
    void setEmbattleOfficers(int index,int value);
    //得到该阵的所有武将array
    CCArray *getEmbattleOfficers();

    //某个阵容是否只有一个武将或没有武将
    int isOneOfficer();
    
    //某个武将卡是否在该阵容内
    bool isOnEmbattle(const SGOfficerCard *card);
    //得到某个阵容的主将卡
    SGOfficerCard *getlordofficer();
    int getPositonId(SGOfficerCard *card);
};




#endif /* defined(__GameSanGuo__SGPlayerInfo__) */
