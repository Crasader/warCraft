//
//  SGGamePlayerInfo.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-6.
//
//

#ifndef __GameSanGuo__SGGamePlayerInfo__
#define __GameSanGuo__SGGamePlayerInfo__

#include <iostream>
#include "cocos2d.h"
#include "SGOfficerCard.h"

USING_NS_CC;
class SGGamePlayerInfo :public CCObject
{
private:

    CCArray *_arrayOfficerCardItem;
    CCArray *_arrayEquipmentCardItem;
    CCArray *_arraySoldierCardItem;
    CCArray *_arrayPropsCardItem;
    CCArray *_arrayEmbattleSoldierArray;
    CCArray *_arraypvpOfficerArray;
    
    
private:
    
    CC_SYNTHESIZE(int, roleId, RoleId);
    CC_SYNTHESIZE_RETAIN(CCString*, name, Name);
    CC_SYNTHESIZE_RETAIN(CCString*, nickName, NickName);
    CC_SYNTHESIZE_RETAIN(CCString*, ssid, Ssid);
    CC_SYNTHESIZE(int, exp, Exp);
    CC_SYNTHESIZE(int, level, Level);
    CC_SYNTHESIZE(int, coins, Coins);
    CC_SYNTHESIZE(int, silver, Silver);
    CC_SYNTHESIZE(int, gold, Gold);
    CC_SYNTHESIZE(int, governing, Governing);
    CC_SYNTHESIZE(int, country, Country);
    CC_SYNTHESIZE(int, image, Image);
    CC_SYNTHESIZE(int, power, Power);
    CC_SYNTHESIZE(int, bagSize, BagSize);
    CC_SYNTHESIZE(int, modifyNickNameCount, ModifyNickNameCount);
    CC_SYNTHESIZE(int, redSoldierId, RedSoldierId);//红色士兵ID
    CC_SYNTHESIZE(int, greenSoldierId, GreenSoldierId);//绿色士兵ID
    CC_SYNTHESIZE(int, blueSoldierId, BlueSoldierId);//蓝色士兵ID
    CC_SYNTHESIZE(int , combatValue, CombatValue);//战斗力
    CC_SYNTHESIZE(int , vipLevle, VipLevle);
    CC_SYNTHESIZE(CCString *, militlity, Militlity);
    CC_SYNTHESIZE(int , lordid, Lordid);
    CC_SYNTHESIZE_RETAIN(CCString*, pvprankname, pvpRankName);//军衔
    CC_SYNTHESIZE(int , prestige, presTige);//天梯声望
    CC_SYNTHESIZE(int , enterpvpcount, enterPVPCount);//进入天梯次数
    CC_SYNTHESIZE(int , pvpwincount, pvpWinCount);//pvp赢的次数
    CC_SYNTHESIZE(int , pvptop, pvpTop);//天梯排名
    CC_SYNTHESIZE(int , hp, Hp);//hp
    CC_SYNTHESIZE(int , speed, Speed);
    CC_SYNTHESIZE(int , governinglimit, GoverningLimit);//现有统御力
  
    
public:
    
    SGGamePlayerInfo();
    ~SGGamePlayerInfo();
    
    CCArray *getOfficerCardItemArray();
    CCArray *getEquipmentCardItemArray();
    CCArray *getSoldierCardItemArray();
    CCArray *getPropsCardItem();
//    void setLordOfficer(int lordid);
    SGOfficerCard* getLordOfficer();
    CCArray*getembattlesoldierArray();//获取玩家布阵的士兵信息
     CCArray*getePVPOfficerArray();//获取跨服PVP玩家信息
    
};

class   pvpInfo:public CCObject
{
public:
    
    pvpInfo();
    ~pvpInfo();
    
    CC_SYNTHESIZE(int , itmeid, itmeId);
    CC_SYNTHESIZE(int , level, level);


};

#endif /* defined(__GameSanGuo__SGGamePlayerInfo__) */
