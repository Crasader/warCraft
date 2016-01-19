//
//  SGMenuLabel.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-5-8.
//
//
#include "SGLayout.h"
#include "SGMenuLabel.h"
SGMenuLabel *SGMenuLabel::create(cocos2d::CCNode *label, cocos2d::CCObject *target, SEL_MenuHandler selector)
{
    SGMenuLabel *btn = new SGMenuLabel();
    
    if (btn && btn->init(label,target,selector))
    {
        btn->autorelease();
        return btn;
    }
    CC_SAFE_DELETE(btn);
    return NULL;
}
bool SGMenuLabel::init(cocos2d::CCNode *label, cocos2d::CCObject *target, SEL_MenuHandler selector)
{
    if (!CCMenuItemLabel::initWithLabel(label, target, selector))
    {
        return false;
    }
    return true;
}
void SGMenuLabel::setLabel(CCNode* var)
{
    if (var)
    {
        var->setAnchorPoint(ccp(0, 0));
        setContentSize(CCSize(250, var->getContentSize().height));
        
            CCLabelTTF *centerLabel = (CCLabelTTF*)var;
            CCLabelTTF *label_1 = CCLabelTTF::create(centerLabel->getString(), FONT_BOXINFO, centerLabel->getFontSize());
            CCLabelTTF *label_2 = CCLabelTTF::create(centerLabel->getString(), FONT_BOXINFO, centerLabel->getFontSize());
            CCLabelTTF *label_3 = CCLabelTTF::create(centerLabel->getString(), FONT_BOXINFO, centerLabel->getFontSize());
            CCLabelTTF *label_4 = CCLabelTTF::create(centerLabel->getString(), FONT_BOXINFO, centerLabel->getFontSize());
            addChild(label_1);
            addChild(label_2);
            addChild(label_3);
            addChild(label_4);
        
        label_1->setPosition(ccpAdd(centerLabel->getPosition(), ccp(2, 0)));
        label_2->setPosition(ccpAdd(centerLabel->getPosition(), ccp(-2, 0)));
        label_3->setPosition(ccpAdd(centerLabel->getPosition(), ccp(0, 2)));
        label_4->setPosition(ccpAdd(centerLabel->getPosition(), ccp(0, -2)));
        label_1->setColor(ccBLACK);
        label_2->setColor(ccBLACK);
        label_3->setColor(ccBLACK);
        label_4->setColor(ccBLACK);
        label_1->setAnchorPoint(ccp(0, 0));
        label_2->setAnchorPoint(ccp(0, 0));
        label_3->setAnchorPoint(ccp(0, 0));
        label_4->setAnchorPoint(ccp(0, 0));

        addChild(var);

    }
    
    if (m_pLabel)
    {
        removeChild(m_pLabel, true);
    }
    
    m_pLabel = var;
}
