//
//  SGVipGift.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-13.
//
//

#ifndef __GameSanGuo__SGVipGift__
#define __GameSanGuo__SGVipGift__

#include "cocos2d.h"
#include "SGGift.h"
USING_NS_CC;

class SGVipGift : public CCObject
{
private:
    CCArray *list;
public:
    CC_SYNTHESIZE(int, m_id, GiftID);
    CC_SYNTHESIZE_RETAIN(CCString *, str_name, GiftName);
    CC_SYNTHESIZE_RETAIN(CCString *, str_info, GiftInfo);
    CC_SYNTHESIZE_RETAIN(CCString *, str_condition, GiftCondition);
    
    
    
    
    CC_SYNTHESIZE(int, m_cost, GiftCost);
    CC_SYNTHESIZE(int, m_discount, GiftDiscount);
    CC_SYNTHESIZE(int, m_vipLvl, GiftVipLvl);
    CC_SYNTHESIZE(int, m_Coin, GiftCoin);
    CC_SYNTHESIZE(int, m_Award, GiftAward);
    CC_SYNTHESIZE(int, m_Speaker, GiftSpeak);
    CC_SYNTHESIZE(int, m_liandan, GiftLiandan);
    CC_SYNTHESIZE(int, m_haveBuy,HaveBuy);
    CC_SYNTHESIZE(int, m_pvpgifttype,Pvpgifttype);
    CC_SYNTHESIZE(int, m_activityid,ActivityId);
    CC_SYNTHESIZE(int, m_btnid,BtnId);
    void addGiftList(SGGift *gift);
    CCArray *getGiftList();
    SGVipGift();
    ~SGVipGift();
};

#endif /* defined(__GameSanGuo__SGVipGift__) */
