//
//  TMenuItem.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-28.
//
//

#ifndef __GameSanGuo__TMenuItem__
#define __GameSanGuo__TMenuItem__

#include "cocos2d.h"
using namespace cocos2d;
class TMenuItem : public CCMenuItemSprite
{
    CC_SYNTHESIZE(int, sid, Sid);
    CC_SYNTHESIZE(int, colorid, ColorId);
    CC_SYNTHESIZE(int, type, Type);
public:
    static TMenuItem* create(int sid, int colorid, int type, CCObject *target, SEL_MenuHandler selector);
    bool init(int sid, int colorid, CCObject *target, SEL_MenuHandler selector);
    void setColor(int colorId);
};
#endif /* defined(__GameSanGuo__TMenuItem__) */
