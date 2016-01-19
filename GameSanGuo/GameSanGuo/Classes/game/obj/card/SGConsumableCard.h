//
//  SGConsumableCard.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-13.
//
//

#ifndef __GameSanGuo__SGConsumableCard__
#define __GameSanGuo__SGConsumableCard__

#include "SGBaseMilitaryCard.h"

class SGConsumableCard : public SGBaseMilitaryCard
{
public:
	SGConsumableCard();
	~SGConsumableCard();
	virtual const char * getClassName(){return "SGConsumableCard";};
	CC_SYNTHESIZE(int, _canUseDriving, CanUseDriving);//道具卡能否主动使用
	CC_SYNTHESIZE_RETAIN(CCString*, _consumableDesc, consumableDesc);//道具卡描述
	CC_SYNTHESIZE(int , _moneyType, MoneyType);//货币类型， 卖出时货币是元宝还是铜币
	CC_SYNTHESIZE_RETAIN(CCString*, _limitAndNum, LimitAndNum);//道具使用限制
	CC_SYNTHESIZE(int, _currNum, CurrNum);//道具当前的数量
	CC_SYNTHESIZE(int, _picId, PicId);// 大图标ID
};


#endif /* defined(__GameSanGuo__SGConsumableCard__) */
