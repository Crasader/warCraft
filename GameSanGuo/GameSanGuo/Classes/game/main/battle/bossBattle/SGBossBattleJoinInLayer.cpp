//
//  SGBossBattleJoinInLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-26.
//
//

#include "SGBossBattleJoinInLayer.h"
#include "SGMainManager.h"
#include "FightBaseModule.pb.h"
#include "SGBossBattleDetailLayer.h"
#include "SGStringConfig.h"
#include "SGCantAdvanceBox.h"

using namespace cocos2d;

SGBossBattleJoinInLayer::SGBossBattleJoinInLayer():
bossLevel(110),
startMin(5),
canJoinbattle(0),
leftbattletimelabel(NULL),
joinBattle(NULL),
battletimelabel(NULL),
formerlabel(NULL),
despritionlabel(NULL),
lastbattle(NULL),
despriteion(NULL),
formName(NULL)
{

    formName=CCString::create(str_BossBattleJoinInLayer_str1);
    despriteion=CCString::create(str_BossBattleJoinInLayer_str2);
}
SGBossBattleJoinInLayer:: ~SGBossBattleJoinInLayer(){
    
//    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_ENTER_BOSS);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_bossBattleJoinInLayer);
}
SGBossBattleJoinInLayer *SGBossBattleJoinInLayer::create(BossBattleJoinData *data){

    
    
    SGBossBattleJoinInLayer *JoinInlayer=new SGBossBattleJoinInLayer();
     JoinInlayer->canJoinbattle=data->canJoin;
    JoinInlayer->setbossLevel(data->bossLev);
    JoinInlayer->startMin=data->startMin;
    JoinInlayer->despriteion=data->description;
    JoinInlayer->formName=data->formName;
    if (JoinInlayer && JoinInlayer->init(NULL, sg_bossBattleJoinInLayer)){
        JoinInlayer->autorelease();
       
        return JoinInlayer;
        
    }
    CC_SAFE_DELETE(JoinInlayer);
    return NULL;
}

void SGBossBattleJoinInLayer::initMsg()
{
 
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_bossBattleJoinInLayer);
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/boss_fight_bg.plist", RES_TYPE_LAYER_UI, sg_bossBattleJoinInLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("boss_fight_bg.png");
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    bg->setScaleY((s.height-b-30)/bg->getContentSize().height);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b-10)));
    
    
    CCSprite *up_title = CCSprite::createWithSpriteFrameName("boss_title_bar.png");
    up_title->setScaleX(s.width/bg->getContentSize().width);
   
    this->addChild(up_title,0);
    up_title->setAnchorPoint(ccp(0.5, 0));
    up_title->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,-65)));
    up_title->setFlipY(true);
    CCSprite *bottom_title = CCSprite::createWithSpriteFrameName("boss_title_bar.png");
    bottom_title->setScaleX(s.width/bg->getContentSize().width);
    
    this->addChild(bottom_title,-5);
    bottom_title->setAnchorPoint(ccp(0.5, 0));
    bottom_title->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,b-15)));
    
     float scaleY=(s.height-b-47)/20;
    
    CCSprite *center_title = CCSprite::createWithSpriteFrameName("boss_title.png");
    center_title->setScaleX(s.width/bg->getContentSize().width);
    
    this->addChild(center_title,-5);
    center_title->setAnchorPoint(ccp(0.5, 0));
    center_title->setPosition(ccpAdd( SGLayout::getPoint(kMiddleCenter), ccp(0,-scaleY)));
    
    
    CCSprite *level_sprite = CCSprite::createWithSpriteFrameName("boss_level.png");
    level_sprite->setScaleX(s.width/bg->getContentSize().width);
    
    this->addChild(level_sprite,-5);
    level_sprite->setAnchorPoint(ccp(0.5, 0));
    level_sprite->setPosition(ccpAdd( SGLayout::getPoint(kMiddleCenter), ccp(0,-scaleY*3)));
    
    SGCCLabelTTF *bossLevellabel=SGCCLabelTTF::create(CCString::createWithFormat("%d",bossLevel)->getCString(),FONT_XINGKAI , 27 , ccRED ,ccWHITE);
    addChild(bossLevellabel);
    bossLevellabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(level_sprite->getContentSize().width *0.17, -scaleY*3+level_sprite->getContentSize().height/2)));
   
    
    SGCCLabelTTF *explainlabel=SGCCLabelTTF::create(str_BossBattleJoinInLayer_str3,FONT_BOXINFO , 27 , ccYELLOW , ccBLACK);
    addChild(explainlabel);
    explainlabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-screenwidth*0.04, scaleY* 7)));
    SGButton *indiana=SGButton::createFromLocal("grap_precious.png", "", this,  menu_selector(SGBossBattleJoinInLayer::showgrapPreciousLayer));
    this->addBtn(indiana);
    indiana->setPosition(ccpAdd(ccp(10, 0), ccp(indiana->getContentSize().width/2, b+indiana->getContentSize().height)));
    joinBattle=SGButton::createFromLocal("join_battle.png", "", this,  menu_selector(SGBossBattleJoinInLayer::showBattleDetailLayer));
    this->addBtn(joinBattle);
    joinBattle->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -scaleY*5)));
    CCSprite *spriteEffect = CCSprite::createWithSpriteFrameName("xuanwo.png");
    if (spriteEffect)
    {
        spriteEffect->setAnchorPoint(ccp(0.5,0.5));
        spriteEffect->ignoreAnchorPointForPosition(false);
        spriteEffect->setPosition(ccp(joinBattle->getContentSize().width/2,joinBattle->getContentSize().height/2));
        spriteEffect->runAction(CCRepeatForever::create(CCRotateBy::create(10, -360)));
        joinBattle->addChild(spriteEffect,-1000);
    }
    
    battletimelabel=SGCCLabelTTF::create(str_BossBattleJoinInLayer_str4,FONT_BOXINFO , 40 , ccRED , ccBLACK);
    addChild(battletimelabel);
    battletimelabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -scaleY*4)));
    
    leftbattletimelabel=SGCCLabelTTF::create(CCString::createWithFormat("%s",GameConfig::getStringByTime(startMin))->getCString(),"宋体" , 30 , ccRED , ccBLACK);
    addChild(leftbattletimelabel);
    leftbattletimelabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -scaleY*5.5)));
    
    formerlabel=SGCCLabelTTF::create(formName->getCString(), "" , 20 , ccWHITE);
    addChild(formerlabel);
    formerlabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -scaleY*7)));
    
    despritionlabel=SGCCLabelTTF::create(getdescription()->getCString(), "" , 20 , ccWHITE);
    addChild(despritionlabel);
    
    despritionlabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -scaleY*8)));
    
    
    lastbattle=SGButton::createFromLocal("last_fight.png", "", this,  menu_selector(SGBossBattleJoinInLayer::showLastBattleSituation));
    this->addBtn(lastbattle);
    lastbattle->setPosition(ccpAdd(ccp(0, 0), ccp(s.width-lastbattle->getContentSize().width/2, b+lastbattle->getContentSize().height)));
    
    changeState(canJoinbattle);
    
    
    schedule(schedule_selector(SGBossBattleJoinInLayer::update), 1.0);
    
    EFFECT_PLAY(MUSIC_ITEM);
    
}



void SGBossBattleJoinInLayer::showBattleDetailLayer()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//    int count = player->getOfficerCards()->count() + player->getEquipCards()->count() + player->getPropCards()->count() +player->getSoldierCards()->count();
    int count = player->getGoodsNumInBag();
    if (count >= player->getPlayerBagSize())
    {
        SGCantAdvanceBox *cantadvanceBox = SGCantAdvanceBox::create(SGMainManager::shareMain()->getMainLayer(), NULL, 12, count);
        SGMainManager::shareMain()->showBox(cantadvanceBox);
        
        return ;
    }
	//进入参战界面,发送消息
	main::EnterBossRequest *bossFight = new main::EnterBossRequest();
	
	SGSocketClient::sharedSocketClient()->send(MSG_ENTER_BOSS, bossFight);
}
void SGBossBattleJoinInLayer:: showLastBattleSituation(){
	
	
	// 如果boss战开启的话,就直接再次发送请求进入boss战的消息
	if (SGPlayerInfo::sharePlayerInfo()->getisBossBattleTime())
	{
		SG_SHOW_WINDOW(str_BossBattleJoinInLayer_str5);
		main::EnterBossRequest *req = new main::EnterBossRequest();
		SGSocketClient::sharedSocketClient()->send(MSG_ENTER_BOSS, req);
	}
	else//如果没有开启boss战,那么就直接进入boss战上次排行
	{
		SGMainManager::shareMain()->sendGetLastBossRankList();
	}
	
}

void SGBossBattleJoinInLayer::showgrapPreciousLayer(){
    
    SGMainManager::shareMain()->getMainLayer()->entryTongQueShow();
}

void SGBossBattleJoinInLayer::update(float dt){
    if (canJoinbattle==1) {
        startMin++;
        leftbattletimelabel->setString(CCString::createWithFormat(str_BossBattleJoinInLayer_str6,(int)startMin/60)->getCString());
    }else if(canJoinbattle==0)
    {
        startMin--;
        leftbattletimelabel->setString(GameConfig::getStringByTime(startMin));
    }
    
    if (startMin<=0) {
        canJoinbattle=1;
        changeState(1);
    }
}
void SGBossBattleJoinInLayer::changeState(int canJoin){
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
     float scaleY=(s.height-b-47)/20;
//         leftbattletimelabel->setVisible(!canJoin);
    
   
        joinBattle->setVisible(canJoin);
        battletimelabel->setVisible(!canJoin);
         formerlabel->setVisible(!canJoin);
        despritionlabel->setVisible(!canJoin);
         lastbattle->setVisible(!canJoin);
    
    if (canJoin==1) {
        leftbattletimelabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -scaleY*7.4)));
        leftbattletimelabel->setString(CCString::createWithFormat(str_BossBattleJoinInLayer_str6,startMin/60)->getCString());
        leftbattletimelabel->setVisible(true);
// 
        
    }else if(canJoin==0)
    {
        leftbattletimelabel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -scaleY*5)));
        leftbattletimelabel->setString(GameConfig::getStringByTime(startMin));
    }
    

}
