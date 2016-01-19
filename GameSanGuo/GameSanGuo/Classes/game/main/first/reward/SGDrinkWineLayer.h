//
//  SGDrinkWineLayer.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-16.
//
//

#ifndef __GameSanGuo__SGDrinkWineLayer__
#define __GameSanGuo__SGDrinkWineLayer__

#include "SGBaseLayer.h"
class SGDrinkWineLayer : public SGBaseLayer
{
private:
    int recoverPower;
private:
    void initView(int iscan);
    void backHandler();
    void confirmHandler();
    void recoverListener(cocos2d::CCObject *obj);
    void updatejiuhu();
public:
    SGDrinkWineLayer();
    ~SGDrinkWineLayer();
    static SGDrinkWineLayer *create(int power,int iscan);
    
};

#endif /* defined(__GameSanGuo__SGDrinkWineLayer__) */
