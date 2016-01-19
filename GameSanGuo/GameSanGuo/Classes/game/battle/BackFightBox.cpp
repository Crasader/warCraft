//
//  BackFightBox.cpp
//  GameSanGuo
//
//  Created by geng on 13-7-29.
//
//

#include "BackFightBox.h"
#include "GameMusicmanage.h"
#include "SimpleAudioEngine.h"
#include "ResourceManager.h"
#include "SGBattleManager.h"
#include "SGMainManager.h"
#include "SGCantAdvanceBox.h"
#include "SGStringConfig.h"

BackFightBox::~BackFightBox()
{
}
BackFightBox *BackFightBox::create(SGBoxDelegate *target)
{
    BackFightBox *bfb = new BackFightBox();
    if (bfb->init(target, box_backFight, CCSizeMake(535,324)))
    {
//        bfb->from = from;
        bfb->autorelease();
        bfb->initView();
        return bfb;
    }
    CC_SAFE_DELETE(bfb);
    return NULL;
}
void BackFightBox::initView()
{
    BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
    
//    SGButton *okButton = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(BackFightBox::btnYesHandler),CCPointZero,false,true);
    SGButton *okButton = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this, menu_selector(BackFightBox::btnNoHandler),CCPointZero,false,true);

    okButton->setTag(1);
    SGButton *cancelButton = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this, menu_selector(BackFightBox::btnNoHandler),CCPointZero,false,true);
    cancelButton->setTag(2);
    const char *str;
    
    if (BT_ADVENTURE == btp)
    {
        str = str_back_battale_tips1;
    }
    else if (BT_SURVIVAL == btp)
    {
        str = str_back_battale_tips2;
    }
    else if (BT_LADDER == btp)
    {
        str = str_back_battale_tips3;
    }
    else
    {
        str = str_back_battale_tips4;
    }
    SGCCLabelTTF *label = SGCCLabelTTF::create(str, FONT_PANGWA, 32, CCSizeMake(32*13, 200), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    label->setAnchorPoint(ccp(.5, 1));
    label->setPosition(ccpAdd(biao->getPosition(), ccp(0, -75)));
    this->addChild(label);
    this->addBtn(okButton);
    this->addBtn(cancelButton);
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+40)));
    cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+40)));
    
    data = NULL;
    
}
void BackFightBox::btnYesHandler(SGButton *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    callbackUpward(1);
}
//取消，对守荆州有二次确认。
void BackFightBox::btnNoHandler(SGButton *sender)
{
//    EFFECT_PLAY(MUSIC_BTN);
//    
//    BATTLE_TYPE btp = SGBattleManager::sharedBattle()->getBattleType();
//    
//    if (BT_SURVIVAL == btp)
//    {
//        //放弃守荆州则二次确认
//        SGCantAdvanceBox *box = SGCantAdvanceBox::create(this->deletage, NULL, 52, 0, NULL);
//        SGMainManager::shareMain()->showBox(box);
//        this->boxCloseWithOutCallBack();
//    }
//    else
    {
        callbackUpward(2);
    }
}

//向上的回调，二次确认时也可以触发。
void BackFightBox::callbackUpward(int tag)
{
    CCDictionary *obj = CCDictionary::create();
    obj->setObject(CCString::createWithFormat("%d",tag), "isback");
    data = obj;
    this->boxClose();
}
