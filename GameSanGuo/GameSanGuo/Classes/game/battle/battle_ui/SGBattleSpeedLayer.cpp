//
//  SGBattleSpeedLayer.cpp
//  GameSanGuo
//
//  Created by 江南岸 on 13-4-19.
//
//

#include "SGBattleSpeedLayer.h"
#include "SGBattleManager.h"
#include "SGAIManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#define SPEEDRATE 0.8
SGBattleSpeedLayer::SGBattleSpeedLayer():m_enemySpeed(0)
,m_ourSpeed(0)
,m_bFirst(false)
,m_tick(0)
,m_pEnemyLabel (NULL)
,m_pOurLabel (NULL)
,m_pFirstSprite (NULL)
,m_pGoldenLineUp_1 (NULL)
,m_pGoldenLineUp_2 (NULL)
,m_pGoldenLineDown_1 (NULL)
,m_pGoldenLineDown_2 (NULL)
,m_pEnemyBGSprite (NULL)
,m_pOurBGSprite (NULL)
{

}

SGBattleSpeedLayer::~SGBattleSpeedLayer()
{
    CCLOG("~SGBattleSpeedLayer()");
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_battleSpeedLayer);
}

SGBattleSpeedLayer* SGBattleSpeedLayer::create(int ourSpeed,int enemySpeed,bool first)
{
    SGBattleSpeedLayer *speedLayer = new SGBattleSpeedLayer();
    if(speedLayer && speedLayer->init(NULL, sg_battleSpeedLayer))
    {
        speedLayer->m_ourSpeed = ourSpeed;
        speedLayer->m_enemySpeed = enemySpeed;
        speedLayer->m_bFirst = first;
        speedLayer->viewDidLoad();
        speedLayer->autorelease();
        return speedLayer;
    }
    
    CC_SAFE_DELETE(speedLayer);
    return NULL;
    
}

void SGBattleSpeedLayer::viewDidLoad()
{
    //获取中心点坐标
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_BATTLE_IMAGE, sg_battleSpeedLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemap/sgbattlemap.plist", RES_TYPE_BATTLE_IMAGE, sg_battleSpeedLayer);
    
    //若是守荆州则加载波次资源
    SGBattleManager* sgbm = SGBattleManager::sharedBattle();
    if (BT_SURVIVAL == sgbm->getBattleType())
    if (true)
    {
//        int cur = 79;
        int cur = sgbm->getSjzLevel();
        CCString* str = CCString::createWithFormat("%d", cur);
        //第X波
        CCSprite* xxbo = CCSprite::createWithSpriteFrameName("sjz_bat_which.png");
        xxbo->setPosition(center);
        addChild(xxbo);
        //数字
        CCLabelAtlas *label = CCLabelAtlas::create(str->getCString(), "sgbattlemap/sjz_bat_number.png", 95, 95, '0');
        label->setAnchorPoint(ccp(0.6,0.5));
        label->setPosition(center);
        addChild(label);
        //动作
        float timeIn = 0.3;
        float timeSh = 2;
        float timeOut = 0.3;
        
        CCFadeIn* cfi1 = CCFadeIn::create(timeIn);
        CCDelayTime* cdt1 = CCDelayTime::create(timeSh);
        CCFadeOut* cfo1 = CCFadeOut::create(timeOut);
        CCSequence* csq1 = CCSequence::create(cfi1, cdt1, cfo1, NULL);
        xxbo->runAction(csq1);
        
        CCFadeIn* cfi2 = CCFadeIn::create(timeIn);
        CCDelayTime* cdt2 = CCDelayTime::create(timeSh);
        CCFadeOut* cfo2 = CCFadeOut::create(timeOut);
        CCSequence* csq2 = CCSequence::create(cfi2, cdt2, cfo2, NULL);
        label->runAction(csq2);
    }
    
    
    //两条黑框
    m_pEnemyBGSprite = CCSprite::createWithSpriteFrameName("public_blackBoard.png");
    m_pOurBGSprite = CCSprite::createWithSpriteFrameName("public_blackBoard.png");
    m_pEnemyBGSprite->setPosition(ccpAdd(center, ccp(0, 200)));
    m_pOurBGSprite->setPosition(ccpAdd(center, ccp(0, -200)));
    m_pEnemyBGSprite->setVisible(false);
    m_pOurBGSprite->setVisible(false);
    addChild(m_pEnemyBGSprite);
    addChild(m_pOurBGSprite);
    
    
    //四条金线
    m_pGoldenLineDown_1 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    m_pGoldenLineDown_2 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    m_pGoldenLineUp_1 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    m_pGoldenLineUp_2 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    m_pGoldenLineUp_1->setScaleX(SGLayout::getSkewing(320)/m_pGoldenLineUp_1->getContentSize().width);
    m_pGoldenLineUp_2->setScaleX(SGLayout::getSkewing(320)/m_pGoldenLineUp_2->getContentSize().width);
    m_pGoldenLineDown_1->setScaleX(SGLayout::getSkewing(320)/m_pGoldenLineDown_1->getContentSize().width);
    m_pGoldenLineDown_2->setScaleX(SGLayout::getSkewing(320)/m_pGoldenLineDown_2->getContentSize().width);
    m_pGoldenLineUp_1->setPosition(ccpAdd(center, ccp(-SGLayout::getSkewing(160), 200)));
    m_pGoldenLineUp_2->setPosition(m_pGoldenLineUp_1->getPosition());
    m_pGoldenLineDown_1->setPosition(ccpAdd(center, ccp(SGLayout::getSkewing(480), -200)));
    m_pGoldenLineDown_2->setPosition(m_pGoldenLineDown_1->getPosition());
    addChild(m_pGoldenLineUp_1,2);
    addChild(m_pGoldenLineUp_2,2);
    addChild(m_pGoldenLineDown_1,2);
    addChild(m_pGoldenLineDown_2,2);
    

    

    //先手
    m_pFirstSprite = CCSprite::createWithSpriteFrameName("battle_first_action.png");
    if(m_bFirst)
    {//我方
        m_pFirstSprite->setPosition(ccpAdd(m_pOurBGSprite->getPosition(), ccp(-200, 0)));
    }else{
        SGAIManager::shareManager()->isAi=true;
        m_pFirstSprite->setPosition(ccpAdd(m_pEnemyBGSprite->getPosition(), ccp(-200, 0)));
    }
    m_pFirstSprite->setVisible(false);
    addChild(m_pFirstSprite,128);
    
    //地方速度，我方速度
    m_pEnemyLabel = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_The_speed,m_enemySpeed)->getCString(), FONT_BOXINFO, 28);
    m_pOurLabel = SGCCLabelTTF::create(CCString::createWithFormat(str_Format_The_speed,m_ourSpeed)->getCString(), FONT_BOXINFO, 28);
    m_pEnemyLabel->setPosition(m_pEnemyBGSprite->getPosition());
    m_pOurLabel->setPosition(m_pOurBGSprite->getPosition());
    m_pOurLabel->setVisible(false);
    m_pEnemyLabel->setVisible(false);
    addChild(m_pEnemyLabel);
    addChild(m_pOurLabel);
    
    //展开动画
    CCMoveTo *upMoeve_1 = CCMoveTo::create(0.3*SPEEDRATE, ccp(center.x, center.y+200));
    CCMoveTo *upMoeve_2 = CCMoveTo::create(0.3*SPEEDRATE, ccp(center.x, center.y+200));
    CCMoveTo *downMove_1 = CCMoveTo::create(0.3*SPEEDRATE, ccp(center.x, center.y-200));
    CCMoveTo *downMove_2 = CCMoveTo::create(0.3*SPEEDRATE, ccp(center.x, center.y-200));

    m_pGoldenLineUp_1->runAction(upMoeve_1);
    m_pGoldenLineUp_2->runAction(upMoeve_2);
    m_pGoldenLineDown_1->runAction(downMove_1);
    m_pGoldenLineDown_2->runAction(downMove_2);
    
    CCDelayTime *delay_1 = CCDelayTime::create(1.0*SPEEDRATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleSpeedLayer::showGoldenLine));
    CCFiniteTimeAction *sequence = CCSequence::create(delay_1,call,NULL);
    this->runAction(sequence);
    
    
    
//    
    CCMoveTo *ourMove = CCMoveTo::create(1.0, ccp(center.x, center.y-200));
    CCMoveTo *enemyMove = CCMoveTo::create(1.0, ccp(center.x, center.y+200));
    CCEaseInOut *ourInOut = CCEaseInOut::create(ourMove, 1.0);
    CCEaseInOut *enemyInOut = CCEaseInOut::create(enemyMove, 1.0);
    m_pEnemyBGSprite->runAction(enemyInOut);
    m_pOurBGSprite->runAction(ourInOut);
    
//    CCDelayTime *delayTime = CCDelayTime::create(1.0);
//    CCCallFunc *call1 = CCCallFunc::create(this, callfunc_selector(SGBattleSpeedLayer::startTimer));
//    CCFiniteTimeAction *sequence1 = CCSequence::create(delayTime,call1,NULL);
//    this->ruxnAction(sequence1);
//     
    
    
}

void SGBattleSpeedLayer::showGoldenLine()
{
    //获取中心点坐标
    CCPoint center = SGLayout::getPoint(kMiddleCenter);

    CCMoveTo *upMove_1 = CCMoveTo::create(0.5*SPEEDRATE, ccp(center.x, center.y+229));
    CCMoveTo *upMove_2 = CCMoveTo::create(0.5*SPEEDRATE, ccp(center.x, center.y+200-29));
    CCMoveTo *downMove_1 = CCMoveTo::create(0.5*SPEEDRATE, ccp(center.x, center.y-200+28));
    CCMoveTo *downMove_2 = CCMoveTo::create(0.5*SPEEDRATE, ccp(center.x,center.y-200-28));
    m_pGoldenLineUp_1->runAction(upMove_1);
    m_pGoldenLineUp_2->runAction(upMove_2);
    m_pGoldenLineDown_1->runAction(downMove_1);
    m_pGoldenLineDown_2->runAction(downMove_2);
    
    CCDelayTime *delay = CCDelayTime::create(0.5*SPEEDRATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleSpeedLayer::showBlackBG));
    CCFiniteTimeAction *sequence = CCSequence::create(delay,call,NULL);
    this->runAction(sequence);

}

void SGBattleSpeedLayer::showBlackBG()
{


//    CCFadeIn *upIn1 = CCFadeIn::create(1.0);
//    CCFadeIn *downIn1 = CCFadeIn::create(1.0);
//    m_pEnemyBGSprite->runAction(upIn1);
//    m_pOurBGSprite->runAction(downIn1);
    
    m_pEnemyBGSprite->setVisible(true);
    m_pOurBGSprite->setVisible(true);
    
    m_pOurBGSprite->setOpacity(127);
    m_pEnemyBGSprite->setOpacity(127);
    
    CCDelayTime *delay1 = CCDelayTime::create(0.8f*SPEEDRATE);
    CCCallFunc *call1 = CCCallFunc::create(this, callfunc_selector(SGBattleSpeedLayer::showSpeedLabel));
    CCDelayTime *delay = CCDelayTime::create(0.4f*SPEEDRATE);
    CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleSpeedLayer::updateSpeedCounter));
    CCFiniteTimeAction *seqence = CCSequence::create(delay1,call1,delay,call,NULL);
    this->runAction(seqence);
}

void SGBattleSpeedLayer::startTimer()
{
    m_pEnemyLabel->setVisible(true);
    m_pOurLabel->setVisible(true);
    m_pEnemyLabel->setPosition(m_pEnemyBGSprite->getPosition());
    m_pOurLabel->setPosition(m_pOurBGSprite->getPosition());
    
    this->schedule(schedule_selector(SGBattleSpeedLayer::updateSpeedCounter), 0);
}

void SGBattleSpeedLayer::updateSpeedCounter()
{
    /*
    
    m_tick += MAX(m_enemySpeed, m_ourSpeed)/60;
    
    int ourSpeed = 0;
    if(m_tick <= m_ourSpeed)
    {
        ourSpeed = m_tick;
    }else{
        ourSpeed = m_ourSpeed;
    }
    m_pOurLabel->setString(CCString::createWithFormat("队伍速度 %d",ourSpeed)->getCString());

    int enemySpeed = 0;
    if(m_tick <= m_enemySpeed)
    {
        enemySpeed = m_tick;
    }else{
        enemySpeed = m_enemySpeed;
    }
    m_pEnemyLabel->setString(CCString::createWithFormat("队伍速度 %d",enemySpeed)->getCString());

    
    //计数结束
    if(m_tick > m_ourSpeed && m_tick > m_enemySpeed)
    {
        this->unschedule(schedule_selector(SGBattleSpeedLayer::updateSpeedCounter));
        
        if(m_ourSpeed > m_enemySpeed)
        {
            m_pFirstSprite->setPosition(ccpAdd(m_pOurLabel->getPosition(), ccp(-m_pOurLabel->getContentSize().width, 0)));
        }else if(m_ourSpeed < m_enemySpeed){
            m_pFirstSprite->setPosition(ccpAdd(m_pEnemyLabel->getPosition(), ccp(-m_pEnemyLabel->getContentSize().width, 0)));
        }else
        {
            if(m_bFirst)
            {//我方
                m_pFirstSprite->setPosition(ccpAdd(m_pOurLabel->getPosition(), ccp(-m_pOurLabel->getContentSize().width, 0)));
            }else{
                //敌方
                m_pFirstSprite->setPosition(ccpAdd(m_pEnemyLabel->getPosition(), ccp(-m_pEnemyLabel->getContentSize().width, 0)));
            }
        }
        
     
     */

        //盖印章效果
    
        m_pFirstSprite->setVisible(true);
        m_pFirstSprite->setScale(8.0);
        CCFadeIn *fadeIn = CCFadeIn::create(0.2*SPEEDRATE);
        CCScaleTo *scale = CCScaleTo::create(0.3*SPEEDRATE, 1.0);
        CCFiniteTimeAction *spawn = CCSpawn::create(fadeIn,scale,NULL);
        CCScaleTo *scale_2 = CCScaleTo::create(0.1*SPEEDRATE, 1.2f);
        CCScaleTo *scale_3 = CCScaleTo::create(0.1*SPEEDRATE, 1.0f);
        CCDelayTime *delay = CCDelayTime::create(1.2*SPEEDRATE);
        CCCallFunc *call = CCCallFunc::create(this, callfunc_selector(SGBattleSpeedLayer::removeSelf));
        CCFiniteTimeAction *sequence = CCSequence::create(spawn,scale_2,scale_3,delay,call,NULL);
        m_pFirstSprite->runAction(sequence);
        

}


void SGBattleSpeedLayer::showSpeedLabel()
{
    m_pEnemyLabel->setVisible(true);
    m_pOurLabel->setVisible(true);
}

void SGBattleSpeedLayer::removeSelf()
{
   
    bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    if (round) {
        SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(round);
//        hero->setRoundTime(20);
        hero->beginRoundTime();
    }else{
        if (SGAIManager::shareManager()->isPVE) {
            ;
            CCCallFunc *fun=CCCallFunc::create(this, callfunc_selector(SGBattleSpeedLayer::beginAI));
            CCDelayTime *delay=CCDelayTime::create(3*SPEEDRATE);
            this->runAction(CCSequence::create(delay,fun,NULL));
           //  CCLog("**********速度比较:SGBattleSpeedLayer::RemoveSelf**********");
//            SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(round);
//            SGAIManager::shareManager()->startRound(hero->battleMap);
        }
    }
    
    
    SGBattleManager::sharedBattle()->openReceive();
    
    removeFromParentAndCleanup(true);
}
#pragma mark - beginAI----
void SGBattleSpeedLayer::beginAI(){
    bool round = SGBattleManager::sharedBattle()->getBattleLayer()->getIsMyRound();
    SGHeroLayer *hero = SGBattleManager::sharedBattle()->getHeroLayer(round);
  // SGAIManager::shareManager()->startRound(hero->battleMap);
}
                                         
#pragma mark - ----- CCLayer -----
void SGBattleSpeedLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, true);
}

void SGBattleSpeedLayer::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

bool SGBattleSpeedLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    //屏蔽点击事件
    CCLOG("SGBattleSpeedLayer 屏蔽点击事件");
    return true;
}
