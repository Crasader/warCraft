//
//  SGBattleRoundSurrenderLayer.h
//  GameSanGuo
//
//  Created by wenlong w on 13-4-25.
//
//

#ifndef __GameSanGuo__SGBattleRoundSurrenderLayer__
#define __GameSanGuo__SGBattleRoundSurrenderLayer__

#include <iostream>
#include "SGBaseBox.h"

class SGBattleRoundSurrenderLayer : public SGBaseBox{
    
    
public:
    SGBattleRoundSurrenderLayer();
    ~SGBattleRoundSurrenderLayer();
    
    virtual void initMsg();

    static SGBattleRoundSurrenderLayer *create(SGBoxDelegate *target, int round);
    
    void buttonClose();
    void buttonEnd();
    void buttonSurrender();
    

private:
    SGBoxDelegate *_target;
    
    int _round;
    CCSize size;
    
};



#endif /* defined(__GameSanGuo__SGBattleRoundSurrenderLayer__) */
