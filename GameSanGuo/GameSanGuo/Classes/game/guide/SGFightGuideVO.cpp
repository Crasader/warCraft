//
//  SGFightGuideVO.cpp
//  GameSanGuo
//
//  Created by geng on 13-5-13.
//
//

#include "SGFightGuideVO.h"
SGFightGuideVO *SGFightGuideVO::create(int mid, int mtype, int mtag)
{
    SGFightGuideVO *vo = new SGFightGuideVO();
    if (vo && vo->init(mid, mtype, mtag))
    {
        vo->autorelease();
        return vo;
    }
    CC_SAFE_DELETE(vo);
    return NULL;
}
bool SGFightGuideVO::init(int mid, int mtype, int mtag)
{
    SGGuideVO::initWithInfo(mid, mtype, mtag);

    return true;
}