//
//  SGHaveCards.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-10.
//
//

#ifndef __GameSanGuo__SGHaveCards__
#define __GameSanGuo__SGHaveCards__

#include "cocos2d.h"
USING_NS_CC;
class SGHaveCards : public CCObject
{
public:
    CC_SYNTHESIZE(int, m_itemId, cardItem);
    CC_SYNTHESIZE(int, m_state, cardState);
    CC_SYNTHESIZE(int, m_position, cardPosition);
public:
    SGHaveCards();
    ~SGHaveCards();    
};

#endif /* defined(__GameSanGuo__SGHaveCards__) */
