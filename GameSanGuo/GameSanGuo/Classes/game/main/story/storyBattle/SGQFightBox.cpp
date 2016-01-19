//
//  SGQFightBox.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-16.
//
//

#include "SGQFightBox.h"
#include "SGMainManager.h"
#include "SGMainLayer.h"
//#include "SGFindHelpLayer.h"
#include "SGSocketClient.h"
#include "SGMsgId.h"
#include "PlotModule.pb.h"
#include "SGStaticDataManager.h"
#include "SimpleAudioEngine.h"
#include "SGStorySectionLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "ShopModule.pb.h"
// SGQFightBox
SGQFightBox::SGQFightBox(void)
{
    index = 0;
    plotId = 0;
    storyId = 0;
    roleId = 0;
    canJump = false;
    m_nSaodangVipLevel = 0 ;
    m_nTenSaodangVipLevel = 0;
    t_delegate = NULL;
    m_nTenSaodangPlayerLevel = 0;
    m_nPerTimePower = 0;
    m_nTimes = 0;
}

SGQFightBox::~SGQFightBox(void)
{
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_qFightBox);
}

SGQFightBox* SGQFightBox::create(SGBoxDelegate *delegate, int index)
{
    SGQFightBox *box = new SGQFightBox();
    if (box && box->init(delegate, box_quickfight, CCSizeMake(530, 350),true,true,CCSizeMake(0,0),true)) {
        box->t_delegate = delegate;
        box->index = index;
        box->initView();
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void SGQFightBox::initView()
{
    CCSize size = this->boxSize;
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
	
	ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_box_qFightBox);
    ResourceManager::sharedInstance()->bindTexture("sgstorysectionlayer/sgstorysectionlayer.plist", RES_TYPE_LAYER_UI, sg_box_qFightBox);
	 ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_box_qFightBox);
	ResourceManager::sharedInstance()->bindTexture("sgpvpmainlayer/sgpvpmainlayer.plist", RES_TYPE_LAYER_UI, sg_box_qFightBox);
    ResourceManager::sharedInstance()->bindTexture("skipfightbox/skipfightbox.plist", RES_TYPE_LAYER_UI, sg_box_qFightBox);
    CCSprite *boxbg = CCSprite::createWithSpriteFrameName("skipfightbg.png");
    this->addChild(boxbg);
    boxbg->setPosition(center);
    CCDictionary *dic = SGStaticDataManager::shareStatic()->getDisplayValueDict();
    CCDictionary *dic1 =(CCDictionary *) dic->objectForKey("7");
    m_nSaodangVipLevel = dic1->valueForKey("value")->intValue();
    CCDictionary *dic2 =(CCDictionary *) dic->objectForKey("8");
    m_nTenSaodangVipLevel = dic2->valueForKey("value")->intValue();
    CCDictionary *dic3 =(CCDictionary *) dic->objectForKey("27");
    m_nTenSaodangPlayerLevel = dic3->valueForKey("value")->intValue();

    SGCCLabelTTF* title = SGCCLabelTTF::create(str_sgQfightBox_str1, FONT_PANGWA, 28);
    //title->setOutSideColor(ccc3(143,87,0));
    title->setPosition(ccpAdd(center, ccp(-40, -30)));
    this->addChild(title);
    int times = SGPlayerInfo::sharePlayerInfo()->getSkipFightCount();
    SGCCLabelTTF* labelTimes = SGCCLabelTTF::create(CCString::createWithFormat("%d",times)->getCString(), FONT_PANGWA, 28 , ccGREEN);
    //title->setOutSideColor(ccc3(143,87,0));
    //modify by:zyc. merge into create.
    //labelTimes->setColor(ccc3(0,255,0));
    labelTimes->setPosition(ccpAdd(title->getPosition(), ccp(title->getContentSize().width/2+40, 0)));
    this->addChild(labelTimes);


    const char *desc1 = str_sgQfightBox_str2;
    if (SGPlayerInfo::sharePlayerInfo()->getVictory() >=2) {
        //desc1 = str_OverBattleBox_str9;
    }
    SGCCLabelTTF *label1 = SGCCLabelTTF::create(desc1, FONT_PANGWA, 32, CCSizeMake(size.width*.85f, 100));
    label1->setPosition(ccpAdd(center, ccp(10, -120)));
    this->addChild(label1, 10, 20);
    if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
        SGCCLabelTTF *label_tip = SGCCLabelTTF::create(str_sgQfightBox_str703, FONT_PANGWA, 23, CCSizeMake(size.width*.85f, 100));
        label_tip->setPosition(ccpAdd(center, ccp(20, -200)));
        //label_tip->setColor(ccc3(200, 100, 10));
        this->addChild(label_tip);

    }
    
    
    
    SGButton *skipBtn = SGButton::createFromLocal("an.png", str_sgQfightBox_str4, this, menu_selector(SGQFightBox::skipFightAction),CCPointZero,FONT_PANGWA,ccWHITE,32);
    float btnWidth = skipBtn->getContentSize().width;
    float btnWid = skipBtn->getContentSize().width;
    this->addBtn(skipBtn);
    skipBtn->setScale(0.9);
    skipBtn->setPosition(ccpAdd(center, ccp(-btnWidth, size.height/4-35)));
    
    SGButton *enterBtn = SGButton::createFromLocal("an.png", str_sgQfightBox_str5, this, menu_selector(SGQFightBox::enterFightAction),CCPointZero,FONT_PANGWA,ccWHITE,32);
    enterBtn -> setTag(2013);
    this->addBtn(enterBtn);
    enterBtn->setScale(0.9);
    enterBtn->setPosition(ccpAdd(center, ccp(btnWidth, size.height/4-35)));
    
    SGButton *cancelBtn = SGButton::create("box_btn_newbg.png",
                                         "public_font_quxiao.png",
                                         this,
                                         menu_selector(SGQFightBox::closeBox),
                                         ccp(0, 0),
                                         false,
                                         true);
    this->addBtn(cancelBtn);
    cancelBtn->setPosition(ccpAdd(center, ccp(0, -size.height/2-55)));
    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    if(plotId>=20000)
    {
        
        SGButton *TenButton = SGButton::createFromLocal("an.png", str_sgQfightBox_str202, this, menu_selector(SGQFightBox::twoSkipFightAction),CCPointZero,FONT_PANGWA,ccWHITE,32);
        this->addBtn(TenButton);
        
        TenButton->setScale(0.9);
        
        TenButton->setPosition(ccpAdd(center, ccp(0, size.height/4-35)));
//        if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
//            if(SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel()<m_nTenSaodangVipLevel&&SGPlayerInfo::sharePlayerInfo()->getPlayerLevel()<m_nTenSaodangPlayerLevel)
//            {
//                TenButton->setImage("hui.png");
//            }
//        }
    }
    else{
        SGButton *TenButton = SGButton::createFromLocal("an.png", str_sgQfightBox_str3, this, menu_selector(SGQFightBox::tenSkipFightAction),CCPointZero,FONT_PANGWA,ccWHITE,32);
        this->addBtn(TenButton);
        
        TenButton->setScale(0.9);
        
        TenButton->setPosition(ccpAdd(center, ccp(0, size.height/4-35)));
        if (SGPlayerInfo::sharePlayerInfo()->getShowvip()) {
            if(SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel()<m_nTenSaodangVipLevel&&SGPlayerInfo::sharePlayerInfo()->getPlayerLevel()<m_nTenSaodangPlayerLevel)
            {
                TenButton->setImage("hui.png");
            }
        }
    }
}

void SGQFightBox::setDatas(int pid, int rid, int sid,bool canJumpSection, int power,int times)
{
    plotId = pid;
    roleId = rid;
    storyId = sid;
    canJump = canJumpSection;
    m_nPerTimePower = power;
    m_nTimes = times;
}

void SGQFightBox::twoSkipFightAction()
{
    EFFECT_PLAY(MUSIC_BTN);
 
//    if(SGPlayerInfo::sharePlayerInfo()->getShowvip())
//    {
//        if(SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel()<m_nTenSaodangVipLevel && SGPlayerInfo::sharePlayerInfo()->getPlayerLevel()<m_nTenSaodangPlayerLevel)
//        {
//            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_sgQfightBox_str701,m_nTenSaodangVipLevel,m_nTenSaodangPlayerLevel)->getCString());
//            return;
//            
//        }
//        
//    }
    if(SGPlayerInfo::sharePlayerInfo()->getSkipFightCount()<2)
    {
        SGMainManager::shareMain()->showMessage(str_sgQfightBox_str6);
        return;
    }
    if(m_nTimes<2)
    {
        SGMainManager::shareMain()->showMessage(str_sgQfightBox_str203);
        return;
    }
    //
    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    SGSpotDataModel *model = SGStaticDataManager::shareStatic()->getSpotById(plotId);
    int power = SGPlayerInfo::sharePlayerInfo()->getPlayerPower();
    int pSread = power - model->getSpotRequestPower();
    if(power<m_nPerTimePower*2)
    {
        main::ShopPurchPvEPowerEntryRequest *request = new main::ShopPurchPvEPowerEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_PURCHPVEPOWE_RENTER, request);
        //SGMainManager::shareMain()->showMessage(str_sgQfightBox_str7);
        return;
    }
    
    
    main::PlotQuitEndRequest *request = new main::PlotQuitEndRequest();
 
    request->set_plotid(plotId);
    request->set_storyid(storyId);
    request->set_type(main::DOUBLE);
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_BATTLE_QEND, request);
    SGPlayerInfo::sharePlayerInfo()->setsaodangType(4);

    //this->resetMainTouch(false);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGShadeBox * basebox = SGShadeBox::create(mainScene,1);
    basebox->setTag(1314);
    SGMainManager::shareMain()->showBox(basebox);
    SGBaseBox::boxClose();
    this->resetMainTouch(false);
}
void SGQFightBox::tenSkipFightAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    

    
    if(SGPlayerInfo::sharePlayerInfo()->getShowvip())
    {
        if(SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel()<m_nTenSaodangVipLevel && SGPlayerInfo::sharePlayerInfo()->getPlayerLevel()<m_nTenSaodangPlayerLevel)
        {
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_sgQfightBox_str701,m_nTenSaodangVipLevel,m_nTenSaodangPlayerLevel)->getCString());
            return;

        }
            
    }
    if(SGPlayerInfo::sharePlayerInfo()->getSkipFightCount()<10)
    {
        SGMainManager::shareMain()->showMessage(str_sgQfightBox_str6);
        return;
    }
    //
    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    SGSpotDataModel *model = SGStaticDataManager::shareStatic()->getSpotById(plotId);
    int power = SGPlayerInfo::sharePlayerInfo()->getPlayerPower();
    int pSread = power - model->getSpotRequestPower();
    if(power<m_nPerTimePower*10)
    {
        main::ShopPurchPvEPowerEntryRequest *request = new main::ShopPurchPvEPowerEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_PURCHPVEPOWE_RENTER, request);
        //SGMainManager::shareMain()->showMessage(str_sgQfightBox_str7);
        return;
    }

    
    main::PlotQuitEndRequest *request = new main::PlotQuitEndRequest();
    //    request->set_cooid(0);
    //    request->set_plotid(1);
    //    request->set_storyid(1);
    
    //    request->set_cooid(roleId);
    request->set_plotid(plotId);
    request->set_storyid(storyId);
    request->set_type(main::MULTI);
    
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_BATTLE_QEND, request);
    SGPlayerInfo::sharePlayerInfo()->setsaodangType(2);

    //this->resetMainTouch(false);
     SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGShadeBox * basebox = SGShadeBox::create(mainScene,1);
    basebox->setTag(1314);
    SGMainManager::shareMain()->showBox(basebox);
    SGBaseBox::boxClose();
    this->resetMainTouch(false);
    //basebox->init(mainScene,box_zhezhao, CCSizeMake(530, 350),false,true,CCSizeMake(0,0),true);

}
void SGQFightBox::skipFightAction()
{
    EFFECT_PLAY(MUSIC_BTN);
    if(SGPlayerInfo::sharePlayerInfo()->getShowvip())
    {
        if(SGPlayerInfo::sharePlayerInfo()->getplayerVipLevel()<m_nSaodangVipLevel)
        {
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_sgQfightBox_str701,m_nSaodangVipLevel)->getCString());
            return;
        }
        
    }
    
 
    if(SGPlayerInfo::sharePlayerInfo()->getSkipFightCount()<=0)
    {
        SGMainManager::shareMain()->showMessage(str_sgQfightBox_str6);
        return;
    }
//
    int plotId = SGPlayerInfo::sharePlayerInfo()->getPlotId();
    SGSpotDataModel *model = SGStaticDataManager::shareStatic()->getSpotById(plotId);
    int power = SGPlayerInfo::sharePlayerInfo()->getPlayerPower();
    int pSread = power - model->getSpotRequestPower();
    if(power<m_nPerTimePower)
    {
        SGMainManager::shareMain()->showMessage(str_sgQfightBox_str7);
        return;
    }
    //SGPlayerInfo::sharePlayerInfo()->setPlayerPower(pSread);
    
    main::PlotQuitEndRequest *request = new main::PlotQuitEndRequest();
//    request->set_cooid(0);
//    request->set_plotid(1);
//    request->set_storyid(1);
    
//    request->set_cooid(roleId);
    request->set_plotid(plotId);
    request->set_storyid(storyId);
    request->set_type(main::SINGLE);
    SGSocketClient::sharedSocketClient()->send(MSG_STORY_BATTLE_QEND, request);
    if(plotId>20000)
    {
         SGPlayerInfo::sharePlayerInfo()->setsaodangType(3);
    }else{
        SGPlayerInfo::sharePlayerInfo()->setsaodangType(1);
    }
    SGBaseBox::boxClose();
    this->resetMainTouch(false);
    
}

void SGQFightBox::enterFightAction()
{
    EFFECT_PLAY(MUSIC_32);
    //((SGFindHelpLayer *)t_delegate)->gotoFightingLayer(index);
    ((SGStorySectionLayer *)t_delegate)->gotoFirendRequest();
    
    SGBaseBox::boxClose();
    this->resetMainTouch(true);
}

void SGQFightBox::closeBox()
{
    //((SGFindHelpLayer*)t_delegate)->isBattle_Rend = false;
    EFFECT_PLAY(MUSIC_BTN);
    SGBaseBox::boxClose();
    this->resetMainTouch(true);
}

void SGQFightBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(touch);
    }
}



SGShadeBox::SGShadeBox(void) : t_delegate(NULL), titleDot1(NULL), titleDot2(NULL), titleDot3(NULL), m_showDotCount(0)
{}

SGShadeBox::~SGShadeBox(void)
{}

 SGShadeBox* SGShadeBox::create(SGBoxDelegate *delegate, int index)
{

    SGShadeBox *box = new SGShadeBox();
    if (box && box->init(delegate, box_shade, CCSizeMake(530, 350),false,false,CCSizeMake(0,0),false)) {
        box->t_delegate = delegate;
       
        box->initView();
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void SGShadeBox::initView()
{
     CCPoint center = SGLayout::getPoint(kMiddleCenter);
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    CCLayerColor *bgLayer  =CCLayerColor::create(ccc4(0, 0, 0, 180), skewing(320)*3, skewingY(960)*3);
    this->addChild(bgLayer,-3);
    bgLayer->setPosition(ccpAdd(CCPointZero, ccp(-bgLayer->getContentSize().width*.5, -bgLayer->getContentSize().height*.5)));
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_sgQfightBox_str704, FONT_PANGWA, 28 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    title->setPosition(ccpAdd(center, ccp(-40, -30)));
    this->addChild(title);
    m_showDotCount = 0;
    
    titleDot1 = SGCCLabelTTF::create(".", FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //titleDot1->setOutSideColor(ccc3(143,87,0));
    titleDot1->setPosition(ccpAdd(title->getPosition(), ccp(title->getContentSize().width/2 + 20, 0)));
    this->addChild(titleDot1);
    
    titleDot2 = SGCCLabelTTF::create(".", FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //titleDot2->setOutSideColor(ccc3(143,87,0));
    titleDot2->setPosition(ccpAdd(titleDot1->getPosition(), ccp(30, 0)));
    this->addChild(titleDot2);
    
    titleDot3 = SGCCLabelTTF::create(".", FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //titleDot3->setOutSideColor(ccc3(143,87,0));
    titleDot3->setPosition(ccpAdd(titleDot2->getPosition(), ccp(30, 0)));
    this->addChild(titleDot3);
    
    titleDot1->setVisible(false);
    titleDot2->setVisible(false);
    titleDot3->setVisible(false);
    
    schedule(schedule_selector(SGShadeBox::showTitleDot), 0.2);

}
void SGShadeBox::showTitleDot()
{
    titleDot1->setVisible(m_showDotCount == 1||titleDot1->isVisible());
    titleDot2->setVisible(m_showDotCount == 2||titleDot2->isVisible());
    titleDot3->setVisible(m_showDotCount == 3||titleDot3->isVisible());
    
    m_showDotCount++;
    if (m_showDotCount > 4)
    {
        titleDot1->setVisible(false);
        titleDot2->setVisible(false);
        titleDot3->setVisible(false);
        m_showDotCount = 0;
    }
    
    
}

void SGShadeBox::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(touch);
    }
}
SGShadeBox2::SGShadeBox2(void) : t_delegate(NULL), titleDot1(NULL), titleDot2(NULL), titleDot3(NULL), m_showDotCount(0)
{}

SGShadeBox2::~SGShadeBox2(void)
{}

SGShadeBox2* SGShadeBox2::create(SGBoxDelegate *delegate, int index)
{
    
    SGShadeBox2 *box = new SGShadeBox2();
    if (box && box->init(delegate, box_shade2, CCSizeMake(530, 350),false,false,CCSizeMake(0,0),false)) {
        box->t_delegate = delegate;
        
        box->initView();
        box->autorelease();
        return box;
    }
    CC_SAFE_DELETE(box);
    return NULL;
}

void SGShadeBox2::initView()
{
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    if (t_delegate) {
        t_delegate->setIsCanTouch(false);
        this->resetMainTouch(false);
    }
    CCLayerColor *bgLayer  =CCLayerColor::create(ccc4(0, 0, 0, 180), skewing(320)*3, skewingY(960)*3);
    this->addChild(bgLayer,-3);
    bgLayer->setScaleX(CCDirector::sharedDirector()->getWinSize().width / skewing(320)*3);
    bgLayer->setPosition(ccpAdd(CCPointZero, ccp(-bgLayer->getContentSize().width*.5, -bgLayer->getContentSize().height*.5)));
    SGCCLabelTTF* title = SGCCLabelTTF::create(str_SGShadeBox2_1, FONT_PANGWA, 28 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    title->setPosition(ccpAdd(center, ccp(-40, -30)));
    this->addChild(title);
    m_showDotCount = 0;
    
    titleDot1 = SGCCLabelTTF::create(".", FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
    //modify by:zyc . merge into create.
    //titleDot1->setOutSideColor(ccc3(143,87,0));
    titleDot1->setPosition(ccpAdd(title->getPosition(), ccp(title->getContentSize().width/2 + 20, 0)));
    this->addChild(titleDot1);
    
    titleDot2 = SGCCLabelTTF::create(".", FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
    //modify by:zyc . merge into create.
    //titleDot2->setOutSideColor(ccc3(143,87,0));
    titleDot2->setPosition(ccpAdd(titleDot1->getPosition(), ccp(30, 0)));
    this->addChild(titleDot2);
    
    titleDot3 = SGCCLabelTTF::create(".", FONT_PANGWA, 40 , ccWHITE , ccc3(143,87,0));
    //modify by:zyc . merge into create.
    //titleDot3->setOutSideColor(ccc3(143,87,0));
    titleDot3->setPosition(ccpAdd(titleDot2->getPosition(), ccp(30, 0)));
    this->addChild(titleDot3);
    
    titleDot1->setVisible(false);
    titleDot2->setVisible(false);
    titleDot3->setVisible(false);
    
    schedule(schedule_selector(SGShadeBox2::showTitleDot), 0.2);
    
}
void SGShadeBox2::showTitleDot()
{
    titleDot1->setVisible(m_showDotCount == 1||titleDot1->isVisible());
    titleDot2->setVisible(m_showDotCount == 2||titleDot2->isVisible());
    titleDot3->setVisible(m_showDotCount == 3||titleDot3->isVisible());
    
    m_showDotCount++;
    if (m_showDotCount > 4)
    {
        titleDot1->setVisible(false);
        titleDot2->setVisible(false);
        titleDot3->setVisible(false);
        m_showDotCount = 0;
    }
    
    
}

void SGShadeBox2::resetMainTouch(bool touch)
{
    // make mainlayer buttons touch enable
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(touch);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(touch);
    }
}


