//
//  TMenuItem.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-11-28.
//
//

#include "TMenuItem.h"
TMenuItem *TMenuItem::create(int sid, int colorid, int type, CCObject *target, SEL_MenuHandler selector)
{
    TMenuItem *ti = new TMenuItem();
    if (ti)
    {
        ti->setType(type);
        ti->init(sid, colorid, target, selector);
        ti->autorelease();
        return  ti;
    }
    CC_SAFE_DELETE(ti);
    return NULL;
}
bool TMenuItem::init(int sid, int colorid, CCObject *target, SEL_MenuHandler selector)
{
    char spriteName[256] = {0};
    sprintf(spriteName, "ani_%d.png",sid);
    CCSprite *sp1 = CCSprite::create(spriteName);
    CCSprite * sp2 = CCSprite::create(spriteName);
    sp2->setColor(ccGRAY);
    
    if (!CCMenuItemSprite::initWithNormalSprite(sp1, sp2, NULL, target, selector))
    {
        return false;
    }
    this->setColor(colorid);
    this->setSid(sid);
    this->setColorId(colorid);
    return true;
}
void TMenuItem::setColor(int colorId)
{
    CCSprite *sp = (CCSprite *)this->getNormalImage();
    switch (colorId)
    {
        case 1:
        {
            sp->setColor(ccRED);
        }
            break;
        case 2:
        {
            sp->setColor(ccBLUE);
        }
            break;
        case 3:
        {
            sp->setColor(ccYELLOW);
        }
            break;
        default:
            break;
    }
}