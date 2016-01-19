//
//  SGGMMailLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#include "SGGMMailLayer.h"
SGGMMailLayer::SGGMMailLayer()
{
    
}

SGGMMailLayer::~SGGMMailLayer()
{
    
}

SGGMMailLayer* SGGMMailLayer::create()
{
    SGGMMailLayer *sg = new SGGMMailLayer();
    if(sg && sg->init(NULL, sg_GMMail))
    {
        sg->autorelease();
        return sg;
    }
    
    CC_SAFE_DELETE(sg);
    return NULL;
}