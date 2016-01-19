//
//  SGGift.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-14.
//
//

#include "SGGift.h"

SGGift::SGGift()
:m_id(0),
m_num(0),
str_name(NULL),
m_star(0),
m_advnum(0),
m_iconid(0),
m_type(-1)
{
}
SGGift::~SGGift()
{
    CC_SAFE_RELEASE(str_name);
}