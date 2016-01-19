//
//  SGMessageLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-2-20.
//
//

#include "SGMessageLayer.h"
#include "SGLayout.h"
#include "SGMainManager.h"
#include "ResourceManager.h"

SGMessageLayer::SGMessageLayer():guang(NULL), guang1(NULL)
{
    spriteBackground = NULL;
    spriteBottomLine = NULL;
    spriteUpLine = NULL;
    labelContent = NULL;
}

SGMessageLayer::~SGMessageLayer()
{
		ResourceManager::sharedInstance()->unBindLayerTexture(sg_messageLayer);
}

SGMessageLayer* SGMessageLayer::create()
{
    SGMessageLayer *message = new SGMessageLayer();
    if(message)
    {
        message->constructView();
        message->autorelease();
        return message;
    }
    
    CC_SAFE_DELETE(message);
    return NULL;
}

void SGMessageLayer::constructView()
{
	ResourceManager::sharedInstance()->bindTexture("sgherolayer/sgherolayer.plist", RES_TYPE_LAYER_UI, sg_messageLayer);
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_messageLayer, LIM_PNG_AS_PNG);
    
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    spriteUpLine =CCSprite::createWithSpriteFrameName("gonggao_2.png");
    spriteBottomLine = CCSprite::createWithSpriteFrameName("gonggao_2.png");
    spriteUpLine->setScaleX(s.width/500);
    spriteBottomLine->setScaleX(s.width/500);
    spriteBackground = CCSprite::createWithSpriteFrameName("message_line.png");
    labelContent = SGCCLabelTTF::create("", FONT_BOXINFO, 28);
    
    spriteBackground->setPosition(ccpAdd(center, ccp(0, 200)));
    spriteUpLine->setAnchorPoint(ccp(0.5, 0));
    spriteUpLine->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(0, -1)));
    spriteBottomLine->setAnchorPoint(ccp(0.5, 1));
    spriteBottomLine->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(0, 1)));
    labelContent->setPosition(spriteBackground->getPosition());
    
    //拉伸
    spriteBackground->setScaleX(SGLayout::getSkewing(320)/spriteBackground->getContentSize().width);
    guang =  CCSprite::createWithSpriteFrameName("message_guang.png");
    guang1 = CCSprite::createWithSpriteFrameName("message_guang.png");
    this->addChild(guang,5);
    this->addChild(guang1,5);
    this->addChild(spriteBackground);
    this->addChild(spriteUpLine);
    this->addChild(spriteBottomLine);
    this->addChild(labelContent);
    
    guang->setPosition(ccp(s.width +guang->getContentSize().width/2, 27.5 +spriteBackground->getPosition().y));
    guang1->setPosition(ccp( - guang->getContentSize().width/2, -27.5 +spriteBackground->getPosition().y));
    this->setVisible(false);
}

void SGMessageLayer::showMessage(const char *content)
{
    guang->stopAllActions();
    guang1->stopAllActions();
    spriteBackground->stopAllActions();
    spriteUpLine->stopAllActions();
    spriteBottomLine->stopAllActions();
    spriteUpLine->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(0, -1)));
    spriteBottomLine->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(0, 1)));
    guang->setPosition(ccp(skewing(320) +guang->getContentSize().width/2, 27.5 +spriteBackground->getPosition().y));
    guang1->setPosition(ccp( - guang->getContentSize().width/2, -27.5 +spriteBackground->getPosition().y));
    spriteBackground->setScale((float)1/30);
    this->setVisible(true);
    labelContent->setString(content);
    guang->setVisible(false);
    guang1->setVisible(false);
    labelContent->setVisible(false);
    CCScaleTo *scale = CCScaleTo::create(.3, 26);
    CCMoveBy *upmove = CCMoveBy::create(.3, ccp(0, 26));
    CCMoveBy *downmove = CCMoveBy::create(.3, ccp(0, -26));
    spriteBottomLine->runAction(downmove);
    spriteUpLine->runAction(upmove);
    spriteBackground->runAction(CCSequence::create(scale,CCCallFuncN::create(this, callfuncN_selector(SGMessageLayer::showlab)),CCDelayTime::create(2),CCCallFuncN::create(this, callfuncN_selector(SGMessageLayer::callFunction)),NULL));
//    SGBaseLayer *layer= (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(SGMainManager::shareMain()->getMainScene()->nowLayerTag);
//    layer->setIsCanTouch(false);
}

void SGMessageLayer::callFunction()
{
    this->setVisible(false);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    spriteUpLine->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(0, -1)));
    
    spriteBottomLine->setPosition(ccpAdd(spriteBackground->getPosition(), ccp(0, 1)));
    spriteBackground->setScale((float)1/30);
    guang->setPosition(ccp(s.width/2 +guang->getContentSize().width/2, 27.5 +spriteBackground->getPosition().y));
    guang1->setPosition(ccp(s.width/2 - guang->getContentSize().width/2, -27.5 +spriteBackground->getPosition().y));
    
//    SGBaseLayer *layer= (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(SGMainManager::shareMain()->getMainScene()->nowLayerTag);
//    layer->setIsCanTouch(true);

}
void SGMessageLayer::showlab()
{
    guang->setVisible(true);
    guang1->setVisible(true);
    labelContent->setVisible(true);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCMoveBy *upmove = CCMoveBy::create(1, ccp(-s.width-guang->getContentSize().width, 0));
    CCMoveBy *downmove = CCMoveBy::create(1, ccp(s.width + guang->getContentSize().width, 0));
    guang->runAction(upmove);
    guang1->runAction(downmove);
}

