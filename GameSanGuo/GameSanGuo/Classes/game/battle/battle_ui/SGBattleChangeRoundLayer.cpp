//
//  SGBattleChangeRoundLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-27.
//
//

#include "SGBattleChangeRoundLayer.h"
#include "SGLayout.h"
#include "GameMusicmanage.h"
#include "SGBattleManager.h"
#include "SGHeroLayer.h"
#include "ResourceManager.h"
#include "CCScale9Sprite.h"

#define ANIMATERATE 0.8
SGBattleChangeRoundLayer::SGBattleChangeRoundLayer():m_bIsmyRound(false)
,m_pBackgroundSprite(NULL)
,m_pUpBorderSprite (NULL)
,m_pDownBorderSprite(NULL)
,m_pIsmyRoundSprite (NULL)
{
}

SGBattleChangeRoundLayer::~SGBattleChangeRoundLayer()
{
    CCLOG("~SGBattleChangeRoundLayer()");
}

SGBattleChangeRoundLayer* SGBattleChangeRoundLayer::create(bool ismyRound)
{
    SGBattleChangeRoundLayer *layer = new SGBattleChangeRoundLayer();
    layer->m_bIsmyRound = ismyRound;
    if(layer && layer->init())
    {
        layer->autorelease();
        return layer;
    }
    
    CC_SAFE_DELETE(layer);
    return layer;
}


//五回合切换初始化
bool SGBattleChangeRoundLayer::init()
{
    if(!CCLayer::init())
    {
        return false;
    }
    
    m_pUpBorderSprite = CCSprite::createWithSpriteFrameName("battle_change_bar1.png");
    m_pDownBorderSprite = CCSprite::createWithSpriteFrameName("battle_change_bar2.png");
    m_pBackgroundSprite = CCSprite::createWithSpriteFrameName("battle_change_bar_small_black.png");
    
    //谁的回合
    if(m_bIsmyRound)
    {
//        EFFECT_PLAY(MUSIC_22);
        m_pIsmyRoundSprite = CCSprite::createWithSpriteFrameName("battle_change_me.png");
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(true);
        if (hero) {
            int roundNum = hero->getCntrolRoundNum();
            if (roundNum > 0) {
                CCString *round = CCString::createWithFormat("%d",roundNum);
                CCLabelTTF *label = CCLabelTTF::create(round->getCString(), FONT_XINGKAI, FONTSIZE(18));
                switch (roundNum) {
                    case 5:
                    {
                        label->setColor(ccc3(100, 200, 39));
                    }
                        break;
                    case 4:
                    {
                        label->setColor(ccc3(100, 200, 39));
                    }
                        break;
                    case 3:
                    {
                        label->setColor(ccc3(200, 250, 199));
                    }
                        break;
                    case 2:
                    {
                        label->setColor(ccc3(200, 100, 100));
                    }
                        break;
                    case 1:
                    {
                        label->setColor(ccc3(150, 200, 0));
                    }
                        break;
                    default:
                        break;
                }
                
                CCSprite *tip = CCSprite::createWithSpriteFrameName("roundNumTip.png");
                tip->addChild(label);
                //            tip->setScaleY(2.0f);
                label->setPosition(ccp( -label->getContentSize().width, tip->getContentSize().height * 0.5));
                
                CCScale9Sprite *tipbj = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
                tipbj->setContentSize(CCSizeMake( screenwidth * 0.7f, screenheight * 0.1f));
                CCSprite *s = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
                s->setScaleX(tipbj->getContentSize().width / s->getContentSize().width);
                s->setScaleY(tipbj->getContentSize().height / s->getContentSize().height);
                s->setPosition(ccp(tipbj->getContentSize().width * 0.5f, tipbj->getContentSize().height * 0.5));
                tipbj->addChild(s, -1);
                addChild(tipbj, 100);
                tip->setPosition(ccp(tipbj->getContentSize().width * 0.5 + label->getContentSize().width * 0.5, tipbj->getContentSize().height * 0.5));
                tipbj->addChild(tip);
                tipbj->setPosition(ccp(screenwidth + tipbj->getContentSize().width * 0.5f, screenheight * 0.7f));
                CCMoveTo *move = CCMoveTo::create(0.5f, ccp(screenwidth * 0.5f, screenheight * 0.7f));
                CCDelayTime *delayt = CCDelayTime::create(2.5f);
                CCMoveTo *move1 = CCMoveTo::create(0.5f, ccp(-tipbj->getContentSize().width * 0.5f, screenheight * 0.7f));
                CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGBattleChangeRoundLayer::removeRoundEff));
                tipbj->runAction(CCSequence::create(move, delayt, move1, delayt,call, NULL));
//                int num = roundNum - 1;
//                hero->upDAtaControlRound(num);
				
				////by bugcode, 重置回合数,因为服务器会再次发送 2014-04-24
				hero->upDAtaControlRound(-1);
                
            }
            else
            {
                if (roundNum == 0 && SGBattleManager::sharedBattle()->getIsBattlePve()) {
                    SGBattleManager::sharedBattle()->battleOver();
                }
//                CCLabelTTF *la = CCLabelTTF::create("双方武将都用完，5回合后，我方血量值高于对方，我方胜利！", FONT_XINGKAI, FONTSIZE(18));
//                CCScale9Sprite *tipbj = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
//                tipbj->setContentSize(CCSizeMake( screenwidth * 0.5f, screenheight * 0.3f));
//                CCSprite *s = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
//                s->setScaleX(tipbj->getContentSize().width / s->getContentSize().width);
//                s->setScaleY(tipbj->getContentSize().height / s->getContentSize().height);
//                s->setPosition(ccp(tipbj->getContentSize().width * 0.5f, tipbj->getContentSize().height * 0.5));
//                tipbj->addChild(s, -1);
//                addChild(tipbj, 100);
//                
//                tipbj->setPosition(ccp(screenwidth + tipbj->getContentSize().width * 0.5f, screenheight * 0.3f));

            }
            
            CCScale9Sprite *tipbj = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
            tipbj->setContentSize(CCSizeMake( screenwidth * 0.5f, screenheight * 0.3f));
            CCSprite *s = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
            s->setScaleX(tipbj->getContentSize().width / s->getContentSize().width);
            s->setScaleY(tipbj->getContentSize().height / s->getContentSize().height);
            s->setPosition(ccp(tipbj->getContentSize().width * 0.5f, tipbj->getContentSize().height * 0.5));
            tipbj->addChild(s, -1);
            addChild(tipbj, 100);
            
            tipbj->setPosition(ccp(screenwidth + tipbj->getContentSize().width * 0.5f, screenheight * 0.3f));
        }
    }else{
        m_pIsmyRoundSprite = CCSprite::createWithSpriteFrameName("battle_change_other.png");
    }
    //放大1.5倍
    m_pIsmyRoundSprite->setScale(1.5f);
    
    addChild(m_pUpBorderSprite);
    addChild(m_pDownBorderSprite);
    addChild(m_pBackgroundSprite);
    addChild(m_pIsmyRoundSprite);
    m_pUpBorderSprite->setAnchorPoint(ccp(0, 0.5));
    m_pDownBorderSprite->setAnchorPoint(ccp(1, 0.5));
    
    //谁的回合，放在中间
    m_pBackgroundSprite->setPosition(SGLayout::getPoint(kMiddleCenter));
    m_pIsmyRoundSprite->setPosition(SGLayout::getPoint(kMiddleCenter));
    m_pBackgroundSprite->setVisible(false);
    m_pIsmyRoundSprite->setVisible(false);
    
    //把两个边的位置放在刚好看不见的位置
    m_pUpBorderSprite->setPosition(ccp(SGLayout::getSkewing(320), SGLayout::getPoint(kMiddleCenter).y+45));
    m_pDownBorderSprite->setPosition(ccp(0, SGLayout::getPoint(kMiddleCenter).y-45));
    
    //小个边角出来
    CCMoveTo *move_1 = CCMoveTo::create(0.1, ccp(SGLayout::getSkewing(320)-ANIMATION_STEP_WIDTH, SGLayout::getPoint(kMiddleCenter).y+45));
    CCMoveTo *move_2 = CCMoveTo::create(0.1f, ccp(ANIMATION_STEP_WIDTH, SGLayout::getPoint(kMiddleCenter).y-45));
    m_pUpBorderSprite->runAction(move_1);
    m_pDownBorderSprite->runAction(move_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.1);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::animation_1));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);
    
    return true;
}

void SGBattleChangeRoundLayer::removeRoundEff(cocos2d::CCObject *obj)
{
    CCNode *node = (CCNode*)obj;
//    node->removeAllChildrenWithCleanup(true);
    node->removeFromParentAndCleanup(true);
}

void SGBattleChangeRoundLayer::animation_1()
{
    //换图片,换个大号的
    m_pUpBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar3.png")->displayFrame());
    m_pDownBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar4.png")->displayFrame());
    
    CCMoveTo *move_1 = CCMoveTo::create(0.1*ANIMATERATE, ccp(SGLayout::getSkewing(320)-ANIMATION_STEP_WIDTH*2, SGLayout::getPoint(kMiddleCenter).y+45));
    CCMoveTo *move_2 = CCMoveTo::create(0.1f*ANIMATERATE, ccp(ANIMATION_STEP_WIDTH*2, SGLayout::getPoint(kMiddleCenter).y-45));
    m_pUpBorderSprite->runAction(move_1);
    m_pDownBorderSprite->runAction(move_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.1*ANIMATERATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::animation_2));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);
}

void SGBattleChangeRoundLayer::animation_2()
{
    //换图片,换个更大号的
    m_pUpBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar5.png")->displayFrame());
    m_pDownBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar6.png")->displayFrame());

    CCMoveTo *move_1 = CCMoveTo::create(0.1*ANIMATERATE, ccp(SGLayout::getSkewing(320)-ANIMATION_STEP_WIDTH*3, SGLayout::getPoint(kMiddleCenter).y+45));
    CCMoveTo *move_2 = CCMoveTo::create(0.1f*ANIMATERATE, ccp(ANIMATION_STEP_WIDTH*3, SGLayout::getPoint(kMiddleCenter).y-45));
    m_pUpBorderSprite->runAction(move_1);
    m_pDownBorderSprite->runAction(move_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.1*ANIMATERATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::animation_3));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);
}

void SGBattleChangeRoundLayer::animation_3()
{
    //换图片,换个更大号的
    m_pUpBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar7.png")->displayFrame());
    m_pDownBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar8.png")->displayFrame());

    CCMoveTo *move_1 = CCMoveTo::create(0.1*ANIMATERATE, ccp(SGLayout::getSkewing(320)-ANIMATION_STEP_WIDTH*4, SGLayout::getPoint(kMiddleCenter).y+45));
    CCMoveTo *move_2 = CCMoveTo::create(0.1f*ANIMATERATE, ccp(ANIMATION_STEP_WIDTH*4, SGLayout::getPoint(kMiddleCenter).y-45));
    m_pUpBorderSprite->runAction(move_1);
    m_pDownBorderSprite->runAction(move_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.1f*ANIMATERATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::animation_4));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);

}

void SGBattleChangeRoundLayer::animation_4()
{
    //换图片,换个更大号的
    m_pUpBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar9.png")->displayFrame());
    m_pDownBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar10.png")->displayFrame());
    m_pBackgroundSprite->setVisible(true);
    m_pIsmyRoundSprite->setVisible(true);
    
    CCDelayTime *delay_1 = CCDelayTime::create(0.1*ANIMATERATE);
    CCCallFunc *call_1 = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::changeBG_1));
    CCDelayTime *delay_2 = CCDelayTime::create(0.4*ANIMATERATE);
    CCCallFunc *call_2 = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::changeBG_2));
    CCDelayTime *delay_3 = CCDelayTime::create(0.1*ANIMATERATE);
    CCCallFunc *call_3 = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::animation_5));
    CCFiniteTimeAction *sequence = CCSequence::create(delay_1,call_1,delay_2,call_2,delay_3,call_3,NULL);
    this->runAction(sequence);

}

void SGBattleChangeRoundLayer::animation_5()
{
    m_pIsmyRoundSprite->setVisible(false);
    m_pBackgroundSprite->setVisible(false);
    
    //换图片,换个更大号的
    m_pUpBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar7.png")->displayFrame());
    m_pDownBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar8.png")->displayFrame());
    
    CCMoveTo *move_1 = CCMoveTo::create(0.1*ANIMATERATE, ccp(SGLayout::getSkewing(320)-ANIMATION_STEP_WIDTH*5, SGLayout::getPoint(kMiddleCenter).y+45));
    CCMoveTo *move_2 = CCMoveTo::create(0.1f*ANIMATERATE, ccp(ANIMATION_STEP_WIDTH*5, SGLayout::getPoint(kMiddleCenter).y-45));
    m_pUpBorderSprite->runAction(move_1);
    m_pDownBorderSprite->runAction(move_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.1f*ANIMATERATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::animation_6));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);

}

void SGBattleChangeRoundLayer::animation_6()
{
    //换图片,换个更大号的
    m_pUpBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar5.png")->displayFrame());
    m_pDownBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar6.png")->displayFrame());
    
    CCMoveTo *move_1 = CCMoveTo::create(0.1*ANIMATERATE, ccp(SGLayout::getSkewing(320)-ANIMATION_STEP_WIDTH*6, SGLayout::getPoint(kMiddleCenter).y+45));
    CCMoveTo *move_2 = CCMoveTo::create(0.1f*ANIMATERATE, ccp(ANIMATION_STEP_WIDTH*6, SGLayout::getPoint(kMiddleCenter).y-45));
    m_pUpBorderSprite->runAction(move_1);
    m_pDownBorderSprite->runAction(move_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.1f*ANIMATERATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::animation_7));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);

}

void SGBattleChangeRoundLayer::animation_7()
{
    //换图片,换个更大号的
    m_pUpBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar3.png")->displayFrame());
    m_pDownBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar4.png")->displayFrame());
    
    CCMoveTo *move_1 = CCMoveTo::create(0.1*ANIMATERATE, ccp(SGLayout::getSkewing(320)-ANIMATION_STEP_WIDTH*7, SGLayout::getPoint(kMiddleCenter).y+45));
    CCMoveTo *move_2 = CCMoveTo::create(0.1f*ANIMATERATE, ccp(ANIMATION_STEP_WIDTH*7, SGLayout::getPoint(kMiddleCenter).y-45));
    m_pUpBorderSprite->runAction(move_1);
    m_pDownBorderSprite->runAction(move_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.1*ANIMATERATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::animation_8));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);
    
}

void SGBattleChangeRoundLayer::animation_8()
{
    //换图片,换个更大号的
    m_pUpBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar1.png")->displayFrame());
    m_pDownBorderSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar2.png")->displayFrame());
    
    CCMoveTo *move_1 = CCMoveTo::create(0.1*ANIMATERATE, ccp(SGLayout::getSkewing(320)-ANIMATION_STEP_WIDTH*8, SGLayout::getPoint(kMiddleCenter).y+45));
    CCMoveTo *move_2 = CCMoveTo::create(0.1f*ANIMATERATE, ccp(ANIMATION_STEP_WIDTH*8, SGLayout::getPoint(kMiddleCenter).y-45));
    m_pUpBorderSprite->runAction(move_1);
    m_pDownBorderSprite->runAction(move_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.1*ANIMATERATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleChangeRoundLayer::removeSelf));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);
    
}

void SGBattleChangeRoundLayer::removeSelf()
{
//    this->removeAllChildrenWithCleanup(true);
    this->removeFromParentAndCleanup(true);
}

void SGBattleChangeRoundLayer::changeBG_1()
{
    m_pBackgroundSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar_big_black.png")->displayFrame());
    m_pBackgroundSprite->setColor(ccBLACK);
}

void SGBattleChangeRoundLayer::changeBG_2()
{
    m_pBackgroundSprite->setDisplayFrame(CCSprite::createWithSpriteFrameName("battle_change_bar_small_black.png")->displayFrame());
    m_pBackgroundSprite->setColor(ccBLACK);
}





