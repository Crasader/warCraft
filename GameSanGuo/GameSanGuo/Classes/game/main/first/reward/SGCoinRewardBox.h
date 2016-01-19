//
//  SGCoinRewardBox.h
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-31.
//
//

#ifndef __GameSanGuo__SGCoinRewardBox__
#define __GameSanGuo__SGCoinRewardBox__

#include <iostream>

#include "SGBaseBox.h"

class SGCoinRewardBox:public SGBaseBox {
private:
    void boxClose();
    
public:
    SGCoinRewardBox();
    ~SGCoinRewardBox();
    
    static SGCoinRewardBox *create(SGBoxDelegate *delegate,int coinNum);
    void initView(SGBoxDelegate *delegate,int coinNum);
    
};

#endif /* defined(__GameSanGuo__SGCoinRewardBox__) */
