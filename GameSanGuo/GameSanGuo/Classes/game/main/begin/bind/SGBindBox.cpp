//
//  SGBindBox.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-15.
//
//

#include "SGBindBox.h"
#include "SGStringConfig.h"
SGBindBox *SGBindBox::create(SGBoxDelegate *dg)
{
    SGBindBox *bindBox = new SGBindBox();
    if (bindBox && bindBox->init(dg, box_bindBox, CCSizeMake(411, 318)))
    {
        bindBox->autorelease();
        return bindBox;
    }
    CC_SAFE_DELETE(bindBox);
    return NULL;
}


SGBindBox::SGBindBox()
{
    edit_account = NULL;
    edit_password = NULL;
    edit_password_again = NULL;
    label_account_error = NULL;
    label_password_again_error = NULL;
    label_password_error = NULL;
}

SGBindBox::~SGBindBox()
{
    
}

void SGBindBox::viewDidLoad()
{
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    //标题
    CCSprite *sprite_title = CCSprite::create("");
    sprite_title->setPosition(ccpAdd(center,ccp(0,200)));
    addChild(sprite_title);
    
    
    //红色框
    CCSprite *sprite_red_bg = CCSprite::create("");
    sprite_red_bg->setPosition(ccpAdd(center, ccp(0, 180)));
    addChild(sprite_red_bg);
    

    
    CCLabelTTF *label_word_1 = CCLabelTTF::create(str_TTMainLayer_str5, FONT_BOXINFO, 24);
    label_word_1->setPosition(ccpAdd(center, ccp(0, 170)));
    addChild(label_word_1);
    
    
    CCLabelTTF *label_word_2 = CCLabelTTF::create(str_TTMainLayer_str6, FONT_BOXINFO, 24);
    label_word_2->setDimensions(CCSizeMake(label_word_1->getContentSize().width, 2*label_word_1->getContentSize().height));
    label_word_2->setPosition(ccpAdd(center, ccp(0, 160)));
    addChild(label_word_2);
    
    //输入帐号
    edit_account = CCEditBox::create(CCSizeMake(360, 50), CCScale9Sprite::create(""));
    edit_account->setPosition(ccpAdd(center, ccp(0, 150)));
    addChild(edit_account);
    
    //输入密码
    edit_password = CCEditBox::create(CCSizeMake(360, 50), CCScale9Sprite::create(""));
    edit_password->setPosition(ccpAdd(center, ccp(0, 80)));
    addChild(edit_password);

    //重复输入密码
    edit_password_again = CCEditBox::create(CCSizeMake(360, 50), CCScale9Sprite::create(""));
    edit_password_again->setPosition(ccpAdd(center, ccp(0, -10)));
    addChild(edit_password_again);

    label_account_error = CCLabelTTF::create("", "", 24);
    label_password_error = CCLabelTTF::create("", "", 24);
    label_password_again_error = CCLabelTTF::create("", "", 24);
    
}