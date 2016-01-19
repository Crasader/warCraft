//
//  SGStoryBean.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-6.
//
//

#ifndef __GameSanGuo__SGStoryBean__
#define __GameSanGuo__SGStoryBean__

#include "cocos2d.h"
#include "SGStorySceneBean.h"

USING_NS_CC;

/* 关卡表 */ /*修正版*/ // 对应SGStoryMainCell
class SGStoryBean : public cocos2d::CCObject
{
    //关卡id
    CC_SYNTHESIZE_READONLY(int, _storyId, StoryId);
    //关卡名称
    CC_SYNTHESIZE_READONLY(CCString*, _storyName, StoryName);
    //关卡所含场景
    CC_SYNTHESIZE_READONLY(CCArray*, _storySections, StorySections);
    
    //闯关最低等级限制
    CC_SYNTHESIZE(int, _minLevel, MinLevel);
    
    /************服务端发过来的数据***********/
    //关卡是否显示
    CC_SYNTHESIZE(bool, _visible, Visible);
    //是否通关
    CC_SYNTHESIZE(int, _iswin, IsWin);
    //战斗结束评价： 1 险胜；2 小胜；3 胜利；4 大胜 5 完胜
    CC_SYNTHESIZE(int, _rank, Rank);
    //金钱加成
    CC_SYNTHESIZE(float, _coinRate, CoinRate);
    //经验加成
    CC_SYNTHESIZE(float, _expRate, ExpRate);
    //加成持续时间
    CC_SYNTHESIZE(int, _conTime, ConTime);
    
    CC_SYNTHESIZE(int, _activityState,ActivityState);
    
    CC_SYNTHESIZE(long, _startTime,ActivityStartTime);
    
    CC_SYNTHESIZE(long, _endTime,ActivityEndTime);
	
	//george精英副本数据--来自服务器
	CC_SYNTHESIZE(int, dayJoinTimes, CreamDayTimes);
	CC_SYNTHESIZE(int, buyTimes, CreamBuyTimes);
	CC_SYNTHESIZE(bool, canJoin, CreamCanJoin);
	CC_SYNTHESIZE(int, BuyCompTimes, BuyCompTimes);//今天已经购买的次数
	CC_SYNTHESIZE(int, buyPrice, BuyPrice);//每次购买的价格
	//精英副本_END/////////////////////////////////////////
    
    ////////////2013.12.26
    CC_SYNTHESIZE(int, _storyStars,StoryStars);
    CC_SYNTHESIZE(int, _canReward,CanReward);
    CC_SYNTHESIZE(int, _maxStar,MaxStar);
    CC_SYNTHESIZE(int, _rewardId,RewardId); //掉落奖励IDitemId
    
    ////////////2013.12.24
    CC_SYNTHESIZE(int, _rankStars,RankStars);
    CC_SYNTHESIZE(int, _rankReward,RankReward);
    
    CC_SYNTHESIZE(int, m_nChallengeCount,ChallengeCount);//副本挑战次数
    CC_SYNTHESIZE(int, m_nMaxChallengeCount,MaxChallengeCount);//最大次数

    
public:
    SGStoryBean(void);
    ~SGStoryBean(void);
    
    static SGStoryBean* create(int sid, const char* sname, CCArray *data);
    void initWithInfo(int sid, const char* sname, CCArray *data);
    
    void setSceneVisibleById(int plotId, bool visible);
    SGStorySceneBean* getSceneBeanById(int plotId);
};

#endif /* defined(__GameSanGuo__SGStoryBean__) */
