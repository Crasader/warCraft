//
//  SGDayTaskDataModel.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-26.
//
//

#ifndef __GameSanGuo__SGDayTaskDataModel__
#define __GameSanGuo__SGDayTaskDataModel__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class SGDayTaskDataModel : public CCObject
{
    CC_SYNTHESIZE_RETAIN(CCString*, str_beanId, taskbeanid);
    CC_SYNTHESIZE(int, m_taskId, taskid);
    CC_SYNTHESIZE(int, m_lvl, tasklvl);
    CC_SYNTHESIZE_RETAIN(CCString*, str_name, taskname);
    CC_SYNTHESIZE_RETAIN(CCString*, str_info, taskinfo);
    CC_SYNTHESIZE(int, m_count, taskcount);
    
public:
    
    SGDayTaskDataModel();
    
    ~SGDayTaskDataModel();    
};


#endif /* defined(__GameSanGuo__SGDayTaskDataModel__) */
