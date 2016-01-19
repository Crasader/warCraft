//
//  SGMaterialDependency.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-10-22.
//
//

#include "SGMaterialDependencyDataModel.h"


SGMaterialDependencyDataModel::SGMaterialDependencyDataModel():
_materialDependCondition(NULL),
_materialDependId(-1)
{

}

SGMaterialDependencyDataModel::~SGMaterialDependencyDataModel()
{
    CC_SAFE_RELEASE(_materialDependCondition);
}