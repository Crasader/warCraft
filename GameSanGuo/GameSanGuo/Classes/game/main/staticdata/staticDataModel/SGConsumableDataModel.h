//
//  SGConsumableDataModel.h
//  GameSanGuo
//
//  Created by bugcode on 14-2-13.
//
//

#ifndef __GameSanGuo__SGConsumableDataModel__
#define __GameSanGuo__SGConsumableDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

 //道具表
class SGConsumableDataModel : public CCObject
{
public:
	SGConsumableDataModel();
	~SGConsumableDataModel();
	
	CC_SYNTHESIZE(int, _consumeId, ConsumId);//道具的ID
	CC_SYNTHESIZE_RETAIN(CCString *, _consumeName, ConsumeName);//道具名称
	CC_SYNTHESIZE(int, _consumeType, ConsumeType);//道具类型
	CC_SYNTHESIZE(int, _canUseDriving, CanUseDriving);//是否能主动使用
	CC_SYNTHESIZE(int, _consumeStarLevel, ConsumeStarLvl);//道具星级
	CC_SYNTHESIZE_RETAIN(CCString*, _consumeDesc, ConsumeDesc);//道具描述
	CC_SYNTHESIZE(int, _moneyType, MoneyType);//卖出后获得货币的类型（元宝还是铜币）
	CC_SYNTHESIZE(int, _consumePrice, ConsumePrice);//单个道具价格
	CC_SYNTHESIZE(int, _iconId, IconId);//小图标的ID
	CC_SYNTHESIZE(int, _consumePicId, ConsumePicId);//大图标的ID
	CC_SYNTHESIZE_RETAIN(CCString*, _limitAndNum, LimitAndNum);//使用限制和数量
    CC_SYNTHESIZE_RETAIN(CCString*, _consumeJokeInfo, ConsumeJokeInfo);//使用限制和数量

};




#endif /* defined(__GameSanGuo__SGConsumableDataModel__) */
