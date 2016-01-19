//
//  SG_BuffManager.h
//  GameSanGuo
//
//  Created by wenlong w on 13-1-17.
//
//

#ifndef __GameSanGuo__SG_BuffManager__
#define __GameSanGuo__SG_BuffManager__

#include <iostream>
#include "cocos2d.h"
#include "SG_SkillsBase.h"
#include "SGAttackList.h"



using namespace cocos2d;

typedef enum
{
    BUFFID1 = 2000,
    BUFFID2,
    BUFFID3,
    BUFFID4,
    BUFFID5,
    BUFFID6,
    BUFFID7,
    BUFFID8,
    BUFFID9,
    BUFFID10,
    BUFFID11,
    BUFFID12,
    
}SG_BuffType;


class SG_BuffBase;
class SG_BuffManager: public CCLayer {

    
public:
    SG_BuffManager();
    ~SG_BuffManager();
    
    static SG_BuffManager* initBuffManager( SG_SkillsBase*skbase, SGAttackList* attact);
    
    static SG_BuffManager* repariBuffData(cocos2d::CCDictionary *data);
    
    SG_BuffBase *setBuffType( SG_SkillsBase*skbase, cocos2d::CCDictionary *data);
    
    
public:
public:
    CC_SYNTHESIZE(SG_BuffBase*, m_bfBase, bfBase);
    
};

#endif /* defined(__GameSanGuo__SG_BuffManager__) */
