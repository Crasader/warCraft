//
//  SGSpecialBattleDataModel.h
//  GameSanGuo
//
//  Created by bugcode on 14-12-16.
//
//

#ifndef __GameSanGuo__SGSpecialBattleDataModel__
#define __GameSanGuo__SGSpecialBattleDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;


//特殊副本战斗数据
class SGSpecialBattleDataModel : public CCObject
{
    //节id
    CC_SYNTHESIZE(int, _plotId, PlotId);
    //beanId
    CC_SYNTHESIZE(std::string, _beanId, BeanId);
    //章id
    CC_SYNTHESIZE(int, _chapId, ChapId);
    //节index， 排序
    CC_SYNTHESIZE(int, _sectionIndex, SectionIndex);
    //章名称
    CC_SYNTHESIZE(std::string, _chapName, ChapName);
    //节名称
    CC_SYNTHESIZE(std::string, _storyName, StoryName);
    //所需体力
    CC_SYNTHESIZE(int, _needPower, NeedPower);
    //最低等级限制
    CC_SYNTHESIZE(int, _limitLevel, LimitLevel);
    //章描述
    CC_SYNTHESIZE(std::string, _storyDesc, StoryDesc);
    //参战的前置条件，（是国家条件还是性别条件）1：国家限制 2：性别限制
    CC_SYNTHESIZE(int, _limitsOfJoin, LimitOfJoin);
    //参战具体条件，由_limitsOfJoin决定
    /*
     国家限制时：1蜀国，2魏国，3吴国
     性别限制时： 1男性，2女性
     */
    CC_SYNTHESIZE(int, _detailLimits, DetailLimits);
    //每天可打的次数
    CC_SYNTHESIZE(int, _timesOfDay, TimeOfDay);
    //建议战斗力
    CC_SYNTHESIZE(int, _suggestPower, SuggestPower);
    //掉落组的id
    CC_SYNTHESIZE(int, _dropItemId, DropItemsId);
    

public:
    
    SGSpecialBattleDataModel();
    
    ~SGSpecialBattleDataModel();
    
    
};


#endif /* defined(__GameSanGuo__SGSpecialBattleDataModel__) */
