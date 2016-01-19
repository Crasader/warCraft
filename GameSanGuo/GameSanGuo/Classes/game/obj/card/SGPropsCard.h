//
//  SGPropsCard.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#ifndef __GameSanGuo__SGPropsCard__
#define __GameSanGuo__SGPropsCard__

#include "SGBaseMilitaryCard.h"
class SGPropsCard : public SGBaseMilitaryCard
{
public:
    virtual const char * getClassName(){return "SGPropsCard";};
    CC_SYNTHESIZE(int , c_type, Type);
    CC_SYNTHESIZE(int, c_value, Value);
    CC_SYNTHESIZE_RETAIN(CCString *, m_str, propInfo)
    CC_SYNTHESIZE(int, c_curNum, CurNum);
    SGPropsCard();
    ~SGPropsCard();    
};
#endif /* defined(__GameSanGuo__SGPropsCard__) */
