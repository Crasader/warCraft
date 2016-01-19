//
//  SGStoryBattleLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-7.
//
//

#include "SGStoryBattleLayer.h"
#include "SGNotificationCenter.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PlotModule.pb.h"
#include "SGStoryReward.h"
#include "SGStoryRewardBox.h"
#include "SGMainManager.h"
#include "ResourceManager.h"

SGStoryBattleLayer::SGStoryBattleLayer(void)
{
    
}
SGStoryBattleLayer::~SGStoryBattleLayer(void)
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("public/Main.plist");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battlePreLayer);
    //ResourceManager::sharedInstance()->unBindTexture("public/Main.plist", RES_TYPE_LAYER_UI, sg_battlePreLayer);
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_STORY_BATTLE_OVER);
}

SGStoryBattleLayer* SGStoryBattleLayer::creat()
{
    SGStoryBattleLayer *layer = new SGStoryBattleLayer();
    if (layer &&  layer->init(NULL, sg_battleMainLayer)) {
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGStoryBattleLayer::initView()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_STORY_BATTLE_OVER, this, callfuncO_selector(SGStoryBattleLayer::afterGameOver));
	
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("public/Main.plist");
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_battlePreLayer, LIM_PNG_AS_PNG);


    CCSprite *spriteBlackbg = CCSprite::createWithSpriteFrameName("public_blackBoard.png");
    spriteBlackbg->setPosition(SGLayout::getPoint(kMiddleCenter));
    spriteBlackbg->setScaleX(skewing(320)/spriteBlackbg->getContentSize().width);
    spriteBlackbg->setScaleY(skewing(480)/spriteBlackbg->getContentSize().height);
    this->addChild(spriteBlackbg, -1);
    
    this->fighting(spriteBlackbg->getPosition());
}

void SGStoryBattleLayer::fighting(CCPoint position)
{
    CCLabelTTF *labelFight = CCLabelTTF::create("Fighting!!!", "", FONTSIZE(15));
    labelFight->setScale(2.5f);
    labelFight->setPosition(position);
    CCAction *action = CCSequence::create(CCFadeIn::create(0.05f),
                                          CCScaleTo::create(1.0f, 1.5f),
                                          CCFadeOut::create(0.15f),
                                          CCCallFuncN::create(this, callfuncN_selector(SGStoryBattleLayer::afterFighting)),
                                          NULL);
    this->addChild(labelFight);
    labelFight->runAction(action);
}

void SGStoryBattleLayer::afterFighting(CCNode *sender)
{
    sender->removeFromParentAndCleanup(true);
    CCLabelTTF *label = CCLabelTTF::create("GAME OVER", "", FONTSIZE(15));
    label->setScale(2.5f);
    label->setPosition(SGLayout::getPoint(kMiddleCenter));
    CCAction *action = CCSequence::create(CCFadeIn::create(0.05f),
                                          CCScaleTo::create(1.0f, 1.5f),
                                          CCFadeOut::create(0.15f),
                                          CCCallFuncN::create(this, callfuncN_selector(SGStoryBattleLayer::gameOver)),
                                          NULL);
    this->addChild(label);
    label->runAction(action);
}

void SGStoryBattleLayer::gameOver(CCNode *sender)
{
    sender->removeFromParentAndCleanup(true);
    main::PlotEndRequest *request = new main::PlotEndRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_BATTLE_OVER, request);
}

void SGStoryBattleLayer::afterGameOver(CCObject *sender)
{
    CCAssert(1==0, "fatal error,废弃代码");
    SGSocketRequest *request = (SGSocketRequest *)sender;
    main::PlotEndResponse *response = (main::PlotEndResponse *)request->m_msg;
    if (response)
    {
        SGStoryRewardBox *box = SGStoryRewardBox::create(this, NULL);
        SGMainManager::shareMain()->showBox(box);
    }
}


