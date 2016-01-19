//
//  SGDynamicModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-8.
//
//

#ifndef __GameSanGuo__SGDynamicModel__
#define __GameSanGuo__SGDynamicModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

class SGDynamicModel : public CCObject
{
    CC_SYNTHESIZE(int, dyId, DyId);
private:
    CCArray *dyArray;
    
public:
    
    CCArray *getDyArray();
    
    SGDynamicModel();
    
    ~SGDynamicModel();
    
};


#endif /* defined(__GameSanGuo__SGDynamicModel__) */
