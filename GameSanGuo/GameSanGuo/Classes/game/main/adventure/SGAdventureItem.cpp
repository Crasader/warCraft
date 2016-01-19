//
//  SGAdventureItem.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 12/26/14.
//
//

#include "SGAdventureItem.h"
#include "SGButton.h"

//ctor
SGAdventureItem::SGAdventureItem() : isMoving_(false), menu_(NULL), canTouch_(false), index_(0), selectedSpt_(NULL), deg_(NULL), noticeSpt_(NULL)
{
    
}

//dtor
SGAdventureItem::~SGAdventureItem()
{
    
}

//create。image为传进来的图片，text为文本。
SGAdventureItem* SGAdventureItem::create(SGAdventureDelegate* deg, const std::string& image, const int index, const CCSize& vSize, const std::string& text)
{
    SGAdventureItem* ret = new SGAdventureItem();
    if (ret && ret->initView(image, vSize, text))
    {
        ret->deg_ = deg;
        ret->index_ = index;
        ret->autorelease();
        return ret;
    }
    
    CC_SAFE_DELETE(ret);
    return NULL;
}

//iv
bool SGAdventureItem::initView(const std::string& image, const CCSize& vSize, const std::string& text)
{
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0), vSize.width, vSize.height))
    {
        return false;
    }
    
    //尺寸
    viewSize_ = vSize;
    this->setContentSize(vSize);
    
    //按钮
    SGButton *btn = SGButton::create(image.c_str(), NULL, this,menu_selector(SGAdventureItem::btnHandler),CCPointZero,false,true);
    
    //选中图形
    selectedSpt_ = CCSprite::createWithSpriteFrameName("equipPinzhi_1_3.png");
    selectedSpt_->setPosition(ccp(viewSize_.width/2, viewSize_.height/2));
    this->addChild(selectedSpt_,5);
    
    //红点
    noticeSpt_ = CCSprite::createWithSpriteFrameName("publc_notice.png");
    noticeSpt_->setPosition(ccp(viewSize_.width - noticeSpt_->getContentSize().width * 0.5, viewSize_.height - noticeSpt_->getContentSize().height * 0.5));
    noticeSpt_->setVisible(false);
    this->addChild(noticeSpt_, 10);
    
    
    //菜单
    menu_ = CCMenu::create(btn, NULL);
    menu_->setPosition(ccp(viewSize_.width/2, viewSize_.height/2));
    this->addChild(menu_);
    
    return true;
}

//按钮回调。
void SGAdventureItem::btnHandler(CCObject* sender)
{
    if (deg_)
    {
        deg_->selectHandler(index_);
    }
}

//设置触控开关。
void SGAdventureItem::setEnabled(bool bl)
{
    canTouch_ = bl;
    menu_->setEnabled(bl);
}

//设置选中状态。
void SGAdventureItem::setSelected(bool bl)
{
    selectedSpt_->setVisible(bl);
}

//设置红点显隐。
void SGAdventureItem::setNotice(bool bl)
{
    noticeSpt_->setVisible(bl);
}

//began
bool SGAdventureItem::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!canTouch_)
    {
        return false;
    }

    ((CCScrollView*)getParent()->getParent())->ccTouchBegan(pTouch, pEvent);
    touchBeginLocation_ = pTouch->getLocationInView();
    isMoving_ = false;
    return true;
}

//moved
void SGAdventureItem::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!canTouch_)
    {
        return;
    }
    
    if (!isMoving_)
    {
        CCPoint touchLocation = pTouch->getLocationInView();
        float distance = ccpDistance(touchLocation, touchBeginLocation_);
        if (distance > 10)
        {
            isMoving_ = true;
        }
    }
    
    if (isMoving_)
    {
        ((CCScrollView*)getParent()->getParent())->ccTouchMoved(pTouch, pEvent);
    }

}

//ended
void SGAdventureItem::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!canTouch_)
    {
        return;
    }
    
    ((CCScrollView*)getParent()->getParent())->ccTouchEnded(pTouch, pEvent);
}

