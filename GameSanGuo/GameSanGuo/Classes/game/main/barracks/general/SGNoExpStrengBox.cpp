//
//  SGNoExpStrengBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-20.
//
//

#include "SGNoExpStrengBox.h"
#include "SGFont.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGNoExpStrengBox::~SGNoExpStrengBox()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_noExpStrengBox);
}

SGNoExpStrengBox *SGNoExpStrengBox::create(SGBoxDelegate *bdg, SGOfficerCard *card)
{
    //无处调用
    SGNoExpStrengBox *noExpBox = new SGNoExpStrengBox();
    if (noExpBox && noExpBox->init(bdg,card))
    {
        noExpBox->_card = card;
        noExpBox->autorelease();
        return noExpBox;
    }
    CC_SAFE_DELETE(noExpBox);
    return NULL;
}
bool SGNoExpStrengBox::init(SGBoxDelegate *bdg, SGOfficerCard *card)
{
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_box_noExpStrengBox, LIM_PNG_AS_PNG);
 
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float wid = skewing(220);
    float hgt = skewingY(180);
    if(s.height == 1136)
    {
        hgt = skewing(180);
    }
    if (!SGBaseBox::init(bdg,box_noExpStrengTag,CCSizeMake(wid, hgt)))
    {
        return false;
    }  
   
    SGButton *ok = SGButton::create("box_btnbg.png",
                                    "public_font_queding.png",
                                    this,
                                    menu_selector(SGNoExpStrengBox::confirmHandler),
                                    ccp(0, 0),
                                    false,
                                    true);
    this->addBtn(ok);
    ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.2, -hgt*.62-10)));
    
    
    SGButton *cancel = SGButton::create("box_btnbg.png",
                                        "public_font_quxiao.png",
                                        this,
                                        menu_selector(SGNoExpStrengBox::boxClose),
                                        ccp(0, 0),
                                        false,
                                        true);
    this->addBtn(cancel);
    cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.2, -hgt*.62-10)));
 
//    CCSprite *title = CCSprite::createWithSpriteFrameName("font_notice.png");
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips28, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    title->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.353)));
    this->addChild(title);
    CCLabelTTF* a = CCLabelTTF::create(str_NoExpStrengBox_str1, "Arial-BoldMT", FONTSIZE(14));
    a->setHorizontalAlignment(kCCTextAlignmentLeft);    
    this->addChild(a);
    a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.1)));
    
    CCLabelTTF *b = CCLabelTTF::create(str_NoExpStrengBox_str2, "Courier", FONTSIZE(13));
    this->addChild(b);
    b->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt*.3)));
    b->setHorizontalAlignment(kCCTextAlignmentLeft);
    return true;
}

void SGNoExpStrengBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGMainManager::shareMain()->showStrengLayer(_card, 1);
}