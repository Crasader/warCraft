//
//  SGAreanItemInfo.h
//  GameSanGuo
//  竞技场列表item
//  Created by angel on 14-12-18.
//
//

#ifndef __GameSanGuo__SGAreanItemInfo__
#define __GameSanGuo__SGAreanItemInfo__

#include <iostream>
#include "SNSTableViewCellItem.h"

class SGAreanItemInfoData;
class SGAreanItemInfo: public SNSTableViewCellItem
{
public:
    SGAreanItemInfo();
    ~SGAreanItemInfo();
    static SGAreanItemInfo * create(SGAreanItemInfoData * infodata, int type);
    void setItemInfoData(SGAreanItemInfoData * itemInfoData);
    SGAreanItemInfoData * getItemInfoData();
    void initView();
    void setType(int type){ m_type_arean = type; }
    
    void onClickButtonFight();
    void onClickButtonLook();
    
    void updateItem();
private:
    //void initView();
    SGAreanItemInfoData * m_pItemInfoData;
    CCSprite * m_pItemBg;
    int m_type_arean;
    
    SGButton * m_pButtonFight;
    SGButton * m_pButtonLook;
    SGButton * m_pButtonHeadIcon;
    
    
    SGCCLabelTTF * m_pLabelName;
    SGCCLabelTTF * m_pLabelLevel;//等级
    SGCCLabelTTF * m_pLabelRank;//排名
    SGCCLabelTTF * m_pLabelCombatValue;//战斗力
    SGCCLabelTTF * m_pLabelReward;//排名奖励
};

//
//  SGAreanItemInfoData
//  GameSanGuo
//  竞技场列表item data
//  Created by angel on 14-12-18.
//
//

class SGAreanItemInfoData: public CCObject
{
public:
    
    SGAreanItemInfoData();
    ~SGAreanItemInfoData();
    static SGAreanItemInfoData * create(int roleId, CCString * nickName, int nationId, int officeItemId,
                                        int level, int rank, int combatValue, int areanCoin);
    CC_SYNTHESIZE(int, m_roleId, RoleId);//角色id
    CC_SYNTHESIZE_RETAIN(CCString * , m_nickName, NickName);//玩家昵称
    CC_SYNTHESIZE(int, m_nationId, NationId);//国家id
    CC_SYNTHESIZE(int, m_officeItmeId, OfficeItemId);//主将id
    CC_SYNTHESIZE(int, m_level, Level);//玩家当前的等级
    CC_SYNTHESIZE(int, m_rank, Rank);//玩家的排名
    CC_SYNTHESIZE(int, m_combatValue, CombatValue);//战斗力
    CC_SYNTHESIZE(int, m_areanCoin, AreanCoin);//当前排名奖励的竞技场币
};

#endif /* defined(__GameSanGuo__SGAreanItemInfo__) */
