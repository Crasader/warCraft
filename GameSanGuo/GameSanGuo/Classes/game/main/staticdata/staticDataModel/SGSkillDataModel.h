//
//  SGSkillDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-21.
//
//

#ifndef __GameSanGuo__SGSkillDataModel__
#define __GameSanGuo__SGSkillDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 技能表 */

class SGSkillDataModel :public CCObject
{

    //技能Id
    CC_SYNTHESIZE(int, skillId, SkillId);
    
    //回合数
    CC_SYNTHESIZE(int, round, Round);
    
    
    CC_SYNTHESIZE_RETAIN(CCString*, beanId, BeanId);
    
    //技能所属人物名称
    CC_SYNTHESIZE_RETAIN(CCString*, roleName, RoleName);
    
    //技能介绍
    CC_SYNTHESIZE_RETAIN(CCString*, skillInfo, SkillInfo);
    
    //技能名称
    CC_SYNTHESIZE_RETAIN(CCString*, skillName, SkillName);
    
    //主动技代表等级上限   被动技能代表当前等级   坑爹玩意。
    CC_SYNTHESIZE(int, skillLevel, SkillMaxLevel);
    
    //客户端等级，7级的时候是个“顶”字
    CC_SYNTHESIZE_RETAIN(CCString*, skillClientLevel, SkillClientLevel);
    
    //升级编码
    CC_SYNTHESIZE(int, skillUpId, SkillUpId);
    
    //技能级别
    CC_SYNTHESIZE(int, skillType, SkillType);
    
    //被动技图片
    CC_SYNTHESIZE(int, skillitemid, SkillItemId);
    
    //被动技星级
    CC_SYNTHESIZE(int, skillstar, SkillStarLvl);
    
    //下级id
    CC_SYNTHESIZE(int, nextid, NextId);
    
    //需要经验(武\主)
    CC_SYNTHESIZE(int, needexp, NeedExp);
    
    //需要铜钱
    CC_SYNTHESIZE(int, needcoin, NeedCoin);
    
    //只表示主将计等级
    CC_SYNTHESIZE(int, level, Level);
    
    //主将计提供经验
    CC_SYNTHESIZE(int, haveexp, ProExp);
    
    //主将计或者被动技的图片
    CC_SYNTHESIZE(int, iconid, IconId);

    
    CC_SYNTHESIZE(float, value1, Value1);
    CC_SYNTHESIZE(float, value2, Value2);
    CC_SYNTHESIZE(int, effectRound, EffectRound);
    CC_SYNTHESIZE(int, buff, Buff);
    //CC_SYNTHESIZE(int, clientType, ClientType);
    //CC_SYNTHESIZE(int, atkModeSkill, AtkModeSkill);
    //CC_SYNTHESIZE(int, belongId, BelongId);
    //技能优先级(目前只有武将及有)
    CC_SYNTHESIZE(int, skillPriority, SkillPriority);
    
public:
    
    SGSkillDataModel();
    
    ~SGSkillDataModel();
    
};

#endif /* defined(__GameSanGuo__SGSkillDataModel__) */
