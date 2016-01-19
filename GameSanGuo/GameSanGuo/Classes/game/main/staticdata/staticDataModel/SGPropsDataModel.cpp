//
//  SGPropsDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-22.
//
//

#include "SGPropsDataModel.h"
SGPropsDataModel::SGPropsDataModel()
{
    propsBeanId = NULL;
    propsCurrStarLevel = 0;
    propsId = 0;
    propsName = NULL;
    propsType = 0;
    propsInfo = NULL;
    propsPropsType = 0;
    propsCountry = 0;
    propsNum = 0;
    m_sell = 0;
    m_value = 0;
    m_visittype = 0;
	m_IconId = 0;
    m_propJokeInfo = NULL;
}

SGPropsDataModel::~SGPropsDataModel()
{
    CC_SAFE_RELEASE(propsBeanId);
    CC_SAFE_RELEASE(propsName);
    CC_SAFE_RELEASE(propsInfo);
    CC_SAFE_RELEASE(m_propJokeInfo);
}