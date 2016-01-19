//
//  SGAdventureLayer.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 12/25/14.
//
//

#include "SGAdventureLayer.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGExoticMerchantLayer.h"

//滑框最大宽度
const static float SC_WIDTH = 275.0;

//ctor
SGAdventureLayer::SGAdventureLayer() : scView_(NULL), scContainer_(NULL), curAdvType_(SGAT_GIANT), curAdvLayer_(NULL)
{
    ;
}

//dtor
SGAdventureLayer::~SGAdventureLayer()
{
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(true);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_adventureLayer);
}

//create
SGAdventureLayer* SGAdventureLayer::create()
{
    SGAdventureLayer* ret = new SGAdventureLayer();
    if (ret && ret->init(NULL, sg_adventureLayer))
    {
        ret->initView();
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}

//iv
bool SGAdventureLayer::initView()
{
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
    
    ResourceManager* rm = ResourceManager::sharedInstance();
    rm->bindTexture("sgadventurelayer/sgadventurelayer.plist", RES_TYPE_LAYER_UI, sg_adventureLayer);
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float headhgt = SGMainManager::shareMain()->getVisitHgt();
    
    //背景条，顶距190，底距170。
    float bkgHeight = s.height - 190 - 170 - 1;
    CCSprite* bkg = CCSprite::createWithSpriteFrameName("sgavn_bkg.png");
    bkg->setScaleX( (s.width - 1) / bkg->getContentSize().width );
    bkg->setScaleY( bkgHeight / bkg->getContentSize().height );
    bkg->setPosition(ccp(s.width * 0.5, bkgHeight * 0.5 + 170));
    this->addChild(bkg, 1);
    
    //背景金框
    CCScale9Sprite* goldFrame = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png", CCRectMake(48, 48, 84, 84));
    goldFrame->setContentSize(CCSizeMake(bkg->getContentSize().width * bkg->getScaleX() + 1, bkg->getContentSize().height * bkg->getScaleY() + 1));
    goldFrame->setPosition(bkg->getPosition());
    this->addChild(goldFrame, 2);
    
    //滑框背景。
    CCSprite* scBkg = CCSprite::createWithSpriteFrameName("sgavn_bar.png");
    scBkg->setScaleX(s.width / 640);
    scBkg->setPosition(ccp(s.width * 0.5, s.height - 190 + scBkg->getContentSize().height * 0.5));
    this->addChild(scBkg, 2);
    
    //滑框
    scView_ = CCScrollView::create(CCSizeMake(SGLayout::getSkewing(SC_WIDTH), 112));
    scContainer_ = CCLayerColor::create();
    
    scView_->setContainer(scContainer_);
    scView_->ignoreAnchorPointForPosition(false);
    scView_->setAnchorPoint(ccp(.5,.5));
    
    scView_->setPosition(ccp(s.width*.5, s.height-headhgt));//ccpAdd(ccp(s.width*.6, s.height-headhgt), ccp(0, 0))
    scView_->setDirection(kCCScrollViewDirectionHorizontal);
    this->addChild(scView_, 5);
    
    //滑框的条目
    CCSize iSize = CCSizeMake(113, 113);
    for (int k = 0; k < SGAT_APPLE; k++)
    {
        advItemArray_[k] = SGAdventureItem::create(this, ADVENTURE_ITEM_ICON[k], k, iSize);
        advItemArray_[k]->setPosition(ccp(110 * k,-8));
        scContainer_->addChild(advItemArray_[k]);
    }
    
    return true;
}

//显示第几项模块。
void SGAdventureLayer::showPageByIndex(SGAdventureType at, void* data)
{
    if (at <= SGAT_GIANT || at >= SGAT_APPLE)
    {
        return;
    }
    
    if (at == curAdvType_)
    {
        return;
    }
    else
    {
        if (curAdvLayer_) //移除原先的。
        {
            curAdvLayer_->removeFromParentAndCleanup(true);
            curAdvLayer_ = NULL;
        }
    }
    
    const int laZd = 10;
    
    switch (at)
    {
        case SGAT_EXOTIC_MERCHANT:
            curAdvLayer_ = SGExoticMerchantLayer::create((main::XiyuInfoResponse*) data);
            this->addChild(curAdvLayer_, laZd);
            break;
        default:
            assert(0 == 1);
            break;
    }
    
    curAdvType_ = at;
    
    //控制条目显隐
    for (int k = 0; k < SGAT_APPLE; k++)
    {
        advItemArray_[k]->setSelected(at == k ? true : false);
    }
    
    //触发红点
    handleNotices();
}

//点击了index的条目。
void SGAdventureLayer::selectHandler(int index)
{
    showPageByIndex((SGAdventureType) index);
}

//修改触控。
void SGAdventureLayer::setIsCanTouch(bool isTouch)
{
    SGBaseLayer::setIsCanTouch(isTouch);
    scView_->setTouchEnabled(isTouch);
    for (int k = 0; k < SGAT_APPLE; k++)
    {
        advItemArray_[k]->setEnabled(isTouch);
    }
}

//触发红点逻辑。
void SGAdventureLayer::handleNotices()
{
    advItemArray_[SGAT_EXOTIC_MERCHANT]->setNotice(SGExoticMerchantLayer::getIsForcedOpen());
}

