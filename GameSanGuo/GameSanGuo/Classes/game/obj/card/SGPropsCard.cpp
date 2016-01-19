//
//  SGPropsCard.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//

#include "SGPropsCard.h"
SGPropsCard::SGPropsCard()
:c_type(0),
c_value(0),
m_str(NULL),
c_curNum(0)
{
}
SGPropsCard::~SGPropsCard()
{
    CC_SAFE_RELEASE(m_str);
}