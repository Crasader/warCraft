//
//  SGMaterialDataModel.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#include "SGMaterialDataModel.h"




SGMaterialDataModel::SGMaterialDataModel():
_materialBorderColor(0),
_materialPileLimit(0),
_materialSoldPrice(0),
_materialIconId(-1),
_materialId(-1),
_materialName(NULL),
_materialDesc(NULL),
_materialJokeInfo(NULL)
{
    
}

SGMaterialDataModel::~SGMaterialDataModel()
{
    CC_SAFE_RELEASE(_materialName);
    CC_SAFE_RELEASE(_materialJokeInfo);
    CC_SAFE_RELEASE(_materialDesc);

}