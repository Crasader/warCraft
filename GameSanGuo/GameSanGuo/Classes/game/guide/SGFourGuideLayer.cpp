//
//  SGFourGuideLayer.cpp
//  GameSanGuo
//
//  Created by lizi on 13-6-26.
//
//

#include "SGFourGuideLayer.h"
#include "SGBattleManager.h"
#include "SGPlayerInfo.h"
#include "SGGuideManager.h"
#include "PlotModule.pb.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "SGRichLabel.h"
#include "SGAIManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"

SGFourGuideLayer::SGFourGuideLayer(void) : spLabel(NULL), battleLayer(NULL)
{
    label = NULL;
}

SGFourGuideLayer::~SGFourGuideLayer(void)
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_fourGuideLayer);
}

SGFourGuideLayer* SGFourGuideLayer::create()
{
    SGFourGuideLayer *model = new SGFourGuideLayer();
    if (model && model->init(NULL, sg_fourGuideLayer)) {
        model->autorelease();
        model->initView();
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}

void SGFourGuideLayer::setFguide()
{
    CCString *str1 = CCString::create(str_this_is_first_get_four_);
    CCString *str2 = CCString::create(str_Four_guide_tips);
    CCString *str3 = CCString::create(str_Four_guide_tips_now_explain);
    _datas = CCArray::create();
    _datas->retain();
    _datas->addObject(str1);
    _datas->addObject(str2);
    _datas->addObject(str3);
    
    // init label
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
    CCSize dsize = CCSizeMake(tsize.width-skewing(20), tsize.height-skewing(25));
    spLabel = SGRichLabel::getFomLabel(str1->getCString(), dsize, FONT_PENGYOU, FONTSIZE(14));
    spLabel->setPosition(bboard->getPosition());
    this->addChild(spLabel, 2);
    label->setVisible(false);
}

void SGFourGuideLayer::showPKwords(CCString *word, SGBattleLayer *layer)
{
    // init label
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
    CCSize dsize = CCSizeMake(tsize.width-skewing(20), tsize.height-skewing(25));
    spLabel = SGRichLabel::getFomLabel(word->getCString(), dsize, FONT_PENGYOU, FONTSIZE(14));
    spLabel->setPosition(bboard->getPosition());
    this->addChild(spLabel, 2);
    
    battleLayer = layer;
}

void SGFourGuideLayer::showJoinBtn()
{
    //MMDEBUG: FIXME 有可能到btn_guide_join.png时已经被释放了，非常不科学，暂时即用即添。
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_uiguideLayer);
    SGButton *joinBtn = SGButton::create("public_btn_2.png", "btn_guide_join.png",
                                         this, menu_selector(SGFourGuideLayer::buttonAction), ccp(0, 0), false, true);
    float posX = joinBtn->getContentSize().width*0.75f;
    float posY = bboard->getPositionY() - bboard->getContentSize().height/2 + joinBtn->getContentSize().height*0.75f;
    
    joinBtn->setTag(1);
    joinBtn->setScale(1.15f);
    joinBtn->setPosition(ccp(bboard->getPositionX()-posX, posY));
    this->addBtn(joinBtn);
    

    SGButton *rejectBtn = SGButton::create("public_kuang_hui.png", "btn_guide_nojoin.png",
                                           this, menu_selector(SGFourGuideLayer::buttonAction), ccp(0, 0), false, true);
    rejectBtn->setTag(2);
    rejectBtn->setScale(1.15f);
    rejectBtn->setPosition(ccp(bboard->getPositionX()+posX, posY));
    this->addBtn(rejectBtn);
}

void SGFourGuideLayer::buttonAction(CCObject *sender)
{
    EFFECT_PLAY(MUSIC_BTN);
    CCMenuItem *menu = (CCMenuItem *)sender;
    SGPlayerInfo::sharePlayerInfo()->setIsFirstFour(1);
    switch (menu->getTag()) {
        case 1: { // join
            SGPlayerInfo::sharePlayerInfo()->setPlotId(SUPERGUIDPLOTID);
            SGBattleManager::sharedBattle()->pveStart(5, SUPERGUIDPLOTID, 0);
            this->setVisible(false);
            this->removeFromParentAndCleanup(true);
        }break;
        case 2: { // not join
            main::NewHandProssRequest *request = new main::NewHandProssRequest();
            SGSocketClient::sharedSocketClient()->send(MSG_GUIDE_FOUR, request, false);
            SGPlayerInfo::sharePlayerInfo()->setIsFirstFour(2);
            
            this->setVisible(false);
            this->removeFromParentAndCleanup(true);
            SGGuideManager::shareManager()->setMainTouchEnable(true);
        }break;
        default:
            break;
    }
}

void SGFourGuideLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (_datas != NULL) { // 四格武将
        _wordIndex += 1;
        int count = _datas->count();
        
        if (_wordIndex < count) {
            CCString *str = (CCString *)_datas->objectAtIndex(_wordIndex);
            // init label
            spLabel->removeFromParentAndCleanup(true);
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
            CCSize dsize = CCSizeMake(tsize.width-skewing(20), tsize.height-skewing(25));
            spLabel = SGRichLabel::getFomLabel(str->getCString(), dsize, FONT_PENGYOU, FONTSIZE(14));
            spLabel->setPosition(bboard->getPosition());
            this->addChild(spLabel, 2);
            
            if (_wordIndex==_datas->count()-1) {
                this->showJoinBtn();
            }
        }
    }else { // 战场PK
        battleLayer->setIsCanTouch(true);
    }
}



SGFightDescLayer::SGFightDescLayer(void)
{
    label = NULL;
    bboard = NULL;
    battleLayer = NULL;
}

SGFightDescLayer::~SGFightDescLayer(void)
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_fightDescLayer);
}

SGFightDescLayer* SGFightDescLayer::create()
{
    SGFightDescLayer *model = new SGFightDescLayer();
    if (model && model->init(NULL, sg_fightDescLayer)) {
        model->autorelease();
        model->initView();
        return model;
    }
    CC_SAFE_DELETE(model);
    return NULL;
}

void SGFightDescLayer::initView()
{
    

//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("upwar/upwar.plist");
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("story/storyGuide.plist");

	ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_fightDescLayer);

    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
//    bboard = CCSprite::create("public/public_guide_black.png");
//    bboard = CCSprite::create("story/guide_board_new.png");
    bboard = CCSprite::createWithSpriteFrameName("guide_board_new.png");

    bboard->setScaleX(size.width/bboard->getContentSize().width);
    this->addChild(bboard);
    bboard->setPosition(SGLayout::getPoint(kMiddleCenter));
    
//    CCSprite *actSprite = CCSprite::create("story/uplevel_click.png");
    CCSprite *actSprite = CCSprite::createWithSpriteFrameName("uplevel_click.png");

    float posXX = bboard->getContentSize().width-actSprite->getContentSize().width*.65f;
    float posYY = actSprite->getContentSize().height*.70f;
    actSprite->setPosition(ccp(posXX, posYY));
    bboard->addChild(actSprite);
    //actSprite->setScale(bboard->getContentSize().width/size.width);
    
    //    CCAction *action = CCSequence::create(CCMoveBy::create(0.12f, ccp(0, 2)),
    //                                          CCMoveBy::create(0.12f, ccp(0, 2)),
    //                                          CCMoveBy::create(0.12f, ccp(0, -2)),
    //                                          CCMoveBy::create(0.12f, ccp(0, -2)),
    //                                          NULL);
    //    actSprite->runAction(CCRepeatForever::create((CCActionInterval *)action));
    
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animationFile/djjx_new.plist");
	ResourceManager::sharedInstance()->bindTexture("animationFile/djjx_new.plist", RES_TYPE_LAYER_UI, sg_fightDescLayer);
   
    CCSpriterX *resultx = CCSpriterX::create("animationFile/djjx.scml", true, true);
    resultx->setAnchorPoint(ccp(0.5, 0.5));
    posXX = posXX-actSprite->getContentSize().width*.75f;
    posYY = posYY - actSprite->getContentSize().height*.45f;
    resultx->setPosition(ccp(posXX, posYY));
    resultx->setanimaID(0);
    resultx->setisloop(true);
    bboard->addChild(resultx, 10);
    resultx->play();
    
    CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
    CCSize dsize = CCSizeMake(tsize.width-skewing(20), tsize.height-skewing(15));
    label = CCLabelTTF::create("", FONT_PENGYOU, FONTSIZE(14));
    label->setHorizontalAlignment(kCCTextAlignmentLeft);
    label->setDimensions(dsize);
    label->setPosition(bboard->getPosition());
    this->addChild(label, 2);
}

void SGFightDescLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, false);
}

void SGFightDescLayer::showPKwords(CCString *word, SGBattleLayer *layer)
{
    // init label
    battleLayer = layer;
    label->setString(word->getCString());
}

bool SGFightDescLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void SGFightDescLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    bool ishero = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (!ishero) {
//        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(ishero);
//        SGAIManager::shareManager()->isAi = true;
//        SGAIManager::shareManager()->startRound(hero->battleMap);//????????
    }else { // 我方回合
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(ishero);
        hero->setIsCanTouch(true);
        SGAIManager::shareManager()->isAi = true;
    }
    
    this->setVisible(false);
    this->setTouchEnabled(false);
    this->removeFromParentAndCleanup(true);
    
    SGGuideManager::shareManager()->isFightDesc = false;
}