//
//  SGHelpTitleDataModel.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-8-7.
//
//

#include "SGHelpTitleDataModel.h"
SGHelpTitleDataModel::SGHelpTitleDataModel()
{
    m_beg = 0;
    m_end = 0;
    str_info = NULL;
    m_num = 0;
}
SGHelpTitleDataModel::~SGHelpTitleDataModel()
{
    CC_SAFE_RELEASE(str_info);
}