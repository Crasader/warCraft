//
//  SGBaseShow.h
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-7.
//
//

#ifndef __GameSanGuo__SGBaseShow__
#define __GameSanGuo__SGBaseShow__

#include "cocos2d.h"
#include "GameConfig.h"
#include "SGLayout.h"
#include "SGButton.h"
#include "SGCardAttribute.h"
#include "SGBaseTags.h"

USING_NS_CC;

class SGBaseShow : public CCLayer
{
protected:
    CCSprite *bg;
    bool isCanTouch;
    CCMenu *menu;
    
public:
    
    virtual void addBtn(CCMenuItem *button);
    virtual void removeBtn(CCMenuItem *button);
    
private:
    
protected:
    
    virtual void initMsg();
    
public:
    SGBaseShow();
    ~SGBaseShow();
    
    bool init(const char *fileName_, int tag_, bool isSpriteFrame = true);
//    virtual void initView();
    
    CCNode* getBtnByTag(int tag);
    CCMenu *getMenu();
};
#endif /* defined(__GameSanGuo__SGBaseShow__) */
