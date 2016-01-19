//
//  SGGoldChangCoin.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-13.
//
//

#ifndef __GameSanGuo__SGGoldChangCoin__
#define __GameSanGuo__SGGoldChangCoin__

#include "SGBaseLayer.h"
class SGGoldChangCoin : public SGBaseLayer
{
private:
    int maxChance;
    int currChance;
    int vipLvl;
    int cost_gold;
    int get_coin;
    SGCCLabelTTF *label3;
    SGButton *duihuan;
    SGCCLabelTTF *label1;
private:
    void initView(int lvl,int curr,int max,int gold,int coin);
    void backHandler();
    void becomeVipHandler();
    void exchangeHandler();
public:
    SGGoldChangCoin();
    ~SGGoldChangCoin();
    void ExchangeListener(CCObject *obj);
    void storeEnterListener(CCObject *obj);
    static SGGoldChangCoin *create(int lvl,int curr,int max,int gold,int coin);
};


#endif /* defined(__GameSanGuo__SGGoldChangCoin__) */
