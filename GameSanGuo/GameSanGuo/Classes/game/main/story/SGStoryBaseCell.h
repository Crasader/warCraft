//
//  SGStoryBaseCell.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-3-28.
//
//

#ifndef __GameSanGuo__SGStoryBaseCell__
#define __GameSanGuo__SGStoryBaseCell__

#include "SNSTableViewCellItem.h"
#include "SGMainManager.h"
#include "SGTestSwitch.h"

// get larger
#define ZoomIn(iVar)  (CCDirector::sharedDirector()->getWinSize().width/320.0f)*(iVar)
#define ZoomInX(iVar)  (CCDirector::sharedDirector()->getWinSize().width/320.0f)*(iVar)
#define ZoomInY(iVar)  (CCDirector::sharedDirector()->getWinSize().height/640.0f)*(iVar)
// get smaller
#define ZoomOut(iVar) (iVar)/(CCDirector::sharedDirector()->getWinSize().width/320.0f)
#define ZoomOutX(iVar) (iVar)/(CCDirector::sharedDirector()->getWinSize().width/320.0f)
#define ZoomOutY(iVar) (iVar)/(CCDirector::sharedDirector()->getWinSize().height/640.0f)

#define ITEMBIGBGTAG    93
class SGStoryBaseCell : public SNSTableViewCellItem
{
public:
    SGStoryBaseCell(void);
    virtual ~SGStoryBaseCell(void);
    
    static SGStoryBaseCell* create();
    bool init();
    
    void resetBgImg(int index);
    
    CCSprite *spriteBackground;
    CCSize bgSize;
    /////////
    SGButton*btn;

};

class SGStoryCardBaseCell : public SGStoryBaseCell
{
public:
    SGStoryCardBaseCell(void);
    virtual ~SGStoryCardBaseCell(void);
    
    static SGStoryCardBaseCell* create();
    bool init();
    
public:
    CCScale9Sprite *spriteBorder;
};

#endif /* defined(__GameSanGuo__SGStoryBaseCell__) */
