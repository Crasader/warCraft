//
//  SGSuitDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-2.
//
//

#ifndef __GameSanGuo__SGSuitDataModel__
#define __GameSanGuo__SGSuitDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 套装表 */

class SGSuitDataModel : public CCObject
{
    //套装id
    CC_SYNTHESIZE(int, suitId, SuitId);
    
    //套装件数
    CC_SYNTHESIZE(int, suitCount, SuitCount);
    
    //套装名字
    CC_SYNTHESIZE_RETAIN(CCString*, suitName, SuitName);
  
    //套装武器
    CC_SYNTHESIZE_RETAIN(CCString*, suitArmsName, SuitArmsName);
    
public:
    SGSuitDataModel();
    
    ~SGSuitDataModel();
    
    
};

#endif /* defined(__GameSanGuo__SGSuitDataModel__) */
