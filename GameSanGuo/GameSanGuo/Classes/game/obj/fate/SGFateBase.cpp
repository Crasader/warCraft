//
//  SGFateBase.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 11/9/13.
//
//

#include "SGFateBase.h"

//创建
SGFateBase::SGFateBase() : m_name(NULL), m_conditionString(NULL), m_conditionType(FCT_ERROR) , m_id(0) , m_isOn(0), m_sourceFateInfo(NULL)
{
    this->autorelease();
}

//析构
SGFateBase::~SGFateBase()
{
    ;
}