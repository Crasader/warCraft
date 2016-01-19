//
//  SGFateEffect.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/9/13.
//
//

#ifndef _GameSanGuo_SGFateEffect_h_
#define _GameSanGuo_SGFateEffect_h_

#include "cocos2d.h"

USING_NS_CC;

#include "SGFateManager.h"
//class SGFateManager;

class SGFateEffect : public CCObject
{
public:
    //创建
    SGFateEffect();
    
    //析构
    ~SGFateEffect();
    
    //效果ID
    CC_SYNTHESIZE(int, m_id, Id);
    
    //效果加成类型
    CC_SYNTHESIZE(FateAttrType, m_attrType, AttrType);
    
    //效果加成数值类型
    CC_SYNTHESIZE(FateValueType, m_valueType, ValueType);
    
    //效果加成数值
    CC_SYNTHESIZE(int, m_value, Value);
};

#endif
