//
//  SGExpandFriend.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-1.
//
//

#ifndef __GameSanGuo__SGExpandFriend__
#define __GameSanGuo__SGExpandFriend__

#include <iostream>
#include "SGBaseLayer.h"
#include "cocos-ext.h"
#include "SGCCLabelTTF.h"

USING_NS_CC_EXT;

class SGExpandFriend : public SGBaseLayer {
private:
    SGCCLabelTTF *labelTip;
    SGButton *buttonAdd;
    SGCCLabelTTF *labelFriendLimit;
    SGCCLabelTTF *labelExpandInfo;
    
    void constructView();
    void buttonClickGoBack();
    void buttonClickAdd();
    
    void isCanExpand();
    
    void expandListener(CCObject *obj);
    
public:
    
    
    SGExpandFriend();
    ~SGExpandFriend();
    static SGExpandFriend* create();

};


#endif /* defined(__GameSanGuo__SGExpandFriend__) */
