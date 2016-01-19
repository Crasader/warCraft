//
//  SGPlayerInfo.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-5.
//
//

#ifndef __GameSanGuo__SGTeamgroup__
#define __GameSanGuo__SGTeamgroup__
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
#include "SGTeamInfo.h"

#define TEAMID 3//阵容数
//#define POSITION 10//每阵容最多武将数


USING_NS_CC;

class SGTeamgroup :public SGTeamInfo
{
private:
    SGTeamInfo array[TEAMID];

private:
    SGTeamgroup();
    ~SGTeamgroup();
    
public:
    
    static SGTeamgroup* shareTeamgroupInfo();
    //设置3个武将数组
    void setEmbattleOfficers(int index,int position,int value);
    //得到某个阵的所有武将array
    CCArray *getEmbattleOfficers(int teamid);
    //设置阵容id
    void setteamId(int index,int value);
    //得到阵容index(理论比阵容数小1)
    int getteamindex(int teamid);
    //设置某个阵的主将id
    void setlordId(int index,int value);
    //得到某个阵主将id
    int getlordId(int teamid);
    //设置某个阵是否当前使用
    void setiscurrent(int index,int value);
    //得到当前使用阵的阵号（1，2，3）
    int getiscurrent();
    //设置三种兵的id,得到三种兵id
    void setsoldierblue(int index,int value);
    int getsoldierblue(int teamid);
    void setsoldierred(int index,int value);
    int getsoldierred(int teamid);
    void setsoldiergreen(int index,int value);
    int getsoldiergreen(int teamid);
    //现在已经有几个阵容了
    int embattleNumber();
    //某个阵容是否只有一个武将
    int isOneOfficer(int teamid);
    //某个武将卡是否在阵容内（返回阵容id） 若不在 返回0，阵容以 1开始计数。
    int isOnEmbattle(const SGOfficerCard *card);
    //得到某个阵容的主将卡
    SGOfficerCard *getlordofficer(int teamid);
    //得到所有在阵容中的武将
    CCArray *getAllOfficercards();
    //得到某个阵卡片位置
    int getPositionId(int teamid,SGOfficerCard *card);
    
    CCArray* showoffofficercards(int teamid);
    
    int islord(int teamid,SGOfficerCard *card);
    
    void changelord(int teamid,SGOfficerCard *card);
    
};




#endif /* defined(__GameSanGuo__SGPlayerInfo__) */
