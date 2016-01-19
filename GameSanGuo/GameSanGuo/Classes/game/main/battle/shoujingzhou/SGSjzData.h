//
//  SGSjzData.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/27/13.
//
//

#ifndef __GameSanGuo__SGSjzData__
#define __GameSanGuo__SGSjzData__

#include "cocos2d.h"

USING_NS_CC;

//军魂箱子的INDEX兼TAG
enum SoulTag
{
    ST_NULL = -1, //错误
    ST_SSS = 1, //传奇
    ST_AAA = 2, //精粹
    ST_BBB = 3, //荆州
};

class SGSjzData : public CCObject
{
public:
    //构造
    SGSjzData();
    
    //析构
    ~SGSjzData();
    
    //创建
    static SGSjzData* create();
    
    //当前关卡
    CC_SYNTHESIZE(int, _levelCur, LevelCur);
    
    //可参战次数
    CC_SYNTHESIZE(int, _countCur, CountCur);
    
    //最大参战次数
    CC_SYNTHESIZE(int, _countMax, CountMax);
    //已用vip次数
    CC_SYNTHESIZE(int, _vipcountcur, VIPCountCur);
     //vip总次数
    CC_SYNTHESIZE(int, _vipcountmax, VIPCountMax);
    //购买次数花费
    CC_SYNTHESIZE(int, _vipgold, VIPGold);
    

    
    
    
    
    
        
    //排名
    CC_SYNTHESIZE(int, _rankCur, RankCur);
    
    //最大伤害（最高得分）
    CC_SYNTHESIZE(uint64_t, _damageMax, DamageMax);
    
    //根据索引获得某箱子的最大军魂值
    int getSoulMaxByIndex(SoulTag idx);
    
    //根据索引获得某箱子的当前军魂值
    int getSoulCurByIndex(SoulTag idx);
    
    //根据索引设置某箱子的当前军魂值
    void setSoulCurByIndex(SoulTag idx, int num);
    
    //拥有军魂数量
    CC_SYNTHESIZE(int, _soulAmount, SoulAmount);
    
    //传奇军魂数量
    CC_SYNTHESIZE(int, _soulCurOfS, SoulCurOfS);
    
    //传奇军魂开箱条件值
    CC_SYNTHESIZE(int, _soulMaxOfS, SoulMaxOfS);
    
    //精粹军魂数量
    CC_SYNTHESIZE(int, _soulCurOfA, SoulCurOfA);
    
    //精粹军魂开箱条件值
    CC_SYNTHESIZE(int, _soulMaxOfA, SoulMaxOfA);
    
    //荆州军魂数量
    CC_SYNTHESIZE(int, _soulCurOfB, SoulCurOfB);
    
    //荆州军魂开箱条件值
    CC_SYNTHESIZE(int, _soulMaxOfB, SoulMaxOfB);
    
    //本次军魂UI是否显示上次守荆州军魂收益
    CC_SYNTHESIZE(bool, _isShowBonus, IsShowBonus);
    
    //缓存的消耗敌人士气值
    CC_SYNTHESIZE(int, _bonusDamage, BonusDamage);
    
    //缓存的获得军魂数量
    CC_SYNTHESIZE(int, _bonusSoul, BonusSoul);
    
    //缓存的本次守荆州排名
    CC_SYNTHESIZE(int, _dailyRank, DailyRank);
    
    //缓存的本次守荆州元宝奖励
    CC_SYNTHESIZE(int, _dailyIngot, DailyIngot);
};

#endif /* defined(__GameSanGuo__SGSjzData__) */
