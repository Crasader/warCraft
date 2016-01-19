//
//  SG_BuffID2.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-22.
//
//

#ifndef __GameSanGuo__SG_BuffID2__
#define __GameSanGuo__SG_BuffID2__

#include <iostream>
#include "SG_BuffBase.h"


class SG_BuffID2: public SG_BuffBase {
    
    
public:
    
    SG_BuffID2( SG_SkillsBase*skbase );
    ~SG_BuffID2();

    virtual void activateBuff(GameIndex index);
    
//public:
//    CC_SYNTHESIZE(int, m_roundNum, RoundNum);
//    CC_SYNTHESIZE(float, m_attributeNum, AfftributeNum);
    
};

#endif /* defined(__GameSanGuo__SG_BuffID2__) */
