//
//  SGBaseBox.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-27.
//
//

#ifndef __GameSanGuo__SGBaseBox__
#define __GameSanGuo__SGBaseBox__

#include "SGBaseShow.h"
#include "SGBoxDelegate.h"
#include "CCSpriterX.h"
#include "cocos-ext.h"
#include "SGBaseLayer.h"
 
class SGBaseBox : public SGBaseShow
{
public:
    CCSprite *biao;
    cocos2d::extension::CCScale9Sprite *fontbg;
    
protected:
    SGBoxDelegate *deletage;
protected:
    CCObject *data;
    CCSize boxSize;
protected:
    //BoxTag改为LayerTag，统一进行资源管理
    virtual bool init(SGBoxDelegate *dg, BoxTag bt, CCSize size, bool ishavinfobg= true,bool isHaveBg = true,CCSize bottomsize = CCSize(0, 0),bool useDefaultBg = true);
public:
    SGBaseBox();
    ~SGBaseBox();

    virtual void boxClose();
    virtual void boxCloseWithOutCallBack();
    virtual void setIsCanTouch(bool isTouch);
    virtual void runactionwithscale();///////1125
};
#endif /* defined(__GameSanGuo__SGBaseBox__) */
