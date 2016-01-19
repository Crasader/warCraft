//
//  SGFateBase.h
//  GameSanGuo
//
//  Created by Fygo Woo on 11/9/13.
//
//  缘分类型的基类，不能初始化实例，派生出6种类型：武将，装备，国家，性别，类型，星级。
//
//

#ifndef GameSanGuo_SGFateBase_h
#define GameSanGuo_SGFateBase_h

#include "cocos2d.h"

USING_NS_CC;

#include "SGFateManager.h"

class SGFateBase : public CCObject
{
public:
    //创建
    SGFateBase();
    
    //析构
    ~SGFateBase();
    
    //缘分ID
    CC_SYNTHESIZE(int, m_id, Id);
    
    //缘分名称
    CC_SYNTHESIZE_RETAIN(CCString*, m_name, Name);
    
    //缘分达成条件的类型
    CC_SYNTHESIZE(FateConditionType, m_conditionType, ConditionType);
    
    //缘分达成条件的字符串，当场解析。
    CC_SYNTHESIZE_RETAIN(CCString*, m_conditionString, ConditionString)
    
    //缘分品质
    CC_SYNTHESIZE(FateQuality, m_quality, Quality);
    
    //缘分是否显示，类似总开关
    CC_SYNTHESIZE(bool, m_isOn, IsOn);
    
    //缘分物品来源字符串
    CC_SYNTHESIZE_RETAIN(CCString *, m_sourceFateInfo, SourceFateInfo);
    
    //缘分效果列表
    CC_SYNTHESIZE_PASS_BY_REF(std::vector<int>, m_effectList, EffectList);
//    std::vector<int>* m_effectList;
//    void setEffectList(std::vector<int>* vec) { m_effectList = vec; };
//    std::vector<int>* getEffectList() { return m_effectList; };
    };

#endif
