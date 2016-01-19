//
//  SGLuaDataModel.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-2.
//
//

#include "SGLuaDataModel.h"
SGLuaDataModel::SGLuaDataModel()
{
    m_btntag=0;
    m_childtag=0;
    m_btnname=NULL;
    m_luaname=NULL;
}

SGLuaDataModel::~SGLuaDataModel()
{
    CC_SAFE_RELEASE(m_luaname);
    CC_SAFE_RELEASE(m_btnname);
    
}