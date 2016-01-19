//
//  SGEquipmentDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-22.
//
//

#ifndef __GameSanGuo__SGEquipmentDataModel__
#define __GameSanGuo__SGEquipmentDataModel__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

/* 装备表 *
 
    装备id
    装备基础攻击力
    装备基础防御力
    血量值
    回合数
    基础速度
    统御力
    当前星级最大等级
    当前星级
    beanId 例如 equipmentCardItem/1
    装备名称
    装备类型
    经验类型
 
 */

class SGEquipmentDataModel : public CCObject
{
    CC_SYNTHESIZE(int, equipmentId, EquipmentId);
    CC_SYNTHESIZE(float, equipmentBaseAtk, EquipmentBaseAtk);//
    CC_SYNTHESIZE(float, equipBaseDef, EquipBaseDef);//
    CC_SYNTHESIZE(float, equipHP, EquipHP);//
    CC_SYNTHESIZE(int, equipRound, EquipRound);
    CC_SYNTHESIZE(float, equipBaseSpeed, EquipBaseSpeed);//
    CC_SYNTHESIZE(int, equipGoverning, EquipGoverning);
    CC_SYNTHESIZE(int, equipMaxLevel, EquipMaxLevel);
    CC_SYNTHESIZE(int, equipCurrStarLevel, EquipCurrStarLevel);
    CC_SYNTHESIZE_RETAIN(CCString*, equipBeanId, EquipBeanId);
    CC_SYNTHESIZE_RETAIN(CCString*, equipName, EquipName);
    CC_SYNTHESIZE(int, equipType, EquipType);
    CC_SYNTHESIZE(int, equipNum, EquipNum);
    CC_SYNTHESIZE_RETAIN(CCString*, equipExpType, EquipExpType);
    CC_SYNTHESIZE(float,m_equipgrowvalue,EquipGrowValue);
    CC_SYNTHESIZE_RETAIN(CCString*, str_req, ReqItems);
    CC_SYNTHESIZE(int, m_nextid, EquipNextId);
    CC_SYNTHESIZE(int, m_cost, EquipCost);
    CC_SYNTHESIZE(float, m_exprate, EquipExpRate);
    CC_SYNTHESIZE(int, m_visittype, EquipVisitType);
    CC_SYNTHESIZE(int, m_equipProtoId, EquipProtoId);
    //转生等级
    CC_SYNTHESIZE(int, upgradelevel, Upgradelevel);
    //  武将最大星级
    CC_SYNTHESIZE(int, upgradestar, Upgradestar);
	
	//头像id
	CC_SYNTHESIZE(int, m_IconId, IconId);
    //转生次数+n
	CC_SYNTHESIZE(int, m_advnum, AdvNum);
    
    //转生需要材料组的id
    CC_SYNTHESIZE(int, m_materialDependencyId, MaterialDependencyId);
    
    //武将缘分
    CC_SYNTHESIZE_RETAIN(CCString *, m_fateDesc, FateDesc);
    
public:
    
    SGEquipmentDataModel();
    
    ~SGEquipmentDataModel();
    
    int getEquipStarPinzhi();
    
    
};

#endif /* defined(__GameSanGuo__SGEquipmentDataModel__) */
