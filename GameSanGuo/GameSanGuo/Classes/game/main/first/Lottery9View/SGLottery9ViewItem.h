//
//  SGLottery9ViewItem.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-9.
//
//

#ifndef __GameSanGuo__SGLottery9ViewItem__
#define __GameSanGuo__SGLottery9ViewItem__

#include "cocos2d.h"
USING_NS_CC;
class SGLottery9ViewItem : public CCObject
{
public:
    SGLottery9ViewItem();
    ~SGLottery9ViewItem();
public:
//    CC_SYNTHESIZE(int, m_coin,CoinItem);
//    CC_SYNTHESIZE(int, m_speaker, SpeakerItem);
//    CC_SYNTHESIZE(int, m_liandan, LiandanItem);
    CC_SYNTHESIZE(int, m_itemId, ItemIDItem);
    CC_SYNTHESIZE(int, m_itemtype, ItemTypeItem);
    CC_SYNTHESIZE(int, m_itemNum, ItemNum);
};
class SGRewardNoticeItem : public CCObject
{
public:
    SGRewardNoticeItem();
    ~SGRewardNoticeItem();
public:
    CC_SYNTHESIZE(int, m_type,typeItem);
    CC_SYNTHESIZE(int, m_state, stateItem);
    
};

#endif /* defined(__GameSanGuo__SGLottery9ViewItem__) */
