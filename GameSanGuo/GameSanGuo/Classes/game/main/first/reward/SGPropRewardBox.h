//
//  SGPropRewardBox.h
//  GameSanGuo
//
//  Created by 赤霄 on 13-10-31.
//
//

#ifndef __GameSanGuo__SGPropRewardBox__
#define __GameSanGuo__SGPropRewardBox__

#include <iostream>

#include "SGBaseBox.h"

class SGPropRewardBox:public SGBaseBox {
private:
    void boxClose();
    
public:
    SGPropRewardBox();
    ~SGPropRewardBox();
    
    static SGPropRewardBox *create(SGBoxDelegate *delegate,CCString *giftName,int itemID,int type);
    void initView(SGBoxDelegate *delegate,CCString *giftName,int itemID,int type);
    
};

#endif /* defined(__GameSanGuo__SGPropRewardBox__) */
