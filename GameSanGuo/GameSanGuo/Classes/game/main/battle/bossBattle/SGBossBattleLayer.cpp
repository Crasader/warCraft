//
//  SGBossBattleLayer.cpp
//  GameSanGuo
//
//  Created by 赤霄 on 14-2-26.
//
//

#include "SGBossBattleLayer.h"
#include "CCSpriterX.h"

#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGBossRewardLayer.h"
#include "SGStaticDataManager.h"
#include "SGSoldier.h"
#include "SGStringConfig.h"
#define         SB_ADD_AND_SUB_AP_W 22
#define         SB_ADD_AND_SUB_AP_H 27


SGBossBattleLayer::SGBossBattleLayer():
m_bossColor(1),
m_LordSid(1001),
m_LordColor(1),
m_lordType(2),
m_bossSid(1004),
m_countryId(3103),
isCrit(false),
rewardData(NULL),//光效坐标
bossPos(ccp(0, 0)),
lordPos(ccp(0, 0)),
soldiers(NULL),
bossSprite(NULL),
lordSprite(NULL),
lordSkillId(0),
bossSkillId(0),
bossBaseHp(0),
bossHp(0),
m_lordDamage(0)
{
    rewardData = NULL;
}
 SGBossBattleLayer::~SGBossBattleLayer(){

     CC_SAFE_DELETE(rewardData);
     CC_SAFE_RELEASE(soldiers);
      MUSIC_VOLUME(0.5);
     ResourceManager::sharedInstance()->unBindLayerTexture(sg_bossBattleLayer);
}

SGBossBattleLayer * SGBossBattleLayer::create(BossBattleData *bossData,BossReward *bossReward)
{
    SGBossBattleLayer *layer=new SGBossBattleLayer();
     layer->rewardData= bossReward;
//     layer->rewardData->retain();
    layer->setLordSid(bossData->lordItemId);
    layer->setLordType(bossData->lordType);
    layer->setLordDamge(bossData->lorddamge);
    layer->setbossHp(bossData->bossHp+10);
    layer->setbossBaseHp(bossData->bossBaseHp+10);
    layer->setisCrit(bossData->isCrit);
    layer->setCountryId(SGPlayerInfo::sharePlayerInfo()->getPlayerCountry());
    if (layer && layer->init(NULL, sg_bossBattleLayer))
    {
       
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

void SGBossBattleLayer::initMsg()
{
    
    MUSIC_VOLUME(0.2);
    
    ResourceManager::sharedInstance()->bindTexture("sgherolayer/sgherolayer.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/starSky.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);

    soldiers=CCArray::create();
    soldiers->retain();
    CCSprite *spriteBG = NULL;
    if (NULL == spriteBG) {
        spriteBG = CCSprite::createWithSpriteFrameName("starSky.png");
    }
    spriteBG->setScaleX(CCDirector::sharedDirector()->getWinSize().width/spriteBG->getContentSize().width * 1.01);
    spriteBG->setScaleY(CCDirector::sharedDirector()->getWinSize().height / spriteBG->getContentSize().height);
    spriteBG->setPosition(SGLayout::getPoint(kMiddleCenter));
    addChild(spriteBG, -5);
    
    SGButton *skipItem =SGButton::createFromLocal("round_Time_BJ.png", str_jump_out, this, menu_selector(SGBossBattleLayer::skipBattle));
    skipItem->setTag(121);
    skipItem->setPosition(ccp( screenwidth - skipItem->getContentSize().width/2, skipItem->getContentSize().height/2));
    addBtn(skipItem);
    
    
    soldierPos[0]=ccp(screenwidth/10,screenheight/3+50);
    soldierPos[1]=ccp(screenwidth/10 *4,screenheight*0.10);
    soldierPos[2]=ccp(screenwidth/10 *6,screenheight*0.10);
    soldierPos[3]=ccp(screenwidth/10 *9,screenheight/3+50);
    
    initView();
}
void SGBossBattleLayer::initView()
{
    //boss 位置
    ResourceManager::sharedInstance()->bindTexture("sgbossbattlelayer/bossBullet.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbossbattlelayer/bossTarget1.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbossbattlelayer/bossTarget2.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbossbattlelayer/vortex.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbossbattlelayer/bossEffect1.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbossbattlelayer/bossEffect2.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);

    ResourceManager::sharedInstance()->bindTexture("animationFile/jian_103_bz.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/jian_103.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/szgszg.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
	 
    
    SGMainManager::shareMain()->addOfficerPng(m_bossSid, sg_bossBattleLayer);
    
    
    bossSprite=CCSpriterX::create("sgbossbattlelayer/bossEffect.scml",true,true);
    addChild(bossSprite);
    bossSprite->setPosition(ccp( screenwidth/2, screenheight * 0.8-screenheight*0.42));
    bossSprite->play();
    bossSprite->setanimaID(0);
    bossSprite->setisloop(true);
    bossSprite->setPosition(ccp( screenwidth/2, screenheight * 0.6));
    bossPos=bossSprite->getPosition();
    
    bossSprite->setScale(0.9);
   
    SGOfficerDataModel *officer=SGStaticDataManager::shareStatic()->getOfficerById(m_LordSid);
    SGMainManager::shareMain()->addOfficerPng(officer->getIconId(), sg_bossBattleLayer);
    
    SGOfficerDataModel *data = SGStaticDataManager::shareStatic()->getOfficerById(getLordSid());
    
    CCString  *str = CCString::createWithFormat("animationFile/%d_%d.plist",data->getOfficerFileId(), 1);
    ResourceManager::sharedInstance()->bindTexture(str->getCString(), RES_TYPE_BATTLE_IMAGE, sg_bossBattleLayer);
    
    str = CCString::createWithFormat("animationFile/%d_eff.plist",data->getOfficerFileId());
    ResourceManager::sharedInstance()->bindTexture(str->getCString(), RES_TYPE_BATTLE_IMAGE, sg_bossBattleLayer);
    
    str = CCString::createWithFormat("animationFile/%d_%d.scml",data->getOfficerFileId(), 1);
    //    bool isAcquiesce = false;
    lordSprite=CCSpriterX::create(str->getCString(),true,true);
     addChild(lordSprite);
    lordSprite->setPosition(ccp( screenwidth/2, screenheight * 0.8-screenheight*0.46));
    lordSprite->play();
    lordSprite->setanimaID(0);
    lordSprite->setisloop(true);
    lordPos=lordSprite->getPosition();
  
    
     //我方位置
    
    int row=0;
    if (m_lordType==2) {
        row=3;
    }else
        row=0;
    
    int numCount=0;
    for (int j=1; j<5; j++) {
        
        if (j==row ) {
            continue;
        }
        
        if (j==1) {
            numCount=0;
        }
        else if(j==2 || j==3)
            numCount=1;
        else
            numCount=2;
        for (int i=0; i<3; i++)
        {
            
            str = CCString::createWithFormat("animationFile/%d_%d.plist",m_countryId+3100, numCount);
            ResourceManager::sharedInstance()->bindTexture(str->getCString(), RES_TYPE_BATTLE_IMAGE, sg_bossBattleLayer);
            
            str = CCString::createWithFormat("animationFile/%d_eff.plist",m_countryId+3100);
            ResourceManager::sharedInstance()->bindTexture(str->getCString(), RES_TYPE_BATTLE_IMAGE, sg_bossBattleLayer);
            str = CCString::createWithFormat("animationFile/%d_%d.scml",m_countryId+3100, numCount);
            CCSpriterX *sbSprite=CCSpriterX::create(str->getCString(),true,true);
            addChild(sbSprite);
//            sbSprite->setPosition(ccp(screenwidth/7*j, screenheight * position+screenheight/16*i));
            sbSprite->play();
            sbSprite->setanimaID(i);
            sbSprite->setisloop(true);
            
            if (j==1)
            {
                 sbSprite->setPosition(ccp(screenwidth/9, screenheight * 0.3+screenheight/14*i));
            }
            else if(j==2)
            {
                if(m_lordType==2)
                {
                    sbSprite->setPosition(ccp(screenwidth/9 *4.4, screenheight * 0.12+screenheight/14*i));
                     soldierPos[1]=ccp(screenwidth/10 *4.6,screenheight*0.22);
                }
                else
                     sbSprite->setPosition(ccp(screenwidth/9 *4, screenheight * 0.12+screenheight/14*i));
                 
            }else if(j==3)
            {
                 sbSprite->setPosition(ccp(screenwidth/9*5, screenheight * 0.12+screenheight/14*i));
            }else if(j==4)
            {
                 sbSprite->setPosition(ccp(screenwidth/9*8, screenheight * 0.3+screenheight/14*i));
            }
           
            soldiers->addObject(sbSprite);
            
            ResourceManager *rm=ResourceManager::sharedInstance();
            if (m_countryId==2) {
                CCString *strcoun = CCString::createWithFormat("animationFile/zldj_%d_1.plist",m_countryId);
                rm->bindTexture(strcoun->getCString(), RES_TYPE_BATTLE_IMAGE, sg_herolayer);
                strcoun = CCString::createWithFormat("animationFile/zldj_%d_2.plist",m_countryId);
                rm->bindTexture(strcoun->getCString(), RES_TYPE_BATTLE_IMAGE, sg_bossBattleLayer);
            }else{
              CCString* strcoun = CCString::createWithFormat("animationFile/zldj_%d.plist",m_countryId);
                rm->bindTexture(strcoun->getCString(), RES_TYPE_BATTLE_IMAGE, sg_bossBattleLayer);
            }

            
            str = CCString::createWithFormat("animationFile/zldjh_%d.scml",m_countryId);
            CCSpriterX *fermEffect = CCSpriterX::create(str->getCString(), true, true);
            fermEffect->setanimaID(0);
            fermEffect->setisloop(true);
            fermEffect->setAnchorPoint(ccp(0.5, 0.5));
            sbSprite->addChild(fermEffect, -10, -10);
            fermEffect->play();
            
            str = CCString::createWithFormat("animationFile/zldjq_%d.scml",m_countryId);
            CCSpriterX *fermEffect1 = CCSpriterX::create(str->getCString(), true, true);
            fermEffect1->setanimaID(i);
            fermEffect1->setisloop(true);
            fermEffect1->setAnchorPoint(ccp(0.5, 0.5));
            
            sbSprite->addChild(fermEffect1, 10, 10);
            fermEffect1->play();
            
            fermEffect->setScale(0.5);
            fermEffect1->setScale(0.5);
//
//
            
            
        }

    }
   
  
}

void SGBossBattleLayer::skipBattle(){
    this->finishAction();
}

void SGBossBattleLayer::startAction()
{
    if (getLordDamge()<getbossHp()) {
        CCCallFunc *callf=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfSolider));
        CCCallFunc *callf01=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfSolider2));
        CCCallFunc *callf1=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfLord));
        CCCallFunc *callf11=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfLord2));
        CCCallFunc *callf2=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOFBoss));
        CCCallFunc *callf21=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfBoss2));
        CCCallFunc *callf3=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::removeBufSprite));
        CCCallFunc *callFin=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::finishAction));
        CCCallFunc *callRemove=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::deleteSbObj));
        this->runAction(CCSequence::create(CCDelayTime::create(0.5),callf,CCDelayTime::create(0.5),callf01,CCDelayTime::create(1.2),callf1,CCDelayTime::create(0.8),callf11,CCDelayTime::create(1.5),callf2,CCDelayTime::create(0.5),callf21,CCDelayTime::create(1.1),callf3,CCDelayTime::create(0.5),callRemove,CCDelayTime::create(1.2),callFin,NULL));
    }else
    {
        CCCallFunc *callf=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfSolider));
        CCCallFunc *callf01=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfSolider2));
        CCCallFunc *callf1=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfLord));
        CCCallFunc *callf11=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::showEffectOfLord2));
        CCCallFunc *callf3=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::removeBufSprite));
        CCCallFunc *callFin=CCCallFunc::create(this, callfunc_selector(SGBossBattleLayer::finishAction));
        this->runAction(CCSequence::create(CCDelayTime::create(0.5),callf,CCDelayTime::create(0.2),callf01,CCDelayTime::create(1.2),callf1,CCDelayTime::create(0.4),callf11,CCDelayTime::create(1.5),callf3,CCDelayTime::create(0.5),callFin,NULL));
    }
    
  

    
}
void SGBossBattleLayer::showEffectOFBoss()
{
    
    this->removeBufSprite();
    CCSpriterX *sbSpriter=CCSpriterX::create("sgbossbattlelayer/boss bullet.scml",true,true);
    this->addChild(sbSpriter);
    sbSpriter->setPosition(bossPos);
    sbSpriter->play();
    sbSpriter->setanimaID(0);
    sbSpriter->setisloop(false);
    sbSpriter->runAction(CCMoveTo::create(0.5, ccpAdd(lordPos, ccp(0, -200))));
    
    bossSprite->runAction(CCSequence::create(CCScaleTo::create(0.1, 0.7),CCScaleTo::create(0.1, 0.9),NULL));
    
    soldiers->addObject(sbSpriter);
     EFFECT_PLAY(MUSIC_61);
 
}
void SGBossBattleLayer::showEffectOfBoss2()
{
    this->removeBufSprite();
    CCSpriterX *sbSpriter=CCSpriterX::create("sgbossbattlelayer/bosstarget.scml",true,true);
    this->addChild(sbSpriter);

    sbSpriter->setPosition(ccpAdd(lordPos, ccp(0, -20)));
    
    sbSpriter->play();
    sbSpriter->setanimaID(0);
    sbSpriter->setisloop(false);
    
    soldiers->addObject(sbSpriter);
    this->fuckLayer(true);
    showBattleLabel(999999, SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, false);
     EFFECT_PLAY(MUSIC_60);
}

void SGBossBattleLayer::showEffectOfLord(){
   
    
    this->removeBufSprite();
    CCSpriterX *sbSpriter=CCSpriterX::create("animationFile/jian_103.scml",true,true);
    this->addChild(sbSpriter);
    sbSpriter->setScale(1.5);
    sbSpriter->setPosition(lordPos);
    sbSpriter->play();
    
    lordSprite->runAction(CCSequence::create(CCScaleTo::create(0.1, 0.8),CCScaleTo::create(0.1, 1.0),NULL));
    
    sbSpriter->setanimaID(0);
    sbSpriter->setisloop(false);
    sbSpriter->runAction(CCMoveTo::create(0.2, ccpAdd(bossPos, ccp(0, -50))));
    
    soldiers->addObject(sbSpriter);

    EFFECT_PLAY(MUSIC_59);
}
void SGBossBattleLayer::showEffectOfLord2(){
    this->removeBufSprite();
    CCSpriterX *sbSpriter=CCSpriterX::create("animationFile/jian_103_bz.scml",true,true);
    this->addChild(sbSpriter);
    sbSpriter->setPosition(ccpAdd(bossPos, ccp(0, 150)));
    sbSpriter->setScale(2);
    
    sbSpriter->play();
    sbSpriter->setanimaID(0);
    sbSpriter->setisloop(false);
    //    sbSpriter->runAction(CCMoveTo::create(1, lordPos));
    
    soldiers->addObject(sbSpriter);
    showBattleLabel(this->getLordDamge(), SB_ADD_AND_SUB_AP_W, SB_ADD_AND_SUB_AP_H, true);
    this->fuckLayer();
     EFFECT_PLAY(MUSIC_62);
}

void SGBossBattleLayer::showEffectOfSolider(){

    CCObject *obj=NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
        CCSprite *spriter=(CCSprite *)obj;
        spriter->runAction(CCFadeOut::create(0.5));
    }
    
    
    for (int i =1; i<5; i++) {
        if ( m_lordType==2 && i==3) {
            continue;
        }
        CCSpriterX *sbSpriter=CCSpriterX::create("sgbossbattlelayer/vortex.scml",true,true);
        this->addChild(sbSpriter);
        sbSpriter->setPosition(ccpAdd(soldierPos[i-1],ccp(20, 0)));
        
        sbSpriter->setanimaID(0);
        sbSpriter->setisloop(true);
        sbSpriter->setAnimaSpeed(0.4);
        sbSpriter->play();
      
        
//        sbSpriter->runAction(CCSequence::create(CCDelayTime::create(0.3),CCMoveTo::create(0.5, lordPos),NULL));
        
        soldiers->addObject(sbSpriter);
    }
//    this->showEffectOfLord();
    
}
void SGBossBattleLayer::showEffectOfSolider2(){
     CCObject *obj=NULL;
    
    CCARRAY_FOREACH(soldiers, obj)
    {
        CCSpriterX *spriter=(CCSpriterX *)obj;
        spriter->runAction(CCSequence::create(CCDelayTime::create(0.3),CCMoveTo::create(0.5, lordPos),NULL));
    }
    EFFECT_PLAY(MUSIC_58);

}
void SGBossBattleLayer::removeBufSprite()
{
    CCObject *obj=NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
        CCNode *spriter=(CCNode *)obj;
//        spriter->removeAllChildrenWithCleanup(true);
        spriter->removeFromParentAndCleanup(true);
    }
}


void SGBossBattleLayer::showBattleLabel(int num, int w, int h, bool isBoss)
{
    
    
    if (num == 0) return;
    CCString *strN = CCString::createWithFormat("%d", num);
    
    
    
    CCLabelAtlas *label = CCLabelAtlas::create(strN->getCString(),"battle/sub_hp_label.png", w, h, '0');
    CCPoint pos = isBoss ? bossPos:lordPos;
    label->setPosition(ccpAdd(pos, ccp(0, gridHeight * 1.5 )));
    //    ccBezierConfig bead;
    //    bead.controlPoint_1 = label->getPosition();
    //    bead.controlPoint_2 = ccp(label->getPosition().x - (gridWidth * 0.25), label->getPosition().y + gridHeight);
    //    bead.endPosition = ccp(label->getPosition().x + gridWidth*0.5, label->getPosition().y + gridHeight);
    //    CCBezierTo *bea = CCBezierTo::create(0.2, bead);
    
    label->setScale(0.1f);
    CCJumpTo *jumpBy1=CCJumpTo::create(0.3f, ccpAdd(label->getPosition(),ccp(gridWidth, 0)), gridHeight, 1);
    
 
    CCScaleTo *sc = CCScaleTo::create(0.1, (isCrit && isBoss) ? 3.0f : 2.0f);
    CCScaleTo *sc1 = CCScaleTo::create(0.2, (isCrit&& isBoss) ? 1.3: 1.0f);
    CCSpawn *spa = CCSpawn::create(sc,jumpBy1, NULL);
    
    //    ccBezierConfig bead1;
    //    bead1.controlPoint_1 = ccp(label->getPosition().x + gridWidth, label->getPosition().y + gridHeight);
    //    bead1.controlPoint_2 = ccp(label->getPosition().x + (gridWidth), label->getPosition().y - gridHeight * 0.5f);
    //    bead1.endPosition = ccp(label->getPosition().x + gridWidth, label->getPosition().y - gridHeight * 0.5);
    //    CCBezierTo *bea1 = CCBezierTo::create(0.3, bead1);
    
    CCJumpTo *jumpBy2=CCJumpTo::create(0.3f, ccpAdd(label->getPosition(),ccp(gridWidth*1.3, 0)), gridHeight*0.5, 1);
    
    
    //    CCAction *act = (CCAction*)CCSequence::create( spa, spa1, NULL);
    
    //    CCScaleTo *scale = CCScaleTo::create(0.6, 2);
    //    CCJumpTo *jump = CCJumpTo::create(0.8, ccp(label->getPosition().x + (gridWidth), label->getPosition().y), gridHeight, 1);
    //    CCSpawn *spawn = CCSpawn::createWithTwoActions(scale, jump);
    //    CCScaleTo *scale1 = CCScaleTo::create(0.5, 1);
    CCJumpTo *jump1 = CCJumpTo::create(0.2f, ccp(label->getPosition().x + gridWidth * 1.5, label->getPosition().y), gridHeight*0.3 * 0.2, 1);
    CCFadeOut *fad = CCFadeOut::create(0.2f);
    CCSpawn *spawn1 = CCSpawn::createWithTwoActions(fad, jump1);
    CCDelayTime *delay = CCDelayTime::create(1.);
    CCCallFuncN* call = CCCallFuncN::create(this, callfuncN_selector(SGBossBattleLayer::removeBalleLabel));
    CCAction *act = (CCAction*)CCSequence::create(spa,sc1,jumpBy2, spawn1, delay, call, NULL);
    
    label->runAction(act);
    this->addChild(label, ADD_AP_LAYER_TAG, ADD_AP_LAYER_TAG);
    
}
void SGBossBattleLayer::removeBalleLabel(){

    CCLabelAtlas *label=(CCLabelAtlas *)this->getChildByTag(ADD_AP_LAYER_TAG);

    label->removeFromParentAndCleanup(true);
}

void SGBossBattleLayer::finishAction(){
    this->removeBufSprite();
//    SGStoryReward *_reward=new SGStoryReward();
    
//    BossReward *bossReward=new BossReward();
//    bossReward->damage=10;
//    bossReward->gold=110;
//    bossReward->other=39;
	
	if (!bossHp){
    //if (getLordDamge()>bossHp) {
        rewardData->isKillBoss=true;
    }else
        rewardData->isKillBoss=false;
    SGBossRewardLayer *layer = SGBossRewardLayer::create(rewardData); /////1115
    
    SGMainManager::shareMain()->showLayer(layer,true);
    

    SGMainManager::shareMain()->getMainLayer()->showBossKilledMsg(layer);
    
}




void SGBossBattleLayer::fuckLayer(bool isBoss)
{
    
   
    if (isBoss) {
        CCMoveTo *move1=CCMoveTo::create(0.022, ccp(0, 10));
        CCMoveTo *move2=CCMoveTo::create(0.022, ccp(0, -10));
        CCMoveTo *move3=CCMoveTo::create(0.022, ccp(0, 0));
        CCSequence *action = CCSequence::create( move1,move3, move2,move3 ,NULL);
        CCRepeat *repeat=CCRepeat::create(action, 20);
        action->setTag(101);
        this->runAction(repeat);
    }
    else
    {
        CCMoveTo *move1=CCMoveTo::create(0.022, ccp(0, 5));
        CCMoveTo *move2=CCMoveTo::create(0.022, ccp(0, -5));
        CCMoveTo *move3=CCMoveTo::create(0.022, ccp(0, 0));
        CCSequence *action = CCSequence::create( move1,move3, move2,move3 ,NULL);
//
//        CCMoveTo *move1=CCMoveTo::create(0.022, ccp(0, 5));
//        CCMoveTo *move2=CCMoveTo::create(0.022, ccp(0, -5));
//        CCMoveTo *move3=CCMoveTo::create(0.022, ccp(0, 0));
//        CCSequence *action = CCSequence::create( move1,move3, move2,move3 ,NULL);
    
        CCRepeat *repeat=CCRepeat::create(action, 12);
        action->setTag(101);
        this->runAction(repeat);
    }
}
void SGBossBattleLayer::deleteSbObj(){
//    CCDelayTime *_time = CCDelayTime::create(1);
    CCCallFuncN *_call = CCCallFuncN::create(this, callfuncN_selector(SGBossBattleLayer::showRemoveEff));
    CCHide  *hide = CCHide::create();
    CCDelayTime *time = CCDelayTime::create(0.5);
    CCCallFuncN * call = CCCallFuncN::create(this, callfuncN_selector(SGBossBattleLayer::callRemoveSb));
    lordSprite->runAction(CCSequence::create(_call, time, call,NULL));
    
}
void SGBossBattleLayer::showRemoveEff(CCObject *obj)
{
    CCSpriterX *sb=(CCSpriterX *)obj;
    ResourceManager::sharedInstance()->bindTexture("animationFile/wqlg.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    ResourceManager::sharedInstance()->bindTexture("effects/xiaoshi.plist", RES_TYPE_LAYER_UI, sg_bossBattleLayer);
    CCSpriterX* remove = CCSpriterX::create("animationFile/xcd.scml", true, true);
    if (remove) {
        remove->setanimaID(0);
        remove->setisloop(true);
        remove->setAnchorPoint(ccp(0.5, 0.5));
        sb->addChild(remove, 100111, 11111);
        remove->play();
        
        remove->setPosition(ccp(sb->getContentSize().width * 0.5, sb->getContentSize().height * 0.5));
        
    }
    
   
}
void SGBossBattleLayer::callRemoveSb(CCObject *obj)
{
    CCSpriterX *sb = (CCSpriterX*) obj;
    sb->removeFromParentAndCleanup(true);
}