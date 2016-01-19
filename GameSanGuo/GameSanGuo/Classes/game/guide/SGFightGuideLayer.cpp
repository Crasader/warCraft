 //
//  SGFightGuideLayer.cpp
//  GameSanGuo
//
//  Created by geng on 13-5-9.
//
//

#include "SGFightGuideLayer.h"
#include "SGBattleManager.h"
#include "SGStaticDataManager.h"
#include "SGGuideManager.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "AppMacros.h"
#include "SGStringConfig.h"


#define MOVETIME    0.5
#define GAPTIME     0.8
#define DOWNTIME    0.1
#define UPTIME      0.2
#define INSTANTIME  0.01
#define DOWNSCALE   0.6
#define POPLDTAG    1111
#define POPMDTAG    2222
#define POPLABELTAG    3333

SGFightGuideLayer::~SGFightGuideLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_fightguideLayer);
    CC_SAFE_RELEASE(msgArrs);
}

SGFightGuideLayer *SGFightGuideLayer::create(SGBattleLayer *battleLayer)
{
    SGFightGuideLayer *layer = new SGFightGuideLayer();
    if (layer->init(battleLayer))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}
bool SGFightGuideLayer::init(SGBattleLayer *bl)
{
    if (!CCLayer::init())
    {
        return false;
    }
    ResourceManager::sharedInstance()->bindTexture("sguiguidelayer/sguiguidelayer.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);
	ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/djjx_new.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jiantou_new.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);
    
    msgArrs = NULL;
//    this->setTouchEnabled(true);
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    battleLayer = bl;
    battleLayer->setIsCanTouch(false);
    step = 1;
//    bboard = CCSprite::create("story/guide_board_new.png");
    bboard = CCSprite::createWithSpriteFrameName("guide_board_new.png");

    bboard->setScaleX(size.width/bboard->getContentSize().width);
    this->addChild(bboard);
    bboard->setVisible(false);
//    bboard->setPosition(SGLayout::getPoint(kMiddleCenter));
//    CCLayerColor *bgLayer  =CCLayerColor::create(ccc4(0, 0, 0, 220));
//    this->addChild(bgLayer,-3);
    
//    actSprite = CCSprite::create("story/uplevel_click.png");
    actSprite = CCSprite::createWithSpriteFrameName("uplevel_click.png");

    float posXX = bboard->getContentSize().width-actSprite->getContentSize().width*.75f;
    float posYY = actSprite->getContentSize().height*.70f;
    actSprite->setPosition(ccp(posXX, posYY));
    //actSprite->setScale(bboard->getContentSize().width/size.width + 0.2);
    bboard->addChild(actSprite);
    
//    CCAction *action = CCSequence::create(CCMoveBy::create(0.12f, ccp(0, 2)),
//                                          CCMoveBy::create(0.12f, ccp(0, 2)),
//                                          CCMoveBy::create(0.12f, ccp(0, -2)),
//                                          CCMoveBy::create(0.12f, ccp(0, -2)),
//                                          NULL);
//    actSprite->runAction(CCRepeatForever::create((CCActionInterval *)action));

    CCSpriterX *resultx = CCSpriterX::create("animationFile/djjx.scml", true, true);
    resultx->setAnchorPoint(ccp(0.5, 0.5));
    posXX = posXX-actSprite->getContentSize().width*.75f;
    posYY = posYY - actSprite->getContentSize().height*.45f;
    resultx->setPosition(ccp(posXX, posYY));
    resultx->setanimaID(0);
    resultx->setisloop(true);
    bboard->addChild(resultx, 10,10);
    resultx->play();
    
    CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
    CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-40);
    label = SGRichLabel::getFomLabel("", dsize, FONT_PENGYOU, 32);
    label->setPosition(this->getPosition());
    this->addChild(label, 2);
    
    showNum = 0;

    //指引的小手指
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
    
    
    ///
    guidePopLeft = CCSprite::createWithSpriteFrameName("guidePopLeft.png");
    addChild(guidePopLeft);
    guidePopLeft->setVisible(false);
    
    CCSprite *guidePopMid = CCSprite::createWithSpriteFrameName("guidePopMid.png");
    CCSprite *guidePopRight = CCSprite::createWithSpriteFrameName("guidePopLeft.png");
    
    guidePopWid = size.width*0.65;
	guidePopWid = guidePopWid - guidePopLeft->getContentSize().width *2;
    int midCount = (int)( guidePopWid / guidePopMid->getContentSize().width);
    guidePopWid = guidePopLeft->getContentSize().width * 2 + guidePopMid->getContentSize().width * midCount;
    
    guidePopRight->setFlipX(true);
    guidePopRight->cocos2d::CCNode::setPosition(ccp(guidePopWid - guidePopRight->getContentSize().width/2,guidePopLeft->getContentSize().height/2));
    
    guidePopMid->setAnchorPoint(ccp(0, 0.5));
    guidePopMid->cocos2d::CCNode::setPosition(ccp(guidePopLeft->getContentSize().width,guidePopLeft->getContentSize().height/2));
    
    guidePopLeft->addChild(guidePopMid);
    guidePopLeft->addChild(guidePopRight);
    
    for (int i=1; i<midCount; i++) {
        guidePopMid = CCSprite::createWithSpriteFrameName("guidePopMid.png");
        guidePopMid->setAnchorPoint(ccp(0, 0.5));
        guidePopMid->cocos2d::CCNode::setPosition(ccp(guidePopLeft->getContentSize().width + guidePopMid->getContentSize().width*i,guidePopLeft->getContentSize().height/2));
        guidePopLeft->addChild(guidePopMid);
    }
    
    CCSprite *guidePopLd = CCSprite::createWithSpriteFrameName("guidePopLd.png");
    CCSprite *guidePopMd = CCSprite::createWithSpriteFrameName("guidePopMd.png");
    guidePopLd->setPosition(ccp(guidePopWid*0.2, -guidePopLd->getContentSize().height/2+5));
    guidePopMd->setPosition(ccp(guidePopWid*0.5, -guidePopMd->getContentSize().height/2+5));
    guidePopLeft->addChild(guidePopLd,0,POPLDTAG);
    guidePopLeft->addChild(guidePopMd,0,POPMDTAG);
    
    CCSprite* popLabel = SGRichLabel::getFomLabel("pop info ", CCSizeMake(guidePopWid-60, guidePopLeft->getContentSize().height-20), FONT_PENGYOU, 32);
    popLabel->setPosition(ccp(guidePopWid/2, guidePopLeft->getContentSize().height/2-20 ));
    guidePopLeft->addChild(popLabel,10,POPLABELTAG);
    
    CCSprite *actSprite = CCSprite::createWithSpriteFrameName("uplevel_click.png");
    posXX = guidePopWid-actSprite->getContentSize().width*.6f;
    posYY = actSprite->getContentSize().height*.70f;
    actSprite->setPosition(ccp(posXX, posYY));
    //actSprite->setScale(guidePopWid/size.width + 0.15);
    guidePopLeft->addChild(actSprite);

    resultx = CCSpriterX::create("animationFile/djjx.scml", true, true);
    resultx->setAnchorPoint(ccp(0.5, 0.5));
    posXX = posXX-actSprite->getContentSize().width*.6f;
    posYY = posYY - actSprite->getContentSize().height*.45f;
    resultx->setPosition(ccp(posXX, posYY));
    resultx->setanimaID(0);
    resultx->setisloop(true);
    guidePopLeft->addChild(resultx, 10);
    resultx->play();
    
    tuodong = SGCCLabelTTF::create(str_TouDong_, FONT_PENGYOU, 30 , ccYELLOW);
    tuodong->setAnchorPoint(ccp(0.5, 0.5));
//modify by : zyc.  merge in create.
//    tuodong->setInsideColor(ccYELLOW);
    tuodong->setVisible(false);
    this->addChild(tuodong,1);
    
    isShowDemo = false;
    stopBlink = true;
    return true;
}

//-------------------------
void SGFightGuideLayer::creatTouchTipScaling(CCPoint startPos)
{
    //SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
//    CCArray *array = selfLayer->getMenu()->getChildren();
//    for (int ii = 0; ii < array->count(); ii++) {
//        SGButton *button = (SGButton *)array->objectAtIndex(ii);
//        button->setEnabled(true);
//    }
	spTouch->stopAllActions();
    spTouch->stopActionByTag(0);
    spTouch->setVisible(true);
    spTouch->setPosition(ccpAdd(startPos, ccp(0, 0)));
    spTouch->setanimaID(0);
    spTouch->stop();
    

    CCAction *moveAction = CCRepeatForever::create(
                                                   CCSequence::create(
																	  CCScaleTo::create(DOWNTIME, DOWNSCALE),
																	  CCScaleTo::create(UPTIME, 1.0),
																	  CCDelayTime::create(GAPTIME),
																	  CCDelayTime::create(GAPTIME),
																	  CCScaleTo::create(INSTANTIME, 1.0),
																	  CCScaleTo::create(DOWNTIME, DOWNSCALE),
																	  CCDelayTime::create(GAPTIME/2),
																	  CCScaleTo::create(UPTIME, 1.0),
																	  CCDelayTime::create(GAPTIME),
																	  CCDelayTime::create(GAPTIME),
																	  CCDelayTime::create(GAPTIME/2),
																	  CCScaleTo::create(INSTANTIME, 1.0),
																	  NULL));
    
	
    moveAction->setTag(0);
    spTouch->runAction(moveAction);
}
//-------------------------------------


void SGFightGuideLayer::creatTouchTipMoving(CCPoint startPos,CCPoint endPos,bool showBoard)
{
    //SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
    //CCArray *array = selfLayer->getMenu()->getChildren();
    
    //removed by cgp
//    for (int ii = 0; ii < array->count(); ii++) {
//        SGButton *button = (SGButton *)array->objectAtIndex(ii);
//        button->setEnabled(true);
//    }
    float distX = endPos.x - startPos.x;
    float distY = endPos.y - startPos.y;
    this->setVisible(true);
    bboard->setVisible(showBoard && (label != NULL));
    spTouch->stopActionByTag(0);
    spTouch->setRotation(0);
    spTouch->setVisible(true);
    spTouch->setPosition(startPos);
    spTouch->setanimaID(0);
    spTouch->stop();
    
    tuodong->setPosition(ccpAdd(startPos, ccp(tuodong->getContentSize().width/2, -tuodong->getContentSize().height*2.4)));
    tuodong->setVisible(true);
    
    guidePopLeft->setVisible(false);
 
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    CCAction *moveAction = CCRepeatForever::create(
                                                   CCSequence::create(
                                                       CCScaleTo::create(DOWNTIME, DOWNSCALE),
                                                       CCMoveBy::create(MOVETIME, ccp(distX,distY)),
                                                       CCScaleTo::create(UPTIME, 1.0),
                                                       CCDelayTime::create(GAPTIME),
                                                       CCMoveBy::create(INSTANTIME, ccp(winSize.width,0)),
                                                       CCDelayTime::create(GAPTIME),
                                                       CCMoveBy::create(INSTANTIME, ccp(-distX - winSize.width,-distY)),
                                                       CCScaleTo::create(INSTANTIME, 1.0),
                                                       NULL));
    
    CCAction *moveAction1 = CCRepeatForever::create(
                                                   CCSequence::create(
                                                                      CCDelayTime::create(DOWNTIME),
                                                                      CCMoveBy::create(MOVETIME, ccp(distX,distY)),
                                                                      CCDelayTime::create(UPTIME),
                                                                      CCDelayTime::create(GAPTIME),
                                                                      CCMoveBy::create(INSTANTIME, ccp(winSize.width,0)),
                                                                      CCDelayTime::create(GAPTIME),
                                                                      CCMoveBy::create(INSTANTIME, ccp(-distX - winSize.width,-distY)),
                                                                      CCDelayTime::create(INSTANTIME),
                                                                      NULL));
    tuodong->runAction(moveAction1);
    moveAction->setTag(0);
    spTouch->runAction(moveAction);
}

void SGFightGuideLayer::showTouchTip(int fromId)
{
    // btnType 1:援军 2:行动力 3:结束回合，投降 4:主将计 5:发动技能
    CCPoint position = ccp(0, 0);
    if (fromId == 1 || fromId == 2 || fromId == 3 || fromId == 4 || fromId == 5)
    {
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        //CCArray *array = selfLayer->getMenu()->getChildren();
        selfLayer->setIsCanTouch(true);
 
        //removed by cgp
//        for (int ii = 0; ii < array->count(); ii++) {
//            SGButton *button = (SGButton *)array->objectAtIndex(ii);
//            button->setEnabled(false);
//        }
        
        if (fromId == 1)
        { //1-1 援军按钮
            selfLayer->yb_btn->setEnabled(true);
            position = selfLayer->yb_btn->getPosition();
            position.x -= 50;
            position.y += 20;
        }
        else if (fromId == 2)
        { //1-1 行动力按钮，投降
            selfLayer->yj_btn->setEnabled(true);
            position = selfLayer->yj_btn->getPosition();
        }
        else if(fromId == 3)   //结束回合
        {
            selfLayer->yj_btn->setEnabled(true);
            position = selfLayer->yj_btn->getPosition();
        }
        else if (fromId == 4)
        { //1-1 主将计按钮
            selfLayer->zjj_btn->setEnabled(true);
            position = selfLayer->zjj_btn->getPosition();
            position.x += 100;
            position.y += 40;
        }
        else if(fromId == 5)
        {
            selfLayer->zjj_btn->setEnabled(true);
            position = selfLayer->zjj_btn->getPosition();
            position.x += 300;
            position.y += 200;
        }
    }
//    else if (fromId == 5)
//    { //1-1 结束回合按钮，行动力弹框
//        SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//        SGBaseBox *baseBox = (SGBaseBox *)mainScene->getChildByTag(mainScene->nowBoxTag);
//        CCArray *array = baseBox->getMenu()->getChildren();
//        for (int ii = 0; ii < array->count(); ii++)
//        {
//            SGButton *button = (SGButton *)array->objectAtIndex(ii);
//            button->setEnabled(false);
//        }
//        SGButton *button = (SGButton *)baseBox->getBtnByTag(101);
//        button->setEnabled(true);
//        position = button->getPosition();
//    }
    
    if (!(position.x==0&&position.y==0))
    {
        bboard->setVisible(false);
        if (label)
        {
            label->setVisible(false);
        }
        this->setVisible(true);

        if (guidePopLeft) {
            guidePopLeft->setVisible(false);
        }
        
    
        float disH = -spDirect->getContentSize().height*.35f + 25;
        
        CCPoint point = ccpAdd(position, ccp(0, disH));
        
        //援军小手的提示，这里的做翻转，由上向下指，并做对应的6 位置调整
        spTouch->setPosition(point);
        
        //int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        if (fromId == 1)//援军上的提示翻转90度，向右偏移
        {
            spTouch->setRotation(-90);
            spTouch->setPosition(ccpAdd(point, ccp(10, 25)));
        }
        else
        {
            spTouch->setRotation(0);
            spTouch->setPosition(ccpAdd(point, ccp(0, 0)));
        }
        spTouch->setVisible(true);
        
    }
}

#if 0
void SGFightGuideLayer::showTouchTip(int fromId)
{
    // btnType 1:援军 2:行动力 3:结束回合，投降 4:主将计 5:发动技能
    CCPoint position = ccp(0, 0);
    if (fromId == 1 || fromId == 2 || fromId == 4)
    {
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        CCArray *array = selfLayer->getMenu()->getChildren();
        selfLayer->setIsCanTouch(true);
        
        //removed by cgp
        //        for (int ii = 0; ii < array->count(); ii++) {
        //            SGButton *button = (SGButton *)array->objectAtIndex(ii);
        //            button->setEnabled(false);
        //        }
        
        if (fromId == 1)
        { //1-1 援军按钮
            selfLayer->yb_btn->setEnabled(true);
            position = selfLayer->yb_btn->getPosition();
        }
        else if (fromId == 2)
        { //1-1 行动力按钮，投降
            selfLayer->xdl_btn->setEnabled(true);
            position = selfLayer->xdl_btn->getPosition();
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
        if (label)
        {
            label->setVisible(false);
        }
        this->setVisible(true);
        
        if (guidePopLeft) {
            guidePopLeft->setVisible(false);
        }
        
        
        float disH = -spDirect->getContentSize().height*.35f + 25;
        
        CCPoint point = ccpAdd(position, ccp(0, disH));
        
        //援军小手的提示，这里的做翻转，由上向下指，并做对应的6 位置调整
        spTouch->setPosition(point);
        
        //int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
        if (fromId == 1)//援军上的提示翻转90度，向右偏移
        {
            spTouch->setRotation(-90);
            spTouch->setPosition(ccpAdd(point, ccp(10, 25)));
        }
        else
        {
            spTouch->setRotation(0);
            spTouch->setPosition(ccpAdd(point, ccp(0, 0)));
        }
        spTouch->setVisible(true);
        
    }
}
#endif


void SGFightGuideLayer::showDirectTip(int fromId)
{
    //
    
    // tipType 1:对方血条 2:行动力 3:援军 4:主将计 5:回合数 6:武将
    CCPoint position = ccp(0, 0);
    tipDirect dirType = tip_down;
    float disc = spDirect->getContentSize().height*.5f+skewing(15);
    float right_shift = 0.0;//敌方箭头右侧偏移
	float down_shift = 0.0;//第二关箭头向下偏移
    float popDiscY = 0;
    float popMoveActDis = guidePopLeft->getContentSize().height * 1.2;
    
    if (fromId == 1)
    {
        dirType = tip_up;
		//ipad适配
		if (CCEGLView::sharedOpenGLView()->getFrameSize().width == iphonehdResource.size.width)//ipad
		{
			right_shift = 60.0;
			down_shift = 875 +  CCEGLView::sharedOpenGLView()->getFrameSize().height - 1136;
		}
		else if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width)
		{
			right_shift = 122.0;
			down_shift = 755;
		}
		else if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
		{
			right_shift = 122.0;
			down_shift = 755;
		}
#if (PLATFORM == ANDROID)
		{
			right_shift = 77.0;
			down_shift = 885;
		}
#endif
		
        disc = (spDirect->getContentSize().height*.5f+skewing(15)) + down_shift;
        //spDirect->setRotation(-90.0f);
        SGHeroLayer *selfLayer = battleLayer->getOpponent(true);
        position = selfLayer->hpProgress->getPosition();
        position.x += 120;
        position.y += 60;
        disc -= selfLayer->hpProgress->getContentSize().height*.5f;
        
        guidePopLeft->getChildByTag(POPLDTAG)->setVisible(false);
        CCSprite *popMid = (CCSprite *) guidePopLeft->getChildByTag(POPMDTAG);
        popMid->setVisible(true);
        if (!popMid->isFlipY()) {
            popMid->setFlipY(true);
        }
        popMid->setPosition(ccp(guidePopWid*0.5, popMid->getContentSize().height/2 + guidePopLeft->getContentSize().height-5));
        popDiscY = -popMid->getContentSize().height;
        popMoveActDis = -popMoveActDis;
        
    }
    else if (fromId == 2)
    {
        //spDirect->setRotation(90.0f);
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
       
        if (!SGGuideManager::shareManager()->isWanXDL)
        {
            position = selfLayer->xdl_btn->getPosition();
            position.x -= 60;   //modified by cgp
            
            disc += selfLayer->xdl_btn->getContentSize().height*.5f;
            
            guidePopLeft->getChildByTag(POPLDTAG)->setVisible(false);
            CCSprite *popMid = (CCSprite *) guidePopLeft->getChildByTag(POPMDTAG);
            popMid->setVisible(true);
            if (popMid->isFlipY()) {
                popMid->setFlipY(false);
            }
            popMid->setPosition(ccp(guidePopWid*0.5, -popMid->getContentSize().height/2+5));
            popDiscY = popMid->getContentSize().height;
  
        }
        else
        {
            position = selfLayer->yj_btn->getPosition();
            position.x -= 50;
            position.y -=20;
            SGGuideManager::shareManager()->isWanXDL = false;

            disc += selfLayer->xdl_btn->getContentSize().height*.5f;
            
            guidePopLeft->getChildByTag(POPMDTAG)->setVisible(false);
            CCSprite *popRd = (CCSprite *) guidePopLeft->getChildByTag(POPLDTAG);
            popRd->setVisible(true);
            popRd->setFlipX(true);
            if (popRd->isFlipY())
            {
                popRd->setFlipY(false);
            }
            popRd->setPosition(ccp(guidePopWid*0.5, -popRd->getContentSize().height/2+5));
            popDiscY = popRd->getContentSize().height;
        }

    }
    else if (fromId == 3)  //援军
    {
        //spDirect->setRotation(90.0f);
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        position = selfLayer->yb_btn->getPosition();
        disc += selfLayer->yb_btn->getContentSize().height*.5f;
        
        
        guidePopLeft->getChildByTag(POPMDTAG)->setVisible(false);
        CCSprite *popRd = (CCSprite *) guidePopLeft->getChildByTag(POPLDTAG);
        popRd->setFlipX(true);
        popRd->setVisible(true);
        if (popRd->isFlipY())
        {
            popRd->setFlipY(false);
        }
        popRd->setPosition(ccp(guidePopWid*0.7, -popRd->getContentSize().height/2+5));
        popDiscY = popRd->getContentSize().height;
        right_shift = -guidePopWid/4;
        position.x -= 100;
    }
    else if (fromId == 4)   //主将计
    {
        //spDirect->setRotation(90.0f);
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        position = selfLayer->zjj_btn->getPosition();
        position.x += 80;
        disc += selfLayer->zjj_btn->getContentSize().height*.5f;
        
        guidePopLeft->getChildByTag(POPMDTAG)->setVisible(false);
        CCSprite *popLf = (CCSprite *) guidePopLeft->getChildByTag(POPLDTAG);
        popLf->setVisible(true);
        if (popLf->isFlipX()) {
            popLf->setFlipX(false);
        }
        popLf->setPositionX(guidePopWid*0.2);
        popDiscY = popLf->getContentSize().height;
        right_shift = guidePopWid*0.4;
    }
    if (fromId == 5 || fromId == 6)
    {
        int story = SGPlayerInfo::sharePlayerInfo()->getCurrentStoryId();
        int scene = SGPlayerInfo::sharePlayerInfo()->getCurrentSceneId();
        int step = SGGuideManager::shareManager()->step;
        SGStaticDataManager *manager = SGStaticDataManager::shareStatic();
        SGFightGuideVO *model = manager->getFightGuideModel(story, scene, step);
        if (!model)
        {
            return;
        }
        CCPoint point = model->getPoint();
        GameIndex index = gameIndex(point.x, point.y);
        position = GameConfig::getGridPoint(index.i, index.j);
        disc = -(gridHeight*.5f+skewing(15));
        dirType = tip_up;
        //spDirect->setRotation(-90.0f);
        
        if (fromId == 6)
        {
            bool needFlipX = true;
            if (index.j <= mapList/2)
            {
                needFlipX = false;
            }
            guidePopLeft->getChildByTag(POPMDTAG)->setVisible(false);
            CCSprite *popLf = (CCSprite *) guidePopLeft->getChildByTag(POPLDTAG);
            popLf->setVisible(true);
            
            if (popLf->isFlipX() != needFlipX)
            {
                popLf->setFlipX(needFlipX);
            }
            popLf->setPositionX(guidePopWid*(needFlipX?0.7:0.2));
            popDiscY = popLf->getContentSize().height + guidePopLeft->getContentSize().height*1.5;
            right_shift = guidePopWid/2;
        }
        else   //回合数 5
        {
            bool needFlipX = true;
            if (index.j <= mapList/2)
            {
                needFlipX = false;
            }
            guidePopLeft->getChildByTag(POPMDTAG)->setVisible(false);
            CCSprite *popLf = (CCSprite *) guidePopLeft->getChildByTag(POPLDTAG);
            popLf->setVisible(true);
            
            if (popLf->isFlipX() != needFlipX)
            {
                popLf->setFlipX(needFlipX);
            }
            popLf->setPositionX(guidePopWid*(needFlipX?0.7:0.2));
            popDiscY = popLf->getContentSize().height + guidePopLeft->getContentSize().height*1.3;
            right_shift = -guidePopWid*0.3;
            position.x -= 30;
            position.y += 20;
        }
        
    }
    if (!( position.x == 0 && position.y == 0))
    {
        spTouch->setVisible(false);
        guidePopLeft->setPosition(ccpAdd(ccpAdd(position, ccp(0 + right_shift, disc)), ccp((guidePopLeft->getContentSize().width - guidePopWid)/2, popDiscY + popMoveActDis)) );
		this->setTouchEnabled(false);
        CCAction *seq1 = CCSequence::create(CCMoveBy::create(0.2f, ccp(0, -popMoveActDis)), CCCallFunc::create(this, callfunc_selector(SGFightGuideLayer::setTouch)), NULL);
        guidePopLeft->runAction(seq1);
        SGRichLabel::setString( ((CCString *)msgArrs->objectAtIndex(showNum-1))->getCString(), (CCSprite*)guidePopLeft->getChildByTag(POPLABELTAG),guidePopLeft);
        dealPopVisible(true);
    }
}
void SGFightGuideLayer::setNoTouch()
{
	this->setTouchEnabled(false);
}
void SGFightGuideLayer::setTouch()
{
	this->setTouchEnabled(true);
}

void SGFightGuideLayer::dealPopVisible( bool popVisible)
{
    if (guidePopLeft) {
         guidePopLeft->setVisible(popVisible);
    }
    if (bboard) {
        bboard->setVisible(!popVisible);
    }
    if (label) {
        label->setVisible(!popVisible);
    }
    
}

void SGFightGuideLayer::closeTips( bool selfTouch)
{
    if (SGGuideManager::shareManager()->isGuide)
    {
        spTouch->setVisible(false);
        if (!spTouch->isplaying())
        {
            spTouch->play();
        }
        if (spTouch->getActionByTag(0))
        {
            spTouch->stopActionByTag(0);
        }
        if (tuodong)
        {
            tuodong->setVisible(false);
            tuodong->stopAllActions();
        }
        guidePopLeft->setVisible(false);
        
        this->setVisible(false);
        this->setTouchEnabled(false);
        bboard->setVisible(true);
        if (label)
        {
            label->setVisible(true);
        }
        
        SGHeroLayer *selfLayer = battleLayer->getOpponent(false);
        selfLayer->setIsCanTouch(selfTouch);
        //CCArray *array = selfLayer->getMenu()->getChildren();
        
        //removed by cgp
//        for (int ii = 0; ii < array->count(); ii++) {
//            SGButton *button = (SGButton *)array->objectAtIndex(ii);
//            button->setEnabled(true);
//        }
    }
}

void SGFightGuideLayer::setBattleLayer(SGBattleLayer *bl)
{
    battleLayer = bl;
    battleLayer->setIsCanTouch(false);
}
void SGFightGuideLayer::initTips(std::vector<int> tips)
{
    msgTips.clear();
    for (int ii = 0; ii < tips.size(); ii++) {
        msgTips.push_back(tips[ii]);
    }
}
void SGFightGuideLayer::nextMsg(bool showResultSprite)
{
    int plotID= SGPlayerInfo::sharePlayerInfo()->getPlotId();
    if (plotID==1 )
    {
        stopBlink=true;
        this->unschedule(schedule_selector(SGFightGuideLayer::updateBlink));
        
    }
    
    dealPopVisible(false);

    bboard->setVisible(true);
    bboard->getChildByTag(10)->setVisible(showResultSprite);
    actSprite->setVisible(showResultSprite);

    if (showNum < msgArrs->count())
    {
        CCString *str = (CCString *)msgArrs->objectAtIndex(showNum);
        
        if (label)
        {
            label->removeFromParentAndCleanup(true);
            label = NULL;
        }
        CCSize size = CCDirector::sharedDirector()->getWinSize();
        CCSize tsize = CCSizeMake(size.width, bboard->getContentSize().height);
        CCSize dsize = CCSizeMake(tsize.width-40, tsize.height-50);
        label = SGRichLabel::getFomLabel(str->getCString(), dsize, FONT_PENGYOU, 32);
        label->setPosition(bboard->getPosition());
        this->addChild(label, 2);
        showNum++;
        
        for (int ii = 0; ii < msgTips.size(); ii += 2)
        {
            if (showNum == msgTips[ii])
            {
                this->showDirectTip(msgTips[ii+1]);
                
                //闪动的血条量
                
                int plotID = SGPlayerInfo::sharePlayerInfo()->getPlotId();
                if (plotID == 1 && msgTips[ii+1] == 1 && showNum == 3)
                {
                    stopBlink=false;
                    this->schedule(schedule_selector(SGFightGuideLayer::updateBlink), 0.2);
                    
                }
            }
        }
    }
    else
    {
        msgTips.clear();
        showNum = 0;
        if(label)
        {
            label->removeFromParentAndCleanup(true);
            label = NULL;
        }

        bboard->setVisible(false);
        bboard->getChildByTag(10)->setVisible(false);
        SGGuideManager::shareManager()->startFightGuide(2);
    }
}
void SGFightGuideLayer::creatWujiangDemo(CCArray *arr,bool iswujiangdemo )
{
    if (msgArrs)
    {
        msgArrs->release();
    }
    msgArrs = arr;
    msgArrs->retain();
    showNum = 0;
    isShowDemo = true;
    isShowDemoTouch = true;
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    
    cocos2d::extension::CCScale9Sprite *generals = NULL;
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/helpGeneralGroup.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/helpShiBinGroup.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);

    CCSprite *tmp1 = CCSprite::createWithSpriteFrameName("helpGeneralGroup.png");
    CCSprite *tmp2 = CCSprite::createWithSpriteFrameName("helpShiBinGroup.png");

    if (iswujiangdemo) {
        generals = cocos2d::extension::CCScale9Sprite::createWithSpriteFrame(tmp1->displayFrame());
    }
    else
    {
       generals = cocos2d::extension::CCScale9Sprite::createWithSpriteFrame(tmp2->displayFrame());
    }
    
    this->addChild(generals,3,-99);
    generals->setPosition(ccp(size.width/2, size.height*0.62 ));


    cocos2d::extension::CCScale9Sprite *frame = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("box_kuang.png");
    frame->setPreferredSize(CCSizeMake(size.width*0.86, size.height*0.7));
    this->addChild(frame,3,-100);
    frame->setPosition(ccp(size.width/2, size.height/2));
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/Tips.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/tipsinside.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/djjx_new.plist", RES_TYPE_LAYER_UI, sg_fightguideLayer);
    
    cocos2d::extension::CCScale9Sprite *frame2 = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("Tips.png");
    this->addChild(frame2,0,-101);
    frame2->setScaleX(size.width*0.86/frame2->getContentSize().width);
    frame2->setScaleY(size.height*0.7/frame2->getContentSize().height);
    frame2->setPosition(ccp(size.width/2, size.height/2));
    
    cocos2d::extension::CCScale9Sprite *fontbg = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("tipsinside.png");
    this->addChild(fontbg,1,-102);
    fontbg->setPreferredSize(CCSizeMake(generals->getContentSize().width, size.height*0.18));
    fontbg->setPosition(ccp(size.width/2, size.height*0.32));
    
    
    CCSprite *actSprite = CCSprite::createWithSpriteFrameName("uplevel_click.png");
    float posXX = fontbg->getContentSize().width/2;
    float posYY = -fontbg->getContentSize().height/4;
    actSprite->setPosition(ccp(posXX, posYY));
    //actSprite->setScale(guidePopWid/size.width + 0.15);
    fontbg->addChild(actSprite,0,-103);
    
    CCSpriterX *resultx = CCSpriterX::create("animationFile/djjx.scml", true, true);
    resultx->setAnchorPoint(ccp(0.5, 0.5));
    posXX = posXX - actSprite->getContentSize().width*.75f;
    posYY = posYY - actSprite->getContentSize().height*.45f;
    resultx->setPosition(ccp(posXX, posYY));
    resultx->setanimaID(0);
    resultx->setisloop(true);
    fontbg->addChild(resultx, 10);
    resultx->play();
    
    this->nextDemoMsg();
}
void SGFightGuideLayer::nextDemoMsg()
{
    int plotID= SGPlayerInfo::sharePlayerInfo()->getPlotId();
    if (plotID==1 )
    {
        stopBlink=true;
        this->unschedule(schedule_selector(SGFightGuideLayer::updateBlink));
        
    }
    
    dealPopVisible(false);
    bboard->setVisible(false);
    
    bboard->getChildByTag(10)->setVisible(true);
    actSprite->setVisible(true);
    
    if (showNum<msgArrs->count())
    {
        CCString *str = (CCString *)msgArrs->objectAtIndex(showNum);
        
        if (label)
        {
            label->removeFromParentAndCleanup(true);
            label = NULL;
        }
        label = SGRichLabel::getFomLabel(str->getCString(), CCSizeMake(getChildByTag(-102)->getContentSize().width - 8, getChildByTag(-102)->getContentSize().height), FONT_PANGWA, 32,ccRED,ccWHITE,ccBLACK);
        label->setPosition(ccp(getChildByTag(-102)->getPositionX()+4,getChildByTag(-102)->getPositionY()-40*CCDirector::sharedDirector()->getWinSize().height/1136));
        
        this->addChild(label,6);
        showNum++;
    }
    else
    {
        if (isShowDemoTouch)
        {
            isShowDemoTouch = false;
        }
        else
        {
            isShowDemo = false;
            showNum = 0;
            if (label)
            {
                label->removeFromParentAndCleanup(true);
                label = NULL;
            }

            bboard->setVisible(false);
            bboard->getChildByTag(10)->setVisible(false);
            
            SGGuideManager::shareManager()->startFightGuide(2);
            removeChildByTag(-99);
            removeChildByTag(-100);
            removeChildByTag(-101);
            removeChildByTag(-102);
            isShowDemoTouch = true;
        }
    }
}

void SGFightGuideLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);
}

bool SGFightGuideLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

void SGFightGuideLayer::showTipLine()
{

}

void SGFightGuideLayer::deleteLine()
{
    CCSprite *sprte = (CCSprite *)this->getChildByTag(101);
    sprte->stopAllActions();
    sprte->removeFromParentAndCleanup(true);
}

void SGFightGuideLayer::showFreeTip()
{
    bboard->setVisible(false);
    if (label)
    {
        label->setVisible(false);
    }
    this->setVisible(true);
    
    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    SGCCLabelTTF * resultLable = SGCCLabelTTF::create(str_Try_three_samecolor_soldier, FONT_PENGYOU, 30 , ccc3(0xf9,0x5f,0xff) );
    resultLable->setAnchorPoint(ccp(0.5, 0.5));
    resultLable->setPosition(ccpAdd(center, ccp(0, size.height*.3f)));
    this->addChild(resultLable, 1, 99);
    
    SGCCLabelTTF * resultLable2 = SGCCLabelTTF::create(str_Only_move_last_soldier, FONT_PENGYOU, 30 ,ccWHITE ,ccc3(0xf9,0x5f,0xff));
    resultLable2->setAnchorPoint(ccp(0.5, 0.5));
    resultLable2->setPosition(ccpAdd(resultLable->getPosition(), ccp(0, -resultLable->getContentSize().height*1.5)));
    this->addChild(resultLable2, 1, 99*2);
    
    resultLable->runBlink(0.5);
    resultLable2->runBlink(0.5);
}
void SGFightGuideLayer::deleteFreeTip()
{
    if (getChildByTag(99) || getChildByTag(99*2))
    {
        this->setVisible(false);
        bboard->setVisible(true);
        if (label)
        {
            label->setVisible(true);
        }
        this->removeChildByTag(99, true);
        this->removeChildByTag(99*2, true);
    }
}

void SGFightGuideLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if (!isShowDemo)
    {
        this->nextMsg();
    }
    else
    {
        this->nextDemoMsg();
    }
    
}
SGBattleLayer *SGFightGuideLayer::getBattleLayer()
{
    return battleLayer;
}
void SGFightGuideLayer::setPlace(int place)
{
    int move = (place == 2)?-1:1;
    GPCCLOG("move:%d",move);
    bboard->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, move*140)));
    if (label)
    {
         label->setPosition(bboard->getPosition());
    }
   
   
}
void SGFightGuideLayer::draw()
{
    if (!stopBlink)
    {
        if (blink)
        {
            CCSize  size = CCDirector::sharedDirector()->getWinSize();
            //    设置黑色
            ccDrawColor4B(250, 0, 0, 1);
            glLineWidth(3);
            ccDrawRect(ccp(size.width/12+15,size.height-82), ccp (size.width*10/12+40,size.height-102.5));
            
        }
        else
        {
            
            CCSize  size = CCDirector::sharedDirector()->getWinSize();
            //    设置白色
            ccDrawColor4B(255,255,255,1);
            glLineWidth(3);
            ccDrawRect(ccp(size.width/12+15,size.height-82), ccp (size.width*10/12+40,size.height-102.5));
        }
    }
    
}
void SGFightGuideLayer::updateBlink()
{
    
    blink=!blink;
}

void SGFightGuideLayer::initMsg(CCArray *arr,bool showResultSprite)
{
    if (msgArrs)
    {
        msgArrs->release();
    }
    msgArrs = arr;
    msgArrs->retain();
    showNum = 0;
    
    this->nextMsg(showResultSprite);
}


