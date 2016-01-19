//
//  SGBattleFillUpLayer.h
//  GameSanGuo
//
//  Created by wenlong w on 13-8-5.
//
//

#ifndef __GameSanGuo__SGBattleFillUpLayer__
#define __GameSanGuo__SGBattleFillUpLayer__

#include <iostream>
#include "SGBaseBox.h"

class SGBattleFillUpLayer : public SGBaseBox {
    
    
public:
    
    SGBattleFillUpLayer();
    ~SGBattleFillUpLayer();
    static SGBattleFillUpLayer *create( SGBoxDelegate *delegate );
    void initLayer();
    void buttonOk();
    void buttonClose();
    void setState(int num);
};


#endif /* defined(__GameSanGuo__SGBattleFillUpLayer__) */
