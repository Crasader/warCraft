//
//  SGBattleReviveLayer.h
//  GameSanGuo
//
//  Created by wenlong w on 13-5-21.
//
//

#ifndef __GameSanGuo__SGBattleReviveLayer__
#define __GameSanGuo__SGBattleReviveLayer__

#include <iostream>
#include "SGBaseBox.h"

class  SGBattleReviveLayer : public SGBaseBox{
    
    
public:
    
    SGBattleReviveLayer();
    ~SGBattleReviveLayer();
    static SGBattleReviveLayer *create(SGBoxDelegate *target, int round);
    virtual void initMsg();

	void fixBackgroundPos();
    void buttonOk();
    void buttonClose();

    
public:
    SGBoxDelegate *_target;
};

#endif /* defined(__GameSanGuo__SGBattleReviveLayer__) */
