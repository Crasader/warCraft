//
//  SGTTMainLayer.h
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-8.
//
//

#ifndef __GameSanGuo__SGTTMainLayer__
#define __GameSanGuo__SGTTMainLayer__

#include "SGBaseLayer.h"

//@NOTICE: 本类废弃。bymm
/* for test off line */
class SGTTMainLayer: public SGBaseLayer
{
private:
    CCLabelTTF *gonggao;
    CCLabelTTF *chat;
    CCLabelTTF *name;
    CCLabelTTF *experience;
    CCLabelTTF *tili;
    CCLabelTTF *jl;
    CCLabelTTF *CD;
    CCLabelTTF *gold;
    CCLabelTTF *copper;
    CCLabelTTF *level;
    CCLayerColor *layer;
    CCSprite *gonggao_bg_3;
    CCSprite *gonggaobg;
    CCLabelTTF *tilicd;
    CCLabelTTF *junlcd;
    CCSprite *tilicdbg;
    CCSprite *junlcdbg;

public:
    CCMenu *portraitMenu;
    SGTTMainLayer();
    ~SGTTMainLayer();
    static SGTTMainLayer* create();
    void initView();
};



#endif /* defined(__GameSanGuo__SGTTMainLayer__) */
