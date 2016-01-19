//
//  SGBaseLayer.h
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-20.
//
//

#ifndef __GameSanGuo__SGBaseLayer__
#define __GameSanGuo__SGBaseLayer__

#include "SGBaseShow.h"
#include "SGBoxDelegate.h"
#include "SGButton.h"
#include "SNSTableView.h"
#include "GameMusicmanage.h"
USING_NS_CC;

enum ChildZ
{
    kbgz = -1,
    kmenuz = 10
};

/*
 * ICON相关的物品数据原型。
 * 对应的消息类是main::ItemDataProro，两者的字段完全一致。
 * 至于是使用client的数据类型SGItemDataProro，还是使用message的数据类型main::ItemDataProro，请自行决定。
 * PS:SGDrawCards这个绘制类里有直接将这两个消息转化成图形的快捷方法。
 */
struct SGItemDataProto
{
    //必有
    int type;
    int id;
    int num;
    //预留
    int level;
};


#define         TEST_HERO_LAYER_TAG 5555

class SGBaseLayer : public SGBaseShow, public SGBoxDelegate
{
public:
    SGBaseLayer();
    ~SGBaseLayer();
    bool init(const char *fileName_, LayerTag tag_, bool isSpriteFrame = true);
    
public:
    bool m_isCocostudio;
    virtual void showBoxCall(CCObject *obj);
    virtual void setIsCanTouch(bool isTouch);
    //用于保持前一个页面同时刷新本页面，目前只用于SGGeneralInfoLayer
    virtual void refreshView();
    virtual void setIsCocostudio(bool  isCocostudio = false);
    bool getIsCocostudio();
};

#endif /* defined(__GameSanGuo__SGBaseLayer__) */
