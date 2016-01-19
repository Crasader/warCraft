//
//  SGInviteNum.h
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-12.
//
//

#ifndef __GameSanGuo__SGInviteNum__
#define __GameSanGuo__SGInviteNum__
#include "SGBaseLayer.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
class SGInviteNum : public SGBaseLayer
{
    
private:
    CCEditBox *editNum;
private:
    void initView();
    void backHandler();
    void okHandler();
public:    
    SGInviteNum();
    ~SGInviteNum();
    static SGInviteNum* create();
    
};


#endif /* defined(__GameSanGuo__SGInviteNum__) */
