//
//  SGRobotGuideVO.h
//  GameSanGuo
//
//  Created by geng on 13-5-15.
//
//

#ifndef __GameSanGuo__SGRobotGuideVO__
#define __GameSanGuo__SGRobotGuideVO__

#include "cocos2d.h"
USING_NS_CC;
class SGRobotGuideVO : public CCObject
{
public:
    SGRobotGuideVO() : rg_Type(0)
    {};
    CC_SYNTHESIZE(int, rg_Type, Type);//1:移出；2：移入；3：删除
    CC_SYNTHESIZE(CCPoint, rg_Point, Point);
public:
    static SGRobotGuideVO *create(int type, CCPoint point);
    virtual void init(int type, CCPoint point);
};
#endif /* defined(__GameSanGuo__SGRobotGuideVO__) */
