//
//  SGMailSystemDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-5-9.
//
//

#ifndef __GameSanGuo__SGMailSystemDataModel__
#define __GameSanGuo__SGMailSystemDataModel__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;

class SGMailSystemDataModel : public CCObject
{
    
    CC_SYNTHESIZE(int, m_ssid, Ssid);
    CC_SYNTHESIZE(int, m_roleId, RoleId);
    CC_SYNTHESIZE(int, m_isRead, IsRead);
    CC_SYNTHESIZE(int, m_isReceieve, IsReceive);
    CC_SYNTHESIZE(long long, m_sendTime, SendTime);
    CC_SYNTHESIZE_RETAIN(CCString*, m_pContent, content);
    
private:
    CCArray *m_pAccessoryArray;
    
    
public:
    SGMailSystemDataModel();
    ~SGMailSystemDataModel();
    
    //添加对象进入数组
    void addAccessoryToArray(CCObject *object);
    
    //获取存放附件的数组
    CCArray *getAccessoryArray();
    
};


////////////////////////////////////////////////////////////////

/*
 
 message AttachmentProto {
 //附件主体
 optional int32 ssid = 1;
 optional int32 systemMailId = 2;
 optional int32 itemId = 3;
 optional int32 count = 4;
 optional int32 level = 5;
 //1:将领卡牌 2:装备卡牌 3:士兵卡牌 4:道具卡牌 5:铜钱 6:金子 7:军功 8:炼丹秘方 9:小喇叭
 optional int32 type = 6;
 optional OfficerCardItemProto officerCardItemProto = 7;
 optional EquipmentCardItemProto equipmentCardItemProto = 8;
 optional SoldierCardItemProto soldierCardItemProto = 9;
 optional PropsCardItemProto propsCardItemProto = 10;
 }
 
 */

class SGMailSystemAccessoryDataModel : public CCObject
{

    CC_SYNTHESIZE(int, m_ssid, Ssid);
    CC_SYNTHESIZE(int, m_SystemMailId, SystemMailId);
    CC_SYNTHESIZE(int, m_ItemId, ItemId);
    CC_SYNTHESIZE(int, m_Count, Count);
    CC_SYNTHESIZE(int, m_Level, Level);
      //6:将领卡牌 7:装备卡牌 8:士兵卡牌 9:道具卡牌 1:铜钱 0:金子 3:军功 5:炼丹秘方 4:小喇叭 10:碎片 11军魂 12体力 13军粮 14天梯积分 15 消耗品

    CC_SYNTHESIZE(int, m_Type, Type);
    
private:
    CCArray *m_pAccessoryArray;
    
    
public:
    SGMailSystemAccessoryDataModel();
    ~SGMailSystemAccessoryDataModel();

    //获取附件数组
    CCArray *getAccessoryArray();
};


#endif /* defined(__GameSanGuo__SGMailSystemDataModel__) */
