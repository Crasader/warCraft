//
//  CGPBattleLoadingLayer.cpp
//  GameSanGuo
//
//  Created by caoguoping on 15/12/17.
//
//

#include "CGPBattleLoadingLayer.h"
#include "SGAIManager.h"
#include "SGBattleManager.h"
#include "ResourceManager.h"
#include "SGStaticDataManager.h"
#include "ShiBing.h"
#include "SGMainManager.h"
#include "SGSliceEffectLayer.h"
#include "GlobalConfig.h"


#define DisplayOfficerMax 3   //最大武将介绍数目

CGPBattleLoadingLayer::CGPBattleLoadingLayer(void)
{
    updateTimes = 0;
    
}
CGPBattleLoadingLayer::~CGPBattleLoadingLayer(void)
{
     ResourceManager::sharedInstance()->unBindLayerTexture(sg_batttleLoadingLayer);
}

CGPBattleLoadingLayer* CGPBattleLoadingLayer::create()
{
    CGPBattleLoadingLayer *layer = new CGPBattleLoadingLayer();
    if (layer &&  layer->init(NULL, sg_batttleLoadingLayer))
    {
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}



void CGPBattleLoadingLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("loadscene/loading.plist", RES_TYPE_BATTLE_IMAGE, sg_batttleLoadingLayer);
    ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1017-1026.plist", RES_TYPE_BATTLE_IMAGE, sg_batttleLoadingLayer);
    
    ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1756-1760.plist", RES_TYPE_BATTLE_IMAGE, sg_batttleLoadingLayer);
    
    ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1037-1047.plist", RES_TYPE_BATTLE_IMAGE, sg_batttleLoadingLayer);
    
    ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1008-1013.plist", RES_TYPE_BATTLE_IMAGE, sg_batttleLoadingLayer);
    
    CCLayerColor * colorLayerBg = CCLayerColor::create(ccc4(0, 0, 0,255), 640, 1136 );//背景遮罩
    this->addChild(colorLayerBg, -1);
    
    CCPoint loadingBarPos = ccp(320, 100);
    
    CCSprite*   bgLoading = CCSprite::createWithSpriteFrameName("loading_b.png");
    bgLoading->setPosition(loadingBarPos);
    this->addChild(bgLoading, 1);
    
    barLoading = LoadingBar::create();
    barLoading->loadTexture("loading_c.png",  UI_TEX_TYPE_PLIST);
    barLoading->setPercent(0);
    barLoading->setDirection(LoadingBarTypeLeft);
    barLoading->setPosition(ccpAdd(loadingBarPos, ccp(35, 0)));
    this->addChild(barLoading, 2);
    
    CCSprite*  flowerLoading = CCSprite::createWithSpriteFrameName("loading_a.png");
    flowerLoading->setPosition(loadingBarPos);
    this->addChild(flowerLoading, 3);
    

    
    
    

    
    cc_timeval psv;
    CCTime::gettimeofdayCocos2d(&psv, NULL);
    unsigned long int rand_seed = psv.tv_sec * 1000 + psv.tv_usec/1000;
    srand((unsigned int)rand_seed);

    int  randomId = (int)(CCRANDOM_0_1() * DisplayOfficerMax + 1);//产生一个随机数
    GPCCLOG("############# randomID : %d ",  randomId);
   
    int bigIconId  = SGStaticDataManager::shareStatic()->getLoadingOfficerByRandomId(randomId)->getBigIcon();
    fileId = SGStaticDataManager::shareStatic()->getLoadingOfficerByRandomId(randomId)->getFileId();


    
#define MoveTime 0.5
    
    CCSprite*  infoBg = CCSprite::createWithSpriteFrameName("loadinginfo.png");
    infoBg->setPosition(ccp(320, -330));
    this->addChild(infoBg, 3);
    CCMoveTo* bgMove = CCMoveTo::create(MoveTime, ccp(320, 330));
 
    
    
    SGCCLabelTTF*  labelName = SGCCLabelTTF::create(SGStaticDataManager::shareStatic()->getLoadingOfficerByRandomId(randomId)->getName()->getCString(), FONT_PANGWA, 30 , ccShitYellow);
    labelName->setPosition(ccp(270, 310));
    infoBg->addChild(labelName);
    
    SGCCLabelTTF* labelDescription = SGCCLabelTTF::create(SGStaticDataManager::shareStatic()->getLoadingOfficerByRandomId(randomId)->getDescription()->getCString(), FONT_PANGWA, 24, CCSize(380, 0), ccWHITE);
    labelDescription->setPosition(ccp(260, 140));
    infoBg->addChild(labelDescription);
    
    infoBg->runAction(bgMove);
    
//    CCDelayTime*  showInfoDelay = CCDelayTime::create(MoveTime);
//    CCFadeIn*   fadeInName = CCFadeIn::create(MoveTime);
//    CCFadeIn*   fadeInDes = CCFadeIn::create(MoveTime);
//    labelName->runAction(CCSequence::create(CCFadeOut::create(0.1), showInfoDelay, fadeInName, nullptr));
//    labelDescription->runAction(CCSequence::create(CCFadeOut::create(0.1),  showInfoDelay, fadeInDes, nullptr));
    labelName->fadin(MoveTime * 6);
    labelDescription->fadin(MoveTime * 6);
    
    
    
    
    
    CCString* officerPath = CCString::createWithFormat("officer_%d.png",  bigIconId);

    CCSprite*  spriteOfficer = CCSprite::createWithSpriteFrameName(officerPath->getCString());
    spriteOfficer->setPosition(ccp(-200, 800));
    spriteOfficer->setScale(1.0);
    spriteOfficer->runAction(CCMoveTo::create(MoveTime, ccp(420, 800)));
    this->addChild(spriteOfficer ,   6);
    
    CCSprite*  spriteMirror = CCSprite::createWithSpriteFrameName(officerPath->getCString());
    CCFiniteTimeAction* fade = CCFadeTo::create(MoveTime, 100);
    CCFiniteTimeAction* scale = CCScaleTo::create(MoveTime, 2.0);
    CCFiniteTimeAction* move = CCMoveTo::create(MoveTime, ccp(300, 850));
    spriteMirror->setPosition(ccp(800, 800));
    spriteMirror->setScale(1.4f);
    spriteMirror->runAction(CCSpawn::create(fade, scale, move, nullptr));
    this->addChild(spriteMirror, 5);
    
    
    
    spineHero = CCSkeletonAnimation::createWithFile(CCString::createWithFormat("spine/wujiang_%d_r.json", fileId)->getCString(),  CCString::createWithFormat("spine/wujiang_%d.atlas", fileId)->getCString(),   0.15);
    spineHero->setAnimation(acActionNames[ACT_moveDown], true, 0);
    spineHero->setPosition(ccp(500, 70));
    spineHero->timeScale = 1.2;
    this->addChild(spineHero, 6);
    
    
    CCSkeletonAnimation*  reading = CCSkeletonAnimation::createWithFile("effSpine/loading.json", "effSpine/loading.atlas", 1.0);
    reading->setAnimation("animation", true, 0);
    reading->setPosition(ccp(300, 150));
    reading->timeScale = 1.0;
    this->addChild(reading, 6);

    schedule(schedule_selector(CGPBattleLoadingLayer::loadingBarUpdate), 0.1, 10, 0.0);

    CCDelayTime*  delaySplit = CCDelayTime::create(1.0);
    CCCallFunc*   callSp = CCCallFunc::create(this, callfunc_selector(CGPBattleLoadingLayer::delayCallSplit));
    this->runAction(CCSequence::create(delaySplit, callSp, nullptr));
    
    

 
}


void CGPBattleLoadingLayer::delayCallSplit()
{
    spineHero->stopAllActions();
    gotoFightLayer();
    schedule(schedule_selector(CGPBattleLoadingLayer::updateSplit), 0.1);
}

void CGPBattleLoadingLayer::reupdateLoading(float times)
{
    updateTimes ++;
    barLoading->setPercent(updateTimes * 8 + 20);
    if (updateTimes == 10)
    {
        restartUpdate();
    }
    
    
}

void CGPBattleLoadingLayer::updateSplit(float times)
{
    if (SGBattleManager::sharedBattle()->getIsLoadingReady())
    {
        unschedule(schedule_selector(CGPBattleLoadingLayer::updateSplit));
        SGBattleManager::sharedBattle()->setIsLoadingReady(false);
        schedule(schedule_selector(CGPBattleLoadingLayer::reupdateLoading), 0.1, 10, 0);
        updateTimes = 0;
        spineHero->setAnimation(acActionNames[ACT_moveDown], true);
        
        

      //  allDelayTime = 1.0 + efftime + 0.2;
    }
}

void CGPBattleLoadingLayer::restartUpdate()
{
    float efftime = SGStaticDataManager::shareStatic()->getSbLocalByFileId(fileId)->effTime;
    
    spineHero->setAnimation(acActionNames[ACT_atkDown], false);
    CCDelayTime*  delay1 = CCDelayTime::create(efftime);
    CCCallFunc*   call1 = CCCallFunc::create(this, callfunc_selector(CGPBattleLoadingLayer::showSplitLight));
    this->runAction(CCSequence::create(delay1, call1, nullptr));
    
}



void CGPBattleLoadingLayer::loadingBarUpdate(float times)
{
    updateTimes ++;
    //if (updateTimes >= 10)
    {
        //updateTimes = 0;
       // unschedule(schedule_selector(CGPBattleLoadingLayer::loadingBarUpdate));
        
        
    }
   // else
    {
        barLoading->setPercent(updateTimes * 2);
    }
    
}

void CGPBattleLoadingLayer::showSplitLight()
{
    lightEff = CCSkeletonAnimation::createWithFile("effSpine/daoguang.json", "effSpine/daoguang.atlas", 1);
    lightEff->setAnimation("animation", false, 0);
    lightEff->setPosition(320, 568);
    this->addChild(lightEff,1000);
    
    CCDelayTime*  delay2 = CCDelayTime::create(0.5);
    CCCallFunc*   call2 = CCCallFunc::create(this, callfunc_selector(CGPBattleLoadingLayer::splitScreen));
    this->runAction(CCSequence::create(delay2, call2, nullptr));
    


}

void CGPBattleLoadingLayer::splitScreen()
{
    
    
    
    
    //goto Fight
//    CCDelayTime* delay = CCDelayTime::create(0.2);
//    CCCallFunc*  call = CCCallFunc::create(this, callfunc_selector(CGPBattleLoadingLayer::gotoFightLayer));
//    this->runAction(CCSequence::create(delay, call, nullptr));
    
//    CCSize size =  CCDirector::sharedDirector()->getVisibleSize();
//
//   
//    //左边屏幕
//    CCRenderTexture* leftTexture = CCRenderTexture::create(size.width/2, size.height);
//
//    leftTexture->getSprite()->setAnchorPoint(CCPoint(1,1));
//    leftTexture->getSprite()->setPosition(CCPoint(size.width/2,0));
//
//    leftTexture->begin();
//    CCDirector::sharedDirector()->getRunningScene()->visit();
//    leftTexture->end();
//
//    leftTexture->getSprite()->setTextureRect(CCRect(0,0,size.width / 2,size.height));
//
//    //右边屏幕
//    CCRenderTexture* rightTexture = CCRenderTexture::create(size.width, size.height);
//
//    rightTexture->getSprite()->setAnchorPoint(CCPoint(0,1));
//    rightTexture->getSprite()->setPosition(CCPoint(size.width/2,0));
//
//    rightTexture->begin();
//    CCDirector::sharedDirector()->getRunningScene()->visit();
//    rightTexture->end();
//
//    rightTexture->getSprite()->setTextureRect(CCRect(size.width/2,0,size.width / 2,size.height));
//
//    SGMainManager::shareMain()->getMainScene()->addChild(rightTexture, 2);
//    SGMainManager::shareMain()->getMainScene()->addChild(leftTexture, 2);
//
//    //this->addChild(leftTexture);
//
//    CCRotateTo* cwAction = CCRotateTo::create(1, 180);
//
//    CCRotateTo* ccwAction = CCRotateTo::create(1, -180);
//    
//    
//    leftTexture->getSprite()->runAction(ccwAction);
//    
//    rightTexture->runAction(cwAction);
//    
    
    SGSliceEffectLayer* effectLayer =  SGSliceEffectLayer::create();
    SGMainManager::shareMain()->getMainScene()->addChild(effectLayer, 2);
    //this->getParent()->addChild(effectLayer,9999);
    effectLayer->play();
    //CCTextureCache::sharedTextureCache()->setLoadingMode((LIM_OPTION)CGP_PLATFORM);
    
    
    this->setVisible(false);

    
    
    CCDelayTime*   delayFi = CCDelayTime::create(1.0);
    CCCallFunc*   callFi = CCCallFunc::create(this, callfunc_selector(CGPBattleLoadingLayer::delayFight));
    this->runAction(CCSequence::create(delayFi, callFi, nullptr));


    
}

void CGPBattleLoadingLayer::delayFight()
{
    SGBattleManager::sharedBattle()->pveListenerDelay();
    
    
}


void CGPBattleLoadingLayer::gotoFightLayer()  //cgp_pro
{

    bool isPVE = SGAIManager::shareManager()->isPVE;
    if (isPVE == true)
    {
        SGAIManager *aiMgr = SGAIManager::shareManager();
        //重新进战斗重置自动战斗所有变量
        aiMgr->setIsCompleteAttack(false);
        aiMgr->setIsCompleteSkill(true);
        aiMgr->setIsOpenAutoBattle(false);
        aiMgr->setIsDispSkillHead(false);
        aiMgr->setAnimationSpeed(1);
        SGBattleManager::sharedBattle()->pveStarts();   //cgp_pro
    }
    else
    {
        SGBattleManager::sharedBattle()->pvpStarts();
    }
}
