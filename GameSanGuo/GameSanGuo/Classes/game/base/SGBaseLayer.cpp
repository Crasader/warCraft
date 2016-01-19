//
//  SGBaseLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 12-12-20.
//
//

#include "SGBaseLayer.h"
SGBaseLayer::SGBaseLayer()
{
    
}
SGBaseLayer::~SGBaseLayer()
{
//    CCLOG("~SGBaseLayer");
}
bool SGBaseLayer::init(const char *fileName_, LayerTag tag_, bool isSpriteFrame)
{
    setIsCocostudio();     //默认不是cocostudio,  后期统一处理，cgp cocos
    if (!SGBaseShow::init(fileName_, tag_, isSpriteFrame))
    {
        return false;
    }
    return true;
}
void SGBaseLayer::showBoxCall(CCObject *obj)
{
    
}
void SGBaseLayer::setIsCanTouch(bool isTouch)
{
    isCanTouch = isTouch;
    menu->setTouchEnabled(isTouch);
}

//用于保持前一个页面同时刷新本页面，目前只用于SGGeneralInfoLayer
void SGBaseLayer::refreshView()
{
    ;
}

void SGBaseLayer::setIsCocostudio(bool isCocos)
{
    m_isCocostudio = isCocos;
}

bool SGBaseLayer::getIsCocostudio()
{
    return m_isCocostudio;
}




