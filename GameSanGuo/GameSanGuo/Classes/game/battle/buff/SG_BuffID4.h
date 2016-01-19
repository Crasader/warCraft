//
//  SG_BuffID4.h
//  GameSanGuo
//
// Created by zenghui w on 14-5-12.
//
//

#ifndef __GameSanGuo__SG_BuffID4__
#define __GameSanGuo__SG_BuffID4__

#include <iostream>
#include "SG_BuffBase.h"

class SG_BuffID4: public SG_BuffBase {
    
    
public:
    SG_BuffID4( SG_SkillsBase*skbase );
    ~SG_BuffID4();
    
    virtual void activateBuff(SGAttackList *attack);
    
    
};



#endif /* defined(__GameSanGuo__SG_BuffID4__) */
