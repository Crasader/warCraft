//
//  SG_BuffID1.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-17.
//
//

#ifndef __GameSanGuo__SG_BuffID1__
#define __GameSanGuo__SG_BuffID1__

#include <iostream>
#include "SG_BuffBase.h"


class SG_BuffID1: public SG_BuffBase {
    
    
public:
    
    SG_BuffID1( SG_SkillsBase*skbase );
    ~SG_BuffID1();
    CCSpriterX *sbEff;
    
    virtual void activateBuff(GameIndex index);
    
//public:
//    CC_SYNTHESIZE(int, m_roundNum, RoundNum);
//    CC_SYNTHESIZE(float, m_attributeNum, AfftributeNum);
    
};

#endif /* defined(__GameSanGuo__SG_BuffID1__) */
