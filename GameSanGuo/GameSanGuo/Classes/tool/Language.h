//
//  Language.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-31.
//
//

#ifndef __GameSanGuo__Language__
#define __GameSanGuo__Language__

#include "cocos2d.h"
USING_NS_CC;
class Language : public CCObject
{
public:
    static const char * getStringByKey(const char *key);
};

#endif /* defined(__GameSanGuo__Language__) */
