//
//  SGHelpTitleDataModel.h
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-8-7.
//
//

#ifndef __GameSanGuo__SGHelpTitleDataModel__
#define __GameSanGuo__SGHelpTitleDataModel__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class SGHelpTitleDataModel : public CCObject
{
    CC_SYNTHESIZE(int, m_beg, beg);
    CC_SYNTHESIZE(int, m_end, end);
    CC_SYNTHESIZE_RETAIN(CCString*, str_info, title);
    CC_SYNTHESIZE(int, m_num,num);
    
public:
    
    SGHelpTitleDataModel();
    
    ~SGHelpTitleDataModel();
};

#endif /* defined(__GameSanGuo__SGHelpTitleDataModel__) */
