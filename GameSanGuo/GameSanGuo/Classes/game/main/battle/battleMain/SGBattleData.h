//
//  SGBattleData.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-8.
//
//

#ifndef __GameSanGuo__SGBattleData__
#define __GameSanGuo__SGBattleData__

#include "cocos2d.h"
#include "SGRankPlayer.h"

/* 战斗信息 -> PvpFight.proto */

class SGBattleData : public cocos2d::CCObject
{
public:
    SGBattleData(void);
    ~SGBattleData(void);
    
public:
    static SGBattleData* create(const char *tdesc, int maxl, int lian, int score, int shengw,int winrate,int rankshengw,const char* junxian,int jieshu , bool hasDayReward , bool hasSeasonReward);
    virtual bool initWithInfo(const char *tdesc, int maxl, int lian, int score, int shengw,int winrate,int rankshengw,const char* junxian,int jieshu , bool hasDayReway , bool hasSeasonReward);
    
private:
    CC_SYNTHESIZE_READONLY(const char*, _timeDesc, TimeDesc); // 截止时间
    CC_SYNTHESIZE_READONLY(int, _maxlian, MaxLiansheng);      // 最大连胜
     CC_SYNTHESIZE_READONLY(int, _lian, Liansheng);     // 连胜
    CC_SYNTHESIZE_READONLY(int, _score, Score);         // 个人积分
    CC_SYNTHESIZE_READONLY(int, _shengwang, Shengwang); // 天梯声望
    CC_SYNTHESIZE_READONLY(float, _winrate, Winrate); // 当前胜率
    CC_SYNTHESIZE_READONLY(int, _rankshengwang, Rankshengwang); // 声望排名
    CC_SYNTHESIZE_READONLY(const char*, _junxian, Junxian); // 当前军衔
    CC_SYNTHESIZE_READONLY(int, _jieshu, Jieshu); // 第几届
    
    CC_SYNTHESIZE_READONLY(bool, _hasSeasonReward, HasSeasonReward); // 是否有可领取的赛季奖励奖励
    CC_SYNTHESIZE_READONLY(bool, _hasDayReward, HasDayReward); // 是否有可领取的每日奖励
    CC_SYNTHESIZE_RETAIN(cocos2d::CCString * , m_pStartTime, StartTime);//三倍声望开始时间
    CC_SYNTHESIZE_RETAIN(cocos2d::CCString * , m_pEndTime, EndTime);//三倍声望结束时间
    
    
    
    
    
//    CC_SYNTHESIZE_READONLY(int, _timeMax, TimeMax);     // 今日参战最大数
//    CC_SYNTHESIZE_RETAIN(SGRankPlayer*, player, Player);// 玩家自己信息
//    CC_SYNTHESIZE_READONLY(int, _isOpen, IsOpen);      // 战斗是否开放
//    CC_SYNTHESIZE_READONLY(int, _timeTday, TimeTday); // 今日参战次数
};

#endif /* defined(__GameSanGuo__SGBattleData__) */
