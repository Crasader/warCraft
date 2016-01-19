//
//  SGSoldierCard.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-9.
//
//士兵卡信息

#ifndef __GameSanGuo__SGSoldierCard__
#define __GameSanGuo__SGSoldierCard__
#include "SGBaseMilitaryCard.h"
class SGSoldierCard : public SGBaseMilitaryCard
{
public:
    virtual const char * getClassName(){return "SGSoldierCard";};
    CC_SYNTHESIZE(int, s_type , soldiertype);
    CC_SYNTHESIZE(int, s_growskill , soldiergrowskill);
    CC_SYNTHESIZE(int, s_talentskill , soldiertalentskill);
       
    SGSoldierCard();
    ~SGSoldierCard();
};
#endif /* defined(__GameSanGuo__SGSoldierCard__) */



    
   