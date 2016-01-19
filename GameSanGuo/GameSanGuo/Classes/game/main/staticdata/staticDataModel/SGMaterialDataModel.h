//
//  SGMaterialDataModel.h
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#ifndef __GameSanGuo__SGMaterialDataModel__
#define __GameSanGuo__SGMaterialDataModel__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class SGMaterialDataModel : public CCObject
{
public:
    SGMaterialDataModel();
    ~SGMaterialDataModel();
private:
    CC_SYNTHESIZE(int, _materialId, MaterialId);//材料的id
    CC_SYNTHESIZE_RETAIN(CCString *, _materialName, MaterialName);//材料的名称
    CC_SYNTHESIZE(int, _materialIconId, MaterialIconId);//对应的这个材料的iconid
    CC_SYNTHESIZE(int, _materialBorderColor, MaterialBorderColor );//材料icon的边框颜色
    CC_SYNTHESIZE(int, _materialPileLimit, MaterialPileLimit);//材料的堆叠上限
    CC_SYNTHESIZE(int, _materialSoldPrice, MaterialSoldPrice);//材料的价格
    CC_SYNTHESIZE_RETAIN(CCString *, _materialDesc, MaterialDesc);//材料的名称
    CC_SYNTHESIZE_RETAIN(CCString *, _materialJokeInfo, MaterialJokeInfo);//材料的名称

};



#endif /* defined(__GameSanGuo__SGMaterialDataModel__) */
