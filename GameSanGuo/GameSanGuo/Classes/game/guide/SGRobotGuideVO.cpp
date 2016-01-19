//
//  SGRobotGuideVO.cpp
//  GameSanGuo
//
//  Created by geng on 13-5-15.
//
//

#include "SGRobotGuideVO.h"
SGRobotGuideVO *SGRobotGuideVO::create(int type, cocos2d::CCPoint point)
{
    SGRobotGuideVO *vo = new SGRobotGuideVO();
    vo->init(type, point);
    vo->autorelease();
    return vo;
}
void SGRobotGuideVO::init(int type, cocos2d::CCPoint point)
{
    this->setType(type);
    this->setPoint(point);
}