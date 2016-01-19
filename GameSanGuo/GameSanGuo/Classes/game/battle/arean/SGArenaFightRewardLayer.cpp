//
//  SGArenaFightRewardLayer.cpp
//  GameSanGuo
//
//  Created by angel on 14-12-26.
//
//

#include "SGArenaFightRewardLayer.h"
#include "ResourceManager.h"
#include "SGTestSwitch.h"
#include "SGMainManager.h"
#include "ArenaModule.pb.h"
#include "SGStringConfig.h"

SGArenaFightRewardLayer::SGArenaFightRewardLayer():m_rewardData(NULL),m_winOrLost(NULL)
{
}
SGArenaFightRewardLayer::~SGArenaFightRewardLayer()
{
     ResourceManager::sharedInstance()->unBindLayerTexture(sg_arenaFightRewardLayer);
}
SGArenaFightRewardLayer * SGArenaFightRewardLayer::create( SGArenaFightRewardData * data)
{
    SGArenaFightRewardLayer * layer = new SGArenaFightRewardLayer();
    if(layer && layer->init(NULL,sg_arenaFightRewardLayer))
    {
        layer->m_rewardData = data;
        layer->initView();
        layer->autorelease();
        return layer;
    }
    CC_SAFE_RELEASE(layer);
    return NULL;
    
}
void SGArenaFightRewardLayer::initView()
{
    ResourceManager * resManager = ResourceManager::sharedInstance();
    resManager->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_arenaFightRewardLayer);
    resManager->bindTexture("sgfightrewardlayer/sgfightrewardlayer.plist", RES_TYPE_LAYER_UI, sg_arenaFightRewardLayer);
    resManager->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_arenaFightRewardLayer);
    resManager->bindTexture("sgpvpfightrewardlayer/sgpvpfightrewardlayer.plist", RES_TYPE_LAYER_UI, sg_arenaFightRewardLayer);
    resManager->bindTexture("updatelayer/updatelayer.plist", RES_TYPE_LAYER_UI, sg_arenaFightRewardLayer);
    resManager->bindTexture("animationFile/ybjs.plist", RES_TYPE_LAYER_UI, sg_arenaFightRewardLayer);
    resManager->bindTexture("sgAreanLayer/sgAreanLayer.plist", RES_TYPE_LAYER_UI, sg_arenaFightRewardLayer);
    
    float headH = H_TOP;
    
    //标题背景
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("fight_over_titlebg.png");
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height*.81f)));//,6
    this->addChild(title_bg, -1);
    
    //标题背景左右 花边 图片在sgcountrylayer.plist中
    CCSprite *title_Lbg = CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_Lbg->setAnchorPoint(ccp(0, 0.5f));
    title_Lbg->setPosition(ccpAdd(title_bg->getPosition(), ccp(-title_bg->getContentSize().width*.5f, 0)));
    this->addChild(title_Lbg, -1);
    CCSprite *title_Rbg =CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_Rbg->setAnchorPoint(ccp(1, 0.5f));
    title_Rbg->setFlipX(true);
    title_Rbg->setPosition(ccpAdd(title_bg->getPosition(), ccp(title_bg->getContentSize().width*.5f, 0)));
    this->addChild(title_Rbg, -1);
    
    //标题
    CCSprite *title = CCSprite::createWithSpriteFrameName("fight_over_title_n.png");
    title->setPosition(title_bg->getPosition());
    this->addChild(title);
    
    //标题下方  一横杆
    CCSprite * stick = CCSprite::createWithSpriteFrameName("boss_title_bar.png");
    stick->setPosition(ccp( title_bg->getPositionX() , title_bg->getPositionY()  - title_bg->getContentSize().height*0.5 - stick->getContentSize().height * 0.6));
    this->addChild(stick , -1);
    
    //奖励内容背景图片kuang 	原始尺寸180*180
    CCScale9Sprite * bg = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    //bg->setPreferredSize(CCSize( winSize.width - 70 , winSize.height -  headH - bottomH - title_bg->getContentSize().height - stick->getContentSize().height - 40 ));
    bg->setPreferredSize(CCSize(590, 460));
    //bg->setPosition(ccp(stick->getPositionX() , stick->getPositionY() - stick->getContentSize().height - bg->getContentSize().height * 0.5) );
    bg->setPosition(SGLayout::getPoint(kMiddleCenter));
    this->addChild(bg , -1);
    
    //框的背景图片
    CCSprite * kuang_bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    kuang_bg->setScaleX(bg->getContentSize().width / kuang_bg->getContentSize().width);
    kuang_bg->setScaleY(bg->getContentSize().height / kuang_bg->getContentSize().height);
    kuang_bg->setPosition(bg->getPosition() );
    kuang_bg->setOpacity(150);
    this->addChild(kuang_bg , -10);
    
    //点击屏幕继续的图片字 图片在 sggeneralslayer
    CCSprite  *m_clickToContinue = CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
    m_clickToContinue->setVisible(true);
    m_clickToContinue->setPosition(ccp(stick->getPositionX() , bg->getPositionY() - bg->getContentSize().height * 0.5 - m_clickToContinue->getContentSize().height - 40) );
    this->addChild(m_clickToContinue);
    
    
    
    //胜利或者失败的图片
    m_winOrLost = NULL;
    if( m_rewardData->getIsWin())
        m_winOrLost = CCSprite::createWithSpriteFrameName("pvpFightRewardWin.png");
    else
        m_winOrLost = CCSprite::createWithSpriteFrameName("pvpFightRewardLost.png");
    m_winOrLost->setPosition(ccp(SGLayout::getPoint(kMiddleCenter).x + 60 + m_winOrLost->getContentSize().width*0.5 ,SGLayout::getPoint(kMiddleCenter).y + 80 ));
    this->addChild(m_winOrLost);
    
    //播放胜利特效
    m_winOrLost->setScale(3);
    m_winOrLost->setOpacity(0);
    CCDelayTime * delayTime = CCDelayTime::create(0.5);
    CCFadeTo * fadeTo = CCFadeTo::create(0.2, 255);
    CCScaleTo * scaleTo = CCScaleTo::create(0.2, 1);
    CCSpawn * spawn = CCSpawn::create(fadeTo , scaleTo , NULL);
    
    CCCallFunc * callFunc = CCCallFunc::create(this, callfunc_selector(SGArenaFightRewardLayer::labelAction ) );
    CCDelayTime * delayTime2 = CCDelayTime::create(0.5);
    CCPlaySound::sharePlaySound()->preloadeffectByType(MUSIC_63);
    CCSequence * sequence = NULL;
    //胜利特效
    if(m_rewardData->getIsWin())
        sequence = CCSequence::create(delayTime,spawn ,
                                      CCCallFuncO::create(this,callfuncO_selector(SGArenaFightRewardLayer::playSoundCallback) ,CCString::create(MUSIC_63)  ),
                                      CCDelayTime::create(0.05) , CCScaleTo::create(0.05, 1.3),CCScaleTo::create(0.05, 1) , delayTime2 , callFunc, NULL );
    else
        sequence = CCSequence::create(delayTime,spawn ,
                                      CCCallFuncO::create(this,callfuncO_selector(SGArenaFightRewardLayer::playSoundCallback) ,CCString::create(MUSIC_67)  ),delayTime2 , callFunc, NULL );
    m_winOrLost->runAction(sequence);
    
    int nBeforeRank = m_rewardData->getBeforeRank();
    int nAfterRank = m_rewardData->getAfterRank();
    int nDeltaRank =  abs(nBeforeRank - nAfterRank);
    
    SGCCLabelTTF * rankLabel = SGCCLabelTTF::create(str_sgareanlayer_str19,FONT_PANGWA , 22);
    rankLabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-180,100)));
    this->addChild(rankLabel);
    
    SGCCLabelTTF * beforRank = SGCCLabelTTF::create(CCString::createWithFormat("%d", m_rewardData->getBeforeRank())->getCString(),FONT_PANGWA, 22);
    beforRank->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-140,100)));
    this->addChild(beforRank);
    
    CCSprite * arrow = CCSprite::createWithSpriteFrameName("arrows_right.png");
    arrow->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-80,100)));
    this->addChild(arrow);
    
    SGCCLabelTTF * afterRank = SGCCLabelTTF::create(CCString::createWithFormat("%d", m_rewardData->getAfterRank())->getCString(),FONT_PANGWA, 22);
    afterRank->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-20,100)));
    this->addChild(afterRank);
    CCSprite * pRankState = CCSprite::createWithSpriteFrameName("equal_win.png");
    pRankState->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-80,30)));
    this->addChild(pRankState);
    
    SGCCLabelTTF * pLabelDeltaRank  = SGCCLabelTTF::create(CCString::createWithFormat("%d", m_rewardData->getAfterRank())->getCString(),FONT_PANGWA, 22);
    pLabelDeltaRank->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-80,30)));
    this->addChild(pLabelDeltaRank);
    
    if(nDeltaRank == 0)
    {
        if(m_rewardData->getIsWin())
        {
            pRankState->setDisplayFrame(CCSprite::createWithSpriteFrameName("equal_win.png")->displayFrame());
            pLabelDeltaRank->setString("");
        }
        else
        {
            pRankState->setDisplayFrame(CCSprite::createWithSpriteFrameName("equal_fail.png")->displayFrame());
            pLabelDeltaRank->setString("");
        }
    }
    else
    {
        if(m_rewardData->getIsWin())
        {
            pRankState->setDisplayFrame(CCSprite::createWithSpriteFrameName("arean_up.png")->displayFrame());
            pLabelDeltaRank->setString(CCString::createWithFormat("%d",nDeltaRank)->getCString());
        }
        else
        {
            pRankState->setDisplayFrame(CCSprite::createWithSpriteFrameName("arean_down.png")->displayFrame());
            pLabelDeltaRank->setString(CCString::createWithFormat("%d",nDeltaRank)->getCString());
        }
        
    }

    
    SGCCLabelTTF * count = SGCCLabelTTF::create(CCString::createWithFormat(str_sgareanlayer_str18,m_rewardData->getCount(),m_rewardData->getMaxCount())->getCString(), FONT_PANGWA, 22);
    count->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,-80)));
    this->addChild(count);
    
     CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    
}
void SGArenaFightRewardLayer::playSoundCallback(CCObject * obj)
{
    CCString * str = (CCString *)obj;
    EFFECT_PLAY(str->getCString());
}
void SGArenaFightRewardLayer::labelAction()
{
}

bool SGArenaFightRewardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}
void SGArenaFightRewardLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    main::ArenaMainInfoRequest * request = new main::ArenaMainInfoRequest();
    
    SGSocketClient::sharedSocketClient()->send(MSG_ARENA_MAIN_INFO, request);
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}