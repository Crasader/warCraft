//
//  SGHTTPDelegate.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-29.
//
//

#ifndef __GameSanGuo__SGHTTPDelegate__
#define __GameSanGuo__SGHTTPDelegate__

#include "cocos2d.h"

class SGHTTPDelegate
{
public:
    virtual void requestFinished(int msgid, cocos2d::CCObject *data) = 0;
    virtual void requestFailed(){};
};

#endif /* defined(__GameSanGuo__SGHTTPDelegate__) */
