//
//  SGMonthlySignItem.cpp
//  GameSanGuo
//
//  Created by Fygo Woo on 12/16/14.
//
//

#include "SGMonthlySignItem.h"
#include "SGDrawCards.h"

//ctor
SGMonthlySignItem::SGMonthlySignItem() : scData_(NULL), frame_(NULL), markTag_(NULL), markLabel_(NULL), mask_(NULL), tick_(NULL), button_(NULL), effect_(NULL), container_(NULL)
{
    ;
}

//dtor
SGMonthlySignItem::~SGMonthlySignItem()
{
    ;
}

//static create
SGMonthlySignItem* SGMonthlySignItem::create(main::SigninCell* sc)
{
    SGMonthlySignItem * ret = new SGMonthlySignItem();
    if (ret && ret->init() )
    {
        ret->scData_ = sc;
        ret->initView();
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return  ret ;
}

//加载视图
void SGMonthlySignItem::initView()
{
    this->ignoreAnchorPointForPosition(false);
    
    regulateFrame();
    
    //角标的背板和文本
    markTag_ = CCSprite::createWithSpriteFrameName("ms_corner_mask.png");
    markTag_->setPosition(ccp(markTag_->getContentSize().width * 0.5, frame_->getContentSize().height - markTag_->getContentSize().height * 0.5));
    this->addChild(markTag_, 14);
    
    markLabel_ = SGCCLabelTTF::create("", FONT_PANGWA, 14, CCSizeZero, kCCTextAlignmentCenter, kCCVerticalTextAlignmentCenter, ccc3(255,236,2), ccWHITE, 0.0, false);
    markLabel_->setAnchorPoint(ccp(0.5,0.5));
//    markLabel_->setPosition(ccpAdd(markTag_->getPosition(), ccp(markTag_->getContentSize().width + 2, 0)));
    markLabel_->setPosition(ccpAdd(markTag_->getPosition(), ccp(-11, 11)));
    markLabel_->setRotation(-45);
    this->addChild(markLabel_, 15);
    
    //蒙版和对勾
    mask_ = CCLayerColor::create(ccc4(44, 20, 0, 180), frame_->getContentSize().width, frame_->getContentSize().height);
    mask_->ignoreAnchorPointForPosition(false);
    mask_->setPosition(frame_->getPosition());
    this->addChild(mask_, 9);
    tick_ = CCSprite::createWithSpriteFrameName("ms_tick.png");
    tick_->setPosition(frame_->getPosition());
    this->addChild(tick_, 10);
    
    //容器
    container_ = CCLayer::create();
    container_->setContentSize(frame_->getContentSize());
    container_->setAnchorPoint(ccp(0.5, 0.5));
    container_->ignoreAnchorPointForPosition(false);
    container_->setPosition((frame_->getPosition()));
    this->addChild(container_, 5);
    
    regulateElements();
    
    this->setContentSize(frame_->getContentSize());
}

//滑动时更新来源
void SGMonthlySignItem::updateItem(main::SigninCell* sc)
{
    scData_ = sc;
    
    regulateFrame();
    regulateElements();
}

//校准基础底板。
void SGMonthlySignItem::regulateFrame()
{
    //底板
    std::string str;
    if (main::NORMAL_HAS == scData_->state()) //已领取
    {
        str = "ms_slot_gotten.png";
    }
    else if (main::NORMAL_NO == scData_->state()) //未领取
    {
        str = "ms_slot_canget.png";
    }
    else if (main::NORMAL_CAN == scData_->state()) //可领取
    {
        str = "ms_slot_gotten.png";
    }
    else if (main::VIP_HALF == scData_->state()) //VIP可再领取一次
    {
        str = "ms_slot_vipmore.png";
    }
    
    if (frame_)
    {
        frame_->removeFromParent();
    }
    frame_ = CCSprite::createWithSpriteFrameName(str.c_str());
    frame_->setPosition(ccp(frame_->getContentSize().width * 0.5, frame_->getContentSize().height * 0.5));
    this->addChild(frame_, 0);
}

//校准各元素，主要是整合new与update的相同过程。
void SGMonthlySignItem::regulateElements()
{
    //角标的背板和文本
    if (scData_->showstring().size() > 0)
    {
        markTag_->setVisible(true);
        markLabel_->setVisible(true);
        markLabel_->setString(scData_->showstring().c_str());
    }
    else
    {
        markTag_->setVisible(false);
        markLabel_->setVisible(false);
    }
    
    //蒙版和对勾
    if (main::NORMAL_HAS == scData_->state())
    {
        mask_->setVisible(true);
        tick_->setVisible(true);
    }
    else
    {
        mask_->setVisible(false);
        tick_->setVisible(false);
    }

    //按钮
    if (button_)
    {
        this->getAllBtn()->removeObject(button_);
        button_->removeFromParent();
    }
    button_ = SGDrawCards::createButton(scData_->itemdata(), sg_monthlySignLayer, SGIT_DETAILED, this, menu_selector(SGMonthlySignItem::onClicked), false);
    button_->setScale(82.0 / 94.0);
    button_->setPosition(frame_->getPosition());
    button_->setZOrder(5);
    container_->addChild(button_); //奇葩的addBtn。
    this->addBtn(button_);
    
    //NB特效
    if (scData_->niubility() >= 1 && main::NORMAL_HAS != scData_->state()) //只有不是“已领取”的才显示特效。
    {
        if (!effect_)
        {
            effect_ = CCSpriterX::create("animationFile/ms_niubility.scml", true);
            effect_->setanimaID(0);
            effect_->setisloop(true);
            effect_->setPosition(frame_->getPosition());
            effect_->play();
            this->addChild(effect_, 3);
        }
        
        if (effect_)
        {
            effect_->setVisible(true);
        }
    }
    else
    {
        if (effect_)
        {
            effect_->setVisible(false);
        }
    }
}

//点击。
void SGMonthlySignItem::onClicked(CCObject* sender)
{
    //FIXME: 发生什么了？
    return;
}

