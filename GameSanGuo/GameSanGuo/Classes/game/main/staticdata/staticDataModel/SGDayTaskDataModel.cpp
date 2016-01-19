//
//  SGDayTaskDataModel.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-26.
//
//

#include "SGDayTaskDataModel.h"
SGDayTaskDataModel::SGDayTaskDataModel()
:m_count(0),
m_lvl(0),
m_taskId(0),
str_info(NULL),
str_name(NULL),
str_beanId(NULL)
{
    
}

SGDayTaskDataModel::~SGDayTaskDataModel()
{
    CC_SAFE_RELEASE(str_name);
    CC_SAFE_RELEASE(str_info);
    CC_SAFE_RELEASE(str_beanId);
}