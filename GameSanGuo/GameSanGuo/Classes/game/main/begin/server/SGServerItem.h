//
//  SGServerItem.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-11.
//
//

#ifndef __GameSanGuo__SGServerItem__
#define __GameSanGuo__SGServerItem__

#include "SNSTableViewCellItem.h"
#include "cocos-ext.h"
#include "SGCCLabelTTF.h"

using namespace cocos2d::extension;

class SGServerItem : public SNSTableViewCellItem
{
    
private:
    CCSprite *borderSprite;
    CCSprite *spriteServerNew;
    CCSprite *spriteIsLogin;
    SGCCLabelTTF *labelserverState;
//    SGCCLabelTTF *labelRoleCountry;
    SGCCLabelTTF *labelRoleNike;
//    SGCCLabelTTF *labelRoleLevel;
    
    SGCCLabelTTF *labelAr;
    SGCCLabelTTF *labelNoRoles;
    SGCCLabelTTF *labelServerName;
    
    
public:
    CREATE_FUNC(SGServerItem);
	//10.25//goerge
    SGServerItem();
	~SGServerItem();
    void updataData(CCObject *obj);
    bool init();
};
#endif /* defined(__GameSanGuo__SGServerItem__) */
