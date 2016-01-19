//
//  SGMaterialDependency.h
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#ifndef __GameSanGuo__SGMaterialDependencyDataModel__
#define __GameSanGuo__SGMaterialDependencyDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;


class SGMaterialDependencyDataModel : public CCObject
{
public:
    SGMaterialDependencyDataModel();
    ~SGMaterialDependencyDataModel();

private:
    CC_SYNTHESIZE(int, _materialDependId, MaterialDependId);
    CC_SYNTHESIZE_RETAIN(CCString *, _materialDependCondition, MaterialDependCondition);
};


#endif /* defined(__GameSanGuo__SGMaterialDependency__) */
