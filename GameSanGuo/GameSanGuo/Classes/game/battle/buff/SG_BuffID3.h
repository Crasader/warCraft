//
//  SG_BuffID3.h
//  GameSanGuo
//
//  Created by wenlong w on 13-3-1.
//
//

#ifndef __GameSanGuo__SG_BuffID3__
#define __GameSanGuo__SG_BuffID3__

#include <iostream>
#include "SG_BuffBase.h"

class SG_BuffID3: public SG_BuffBase {
    
    
public:
    SG_BuffID3( SG_SkillsBase*skbase );
    ~SG_BuffID3();
    
    virtual void activateBuff(SGAttackList *attack);
    
    
};



#endif /* defined(__GameSanGuo__SG_BuffID3__) */
