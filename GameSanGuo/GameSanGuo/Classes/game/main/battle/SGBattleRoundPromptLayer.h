//
//  SGBattleRoundPromptLayer.h
//  GameSanGuo
//
//  Created by wenlong w on 13-4-25.
//
//

#ifndef __GameSanGuo__SGBattleRoundPromptLayer__
#define __GameSanGuo__SGBattleRoundPromptLayer__

#include <iostream>
#include "SGBaseBox.h"

class SGBattleRoundPromptLayer : public SGBaseBox{
    
    
public:
    SGBattleRoundPromptLayer();
    ~SGBattleRoundPromptLayer();
    
    virtual void initMsg();

    static SGBattleRoundPromptLayer *create(SGBoxDelegate *target, int round);
    
    void buttonClose();
    void buttonEnd();
    void buttonSurrender();
    

private:
    SGBoxDelegate *_target;
    
    int _round;
    CCSize size;
    
};



#endif /* defined(__GameSanGuo__SGBattleRoundPromptLayer__) */
