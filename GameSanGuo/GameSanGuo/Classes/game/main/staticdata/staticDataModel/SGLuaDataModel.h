//
//  SGLuaDataModel.h
//  GameSanGuo
//
//  Created by 董博谦 on 14-1-8.
//
//

#ifndef __GameSanGuo__SGLuaDataModel__
#define __GameSanGuo__SGLuaDataModel__

#include <iostream>
#include "cocos2d.h"
USING_NS_CC;

/*  vip表 */

class SGLuaDataModel : public CCObject
{
    //btn tag
    CC_SYNTHESIZE(int, m_btntag, Btntag);
    
    //lua addchildtag
    CC_SYNTHESIZE(int, m_childtag, Childtag);
    
    //btn sprite name
    CC_SYNTHESIZE_RETAIN(CCString*, m_btnname, Btnname);
    
    //lua file name
    CC_SYNTHESIZE_RETAIN(CCString*, m_luaname, Luaname);   
    
public:
    SGLuaDataModel();
    
    ~SGLuaDataModel();
    
};

#endif /* defined(__GameSanGuo__SGLuaDataModel__) */
