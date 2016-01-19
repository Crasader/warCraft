//
//  SGConnectLoading.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-6.
//
//

#include "SGConnectLoading.h"
#include "CCSpriterX.h"

//静态delayShow的时间。
float SGConnectLoading::_delayShowTime = 0.0f;

//ctor
SGConnectLoading::SGConnectLoading() :
_MaxCdSeconds(0)
,_curCdSeconds(0)
,_isDelayShow(false)
{
    ;
}

SGConnectLoading* SGConnectLoading::create()
{
    SGConnectLoading *loadLayer = new SGConnectLoading();
    if (loadLayer && loadLayer->init(NULL, sg_conloadingLayer))
    {
        loadLayer->initView();
        loadLayer->autorelease();
        return loadLayer;
    }
    CC_SAFE_DELETE(loadLayer);
    return NULL;
}
void SGConnectLoading::initView()
{
//    CCSprite *bg = CCSprite::createWithSpriteFrameName("beijing.png");
//    this->addChild(bg);
//    bg->setPosition(SGLayout::getPoint(kMiddleCenter));
//
//    CCSpriterX* fermEffect = CCSpriterX::create("animationFile/animation_loading.scml", true, true);
//    fermEffect->setanimaID(0);
//    fermEffect->setisloop(true);
//    addChild(fermEffect);
//    fermEffect->setPosition(ccpAdd(bg->getPosition(), ccp(0, -90)));
//    fermEffect->play();
   
    
    //removed by cgp
//    CCSpriterX* fermEffect1 = CCSpriterX::create("animationFile/animation_loadingzi.scml", true, true);
//    fermEffect1->setanimaID(0);
//    fermEffect1->setisloop(true);
//    addChild(fermEffect1);
//    fermEffect1->setPosition(ccpAdd(bg->getPosition(), ccp(-10, -136)));
//    fermEffect1->play();
//    this->setVisible(false);
    
//    CCLayerColor* lc = CCLayerColor::create(ccc4(0, 0, 0, 0));
//    this->addChild(lc, 100);
    
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchPriority(-256);
    
    CCDirector::sharedDirector()->getScheduler()->scheduleUpdateForTarget(this, -256, false);
}

void SGConnectLoading::setisshow(bool isshow, float cdSecs /*= 0*/)
{
    if (isshow)
    {
        cdSecs = _delayShowTime;
        if (cdSecs > 0) //启动延时展示。
        {
            this->_curCdSeconds = 0;
            this->_MaxCdSeconds = cdSecs;
            this->_isDelayShow = true;
        }
        else
        {
            this->setVisible(true);
        }
    }
    else
    {
        this->_resetDelayShowParams();
        this->setVisible(false);
    }
    
    
    //开启触控，拦截所有。
    this->setTouchEnabled(isshow);
}

bool SGConnectLoading::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

//延迟展示的检测函数。
void SGConnectLoading::update(float dt)
{
    if (_isDelayShow)
    {
        _curCdSeconds += dt;
        if (_curCdSeconds >= _MaxCdSeconds)
        {
            this->_resetDelayShowParams();
            this->setVisible(true);
        }
    }
}

//静态设置delayShow的时间，防止改变过多api。
void SGConnectLoading::setDelayShowTime(float dsSec)
{
    _delayShowTime = dsSec;
}

//重置延迟展示参数。
void SGConnectLoading::_resetDelayShowParams()
{
    this->_curCdSeconds = 0;
    this->_MaxCdSeconds = 0;
    this->_isDelayShow = false;
}
