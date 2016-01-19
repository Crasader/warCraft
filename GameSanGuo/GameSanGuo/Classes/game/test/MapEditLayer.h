//
//  MapEditLayer.h
//  TestSG
//
//  Created by gengshengcai on 12-11-28.
//
//

#ifndef __TestSG__MapEditLayer__
#define __TestSG__MapEditLayer__

#include "cocos2d.h"
#include "TMenuItem.h"
#include "GameConfig.h"
using namespace cocos2d;
//typedef enum
//{
//    kcolorRed = 1,
//    kcolorBlue,
//    kcolorYellow,
//    
//}sgColorType;
class GridData :public CCObject
{
public:
    int sid;
    GameIndex index;
    CCSprite *sp;
};
class MapEditLayer : public CCLayerColor
{
private:
    TMenuItem *selectItem;
    CCArray *maps;
    GridData *grid[6][8];
public:
    static MapEditLayer * create();
    bool init();
    virtual void draw();
    void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
private:
    void menuSelect(CCNode *sender);
    void putOnScene(int i, int j);
    void back();
    
    
};
#endif /* defined(__TestSG__MapEditLayer__) */
