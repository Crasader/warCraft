//
//  SGBindSuccessBox.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-10.
//
//

#include "SGBindSuccessBox.h"
#include "GameMusicmanage.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGBindSuccessBox::SGBindSuccessBox()
{
    CCLOG("SGBindSuccessBox 构造函数");
}

SGBindSuccessBox::~SGBindSuccessBox()
{
    CCLOG("SGBindSuccessBox 析构函数");
 
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_bindSuccessBox);
}

SGBindSuccessBox* SGBindSuccessBox::create(SGBoxDelegate *delegate)
{
    SGBindSuccessBox *success = new SGBindSuccessBox();
    if(success && success->init(delegate, box_bindSuccessBox, CCSizeMake(535, 324)))
    {
        success->viewDidLoad();
        success->autorelease();
        return success;
    }
    
    CC_SAFE_DELETE(success);
    return NULL;
    
}

void SGBindSuccessBox::viewDidLoad()
{
    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_bindSuccessBox);
  
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    //标题
    CCSprite *sprite_title = CCSprite::createWithSpriteFrameName("login_bind_success.png");
    sprite_title->setPosition(biao->getPosition());
    addChild(sprite_title);
    
    //内容1
    SGCCLabelTTF *label_content = SGCCLabelTTF::create(str_TTMainLayer_str7,FONT_BOXINFO, 32);
    label_content->setAnchorPoint(ccp(0, 0.5));
    label_content->setPosition(ccpAdd(center, ccp(-230, 73)));
    addChild(label_content);
    
    //内容2
    SGCCLabelTTF *label_content_2 = SGCCLabelTTF::create(str_TTMainLayer_str8, FONT_BOXINFO, 32);
    label_content_2->setAnchorPoint(ccp(0, 0.5));
    label_content_2->setPosition(ccpAdd(label_content->getPosition(), ccp(0, -55)));
    addChild(label_content_2);
    
    SGCCLabelTTF *label_content_3 = SGCCLabelTTF::create(str_TTMainLayer_str9, FONT_BOXINFO, 32);
    label_content_3->setAnchorPoint(ccp(0, 0.5));
    label_content_3->setPosition(ccpAdd(label_content_2->getPosition(), ccp(0, -55)));
    addChild(label_content_3);
    
    //确定按钮
    SGButton *buttonOK = SGButton::create("box_btnbg.png", "public_font_queding.png", this, menu_selector(SGBindSuccessBox::buttonClickOK),CCPointZero,false,true);
    buttonOK->setPosition(ccpAdd(center, ccp(0, -324/2 +5 -40)));
    addBtn(buttonOK);
}

void SGBindSuccessBox::buttonClickOK(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    CCDictionary *dict = CCDictionary::create();
    dict->setObject(CCString::create("BingSuccess"), "key");
    this->data = dict;
    this->boxClose();
}










