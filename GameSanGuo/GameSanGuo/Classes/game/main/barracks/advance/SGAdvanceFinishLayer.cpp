//
//  SGAdvanceFinishLayer.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-5-17.
//
//
#include "CCSpriterX.h"
#include "SGMainManager.h"
#include "SGAdvanceFinishLayer.h"
#include "SGMainLayer.h"
#include "GameMusicmanage.h"
#include "SGOfficerDataModel.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"

SGAdvanceFinishLayer::SGAdvanceFinishLayer()
:str_kuang(NULL)
,_card(NULL)
,cantouch(false),
time(0.0),
cardType(0),
isfromfirst(false)
{
	ResourceManager::sharedInstance()->bindTexture("animationFile/tongyongshandan.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("animationFile/advancetexiao2.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("animationFile/advancetexiao1.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("animationFile/publiclongyan.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("animationFile/advancemaoyan.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);	
}
SGAdvanceFinishLayer::~SGAdvanceFinishLayer()
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_advanceLayer);
    str_kuang->release();
    MUSIC_PLAY(MUSIC_SCENE);
}
SGAdvanceFinishLayer * SGAdvanceFinishLayer::create(int itemid,SGBaseMilitaryCard *card_,int type,bool isfirst)
{
    SGAdvanceFinishLayer *advanceLayer = new SGAdvanceFinishLayer();
    if (advanceLayer->init(NULL, sg_advancetexiao))
    {
        advanceLayer->isfromfirst = isfirst;
        advanceLayer->cardType = type;
        advanceLayer->_card = card_;
        advanceLayer->initView(itemid);
        MUSIC_STOP;
        advanceLayer->autorelease();
        return advanceLayer;
    }
    CC_SAFE_DELETE(advanceLayer);
    return NULL;
}
void SGAdvanceFinishLayer::initView(int itemid)
{

	ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    
    this->setIsCanTouch(false);
    str_kuang = CCString::create("");
    str_kuang->retain();
    CCString *str_ = CCString::create(_card->getClassName());
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
        int starlvl = temp->getOfficerCurrStarLevel();
        if (starlvl>1) {
            starlvl-=1;
        }
        str_kuang = CCString::createWithFormat("officerPinzhi_%d_0.png",starlvl);

//        str_kuang = CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType());
//        str_kuang->setString(CCString::createWithFormat("officerPinzhi_%d.png",temp->getOfficerVisitType())->getCString());
    }
    else if (str_->isEqual(CCString::create("SGEquipCard")))
    {
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        str_kuang = CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi());
    }

    s = CCDirector::sharedDirector()->getWinSize();
    pos = ccp(s.width/2 +15,375);
    CCSprite *bg = CCSprite::create("visit/visit_cardbg1.png");
    bg->setScaleX(s.width/768);
    bg->setScaleY(s.height/1136);
    bg->setPosition(SGLayout::getPoint(kMiddleCenter));
    this->addChild(bg,-1);
    time = 1;
    
	
	//12.06//通过itemid索引对应的headIcon
	SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_advanceLayer);//12.06
	SGButton *advance_manzubg = SGButton::create(CCString::createWithFormat("head%d.png",_card->getHeadSuffixNum())->getCString(),str_kuang->getCString(), this, NULL,CCPointZero,false,true);//12.05

    this->addChild(advance_manzubg, 111, 111);
    advance_manzubg->setPosition(pos);
    CCOrbitCamera *actionFlip1 = CCOrbitCamera::create(1, -1, 0, 0, 360, 0, 0);
    
    if (str_->isEqual(CCString::create("SGOfficerCard")))
    {
        advance_manzubg->removeChildByTag(30, true);
        CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",_card->getRace()))->getCString());
        countryName->setAnchorPoint(ccp(1,1));
        countryName->ignoreAnchorPointForPosition(false);
        countryName->setPosition(ccp(advance_manzubg->getContentSize().width*0.3,advance_manzubg->getContentSize().height - countryName->getContentSize().height/4));
        advance_manzubg->addChild(countryName,30,30);
    }
    
    
    CCSpeed *speed = CCSpeed::create(CCRepeatForever::create((CCActionInterval *)actionFlip1), 1);
    speed->setTag(19);
    advance_manzubg->runAction(speed);
    schedule(schedule_selector(SGAdvanceFinishLayer::setActionTime), 0.5);
    //常态雷
    CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/jj_001.scml", true, true);
    fermEffect2->setanimaID(0);
    fermEffect2->setisloop(true);
    fermEffect2->setPosition(ccp(s.width/2 - 1, s.height - 325));
    addChild(fermEffect2, -10, 0);
    fermEffect2->play();
    
//火苗
    float wid = (float)s.width/768;
    float hgt = (float)s.height/1136;
    CCSpriterX *fermEffect11 = CCSpriterX::create("animationFile/lyhm.scml", true, true);
    fermEffect11->setanimaID(0);
    fermEffect11->setisloop(true);
    fermEffect11->setPosition(ccp(s.width/2 - wid*265, s.height - 720*hgt));
    addChild(fermEffect11, 1, 11);
    fermEffect11->play();
    CCSpriterX *fermEffect12 = CCSpriterX::create("animationFile/lyhm.scml", true, true);
    fermEffect12->setanimaID(0);
    fermEffect12->setisloop(true);
    fermEffect12->setPosition(ccp(s.width/2 - wid*200, s.height - 700*hgt));
    addChild(fermEffect12, 1, 12);
    fermEffect12->play();
    CCSpriterX *fermEffect13 = CCSpriterX::create("animationFile/lyhm.scml", true, true);
    fermEffect13->setanimaID(0);
    fermEffect13->setisloop(true);
    fermEffect13->setPosition(ccp(s.width/2 + wid*200, s.height - 620*hgt));
    addChild(fermEffect13, 1, 13);
    fermEffect13->play();
    CCSpriterX *fermEffect14 = CCSpriterX::create("animationFile/lyhm.scml", true, true);
    fermEffect14->setanimaID(0);
    fermEffect14->setisloop(true);
    fermEffect14->setPosition(ccp(s.width/2 + wid*265, s.height - 645*hgt));
    addChild(fermEffect14, 1, 14);
    fermEffect14->play();
    
}
void SGAdvanceFinishLayer::setActionTime()
{
    time += 0.5;
    
    SGButton *sp = (SGButton*)this->getChildByTag(111);
    CCSpeed *speed = (CCSpeed*)sp->getActionByTag(19);
    
    if (time <= 4) {
        speed->setSpeed(time);
    }
    if (time >= 5) {
        unschedule(schedule_selector(SGAdvanceFinishLayer::setActionTime));
        sp->stopAllActions();
//      卡变球
        CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/jj.scml", true, true);
        fermEffect2->setanimaID(0);
        fermEffect2->setisloop(false);
        fermEffect2->setPosition(sp->getPosition());
        addChild(fermEffect2, 10, 1);
        fermEffect2->play();
        this->removeChildByTag(111, true);
        runAction(CCSequence::create(CCDelayTime::create(0.95),CCCallFuncN::create(this, callfuncN_selector(SGAdvanceFinishLayer::callback)),NULL));
    }
}

void SGAdvanceFinishLayer::callback(cocos2d::CCNode *node)
{
    //被雷前的光球
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/jj_000.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    fermEffect->setPosition(pos);
    addChild(fermEffect, 11, 2);
    fermEffect->play();
    
    //前端雷
    CCSpriterX *fermEffect3 = CCSpriterX::create("animationFile/jj_002.scml", true, true);
    fermEffect3->setanimaID(0);
    fermEffect3->setisloop(false);
    fermEffect3->setPosition(ccp(s.width/2, s.height - 325));
    addChild(fermEffect3, -3, 3);
    fermEffect3->play();
    
    //霹雷
    CCSpriterX *fermEffect4 = CCSpriterX::create("animationFile/jj_003.scml", true, true);
    fermEffect4->setanimaID(0);
    fermEffect4->setisloop(false);
    fermEffect4->setPosition(ccp(s.width/2, 650));
    addChild(fermEffect4, 10, 4);
    fermEffect4->play();
    //1.452
    runAction(CCSequence::create(CCDelayTime::create(1.6),CCCallFuncN::create(this, callfuncN_selector(SGAdvanceFinishLayer::callBreakBall)),CCDelayTime::create(0.1),CCCallFuncN::create(this, callfuncN_selector(SGAdvanceFinishLayer::removebreakball)),CCDelayTime::create(0.037),CCCallFuncN::create(this, callfuncN_selector(SGAdvanceFinishLayer::callleihou)),NULL));
}
void SGAdvanceFinishLayer::removebreakball()
{
    this->removeChildByTag(99, true);
}
void SGAdvanceFinishLayer::callBreakBall()
{
    this->removeChildByTag(2, true);
    CCSprite *ball = CCSprite::createWithSpriteFrameName("avance_break.png");
    this->addChild(ball,1,99);
    ball->setPosition(pos);
}
void SGAdvanceFinishLayer::callleihou()
{
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/jj_004.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    fermEffect->setPosition(ccpAdd(pos, ccp(0, -180)));
    addChild(fermEffect, 5, 5);
    fermEffect->play();
    fermEffect->runAction(CCSequence::create(CCMoveBy::create(0.3, ccp(0, 30)),CCMoveBy::create(0.3, ccp(0, 110)),CCMoveBy::create(0.2, ccp(0, 40)),CCCallFuncN::create(this, callfuncN_selector(SGAdvanceFinishLayer::ballbomb)),NULL));
}
void SGAdvanceFinishLayer::ballbomb(CCNode *node)
{
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/jj_005.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(false);
    fermEffect->setPosition(pos);
    this->removeChild(node, true);
    addChild(fermEffect, 5, 6);
    fermEffect->play();
    runAction(CCSequence::create(CCDelayTime::create(2.143),CCCallFuncN::create(this, callfuncN_selector(SGAdvanceFinishLayer::shownewcard)),NULL));
}
void SGAdvanceFinishLayer::shownewcard()
{
	SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_advanceLayer);//12.05
	 SGButton *a = SGButton::create(CCString::createWithFormat("head%d.png",_card->getHeadSuffixNum())->getCString(), str_kuang->getCString(), this, menu_selector(SGAdvanceFinishLayer::backHandler),CCPointZero,false,true);//12.05
    this->addBtn(a);
    a->setPosition(pos);
    

    CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/mq.scml", true, true);
    fermEffect2->setanimaID(0);
    fermEffect2->setisloop(true);
    fermEffect2->setPosition(ccp(s.width/2 - 1, s.height - 325));
    a->addChild(fermEffect2);
    fermEffect2->setPosition(ccp(a->getContentSize().width/2,a->getContentSize().height/2));
    fermEffect2->play();
    
    a->runAction(CCRepeatForever::create((CCActionInterval*)CCSequence::create(CCMoveBy::create(0.5, ccp(0,20)),CCMoveBy::create(0.5, ccp(0,-20)),CCMoveBy::create(0.5, ccp(0,-20)),CCMoveBy::create(0.5, ccp(0,20)),NULL)));
    cantouch = true;
    this->setIsCanTouch(true);
}
void SGAdvanceFinishLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true);
    if (isfromfirst) {
        if (cardType == 1) {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard*)_card, 2);
        }else
        {
            SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)_card, 5);
        }
        
    }else
    {
        if (cardType == 1) {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard*)_card, 0);
        }else
        {
            SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)_card, 0);
        }
    }
}
void SGAdvanceFinishLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool SGAdvanceFinishLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    if (cantouch) {
        backHandler();
        return true;
    }
    return false;

}

void SGAdvanceFinishLayer::onEnter()
{
    CCLayer::onEnter();
    registerWithTouchDispatcher();
}

void SGAdvanceFinishLayer::onExit()
{
    CCLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

