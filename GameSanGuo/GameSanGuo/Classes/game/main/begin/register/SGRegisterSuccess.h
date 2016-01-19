//
//  SGRegisterSuccess.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-17.
//
//

#ifndef __GameSanGuo__SGRegisterSuccess__
#define __GameSanGuo__SGRegisterSuccess__

#include <iostream>
#include "SGBaseBox.h"
#include "SGHTTPDelegate.h"

class SGRegisterSuccess :public SGBaseBox ,SGHTTPDelegate {
    
private:
    void constructView();
    void buttonClick();
    
    virtual void requestFinished(int msgid, cocos2d::CCObject *data);
    virtual void requestFailed();

    
public:
    
    SGRegisterSuccess();
    ~SGRegisterSuccess();
    
    static SGRegisterSuccess* create(SGBoxDelegate *delegate);

    
};


#endif /* defined(__GameSanGuo__SGRegisterSuccess__) */
