//
//  SGBindSuccessBox.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-10.
//
//

//-----------------------------------------------
// 绑定帐号成功
//-----------------------------------------------

#ifndef __GameSanGuo__SGBindSuccessBox__
#define __GameSanGuo__SGBindSuccessBox__

#include <iostream>
#include "SGBaseBox.h"
#include "cocos2d.h"
USING_NS_CC;

class SGBindSuccessBox : public SGBaseBox
{
private:
    
    void viewDidLoad();
    
    void buttonClickOK(CCObject *sender);
    
public:
    SGBindSuccessBox();
    
    ~SGBindSuccessBox();
    
    static SGBindSuccessBox *create(SGBoxDelegate *delegate);
    
};

#endif /* defined(__GameSanGuo__SGBindSuccessBox__) */
