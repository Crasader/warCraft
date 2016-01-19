//
//  SGGift.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-14.
//
//

#ifndef __GameSanGuo__SGGift__
#define __GameSanGuo__SGGift__

#include "cocos2d.h"
USING_NS_CC;
class SGGift : public CCObject
{
public:
    CC_SYNTHESIZE(int, m_id,ItemId);
    CC_SYNTHESIZE(int, m_num, Num);
    CC_SYNTHESIZE_RETAIN(CCString *, str_name, Name);
    CC_SYNTHESIZE(int, m_star,Star);
    CC_SYNTHESIZE(int, m_advnum,Advnum);
    CC_SYNTHESIZE(int, m_iconid,Iconid);
    CC_SYNTHESIZE(int, m_type,Type);
    SGGift();
    ~SGGift();
};

#endif /* defined(__GameSanGuo__SGGift__) */
