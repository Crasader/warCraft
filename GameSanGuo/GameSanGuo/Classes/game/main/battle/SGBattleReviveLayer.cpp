//
//  SGBattleReviveLayer.cpp
//  GameSanGuo
//
//  Created by wenlong w on 13-5-21.
//
//

#include "SGBattleReviveLayer.h"
#include "SGBattleManager.h"
#include "SGPlayerInfo.h"
#include "SGGuideManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGCCLabelTTF.h"

#define ROUND_PROMPT_W 500
#define ROUND_PROMPT_H 300
#define REVIVE_EXPENSE 60


SGBattleReviveLayer::SGBattleReviveLayer()
:_target(NULL)
{
    
}

SGBattleReviveLayer::~SGBattleReviveLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battleRevivelayer);
}

SGBattleReviveLayer* SGBattleReviveLayer::create(SGBoxDelegate *target, int round)
{
    SGBattleReviveLayer *reviveLayer = new SGBattleReviveLayer();
    
    reviveLayer->_target = target;
    if(reviveLayer && reviveLayer->SGBaseBox::init(target, box_battleReviveLayer, CCSizeMake(500, 390)))
    {
		reviveLayer->fixBackgroundPos();
        return reviveLayer;
    }
    
    CC_SAFE_DELETE(reviveLayer);
    return NULL;
}
//修正背景的位置
void SGBattleReviveLayer::fixBackgroundPos()
{
	if (fontbg)
	{
		fontbg->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, 0)));
	}
}

void SGBattleReviveLayer::initMsg()
{
    int w = this->getContentSize().width;
    int h = this->getContentSize().height;
	
    
    CCSprite *title_Text = CCSprite::createWithSpriteFrameName("revive_prompt_n.png");
    this->addChild(title_Text, 1);
    title_Text->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, 260*.5f+10)));
//    title_Text->setPosition(ccp(w * 0.5,h * 0.5+title_Text->getContentSize().height*1.5));
    int a = SGPlayerInfo::sharePlayerInfo()->getPlayerGold();
    bool guide = SGGuideManager::shareManager()->isGuide=false;

    CCString *text = CCString::createWithFormat( guide ? str_BattlePreLayer_str3:str_BattlePreLayer_str4, REVIVE_EXPENSE, a);
    
    SGCCLabelTTF *label = SGCCLabelTTF::create(text->getCString(), FONT_PANGWA, 30, CCSize(ROUND_PROMPT_W * 0.8f, ROUND_PROMPT_H*0.5f),kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    label->setPosition(ccp(w * 0.5, h * 0.5 - 10));
    //label->setColor(ccBLACK);
    this->addChild(label, 1);
    
    SGButton *Ok = SGButton::create("box_btnbg.png", "public_font_queding.png", this, menu_selector(SGBattleReviveLayer::buttonOk), CCPointZero, false, true);
    SGButton *close = SGButton::create("box_btnbg.png", "public_font_qixiao.png", this, menu_selector(SGBattleReviveLayer::buttonClose),CCPointZero,false,true);
    
    this->addBtn(Ok);
    this->addBtn(close);
    
    Ok->setPosition(ccp(screenwidth * 0.5f - ROUND_PROMPT_W * 0.5f + Ok->getContentSize().width * 0.5f + 25, screenheight * 0.5f - ROUND_PROMPT_H * 0.5f - Ok->getContentSize().height * 0.5f + 30));
    close->setPosition(ccp(screenwidth * 0.5f + ROUND_PROMPT_W * 0.5f - close->getContentSize().width * 0.5f - 25, screenheight * 0.5f - ROUND_PROMPT_H * 0.5f - close->getContentSize().height * 0.5f + 30));
}

void SGBattleReviveLayer::buttonOk()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    bool guide = SGGuideManager::shareManager()->isGuide;
    if (!guide)
    SGBattleManager::sharedBattle()->fightReviveRequest(1);
    
}

void SGBattleReviveLayer::buttonClose()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
    SGBattleManager::sharedBattle()->fightReviveRequest(0);
}

