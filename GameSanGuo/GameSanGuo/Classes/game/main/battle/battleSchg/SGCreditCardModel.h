//
//  SGCreditCardModel.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-25.
//
//

#ifndef __GameSanGuo__SGCreditCardModel__
#define __GameSanGuo__SGCreditCardModel__

#include "cocos2d.h"

class SGCreditCardModel : public cocos2d::CCObject
{
public:
    SGCreditCardModel(void);
    ~SGCreditCardModel(void);
    
    static SGCreditCardModel* create(int uid, int level, int cost, int ccount, int mcount);
    virtual bool init(int uid, int level, int cost, int ccount, int mcount);
    
    // 卡牌类型 ID 或者 消耗道具 数量
    CC_SYNTHESIZE(int, _itemId, itemId);
    CC_SYNTHESIZE_READONLY(int, _uId, UId);
    CC_SYNTHESIZE_READONLY(int, _pvpLev, pvpLev);
    CC_SYNTHESIZE_READONLY(int, _costScore, CostScore);
    CC_SYNTHESIZE(int, _currCount, CurrCount); //获取成功后直接＋1
    CC_SYNTHESIZE_READONLY(int, _maxCout, MaxCout);
    CC_SYNTHESIZE(int,_discount, DisCount);
    CC_SYNTHESIZE(int , _modelType, ModelType);
};

#endif /* defined(__GameSanGuo__SGCreditCardModel__) */
