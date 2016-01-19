//
//  SGConsumableDataModel.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-13.
//
//

#include "SGConsumableDataModel.h"

SGConsumableDataModel::SGConsumableDataModel():
_consumeId(0),
_limitAndNum(NULL),
_moneyType(1),
_canUseDriving(0),
_consumeStarLevel(0),
_consumeDesc(NULL),
_consumeName(NULL),
_consumeType(-1),
_consumePicId(0),
_consumePrice(0),
_iconId(0),
_consumeJokeInfo(NULL)
{
	
}
SGConsumableDataModel::~SGConsumableDataModel()
{
	CC_SAFE_RELEASE(_limitAndNum);
	CC_SAFE_RELEASE(_consumeName);
	CC_SAFE_RELEASE(_consumeDesc);
    CC_SAFE_RELEASE(_consumeJokeInfo);

}