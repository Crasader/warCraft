//
//  SGConsumableCard.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-13.
//
//

#include "SGConsumableCard.h"

SGConsumableCard::SGConsumableCard():
_canUseDriving(false),
_consumableDesc(NULL),
_moneyType(1),
_limitAndNum(NULL),
_currNum(0),
_picId(0)
{
	
}
SGConsumableCard::~SGConsumableCard()
{
	CC_SAFE_RELEASE(_limitAndNum);
	CC_SAFE_RELEASE(_consumableDesc);
}