//
//  SGCreditCardModel.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-25.
//
//

#include "SGCreditCardModel.h"

SGCreditCardModel::SGCreditCardModel(void)
{
    _uId = 0;
    _pvpLev = 0;
    _costScore = 0;
    _currCount = 0;
    _maxCout = 0;
    _itemId = 0;
//    _itemType = 0;
    _discount = 0;
}
SGCreditCardModel::~SGCreditCardModel(void)
{
}

SGCreditCardModel* SGCreditCardModel::create(int uid, int level, int cost, int ccount, int mcount)
{
    SGCreditCardModel *model = new SGCreditCardModel();
    if (model && model->init(uid, level, cost, ccount, mcount)) {
        model->autorelease();
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}

bool SGCreditCardModel::init(int uid, int level, int cost, int ccount, int mcount)
{
    _uId = uid;
    _pvpLev = level;
    _costScore = cost;
    _currCount = ccount;
    _maxCout = mcount;
    return true;
}