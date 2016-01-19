//
//  SGInviteFriend.h
//  GameSanGuo
//
//  Created by 江南岸 on 13-3-1.
//
//

#ifndef __GameSanGuo__SGInviteFriend__
#define __GameSanGuo__SGInviteFriend__

#include <iostream>
#include "SGBaseLayer.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;

class SGInviteFriend : public SGBaseLayer
{    
private:
    CCEditBox *editSNS;
    CCEditBox *editMessage;
    
    void constructView();
    
    void buttonClickSendSNS();
    void buttonClickMessage();
    void buttonClickGoBack();
    
public:
    
    SGInviteFriend();
    ~SGInviteFriend();
    static SGInviteFriend* create();
    
};

#endif /* defined(__GameSanGuo__SGInviteFriend__) */
