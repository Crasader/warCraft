//
//  SGBossBattleDetailLayer.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-25.
//
//

#ifndef __GameSanGuo__SGBossBattleDetailLayer__
#define __GameSanGuo__SGBossBattleDetailLayer__

#include <iostream>
#include "SGBaseTableLayer.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SGBaseBox.h"
USING_NS_CC;

typedef struct bossState : public CCObject
{
    bossState()
    :bossCurHp(0),
    bossMaxHp(0),
    rankData(NULL)
    {
        
    }
	int bossCurHp;//boss当前的血量
	int bossMaxHp;//boss最大血量
	//int bossHurtTimes;//boss被伤害的次数
	CCArray *rankData;//排行信息
}SGBossStateData;

typedef struct killInfo : public CCObject
{
    killInfo()
    :mins(0),
    playerName(""),
    killDamage(0),
    selfDamage(0)
    {
        
    }
    
	int mins;
	std::string playerName;
	int killDamage;
	int selfDamage;
	
}SGKillBossInfo;

typedef struct bossRankList : public CCObject
{
    
    bossRankList()
    :roleId(0),
    roleName(""),
    level(0),
    icon(0),
    country(0),
    damage(0),
    rank(0),
    loc(0)
    {
        
    }
    
	int roleId;
	std::string roleName ;
	int level;
	int icon;
	int country;
	int damage;
	int rank;
	int loc;//排位
}SGBossRankListData;

typedef struct BossRankData : public CCObject
{
    BossRankData()
    :roleId(0),
    roleName(""),
    roleLvl(0),
    roleHurt(0)
    {
        
    }
    
	int roleId;//角色id
	std::string	roleName;//角色名称
	int roleLvl;//角色等级
	int roleHurt;//伤害值
}SGBossRankData;

typedef struct BossBuffData : public CCObject
{
    BossBuffData():
    buffId(0),
    buffName(""),
    buffNeedMoney(0),
    buffValue(0)
    {
        
    }
	//buff相关
	int buffId;//buff的id
	std::string buffName;//buff的名称
	int buffNeedMoney;//buff需要的钱数
	int buffValue;//buff 属性值
	
}SGBossBuffData;

typedef struct BossBattleDataS : public CCObject
{
    BossBattleDataS()
    :bossCurrHp(0),
    bossMaxHp(0),
    residueSec(0),
    bossHurtTimes(0),
    value1(0),
    value2(0),
    value3(0),
    currentHurt(0),
    rankData(NULL),
    buffData(NULL)
    {
        
    }
	int bossCurrHp;//Boss当前hp
	int bossMaxHp;//boss最大hp
	int residueSec;//剩余秒数
	int bossHurtTimes;//boss被伤害次数
	int value1;
	float value2;
	int value3;
	int currentHurt;//当前玩家对boss的伤害量
	
	CCArray *rankData;//排名信息
	CCArray *buffData;//buff信息
}SGBossBattleData;


class SGBossBattleDetailLayer : public SGBaseLayer
{
public:
	SGBossBattleDetailLayer();
	~SGBossBattleDetailLayer();
	
	static SGBossBattleDetailLayer *create(int enterType = 0, SGBossBattleData *bossData = NULL);
	void initView();
	void initData(SGBossBattleData *data = NULL);
	
    void startbossFightListener(CCObject *obj);
	void startBossFight();//开始boss战
	void exitBossFight();//退出boss战
	void updateTimer();//更新倒计时显示
	void setButtonState(CCObject *obj);//设置点击四个按钮的状态
	void unpressBuffButton();
	void bossBattleReadMe();
	void flushBossInfo(CCObject *sender);
	void refreshBossView(SGBossStateData *data);
	void getBossBuffAndChange(CCObject *sender);
	void freshBuff(int v1, float v2, int v3);
	void cantPressStartFight();
	
	void firstEntryBossBattle();
	void sendFreshBossRankMsg();
	int needGold(int tag);
//	SNSTableViewCellItem *tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath);
//	unsigned int tableViewAllItemsNumber(SNSTableView* tableView, int section);
//	void tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath);
    virtual void setIsCanTouch(bool isTouch);
private:
	CCSprite *blood;//血条显示
	CCLabelTTF *atkNumLabel;//攻击
	CCLabelTTF *bombNumLabel;//爆击
	CCLabelTTF *bombPerLabel;//爆击率
    
    CCLabelTTF *bomb;
    CCLabelTTF *bombPercent;
    
	SGCCLabelTTF *bloodPercent;//血量百分比
	SGCCLabelTTF *info4;//伤害次数
	SGCCLabelTTF *info3;//伤害量
	int enterType;//预留
	CCSprite *headIcon;//向上飘飞的头像
	CCScrollView *m_scrollview;//底部的排行榜滑动区
	long long backTimer;//复活倒计时
	CCLabelTTF *timer;//倒计时显示
	bool pressAnyButton;//点击了任意四个按钮中的一个
	SGButton *startFight;//开始战斗
	SGButton *exitButton;
	
	int atkNum;//被攻击的次数
	int hurtBlood;//伤血量
	int atkMultiple;//攻击倍数
	float bombMultiple;//爆击倍数
	int bombPercentage;//爆击率
	
	//四个按钮下对应的元宝数字
	int goldNum1;
	int goldNum2;
	int goldNum3;
	int goldNum4;
	
	int value1;
	int value2;
	int value3;
	int m_bufVipLevelLimit[4];//4个buff的vip等级限制。
	//是否已经打过一次boss战
	bool isFightBoss;
	//
	SGBossBattleData *bossDatas;
};


//提示说明框
class SGBossTips : public SGBaseBox
{
public:
	SGBossTips();
	~SGBossTips();
	static SGBossTips *create(SGBoxDelegate *del = NULL, int enter = 0);
	void initView();
	void pressOkButton();
};





class SGRateItem : public CCLayer
{
public:
	SGRateItem();
	~SGRateItem();
	static SGRateItem *create(CCString *name, int order, int level, int fightPower);
	void initView();
	
private:
	CCString *name;
	int order;
	int lvl;
	int fightPower;
};









#endif /* defined(__GameSanGuo__SGBossBattleDetailLayer__) */
