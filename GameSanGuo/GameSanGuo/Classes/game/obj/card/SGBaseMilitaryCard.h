//
//  SGBaseMilitaryCard.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#ifndef __GameSanGuo__SGBaseMilitaryCard__
#define __GameSanGuo__SGBaseMilitaryCard__

#include "cocos2d.h"
#include "SGBaseItemType.h"

USING_NS_CC;
class SGBaseMilitaryCard : public CCObject
{
public:
    virtual const char * getClassName(){return "SGBaseMilitaryCard";};
    //    统御力
    CC_SYNTHESIZE(int, g_governing,Governing);
    //    数据库ID
    CC_SYNTHESIZE(int, g_ssid, Ssid);
    //    图片ID
    CC_SYNTHESIZE(int, g_itemId, ItemId);
//    最大等级
    CC_SYNTHESIZE(int, g_maxLevel,MaxLevel);
//    攻击力
    CC_SYNTHESIZE(int, g_atk,Atk);
//   防御力
    CC_SYNTHESIZE(int, g_def,Def);
//    蓄力
    CC_SYNTHESIZE(int, g_round,Round);
//  所属国家
    CC_SYNTHESIZE(int, g_race, Race);
//    血量
    CC_SYNTHESIZE(int, g_morale,Morale);
//    强化所加经验
    CC_SYNTHESIZE(float, g_expRate,ExpRate);
//    当前等级
    CC_SYNTHESIZE(int, g_currLevel,CurrLevel);
//    当前星级
    CC_SYNTHESIZE(int, g_currStar,CurrStar);
//    当前经验
    CC_SYNTHESIZE(int, g_currExp,CurrExp);
//    最大经验
    CC_SYNTHESIZE(int, g_maxExp, MaxExp)
//    售价
    CC_SYNTHESIZE(int, g_sell,Sell);
//    是否为最大星级
    CC_SYNTHESIZE(int, g_maxstar, MaxStar);
//    速度
    CC_SYNTHESIZE(float, g_speed,Speed);
//    保护
    CC_SYNTHESIZE(int, g_isprot,IsProt);
//    名字
    CC_SYNTHESIZE_RETAIN(CCString*, g_OfficerName, OfficerName);
//   卡牌类型
    CC_SYNTHESIZE(int, m_itemType, ItemType);
//   转生最小等级
    CC_SYNTHESIZE(int, upgradelevel,Upgradelevel);
//  最大星级
    CC_SYNTHESIZE(int, upgradestar, Upgradestar);
//被动技最大等级
    CC_SYNTHESIZE(int, skillmaxlevel, SkillMaxLevel);
//主将计最大等级
    CC_SYNTHESIZE(int, lordmaxlevel, LordMaxLevel);
//   进阶花费
    CC_SYNTHESIZE(int, m_cost, Cost);
    CC_SYNTHESIZE(int, m_select, selectposition);
	
	CC_SYNTHESIZE(int, m_headSuffixNum, HeadSuffixNum);//headIcon后缀数字//12.05
// 转生+n数据
     CC_SYNTHESIZE(int, m_adnum, AdNum);
    //可堆叠物品数量
    CC_SYNTHESIZE(int, m_canAddNum, CanAddNum);
    
    //出售获得铜钱
    CC_SYNTHESIZE(int, m_sellPrice, SellPrice);
    
    //原型ID
    CC_SYNTHESIZE(int, m_protoId, ProtoId);
    
    //MM:: 本物品的数量。因为CIB只接受这个类及其子类当做参数，而月签奖励界面的又需要特别显示堆叠数量这个东西，所以为了防止较多的修改，在这里将物品数量带上，暂勿用作其他地方。
    CC_SYNTHESIZE(int, m_itemNum, ItemNum);
    
    SGBaseMilitaryCard();
    ~SGBaseMilitaryCard();
    
};

#endif /* defined(__GameSanGuo__SGBaseMilitaryCard__) */
