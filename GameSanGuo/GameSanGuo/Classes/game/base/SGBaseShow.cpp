//
//  SGBaseShow.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-7.
//
//

#include "SGBaseShow.h"

SGBaseShow::SGBaseShow()
:menu(NULL),
bg(NULL),
isCanTouch(true)
{
    
}
SGBaseShow::~SGBaseShow()
{
//    CCLOG("~SGBaseShow");
}
bool SGBaseShow::init(const char *fileName_, int tag_, bool isSpriteFrame)
{
    if (!CCLayer::init())
    {
        return false;
    }
    this->setTag(tag_);
    
    if (fileName_)
    {
        if (isSpriteFrame)
        {
            bg = CCSprite::createWithSpriteFrameName(fileName_);
        }
        else
        {
            bg = CCSprite::create(fileName_);
        }
        this->addChild(bg);
        bg->setPosition(SGLayout::getPoint(kMiddleCenter));
    }

    menu = CCMenu::create();
    this->addChild(menu,2);
    menu->setPosition(CCPointZero);
    
    this->initMsg();
    
    return true;
}
void SGBaseShow::initMsg()
{
    
}

CCNode* SGBaseShow::getBtnByTag(int tag)
{
    CCNode *node = menu->getChildByTag(tag);
    return node;
}

//void SGBaseShow::initView()
//{
//    
//}
void SGBaseShow::addBtn(CCMenuItem *button)
{
    menu->addChild(button);
}
void SGBaseShow::removeBtn(CCMenuItem *button)
{
    if (button != NULL) {
         menu->removeChild(button, true);
    }
}
CCMenu* SGBaseShow::getMenu()
{
    return menu;
}