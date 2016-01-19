//
//  SGStorySceneBean.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-6.
//
//

#ifndef __GameSanGuo__SGStorySceneBean__
#define __GameSanGuo__SGStorySceneBean__

#include "cocos2d.h"

USING_NS_CC;

/* 场景类 */ // 对应SGStorySectionCell
class SGStorySceneBean : public CCObject
{
    /************客户端初始化数据***********/
    //关卡id
    CC_SYNTHESIZE_READONLY(int, _storyId, StoryId);
    //关卡-场景id
    CC_SYNTHESIZE_READONLY(int, _sceneId, SceneId);
    //场景主id
    CC_SYNTHESIZE_READONLY(int, _plotId, PlotId);
    //进入场景所需体力
    CC_SYNTHESIZE_READONLY(int, _needPower, NeedPower);
    //场景名称
    CC_SYNTHESIZE_READONLY(CCString*, _sceneName, SceneName);
    //场景金钱掉落
    CC_SYNTHESIZE_READONLY(int, _dropCoin, DropCoin);
    //场景经验掉落
    CC_SYNTHESIZE_READONLY(int, _dropExp, DropExp);
	
    
    //场景是否显示
    CC_SYNTHESIZE(bool, _visible, Visible);
    //局数 boss开启次数，默认为1，暂时无用
    CC_SYNTHESIZE(int, _sceneCount, SceneCount);
    //战斗结束评价： 0 未通过；1 险胜；2 小胜；3 胜利；4 大胜 5 完胜
    CC_SYNTHESIZE(int, _rank, Rank);
	
	//精英副本
	CC_SYNTHESIZE(int, dayJoinTimes, CreamDayTimes);
	CC_SYNTHESIZE(int, buyTimes, CreamBuyTimes);
	CC_SYNTHESIZE(bool, canJoin, CreamCanJoin);
	CC_SYNTHESIZE(int, BuyCompTimes, BuyCompTimes);//今天已经购买的次数
	CC_SYNTHESIZE(int, buyPrice, BuyPrice);//每次购买的价格
	
    
    
     /************服务器下发数据***********/
    //体力消耗
    CC_SYNTHESIZE(int, _power, Power);
    
    //战斗力
    CC_SYNTHESIZE(int, _zhandou, Zhandou);
    
    //经验
    CC_SYNTHESIZE(int, _exp, Exp);
    
    //金钱
    CC_SYNTHESIZE(int, _gold, Gold);
    
    //是否可点击进入
    CC_SYNTHESIZE(int, _openState, OpenState);
    //掉落物品IDitemId
    std::vector<int> itemIds;
	
	//是否打过本关,用于boss关打完弹出新章节已开启的提示
	CC_SYNTHESIZE(bool, _isBeatIt, IsBeatIt);
    
    
 
public:
    SGStorySceneBean(void);
    ~SGStorySceneBean(void);
    
    static SGStorySceneBean* create(int sid, int scid, int pid, int power, const char* sname);
    void initWithInfo(int sid, int scid, int pid, int power, const char* sname);
    
    
public:
    void setDropInfo(int exp, int coin);
};

#endif /* defined(__GameSanGuo__SGStorySceneBean__) */
