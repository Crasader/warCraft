//
//  SGGMMailLayer.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-27.
//
//

#ifndef __GameSanGuo__SGGMMailLayer__
#define __GameSanGuo__SGGMMailLayer__

#include <iostream>
#include "SGBaseLayer.h"
class SGGMMailLayer : public SGBaseLayer
{
public:
    SGGMMailLayer();
    
    ~SGGMMailLayer();
    
    static SGGMMailLayer *create();
    
};


#endif /* defined(__GameSanGuo__SGGMMailLayer__) */
