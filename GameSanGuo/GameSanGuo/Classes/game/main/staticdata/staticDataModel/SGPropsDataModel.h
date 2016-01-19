//
//  SGPropsDataModel.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-22.
//
//

#ifndef __GameSanGuo__SGPropsDataModel__
#define __GameSanGuo__SGPropsDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/* 道具表 *
 
    beanId 例如propsCardItem/2
    id
    道具类型 相见类型列表
    道具当前星级
    道具名称
    获得途径
 */

class SGPropsDataModel :public CCObject
{
    CC_SYNTHESIZE_RETAIN(CCString*, propsBeanId, PropsBeanId);
    CC_SYNTHESIZE(int, propsId, PropsId);
    CC_SYNTHESIZE(int, propsType, PropsType);
    CC_SYNTHESIZE(int, propsCurrStarLevel, PropsCurrStarLevel);
    CC_SYNTHESIZE_RETAIN(CCString*, propsName, PropsName);
    CC_SYNTHESIZE_RETAIN(CCString*, propsInfo, PropsInfo);
    CC_SYNTHESIZE(int, propsPropsType, PropsPropsType);
    CC_SYNTHESIZE(int, propsCountry, PropsCountry);
    CC_SYNTHESIZE(int, propsNum, propsNum);
    CC_SYNTHESIZE(int, m_sell, propsSell);
    CC_SYNTHESIZE(int, m_value, propsValue);
    CC_SYNTHESIZE(int, m_visittype, propVisitType);
    CC_SYNTHESIZE_RETAIN(CCString*, m_propJokeInfo, PropJokeInfo);

	
	//头像Id
	CC_SYNTHESIZE(int, m_IconId, IconId);
    
public:
    SGPropsDataModel();
    
    ~SGPropsDataModel();
    
    
    
    
};


#endif /* defined(__GameSanGuo__SGPropsDataModel__) */
