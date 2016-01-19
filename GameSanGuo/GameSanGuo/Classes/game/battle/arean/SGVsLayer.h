//
//  SGVsLayer.h
//  GameSanGuo
//  点击对战进入该layer选择相应的pvp模式
//  Created by angel on 14-12-18.
//
//

#ifndef __GameSanGuo__SGVsLayer__
#define __GameSanGuo__SGVsLayer__

#include <iostream>
#include "SGBaseLayer.h"

class SGVsLayer : public SGBaseLayer
{
public:
    SGVsLayer();
    ~SGVsLayer();
    static SGVsLayer * create();
    void onClickTianTiButton();
    void onClickAreanButton();
    void  backHandler();
    
private:
    void initView();
};

#endif /* defined(__GameSanGuo__SGVsLayer__) */
