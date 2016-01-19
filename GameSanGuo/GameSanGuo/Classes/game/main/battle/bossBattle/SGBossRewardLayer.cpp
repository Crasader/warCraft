//
//  SGBossRewardLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-28.
//
//

#include "SGBossRewardLayer.h"
#include "SGMainManager.h"
#include "FightBaseModule.pb.h"
#include "SGStringConfig.h"
 SGBossRewardLayer::SGBossRewardLayer()
:iskillBoss(0)
{

}
SGBossRewardLayer::~SGBossRewardLayer(){
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_bossRewardLayer);

}

SGBossRewardLayer *SGBossRewardLayer::create(BossReward *rewardData){
    SGBossRewardLayer *rewardLayer=new SGBossRewardLayer();
    if (rewardLayer && rewardLayer->init(NULL, sg_bossRewardLayer)) {
        rewardLayer->initView(rewardData);
        rewardLayer->iskillBoss=rewardData->isKillBoss;
        rewardLayer->autorelease();
        return rewardLayer;
    }
    CC_SAFE_DELETE(rewardLayer);
    return NULL;
}
void SGBossRewardLayer::initView(BossReward *rewardData){

    CCSize size = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    //CCPoint center = SGLayout::getPoint(kMiddleCenter);
    float headH = SGMainManager::shareMain()->getTotleHdHgt();
    float bottomH =SGMainManager::shareMain()->getBottomHeight();
    
    ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_bossRewardLayer);
    
    ResourceManager::sharedInstance()->bindTexture("sgfightrewardlayer/sgfightrewardlayer.plist", RES_TYPE_LAYER_UI, sg_bossRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist", RES_TYPE_LAYER_UI, sg_bossRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgfindhelplayer/sgfindhelplayer.plist", RES_TYPE_LAYER_UI, sg_bossRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist", RES_TYPE_LAYER_UI, sg_bossRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sgtongquelayer/sgtongquelayer.plist", RES_TYPE_LAYER_UI, sg_bossRewardLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_bossRewardLayer);

    
    
    float discY = size.height-headH-bottomH+65;
    //    CCRect rect = CCRectMake(768*.5f-size.width*.5f, (1136-discY)*.5f, size.width, discY);
    CCSprite *bgImg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect rect = CCRectMake(0, (1136-discY)*.5f, bgImg->getContentSize().width, discY);
    
    bgImg->setTextureRect(rect);
    bgImg->setScaleX(size.width/bgImg->getContentSize().width * 1.01);
    bgImg->setAnchorPoint(ccp(0.5f, 0));
    bgImg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, bottomH)));
    this->addChild(bgImg, -15);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("fight_over_titlebg.png");
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headH + title_bg->getContentSize().height*.81f)));//,6
    this->addChild(title_bg, -1);
    
    CCSprite *title_Lbg = CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_Lbg->setAnchorPoint(ccp(0, 0.5f));
    title_Lbg->setPosition(ccpAdd(title_bg->getPosition(), ccp(-title_bg->getContentSize().width*.5f, 0)));
    this->addChild(title_Lbg, -1);
    CCSprite *title_Rbg =CCSprite::createWithSpriteFrameName("advance_bian.png");
    title_Rbg->setAnchorPoint(ccp(1, 0.5f));
    title_Rbg->setFlipX(true);
    title_Rbg->setPosition(ccpAdd(title_bg->getPosition(), ccp(title_bg->getContentSize().width*.5f, 0)));
    this->addChild(title_Rbg, -1);
    
    
    CCSprite *infoBg = CCSprite::createWithSpriteFrameName("fight_over_bg_n.png");
    infoBg->setPosition(ccp(size.width*.5f, bottomH + discY*.49f-20));//.45
    
    SGCCLabelTTF *titleLabel = SGCCLabelTTF::create(str_BossRewardLayer_str1, FONT_BOXINFO, 34 , COLOR_UNKNOW_TAN);
    titleLabel->setPosition(title_bg->getPosition());
    //modify by:zyc. merge into create.
    //titleLabel->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(titleLabel);
    
    
    this->addChild(infoBg, -2,1000);
    infoBg->setScaleY(1.1);//1.2
        
        
    float posX = 30.0f;
    float posY = infoBg->getContentSize().height - 75+size.height/3;
    
    if (size.height==1136)
        posY = infoBg->getContentSize().height - 82+size.height/3;
    else if(size.height==960)
        posY = infoBg->getContentSize().height - 105+size.height/3;
    else
        {
            posY = infoBg->getContentSize().height - 100+size.height/3;
            posX=90;
        }
    const char* fontName = FONT_BOXINFO;
    float fontSize = 28;
        
        
    for (int ii = 1; ii <= 4; ii++) {
        if ( ii==2 || ii==3) continue;
        CCSprite *line = CCSprite::createWithSpriteFrameName("uplevel_fontline_n.png");
        line->setPosition(ccp(infoBg->getContentSize().width*.5f, posY-17 - ii*47));
        this->addChild(line);
    }
    
//
    
    CCString *str=CCString::createWithFormat(str_BossRewardLayer_str2,rewardData->damage);
    
    SGCCLabelTTF *labelTTF = SGCCLabelTTF::create(str->getCString(), fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccp(posX+30, posY-20));
    this->addChild(labelTTF);
    
    
    labelTTF = SGCCLabelTTF::create(str_BossRewardLayer_str3, fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccp(posX, posY-47*2-50));
    this->addChild(labelTTF);
  
    
    str= CCString::createWithFormat("%d",rewardData->coinNumber);
    SGCCLabelTTF *labelCoin = SGCCLabelTTF::create(str->getCString(), FONT_BOXINFO, 24);
    labelCoin->setAnchorPoint(ccp(0, 0.5f));
    labelCoin->setPosition(ccpAdd(labelTTF->getPosition(), ccp(labelTTF->getContentSize().width+40, 0)));
    this->addChild(labelCoin);
    
    CCSprite *coincon = CCSprite::createWithSpriteFrameName("coinicon.png");
    float iconPx = labelTTF->getContentSize().width - coincon->getContentSize().width*1.0f;
    coincon->setPosition(ccpAdd(labelTTF->getPosition(), ccp(iconPx+50, 0)));
    this->addChild(coincon);
    
    labelTTF = SGCCLabelTTF::create(str_BossRewardLayer_str4, fontName, fontSize);
    labelTTF->setAnchorPoint(ccp(0, 0.5f));
    labelTTF->setPosition(ccpAdd(ccp(posX, posY-47*2), ccp(320, -50)));
    this->addChild(labelTTF);
    //    int currGolds = playInfo->getPlayerGold();
    //    if (!isLevelUp) playInfo->setPlayerGold((currGolds+reward->getGold()));
    
    str= CCString::createWithFormat("%d",rewardData->MedalNumber);
    SGCCLabelTTF  *labelGold = SGCCLabelTTF::create(str->getCString(), FONT_BOXINFO, 24);
    labelGold->setAnchorPoint(ccp(0, 0.5f));
    labelGold->setPosition(ccpAdd(labelTTF->getPosition(), ccp(labelTTF->getContentSize().width+50, 0)));
    this->addChild(labelGold);

    
    
    CCSprite *goldicon =  CCSprite::createWithSpriteFrameName("tongqueXunLittle.png");
    goldicon->setPosition(ccpAdd(labelTTF->getPosition(), ccp(iconPx+50, 0)));
    this->addChild(goldicon);
    
    
        //"点击屏幕继续"
        CCSprite *conSprite =  CCSprite::createWithSpriteFrameName("uplevel_font_con.png");
        int  tpx = infoBg->getPositionX();
        int   tpy = bottomH + (infoBg->getPositionY() - infoBg->getContentSize().height*.5f-bottomH)*.5f;
        conSprite->setPosition(ccp(tpx, tpy-20));//-35
        conSprite->setVisible(true);
        this->addChild(conSprite);
    
        conSprite->runAction(CCRepeatForever::create(CCBlink::create(1.0f, 2)));
    this->setTouchEnabled(true);
}

void SGBossRewardLayer::registerWithTouchDispatcher()
{
    //    if (SGBattleManager::sharedBattle()->getBattleType() == BT_SURVIVAL && SGBattleManager::sharedBattle()->isWin)
    //        return;
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, false);//11.17
}

bool SGBossRewardLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    
    return true;
    
}
void SGBossRewardLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent){
    
    
}
void SGBossRewardLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
////    SGMainManager::shareMain()->showBossBattleLayer(2, true);
//    main::EnterBossRequest *bossFight = new main::EnterBossRequest();
//	
//	SGSocketClient::sharedSocketClient()->send(MSG_ENTER_BOSS, bossFight);
    
    showBattleDetailLayer();
}
void SGBossRewardLayer::showBattleDetailLayer(){
    
    if(!iskillBoss)
    {
        main::EnterBossRequest *bossFight = new main::EnterBossRequest();
	
        SGSocketClient::sharedSocketClient()->send(MSG_ENTER_BOSS, bossFight);
    }else
    {
        main::MainBossRequest *request=new main::MainBossRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_BOSSBATTLE_JOIN, request);
    }
        
}