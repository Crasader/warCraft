//
//  SGQAConcent.h
//  GameSanGuo
//
//  Created by kanata on 14-1-7.
//
//

#ifndef __GameSanGuo__SGQAConcent__
#define __GameSanGuo__SGQAConcent__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class SGQAConcent : public CCObject
{
public:
    
    CC_SYNTHESIZE(int, m_id, ID);
    CC_SYNTHESIZE_RETAIN(CCString*, m_title, Title);
    CC_SYNTHESIZE(int, m_type, Type);
    CC_SYNTHESIZE_RETAIN(CCString*, m_createdate, createDate);
    CC_SYNTHESIZE(int, state, State);
    //CC_SYNTHESIZE(int, questionstate, QAState);
  
    
    SGQAConcent();
    ~SGQAConcent();
    
private:
};




















#endif /* defined(__GameSanGuo__SGQAConcent__) */
