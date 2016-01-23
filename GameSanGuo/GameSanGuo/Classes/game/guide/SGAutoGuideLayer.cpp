//
//  SGAutoGuideLayer.cpp
//  GameSanGuo
//
//  Created by pengyou on 13-9-18.
//
//

#include "SGAutoGuideLayer.h"
#include "SGBattleManager.h"
#include "SGStaticDataManager.h"
#include "SGGuideManager.h"
#include "SGMainManager.h"
#include "ResourceManager.h"

#define LVBUTAG     -55
#define GUOSITAG    -66
#define LIJUETAG    -77
#define FIRSTMSGTAG     -20
#define FINALMSGTAG     -10
#define FINALMSGMOVEDIS     2
#define FINALMSGMOVERATE    15
#define FINALMSGMOVEGAP     0.04
SGAutoGuideLayer::~SGAutoGuideLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_autoguideLayer);
	//SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_GUIDE_STEP);
	
    CC_SAFE_RELEASE(msgArrs);
}
SGAutoGuideLayer *SGAutoGuideLayer::create(SGBattleLayer *battleLayer)
{
    SGAutoGuideLayer *layer = new SGAutoGuideLayer();
    if (layer->init(battleLayer))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}
bool SGAutoGuideLayer::init(SGBattleLayer *bl)
{
    if (!CCLayer::init())
    {
        return false;
    }
	//SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_GUIDE_STEP, this, callfuncO_selector(SGAutoGuideLayer::recordStepListener));
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_autoguideLayer);
	ResourceManager::sharedInstance()->bindTexture("autoRole/autoRole.plist", RES_TYPE_LAYER_UI, sg_autoguideLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_autoguideLayer);
	
    msgArrs = NULL;
    finaleUpSprite = NULL;
    
    size = CCDirector::sharedDirector()->getWinSize();
    battleLayer = bl;
    battleLayer->setIsCanTouch(false);
    step = 1;

    bboard = CCSprite::createWithSpriteFrameName("guide_board_new.png");

    bboard->setScaleX(size.width/bboard->getContentSize().width);
    this->addChild(bboard);
    actSprite = CCSprite::createWithSpriteFrameName("uplevel_click.png");

    float posXX = bboard->getContentSize().width-actSprite->getContentSize().width*.65f;
    float posYY = actSprite->getContentSize().height*.70f;
    actSprite->setPosition(ccp(posXX, posYY));
    //actSprite->setScale(bboard->getContentSize().width/size.width);
    bboard->addChild(actSprite);
    bboard->setVisible(false);
    

    CCSprite * lvbuSprite = CCSprite::createWithSpriteFrameName("lvbu.png");
    lvbuSprite->setPosition(ccp(0,bboard->getContentSize().height));
    lvbuSprite->setAnchorPoint(CCPointZero);
    lvbuSprite->ignoreAnchorPointForPosition(false);
    lvbuSprite->setScaleX(bboard->getContentSize().width/size.width);
    lvbuSprite->setVisible(false);
    bboard->addChild(lvbuSprite,-1,LVBUTAG);
    
    CCSprite * guoliSprite = CCSprite::createWithSpriteFrameName("guoSi.png");
    guoliSprite->setPosition(ccp(bboard->getContentSize().width - guoliSprite->getContentSize().width -50,bboard->getContentSize().height));
    guoliSprite->setAnchorPoint(CCPointZero);
    guoliSprite->ignoreAnchorPointForPosition(false);
    guoliSprite->setScaleX(bboard->getContentSize().width/size.width);
    guoliSprite->setVisible(false);
    bboard->addChild(guoliSprite,-1,GUOSITAG);
    
    guoliSprite = CCSprite::createWithSpriteFrameName("liJue.png");
    guoliSprite->setPosition(ccp(bboard->getContentSize().width - guoliSprite->getContentSize().width -50,bboard->getContentSize().height));
    guoliSprite->setAnchorPoint(CCPointZero);
    guoliSprite->ignoreAnchorPointForPosition(false);
    guoliSprite->setScaleX(bboard->getContentSize().width/size.width);
    guoliSprite->setVisible(true);
    bboard->addChild(guoliSprite,-1,LIJUETAG);
    
	ResourceManager::sharedInstance()->bindTexture("animationFile/djjx_new.plist", RES_TYPE_LAYER_UI, sg_autoguideLayer);
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
    CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-40);
    label = SGRichLabel::getFomLabel("", dsize, FONT_PENGYOU, 32);
    label->setPosition(this->getPosition());
    this->addChild(label, 2);
    
    showNum = 0;

	ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_LAYER_UI, sg_autoguideLayer);
    spTouch = CCSpriterX::create("animationFile/jiantou.scml", true, true);
    spTouch->setAnchorPoint(ccp(0.5f, 0.5f));
    spTouch->setanimaID(0);
    spTouch->setisloop(true);
    spTouch->play();
    
    spTouch->setVisible(false);
    this->addChild(spTouch);
    
    spDirect = CCSprite::createWithSpriteFrameName("public_jiantou.png");

    spDirect->setVisible(false);
    this->addChild(spDirect);
    
    return true;
}
void SGAutoGuideLayer::showTouchTip(int fromId)
{
    // btnType 1:援军 2:行动力 3:结束回合 4:主将计 5:发动技能
    CCPoint position = ccp(0, 0);
    if (fromId == 1 || fromId == 2 || fromId == 4)
    {
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        CCArray *array = selfLayer->getMenu()->getChildren();
        selfLayer->setIsCanTouch(true);
        
        for (int ii = 0; ii < array->count(); ii++)
        {
            SGButton *button = (SGButton *)array->objectAtIndex(ii);
            button->setEnabled(false);
        }
        
        if (fromId == 1)
        { //1-1 援军按钮
            selfLayer->yb_btn->setEnabled(true);
            position = selfLayer->yb_btn->getPosition();
        }
        else if (fromId == 2)
        { //1-1 行动力按钮
            selfLayer->surrender_btn->setEnabled(true);
            position = selfLayer->surrender_btn->getPosition();
        }
        else if (fromId == 4)
        { //1-1 主将计按钮
            selfLayer->zjj_btn->setEnabled(true);
            position = selfLayer->zjj_btn->getPosition();
        }
    }
    else if (fromId == 3 || fromId == 5)
    { //1-1 结束回合按钮，行动力弹框
        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
        SGBaseBox *baseBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
        CCArray *array = baseBox->getMenu()->getChildren();
        for (int ii = 0; ii < array->count(); ii++)
        {
            SGButton *button = (SGButton *)array->objectAtIndex(ii);
            button->setEnabled(false);
        }
        SGButton *button = (SGButton *)baseBox->getBtnByTag(101);
        button->setEnabled(true);
        position = button->getPosition();
    }
    
    if (!(position.x==0&&position.y==0))
    {
        bboard->setVisible(false);
        label->setVisible(false);
        this->setVisible(true);
        spDirect->setVisible(false);
        float disH = -spDirect->getContentSize().height*.35f + 25;
        spTouch->setPosition(ccpAdd(position, ccp(0, disH)));
        
        spTouch->setVisible(true);
        
    }
}
void SGAutoGuideLayer::showDirectTip(int fromId)
{
    // tipType 1:对方血条 2:行动力 3:援军 4:主将计 5:回合数 6:武将
    CCPoint position = ccp(0, 0);
    tipDirect dirType = tip_down;
    float disc = spDirect->getContentSize().height*.5f+skewing(15);
    
    if (fromId == 1)
    {
        dirType = tip_up;
        disc = -(spDirect->getContentSize().height*.5f+skewing(15));
        spDirect->setRotation(-90.0f);
        SGHeroLayer *selfLayer = battleLayer->getOpponent(true);
        position = selfLayer->hpProgress->getPosition();
        disc -= selfLayer->hpProgress->getContentSize().height*.5f;
    }
    else if (fromId == 2)
    {
        spDirect->setRotation(90.0f);
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        position = selfLayer->surrender_btn->getPosition();
        disc += selfLayer->surrender_btn->getContentSize().height*.5f;
    }
    else if (fromId == 3)
    {
        spDirect->setRotation(90.0f);
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        position = selfLayer->yb_btn->getPosition();
        disc += selfLayer->yb_btn->getContentSize().height*.5f;
    }
    else if (fromId == 4)
    {
        spDirect->setRotation(90.0f);
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        position = selfLayer->zjj_btn->getPosition();
        disc += selfLayer->zjj_btn->getContentSize().height*.5f;
    }

    if (!(position.x==0&&position.y==0))
    {
        spTouch->setVisible(false);
        spDirect->setPosition(ccpAdd(position, ccp(0, disc)));
        spDirect->setVisible(true);
        
        CCAction *action = SGUIGuideLayer::getActionByDirection(dirType);
        spDirect->runAction(CCRepeatForever::create((CCActionInterval *)action));
    }
}
void SGAutoGuideLayer::closeTips()
{
    if (SGGuideManager::shareManager()->isGuide)
    {

        spTouch->setVisible(false);
        spDirect->stopAllActions();
        spDirect->setVisible(false);
        
        this->setVisible(false);
        this->setTouchEnabled(false);
        bboard->setVisible(true);
        label->setVisible(true);
        
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        selfLayer->setIsCanTouch(false);
        CCArray *array = selfLayer->getMenu()->getChildren();
        for (int ii = 0; ii < array->count(); ii++)
        {
            SGButton *button = (SGButton *)array->objectAtIndex(ii);
            button->setEnabled(true);
        }
    }
}
void SGAutoGuideLayer::initMsg(CCArray *arr)
{
    if (msgArrs)
    {
        msgArrs->release();
    }
    msgArrs = arr;
    msgArrs->retain();
    showNum = 0;
    
    this->nextMsg();
}
void SGAutoGuideLayer::setBattleLayer(SGBattleLayer *bl)
{
    
    battleLayer = bl;
    battleLayer->setIsCanTouch(false);
}
void SGAutoGuideLayer::initTips(std::vector<int> tips)
{
    msgTips.clear();
    for (int ii = 0; ii < tips.size(); ii++)
    {
        msgTips.push_back(tips[ii]);
    }
}
void SGAutoGuideLayer::nextMsg()
{
    if (showNum<msgArrs->count())
    {
        CCString *str = (CCString *)msgArrs->objectAtIndex(showNum);
        if (label != NULL)
        {
//            label->removeAllChildrenWithCleanup(true);
            label->removeFromParentAndCleanup(true);
            label = NULL;
        }
        if (str->compare("") != 0)
        {
            CCSize size = CCDirector::sharedDirector()->getWinSize();
            CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
            CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-50);
            label = SGRichLabel::getFomLabel(str->getCString(), dsize, FONT_PENGYOU, 32);
            label->setPosition(ccpAdd(bboard->getPosition(),ccp(0,-15)));
            this->addChild(label, 2);
            bboard->setVisible(true);
            bool newVisible = bboard->getChildByTag(GUOSITAG)->isVisible();
            if (newVisible)
            {
                bboard->getChildByTag(LIJUETAG)->setVisible(true);
                bboard->getChildByTag(GUOSITAG)->setVisible(false);
            }
        }
        else
        {
            bboard->setVisible(false);
        }
        showNum++;
        
        for (int ii = 0; ii < msgTips.size(); ii+=2)
        {
            if (showNum == msgTips[ii])
            {
                this->showDirectTip(msgTips[ii+1]);
            }
        }
    }
    else
    {
        showNum = 0;
		CCLOG("second guide [%s-%d]", __FUNCTION__, __LINE__);
        SGGuideManager::shareManager()->startAutoGuide(2);
    }
}

void SGAutoGuideLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}

bool SGAutoGuideLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    moveCgMsgDis = FINALMSGMOVEDIS *FINALMSGMOVERATE;
    return true;
}

void SGAutoGuideLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if ( this->getChildByTag(FINALMSGTAG) == NULL && this->getChildByTag(FIRSTMSGTAG) == NULL)
    {
        this->nextMsg();
    }
    else 
    {
        if ( posY < 0)
        {
            moveCgMsgDis = FINALMSGMOVEDIS;
        }
    }
}

SGBattleLayer *SGAutoGuideLayer::getBattleLayer()
{
    return battleLayer;
}
void SGAutoGuideLayer::setPlace(int place)
{
    int move = (place == 2)?-2:0.5;
    bboard->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, move*140)));
    

    if (place == 5)
    {
        bboard->getChildByTag(LVBUTAG)->setVisible(false);
        bboard->getChildByTag(GUOSITAG)->setVisible(false);
        bboard->getChildByTag(LIJUETAG)->setVisible(false);
    }
    else
    {
        if (bboard->getChildByTag(LVBUTAG) != NULL)
        {
            bboard->getChildByTag(LVBUTAG)->setVisible((place == 2)?true:false);
        }
        if (bboard->getChildByTag(GUOSITAG) != NULL)
        {
            bboard->getChildByTag(GUOSITAG)->setVisible((place == 2)?false:true);
        }
        if (bboard->getChildByTag(LIJUETAG) != NULL)
        {
            bboard->getChildByTag(LIJUETAG)->setVisible(false);
        }
    }

    if (label != NULL)
    {
        label->setPosition(ccpAdd(bboard->getPosition(),ccp(0,-15)));
    }
}


void SGAutoGuideLayer::initFinalMsg(CCArray *arr)
{
    if (msgArrs)
    {
        msgArrs->release();
    }
    msgArrs = arr;
    msgArrs->retain();
    showNum = 0;
    size = CCDirector::sharedDirector()->getWinSize();
    
    if (bboard != NULL)
    {
        bboard->setVisible(false);
    }
    
    if (label)
    {
        label->removeFromParentAndCleanup(true);
    }
    
    MUSIC_PLAY(MUSIC_SCENE);
    
    CCLayerColor * blackColor = CCLayerColor::create(ccc4(0, 0, 0, 255), size.width, size.height);
    blackColor->ignoreAnchorPointForPosition(false);
    blackColor->setAnchorPoint(ccp(0, 0));
    blackColor->setPosition(ccp(0, 0));
    blackColor->setOpacity(0);
    this->addChild(blackColor , -100);
    blackColor->runAction(CCFadeTo::create(1, 255));
    
    char *spritData = new char[(int)size.width * (int)size.height *4];
    memset(spritData,0,(int)size.width * (int)size.height *4);
    int spriteDataSize = (int)size.width * (int)size.height*4;
    for (int j=0; j<spriteDataSize; j+=4)
    {
        spritData[j] = 0;
        spritData[j+1] = 0;
        spritData[j+2] = 0;
        spritData[j+3] = 255;
    }
    
    CCTexture2D *pTexture = new CCTexture2D();
    pTexture->initWithData(spritData, kCCTexture2DPixelFormat_Default, (int)size.width, (int)size.height, size);
    
    CCSprite *finaleSprite = CCSprite::createWithTexture(pTexture);
    finaleSprite->runAction(CCSequence::create(CCFadeIn::create(1.5),NULL));
    finaleSprite->runAction(CCSequence::create(CCDelayTime::create(1.2),
                                               CCCallFunc::create(this,  callfunc_selector(SGAutoGuideLayer::showFinalMsg))
                                               ,NULL));
    finaleSprite->setOpacity(0);
    finaleSprite->setPosition(CCPointZero);
    finaleSprite->setAnchorPoint(CCPointZero);
    this->addChild(finaleSprite,0,FINALMSGTAG);
    
    
    finaleUpSprite = CCSprite::createWithTexture(pTexture);
    finaleUpSprite->setOpacity(0);
    finaleUpSprite->setPosition(CCPointZero);
    finaleUpSprite->setAnchorPoint(CCPointZero);
    finaleUpSprite->setVisible(false);
    this->addChild(finaleUpSprite,10,FINALMSGTAG+10);
    
    delete [] spritData;
    pTexture->autorelease();
    
    ResourceManager::sharedInstance()->bindTexture("bg/cgBg.plist", RES_TYPE_LAYER_UI, sg_autoguideLayer);
    
    CCSprite *cgBg = CCSprite::createWithSpriteFrameName("cgBg.png");
    cgBg->setAnchorPoint(ccp(0.5, 1));
    cgBg->setOpacity(0);
    cgBg->ignoreAnchorPointForPosition(false);
    cgBg->cocos2d::CCNode::setScaleX(size.width/cgBg->getContentSize().width);
    cgBg->setPosition(ccp(size.width/2, size.height));
    cgBg->runAction(CCFadeIn::create(3));
    finaleSprite->addChild(cgBg,0,0);
    
    CCRect scrollRect = CCRectMake(30, size.height*0.2, size.width, size.height/4);
    cgScrollView = SNSScrollView::create(scrollRect);
    cgScrollView->setFrame(scrollRect);
    cgScrollView->setPosition(scrollRect.origin);
    cgScrollView->setHorizontal(false);
    cgScrollView->setVertical(true);
    cgScrollView->setTouchEnabled(false);
    finaleSprite->addChild(cgScrollView);
    
    
#if (PLATFORM == ANDROID)
    CCLayerColor *color =CCLayerColor::create(ccc4(0, 0, 0, 255), size.width, size.height / 2);
    color->setOpacity(0);
    
    this->addChild(color, -1);
    color->runAction(CCSequence::create(CCFadeIn::create(4),NULL));
    color->setPosition(ccp(0, 0));
#endif
    
    
    SGCCLabelTTF *cgMsg = NULL;
    CCString *str =  NULL;
    posY =  -20;
    int count = msgArrs->count();
    for (int i=0; i<count; i++)
    {
        
        str = (CCString *)msgArrs->objectAtIndex(i);
        cgMsg =  SGCCLabelTTF::create(str->getCString(), FONT_BOXINFO, 34);
        cgMsg->cocos2d::CCNode::setPosition(size.width/2, posY);
        cgScrollView->m_body->addChild(cgMsg);
        posY -= 50;
    }
    moveCgMsgDis = FINALMSGMOVEDIS;
    
}

void SGAutoGuideLayer::recordStepListener()
{
	CCLOG("From newComer Response");
}

void SGAutoGuideLayer::showFinalMsg()
{
    unschedule(schedule_selector(SGAutoGuideLayer::moveCgMsgUp));
    schedule(schedule_selector(SGAutoGuideLayer::moveCgMsgUp), FINALMSGMOVEGAP );
	
	//演示关结束，新手进度
	SGMainManager::shareMain()->sendNewPlayerProcess(show_fight_demo, 0);
}

void SGAutoGuideLayer::moveCgMsgUp()
{
    if (posY > size.height*0.02 && !finaleUpSprite->isVisible())
    {
        finaleUpSprite->setVisible(true);
        finaleUpSprite->runAction(CCSequence::create(CCFadeIn::create(0.5),
                                                   CCCallFunc::create(this,  callfunc_selector(SGAutoGuideLayer::autoGuideToCountry))
                                                   ,NULL));
    }
    else
    {
        posY += moveCgMsgDis;
        cgScrollView->m_body->setPositionY(cgScrollView->m_body->getPositionY() + moveCgMsgDis);
    }
}

void SGAutoGuideLayer::autoGuideToCountry()
{
    unschedule(schedule_selector(SGAutoGuideLayer::moveCgMsgUp));
    this->removeChildByTag(FINALMSGTAG, true);
    this->removeChildByTag(FINALMSGTAG+10, true);
	CCLOG("second guide [%s-%d]", __FUNCTION__, __LINE__);

    SGGuideManager::shareManager()->startAutoGuide(2);
}

void SGAutoGuideLayer::autoAttackSkill(SGBattleMap *bm)
{
    SGHeroLayer *heroLayer = (SGHeroLayer *)bm->getParent();
    heroLayer->mainSkillAttackHandler();
}


