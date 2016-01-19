//
//  SGEquipInfoLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-11.
//
//

#include "SGEquipInfoLayer.h"
#include "SGMainManager.h"
#include "SGButton.h"
#include "cocos-ext.h"
#include "SGEquipBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "ItemModule.pb.h"
#include "SGSocketClient.h"
#include "SGCantAdvanceBox.h"
#include "SGSellBox.h"
#include "SGStaticDataManager.h"
#include "SGSkillInfoBox.h"
#include "SGStrengLvlUpBox.h"
#include "SGMainLayer.h"
#include "SGBattleScoreLayer.h"
#include "ResourceManager.h"
#include "SGEquipPiecesLayer.h"
#include "AppMacros.h"
#include "SGRewardLayer.h"
#include "SGStringConfig.h"
#include "SGLootWiKiMainLayer.h"

#include "SGGuideManager.h"

//解保按钮，锁定
#define IMG_LOCK_ON "ofc_lock_on.png"
//锁定按钮，锁开
#define IMG_LOCK_OFF "ofc_lock_off.png"

//彩色按钮背景
#define IMG_BTN_ON "hotBtnBg.png"
//灰色按钮背景
#define IMG_BTN_OFF "hotBtnBgGray.png"

//icon的缩放比例
#define ICON_SCALE  0.8f

int adpt_wid = 0;
int adpt_hei = 0;
int shift_ipad = 0;

#define  LEVELMOVE  50
SGEquipInfoLayer::SGEquipInfoLayer()
:isback(false)
,isbreak(false),
_currentPage(0),
_currentRow(0),
leftTimes(0),
isfromfirst(false),
_card(NULL),
_before(NULL),
advanceBtn(NULL),
strengBtn(NULL),
sellBtn(NULL),
lock(NULL),
enterType(0),
Direc(NULL),
lvlupAct(NULL)
{
      //  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animationFile/qhtexiao.plist");
}
SGEquipInfoLayer::~SGEquipInfoLayer()
{
    CCLOG("~SGEquipInfoLayer");
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_EQUIP_DISPROTECT);
    notification->removeObserver(this, MSG_EQUIP_PROTECT);
    
    notification->removeObserver(this,MSG_REWARD_GODENTER);
    if (_before) {
        _before->release();
    }
    if (Direc) {
        Direc->release();
    }
 
	
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_equipInfoLayer);

    _card->release();
}
SGEquipInfoLayer *SGEquipInfoLayer::create(SGEquipCard* card,int type,SGEquipCard *before ,bool isfirst,CCDictionary *dic,int currentPage,int currentRow)
{
    SGEquipInfoLayer *equipInfoLayer = new SGEquipInfoLayer();
    if (equipInfoLayer && equipInfoLayer->init(NULL, sg_equipInfoLayer))
    {
        equipInfoLayer->_currentRow=currentRow;
        equipInfoLayer->_currentPage=currentPage;
        equipInfoLayer->Direc = dic;
        equipInfoLayer->_before = before;
        equipInfoLayer->isfromfirst = isfirst;
        equipInfoLayer->enterType = type;
        equipInfoLayer->_card = card;
        equipInfoLayer->_card->retain();
        equipInfoLayer->initView();
        equipInfoLayer->autorelease();
        return equipInfoLayer;
    }
    CC_SAFE_DELETE(equipInfoLayer);
    return NULL;
}
void SGEquipInfoLayer::initView()
{  
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);
	ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);

	if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
        ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
	{
		adpt_wid = -30;
		adpt_hei = -20;
		shift_ipad = 70;
	}
	else if (CCEGLView::sharedOpenGLView()->getFrameSize().height == iphonehdResource.size.height)
	{
		adpt_hei = -50;
	}


    if (enterType == 10000)
    {
        _before->retain();
        SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        layer->setIsCanTouch(false);
    }

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EQUIP_DISPROTECT,
                              this,
                              callfuncO_selector(SGEquipInfoLayer::disprotectListener));
    notification->addObserver(MSG_EQUIP_PROTECT,
                              this,
                              callfuncO_selector(SGEquipInfoLayer::equipprotectListener));
    
    
    notification->addObserver(MSG_REWARD_GODENTER,
                              this,
                              callfuncO_selector(SGRewardLayer::godEnterListener));
    
    
    
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float btmheight = SGMainManager::shareMain()->getBottomHeight();
    float frameheight1 = skewingY(81);
    float hgt=skewingY(233);
    menu->setZOrder(10);
    if (s.height == 1136)
    {
        frameheight1 = skewing(81);
        hgt = skewing(233);
    }

    //左火
    CCSpriterX *fermEffect = CCSpriterX::create("animationFile/qhhyA.scml", true, true);
    fermEffect->setanimaID(0);
    fermEffect->setisloop(true);
    addChild(fermEffect, 10, 0);
    //右火
    CCSpriterX *fermEffect1 = CCSpriterX::create("animationFile/qhhyB.scml", true, true);
    fermEffect1->setCCSpriterXFlipX(true);

    fermEffect1->setanimaID(0);
    fermEffect1->setisloop(true);
    addChild(fermEffect1, 10, 1);

	SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
	
    SGMainManager::shareMain()->addEquipPng(temp->getIconId(),sg_equipInfoLayer);
    CCString *str = CCString::createWithFormat("equip%d.png",temp->getIconId());
	
	
	
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    this->addChild(item,2);


    //背景
    const char *bgArrayIp5[] = {"greenBgI5.png","blueBgI5.png","purpleBgI5.png","orangeBgI5.png"};
    const char *bgArray[] = {"greenBg.png","blueBg.png","purpleBg.png","orangeBg.png"};

    int starLvl = _card->getCurrStar();

    CCSprite *bigbg = NULL;
    CCSprite *bg = NULL;
    item->setAnchorPoint(ccp(0.5, 0));
    if (s.height == 960)
    {
        frameSize = CCRectMake(0, 0 , 768, 413);
        this->setItemID("pad装备底图.png",true);
        fermEffect->setScale(s.width/768);
        fermEffect1->setScale(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287 + 50)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287 + + 50)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -365)));
        
        
        if (starLvl > 2)
        {
            CCSprite *temp = CCSprite::createWithSpriteFrameName("pad装备底图.png");
            bg = CCSprite::createWithSpriteFrameName(bgArray[starLvl - 3]);
            bg->setScaleX(s.width / bg->getContentSize().width);
            bg->setScaleY(temp->getContentSize().height * 0.72 / bg->getContentSize().height);
            this->addChild(bg, 0);
            bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),
                                   ccp(0, -title_bg->getContentSize().height - temp->getContentSize().height * 0.72 /2)));
        }

    }
    else if (s.height == 1136)
    {
        frameSize = CCRectMake(0, 520 -(s.height - hgt*1.18 - title_bg->getContentSize().height), 768, (s.height - hgt*1.18 - title_bg->getContentSize().height));
        this->setItemID("carditembg.png",true);
              fermEffect->setScaleX(s.width/768);
        fermEffect1->setScaleX(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -361+26 + 30)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -361+26 + 30)));
       
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -450)));
        
        if (starLvl > 2)
        {
            CCSprite *temp = CCSprite::createWithSpriteFrameName("carditembg.png");
            bg = CCSprite::createWithSpriteFrameName(bgArrayIp5[starLvl - 3]);
            bg->setScaleX(s.width / bg->getContentSize().width);
            bg->setScaleY(temp->getContentSize().height * 0.75 / bg->getContentSize().height);
            this->addChild(bg, 0);
            bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),
                                       ccp(0, -title_bg->getContentSize().height - temp->getContentSize().height * 0.8 /2 + 16)));
        }
        

    }else
    {
        bigbg = CCSprite::createWithSpriteFrameName("pad装备底图.png");
        bigbg->setScaleY(0.88);
        this->addChild(bigbg,-100);
        bigbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - bigbg->getContentSize().height*.40)));
        fermEffect->setPosition(ccpAdd(bigbg->getPosition(), ccp(-bigbg->getContentSize().width*.41,
                                                                 bigbg->getContentSize().height*.02 -bigbg->getContentSize().height*.098 + 10)));
        fermEffect1->setPosition(ccpAdd(bigbg->getPosition(), ccp(bigbg->getContentSize().width*.41,
                                                                  bigbg->getContentSize().height*.02-bigbg->getContentSize().height*.098 + 10)));
        
        item->setPosition(ccpAdd(bigbg->getPosition(), ccp(0, -bigbg->getContentSize().height*.362)));
        
        
        if (starLvl > 2)
        {
            bg = CCSprite::createWithSpriteFrameName(bgArray[starLvl - 3]);
            bg->setScaleX(s.width / bg->getContentSize().width);
            bg->setScaleY(bigbg->getContentSize().height * 0.815 / bg->getContentSize().height);
            this->addChild(bg, 0);
            bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),
                                   ccp(0, -title_bg->getContentSize().height - bigbg->getContentSize().height * 0.8 /2)));
        }
    }

    fermEffect->play();
    fermEffect1->play();

    this->setstar(_card->getCurrStar(),_card->getUpgradestar());
    this->setTitle(_card->getOfficerName()->getCString());

	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGEquipInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));    

    
    
    CCSprite* jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
    jinbian->setScaleX(s.width/jinbian->getContentSize().width);
    this->addChild(jinbian,5);

    float rate = 1.45;
    int sellBtnShift = 0;
    int globalShift = 0;
    int frame1DownShift = 0;
    if (s.height == 1136)
    {
        rate = 1.45f;
    }
    else if (s.height == 960)
    {
        rate = 1.28f;
        globalShift = 0;
        frame1DownShift = 0;
        
    }
    else
    {
        sellBtnShift = -20;
        rate = 1.25;
    }

    jinbian->setPosition(ccpAdd(ccp(s.width/2, hgt * rate), ccp(0, 0)));

	strengBtn = SGButton::createFromLocal("batch_sell.png", str_qianghua, this,
                                          menu_selector(SGEquipInfoLayer::strengHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(strengBtn);
	strengBtn->setScale(0.7);
	
	advanceBtn = SGButton::createFromLocal("batch_sell.png", str_zhuansheng, this,
                                           menu_selector(SGEquipInfoLayer::advanceHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    advanceBtn->setScale(0.7);
    this->addBtn(advanceBtn);
    /////屏蔽装备转生
    advanceBtn->setVisible(true);
    advanceBtn->setEnabled(true);

    sellBtn = SGButton::create("show_objects.png", NULL, this, menu_selector(SGEquipInfoLayer::sellHandler), CCPointZero, false, true);
    this->addBtn(sellBtn);
    
//android 上特殊处理位置
//#if (PLATFORM == ANDROID)
//        sellBtnShift = sellBtn->getContentSize().height / 4;
//#endif
	
    float btnwidth = skewing(5);
    float btnwid = skewing(33);
    advanceBtn->setPosition(ccp(s.width/2 - btnwid - btnwidth - 90, strengBtn->getPosition().y));
//    sellBtn->setPosition(ccpAdd(ccp(60, 720 + adpt_hei + sellBtnShift), ccp(0, 0)));
    sellBtn->setPosition(ccp(60, bg->getPositionY() - bg->getContentSize().height*0.5*bg->getScaleY() + sellBtn->getContentSize().height*0.8));
    
    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame1->setPreferredSize(CCSizeMake(568 +20, 83));
    CCSprite *frame1bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame1bg,6);
    frame1bg->setScaleX(590/frame1bg->getContentSize().width);
    frame1bg->setScaleY(83/frame1bg->getContentSize().height);
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+20, 200));
    
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg,6);
    frame2bg->setScaleX(590/frame2bg->getContentSize().width);
    frame2bg->setScaleY(205/frame2bg->getContentSize().height);
    
    //放置材料的frame
    CCScale9Sprite *frame3 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame3->setPreferredSize(CCSizeMake(568+20, 130));
    
    CCSprite *frame3bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame3bg,6);
    frame3bg->setScaleX(590/frame3bg->getContentSize().width);
    frame3bg->setScaleY(130/frame3bg->getContentSize().height);
    
    
    
    this->addChild(frame1, 7);
    this->addChild(frame2, 7);
    this->addChild(frame3, 7);
    
    frame2->setPosition(ccpAdd(jinbian->getPosition(), ccp(0, - frame2->getContentSize().height*.5 - 25 + globalShift + frame1DownShift)));
    frame1->setPosition(ccpAdd(frame2->getPosition(), ccp(0, -frame1->getContentSize().height*.5f
                                                          - frame2->getContentSize().height*.5f - 12 + frame1DownShift)));
    frame3->setPosition(ccpAdd(frame1->getPosition(), ccp(0, -frame1->getContentSize().height / 2
                                                          - frame3->getContentSize().height / 2 - 20 + globalShift)));
    
    frame1bg->setPosition(frame1->getPosition());
    frame2bg->setPosition(frame2->getPosition());
    frame3bg->setPosition(frame3->getPosition());
    
    
    //根据frame来放置强化按钮
    strengBtn->setPosition(ccpAdd(frame1->getPosition(), ccp(frame1->getContentSize().width / 2 - strengBtn->getContentSize().width / 2 - 20, 0)));
    advanceBtn->setPosition(ccpAdd(frame3->getPosition(), ccp(frame3->getContentSize().width / 2 - advanceBtn->getContentSize().width / 2 - 20, 0)));
    
    CCArray *materialArray = CCArray::create();
    int needNum[12] = {0};
    
    //获取卡牌的转生条件
    materialArray = this->getMaterialDependency(needNum);
    //填充底部显示的材料相关
    CCLayerColor *colorLayer = this->getMaterialLayer(materialArray, needNum);
    
    colorLayer->setPosition(ccpAdd(frame3->getPosition(), ccp(-frame3->getContentSize().width / 2 + 20, -colorLayer->getContentSize().height / 2)));
    
    
    CCSprite *guang1l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang1l,6);
    guang1l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang1r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang1r,6);
    guang1r->setFlipX(true);
    guang1r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang1m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    this->addChild(guang1m,6);
    guang1m->setScaleX(468/guang1m->getContentSize().width);
    guang1m->setAnchorPoint(ccp(0.5, 1));
    
    guang1m->setPosition(ccpAdd(frame1->getPosition(), ccp(0, frame1->getContentSize().height*.5)));
    guang1r->setPosition(ccpAdd(guang1m->getPosition(), ccp(284+10, 0)));
    guang1l->setPosition(ccpAdd(guang1m->getPosition(), ccp(-284-10, 0)));
    
    CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2l,6);
    guang2l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    this->addChild(guang2r,6);
    guang2r->setFlipX(true);
    guang2r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    this->addChild(guang2m,6);
    guang2m->setScaleX(468/guang2m->getContentSize().width);
    guang2m->setAnchorPoint(ccp(0.5, 1));
    
    guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
    guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(284+10, 0)));
    guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-284-10, 0)));
    
    CCScale9Sprite *font_bg = CCScale9Sprite::createWithSpriteFrameName("barrack_info_bg.png");
    font_bg->setPreferredSize(CCSizeMake(540, 42));
    font_bg->setPosition(ccpAdd(frame2->getPosition(), ccp(0,-70)));
    this->addChild(font_bg,6);
    
    //缘分武将
    SGCCLabelTTF* label = SGCCLabelTTF::create(str_Equipslayer_str11, FONT_PANGWA, 28);
    label->setAnchorPoint(ccp(0, 0.5));
    label->setInsideColor(ccc3(244, 201, 22));
    label->setPosition(ccpAdd(font_bg->getPosition(), ccp(- font_bg->getContentSize().width*.5+15, 0)));
    this->addChild(label,6);
    
    
    SGEquipmentDataModel *equipModel = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    
    //武将缘分
    if (equipModel->getFateDesc())
    {
        SGCCLabelTTF *fateName = SGCCLabelTTF::create(equipModel->getFateDesc()->getCString(), FONT_PANGWA, 28);
        this->addChild(fateName, 6);
        fateName->setPosition(ccpAdd(label->getPosition(), ccp(label->getContentSize().width + fateName->getContentSize().width / 2 + 10, 0)));
    }
    
    
    float move = skewing(46);
    SGCCLabelTTF *equipName = NULL;
    if (_card->getAdNum())
    {
        equipName = SGCCLabelTTF::create(CCString::createWithFormat("%s +%d", _card->getOfficerName()->getCString(),
                                                                    _card->getAdNum())->getCString(), FONT_BOXINFO, 30);

    }
    else
    {
        equipName = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_BOXINFO, 34);
    }
        equipName->setPosition(ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move - 60 + equipName->getContentSize().width/2,
                                   frame1->getPosition().y + frame1->getContentSize().height/5.5 - 20));
    //244	201	22
    equipName->setInsideColor(ccc3(244, 201, 22));
    addChild(equipName,11);
    
    
    //装备等级显示
    this->addLabel(stuff_equipLevel,ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move + equipName->getContentSize().width + 30,
                                        frame1->getPosition().y + frame1->getContentSize().height/5.5 - 22));
    
    this->addLabel(stuff_def,ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 30,23+frame2->getPosition().y));
    this->addLabel(stuff_atk, ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 30,23
                                  +frame2->getPosition().y + frameheight1 * .25));
    this->addLabel(stuff_round,ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 30,23+frame2->getPosition().y - frameheight1 * .25));
    this->addLabel(stuff_speed, ccp(s.width/2 + move*.6+1,23+frame2->getPosition().y + frameheight1 * .25));
    this->addLabel(stuff_mor, ccp(s.width/2 + move*.6+1,23+frame2->getPosition().y));
    this->addLabel(stuff_gov, ccp(s.width/2 + move*.6-13, 23+frame2->getPosition().y - frameheight1 * .25));
    

    
    if (enterType == 1  ||enterType == 4 ||enterType ==3 ||enterType ==2 ||enterType == 10000||enterType == 10 ||enterType == 101 || enterType==10001 ||enterType == 6||enterType == 20||enterType == 24 || enterType == 25 || enterType == 1980 || enterType == 27) {
        advanceBtn->setVisible(false);
        sellBtn->setVisible(false);
        strengBtn->setVisible(false);
        
        if(enterType==24 ||enterType == 25)
        {
            float  hgt = skewingY(210);
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);

            CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
            CCRect e;
            if (s.width > 650)
                e = CCRectMake(0, 1136/2 - (hgt*1.17)/2, bg1->getContentSize().width, hgt*1.38f);
            else
                e = CCRectMake(0, 1136/2 - (hgt*1.17)/2, bg1->getContentSize().width, hgt*1.192f);
            bg1->setTextureRect(e);
            bg1->setScaleX(1.2f);
            bg1->setAnchorPoint(ccp(0.5, 0));
            bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter),ccp(0, -35)) );
            this->addChild(bg1,5);
        
        }
        
    }
    
    SGEquipmentDataModel *general = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    this->setCardType(general->getEquipType());
    
    if (enterType == 10000)
    {
        if (s.height == 960)
        {
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -430));
        }
        else if (s.height == 1136)
        {
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(1.5, -530));
        }
        else
        {
            pos = ccpAdd(bigbg->getPosition(), ccp(1.5, -bigbg->getContentSize().height*.365-bigbg->getContentSize().height*.195));
        }
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.18)/2, s.width, hgt*1.18);
        bg1->setTextureRect(r);
        //bg1->setContentSize(r.size);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
        
        this->addValueLabel(stuff_equipLevel, _card->getCurrLevel(), LEVELMOVE, _card->getMaxLevel(),0,_before->getCurrLevel(),1);//kanata
     
        this->addValueLabel(stuff_atk, _before->getAtk(), move*1.3,NULL);
        this->addValueLabel(stuff_def, _before->getDef(), move*1.3,NULL);
        this->addValueLabel(stuff_round, _before->getRound(), move*1.3,NULL);
        this->addValueLabel(stuff_speed, _before->getSpeed(), move*1.3,NULL);
        this->addValueLabel(stuff_mor, _before->getMorale(), move*1.3,NULL);
        this->addValueLabel(stuff_gov, _before->getGoverning(), move*1.3 +13,NULL);

        Direc->retain();
        int strengstate = ((CCString*)Direc->objectForKey("upstate"))->intValue();
        lvlupAct = CCSpriterX::create(CCString::createWithFormat("animationFile/success%d.scml",strengstate)->getCString(), true, true);
        lvlupAct->setanimaID(0);
        lvlupAct->setisloop(false);
        addChild(lvlupAct,100);
        lvlupAct->setPosition(ccpAdd(pos, ccp(0, 200)));
        lvlupAct->play();
        
        EFFECT_PLAY(MUSIC_35);
        
        leftTimes = _card->getCurrLevel() - _before->getCurrLevel();

//        }
    }
    else if (enterType==10001)
    {
        if (s.height == 960) {
            
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -430));
            
        }
        else if (s.height == 1136) {
            
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(1.5, -530));
            
        }else
        {
            pos = ccpAdd(bigbg->getPosition(), ccp(1.5, -bigbg->getContentSize().height*.365-bigbg->getContentSize().height*.195));
        }
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
        
        this->addValueLabel(stuff_equipLevel, 0, LEVELMOVE, 0);
        
        this->addValueLabel(stuff_atk, _card->getAtk(), move*1.3,NULL);
        this->addValueLabel(stuff_def, _card->getDef(), move*1.3,NULL);
        this->addValueLabel(stuff_round, _card->getRound(), move*1.3,NULL);
        this->addValueLabel(stuff_speed, _card->getSpeed(), move*1.3,NULL);
        this->addValueLabel(stuff_mor,_card->getMorale(), move*1.3,NULL);
        this->addValueLabel(stuff_gov, _card->getGoverning(), move*1.3 +13,NULL);
        
    }
    else
    {
        
        if (enterType == 101)
        {
            ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);

            CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
            CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.18)/2, s.width, hgt*1.18);
            bg1->setTextureRect(r);
            bg1->setAnchorPoint(ccp(0.5, 0));
            bg1->setPosition(SGLayout::getPoint(kBottomCenter));
            this->addChild(bg1,5);
        }
        else
        {
            CCSprite *bg1 = CCSprite::createWithSpriteFrameName("info_redbg.png");
            bg1->setAnchorPoint(ccp(0.5, 0));
            bg1->setScaleX(s.width/bg1->getContentSize().width);
 
            bg1->setScaleY((hgt*rate-btmheight*.68)/bg1->getContentSize().height);

            bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmheight*.68 )));
            this->addChild(bg1,-1);
        }

        this->addValueLabel(stuff_equipLevel, _card->getCurrLevel(), LEVELMOVE,_card->getMaxLevel());
        //////////
        bool hehe=false;
        if(_card->getMaxLevel()==_card->getCurrLevel())
        {
            
             hehe=true;
        }
        this->addValueLabel(stuff_atk, _card->getAtk(), move*1.3,NULL);
        this->addValueLabel(stuff_def, _card->getDef(), move*1.3,NULL);
        this->addValueLabel(stuff_round, _card->getRound(), move*1.3,NULL);
        this->addValueLabel(stuff_speed, _card->getSpeed(), move*1.3,NULL);
        this->addValueLabel(stuff_mor, _card->getMorale(), move*1.3,NULL);
        this->addValueLabel(stuff_gov, _card->getGoverning(), move*1.3 +13,NULL);

        if (enterType == CHATSHOWENTRYTAG)
        {
            strengBtn->setVisible(false);
            advanceBtn->setVisible(false);
            sellBtn->setVisible(false);
        }
    }
    
    if (enterType==25||enterType==20001)
    {
        //查看武将信息
        advanceBtn->setVisible(false);
        strengBtn->setVisible(false);
        sellBtn->setVisible(false);
    }
}


void SGEquipInfoLayer::addValueLabel(stuffType type, int value1,float move, int value2, int anchorType,int qianvalue,int strengtype,bool ismax)
{
    CCSprite *sp = (CCSprite *)this->getChildByTag(type);
    switch (type) {
        case stuff_lvl: //等级
        {

        }
            break;
        case stuff_equipLevel://装备等级
        {
            CCString *lvel = NULL;
            value2 = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
//            if (value1 == value2)
//            {
//                lvel = CCString::create("MAX");
//            }else
            {
                if(strengtype==1)
                {
                    lvel = CCString::createWithFormat(str_Format_lvl,qianvalue, value2);
                }
                
                else
                {
                    lvel = CCString::createWithFormat(str_Format_lvl,value1, value2);
                }
            }
            
            SGCCLabelTTF *abc = SGCCLabelTTF::create(lvel->getCString(),FONT_BOXINFO, 28 , ccWHITE);
            abc->setTag(999);
            abc->setAnchorPoint(ccp(0.5, 0.5));
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move+4, 2)));
        }
            break;
        case stuff_exp: //经验
        {
            CCString *def = CCString::createWithFormat("%d/%d",value1,value2);
            if(ismax)
            {
                def = CCString::createWithFormat(" ");
                
            }
            //            if (value2 == -1) {
            //                def = CCString::create("MAX");
            ////                def->setString("MAX");
            //            }
            
            SGCCLabelTTF *abc = SGCCLabelTTF::create(def->getCString(),FONT_BOXINFO,28);
            this->addChild(abc,9);
            abc->setAnchorPoint(ccp(0.5, 0.5));
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(150, 0)));
            
        }
            break;
        case stuff_atk: //攻击
        {
            CCString *atk = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(atk->getCString(), FONT_BOXINFO, 28 , ccWHITE);
            CCPoint anchor = ccp(0.5, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1000);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                atk_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
                atk_add->setAnchorPoint(ccp(0, 0.5));

                this->addChild(atk_add,9);
                atk_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_def: //防御
        {
            CCString *def = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(def->getCString(), FONT_BOXINFO, 28 ,ccWHITE);

            abc->setAnchorPoint(ccp(0.5, 0.5));
            abc->setTag(1001);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                def_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
                //modify by:zyc. merge into create.
                //def_add->setColor(ccc3(0x0c, 0xff, 0x00));
                def_add->setAnchorPoint(ccp(0, 0.5));
                this->addChild(def_add,9);
                def_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_mor: //血量
        {
            CCString *shiqi = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(shiqi->getCString(), FONT_BOXINFO, 28 , ccWHITE);

            CCPoint anchor;
            anchor = ccp(0.5, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1004);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                mor_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
                //modify by:zyc. merge into create.
                //mor_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(mor_add,9);
                mor_add->setAnchorPoint(ccp(0, 0.5));
                mor_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_speed: //速度
        {
            CCString *speed = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(speed->getCString(), FONT_BOXINFO, 28 ,ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;

            anchor = ccp(0.5, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1003);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                speed_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO, 28 , ccc3(0x0c, 0xff, 0x00));
                //modify by:zyc. merge into create.
                //speed_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(speed_add,9);
                speed_add->setAnchorPoint(ccp(0, 0.5));
                speed_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_gov: //统御
        {
            CCString *shiqi = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(shiqi->getCString(), FONT_BOXINFO, 28 , ccWHITE);

            abc->setAnchorPoint(ccp(0.5, 0.5));
            abc->setTag(1005);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                
            }
        }
            break;
        case stuff_round: //蓄力
        {
            CCString *xuli = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(xuli->getCString(), FONT_BOXINFO, 28 , ccWHITE);
            CCPoint anchor;

            anchor = ccp(0.5, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1002);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                round_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO,28 , ccc3(0x0c, 0xff , 0x00));
                //modify by:zyc. merge into create.
                //round_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(round_add,9);
                round_add->setAnchorPoint(ccp(0, 0.5));
                round_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
        case stuff_area: //占格
        {
            CCString *zhange = CCString::createWithFormat("%d",value1);
            SGCCLabelTTF *abc = SGCCLabelTTF::create(zhange->getCString(), FONT_BOXINFO, 28 , ccWHITE);
            //modify by:zyc. merge into create.
            //abc->setColor(ccWHITE);
            CCPoint anchor;
            if (anchorType == 1)
                anchor = ccp(0, 0.5);
            else
                anchor = ccp(1, 0.5);
            abc->setAnchorPoint(anchor);
            abc->setTag(1002);
            this->addChild(abc,9);
            abc->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            if (value2 > 0)
            {
                CCString *add = CCString::createWithFormat(" +%d",value2);
                round_add = SGCCLabelTTF::create(add->getCString(), FONT_BOXINFO,28 , ccc3(0x0c, 0xff, 0x00));
                //modify by:zyc. merge into create.
                //round_add->setColor(ccc3(0x0c, 0xff, 0x00));
                this->addChild(round_add,9);
                round_add->setAnchorPoint(ccp(0, 0.5));
                round_add->setPosition(ccpAdd(sp->getPosition(), ccp(move, 0)));
            }
        }
            break;
            
            
        default:
            break;
    }
}



void SGEquipInfoLayer::setTitle(const char *title_)
{
    ccColor3B color[4] = {ccc3(0 , 239 , 22) , ccc3(75, 147, 255) ,ccc3(215,56,255) , ccc3(249, 158, 0)};

    labelTitle = SGCCLabelTTF::create(title_, FONT_XINGKAI, 36 , color[_card->getCurrStar() - 3]);//42
    
    this->addChild(labelTitle);
    labelTitle->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -title_bg->getContentSize().height*.5)));
    
    if (_card->getAdNum())
    {
        SGCCLabelTTF *adnum = SGCCLabelTTF::create(CCString::createWithFormat("+%d", _card->getAdNum())->getCString(), FONT_PANGWA, 36);
        adnum->setInsideColor(ccc3(0, 255, 0));
        this->addChild(adnum);
        adnum->setPosition(ccpAdd(labelTitle->getPosition(), ccp(labelTitle->getContentSize().width, 0)));
    }
}


CCArray *SGEquipInfoLayer::getMaterialDependency(int *needNum)
{
    //读取将要转生这个装备卡牌
    SGEquipmentDataModel *model = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    
    //装备的需要材料组id
    int materialDependId = model->getMaterialDependencyId();
    
    //根据装备datamodel里的MaterialDependency获取依赖材料的数据
    SGMaterialDependencyDataModel *depend = SGStaticDataManager::shareStatic()->getMaterialDependencyById(materialDependId);
    
    if (!depend)
    {
        return NULL;
    }
    
    CCString *condition = depend->getMaterialDependCondition();
    //每个条件 materialId:materialNum
    CCArray *conditionGroup = split(condition->getCString(), ";");
    
    //需要的材料原型的的数组
    CCArray *materialModelArray = CCArray::create();
    
    //切分每个分组条件，将内部的条件再次进行切分
    for (int i = 0; i < conditionGroup->count(); i++)
    {
        CCString *temp = static_cast<CCString *>(conditionGroup->objectAtIndex(i));
        //冒号切分
        CCArray *tempConditon = split(temp->getCString(), ",");
        
        //需要的材料id
        int materialId = static_cast<CCString *>(tempConditon->objectAtIndex(0))->intValue();
        //需要的材料的数量
        int materialNum = static_cast<CCString *>(tempConditon->objectAtIndex(1))->intValue();
        //将每次材料需要的数量放到数组中，传给getMaterialLayer
        needNum[i] = materialNum;
        //根据条件组中的id，获取材料的原型
        SGMaterialDataModel *material = SGStaticDataManager::shareStatic()->getMaterialById(materialId);
        if (material)
        {
            materialModelArray->addObject(material);
        }
    }
    return materialModelArray;
}
CCLayerColor *SGEquipInfoLayer::getMaterialLayer(CCArray *materialModelArray, int *needNumArray)
{
    CCLayerColor *color = CCLayerColor::create(ccc4(0, 255, 0, 0), 600, 150);
    color->setAnchorPoint(ccp(0.5, 0.5));
    this->addChild(color, 20, 33);
    //框中放的几个材料
    
    if (materialModelArray == NULL)
    {
        SGCCLabelTTF *label = SGCCLabelTTF::create(str_equipAdvance_top, FONT_PANGWA, 30);
        label->setAnchorPoint(ccp(0, 0.5));
        color->addChild(label, 10);
        label->setPosition(ccpAdd(ccp(30, color->getContentSize().height / 2), ccp(0, 0)));
        
        return color;
    }
    
    CCSize colorContentSize = CCSizeZero;
    SGButton *materialIcon = NULL;
    SGMaterialDataModel *tempMaterial = NULL;
    SGCCLabelTTF *materialName = NULL;
    CCSprite *cardBg = NULL;
    
    CCSprite *nameBg = NULL;
    
    SGCCLabelTTF *selfMaterialNumLabel = NULL;
    SGCCLabelTTF *needMaterialNumLabel = NULL;
    
    // 为icon点击
    CCMenu * menu = CCMenu::create();
    menu->setContentSize(color->getContentSize());
    menu->ignoreAnchorPointForPosition(false);
    menu->setAnchorPoint(ccp(0, 0));
    menu->setPosition(0, 0);
    menu->setTouchPriority(0);
    color->addChild(menu, 0, 55);
    
    //在有新手引导的时候，不可触摸
    
    

    
    
    char iconName[32] = "\0";
    char border[32] = "\0";
    
    if (materialModelArray)
    {
        for (int i = 0; i < materialModelArray->count(); i++)
        {
            tempMaterial = static_cast<SGMaterialDataModel *>(materialModelArray->objectAtIndex(i));
            
            SGMainManager::shareMain()->addHeadIconPlistByNum(tempMaterial->getMaterialIconId(), sg_equipAdvancedLayer);
            sprintf(iconName, "head%d.png", tempMaterial->getMaterialIconId());
            
            int borderColor = tempMaterial->getMaterialBorderColor();
            if (borderColor > 1)
            {
                borderColor -= 1;
            }
            
            sprintf(border, "equipPinzhi_%d_0.png", borderColor);
            
            //头像，边框，名称
            //materialBorder = CCSprite::createWithSpriteFrameName(border);
            materialIcon = SGButton::create(iconName, border, this, menu_selector(SGEquipInfoLayer::onLootSourceBtnClicked) , CCSizeZero , false , true);
            
            materialName = SGCCLabelTTF::create(tempMaterial->getMaterialName()->getCString(), FONT_PANGWA, 18);
            materialIcon->setScale(ICON_SCALE);
            //materialBorder->setScale(ICON_SCALE);
            
            
            menu->addChild(materialIcon , 2 , i);
            
            //自己拥有的材料数
            int selfMaterialNum = 0;

            SGMaterialCard *tmpMaterial = SGPlayerInfo::sharePlayerInfo()->getMaterialCardByItemId(tempMaterial->getMaterialId());
            if (tmpMaterial)
            {
                selfMaterialNum = tmpMaterial->getMaterialCurrNum();
            }
            
            selfMaterialNumLabel = SGCCLabelTTF::create(CCString::createWithFormat("%d", selfMaterialNum)->getCString(), FONT_PANGWA, 18);
            selfMaterialNumLabel->setAnchorPoint(ccp(0, 0.5));
            needMaterialNumLabel = SGCCLabelTTF::create(CCString::createWithFormat("/%d", needNumArray[i])->getCString(), FONT_PANGWA, 18);
            needMaterialNumLabel->setAnchorPoint(ccp(0, 0.5));
            
            if (i == 0)
            {
                selfMaterialNumLabel->setTag(1);
                needMaterialNumLabel->setTag(2);
            }
            
            nameBg = CCSprite::createWithSpriteFrameName("box_name_bg.png");
            nameBg->setScaleX(0.7);
            nameBg->setScaleY(0.85);

            //某一种数量不足
            if (selfMaterialNum < needNumArray[i])
            {
                selfMaterialNumLabel->setInsideColor(ccRED);
            }
            cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
            cardBg->setTag(11);
            
            //color->addChild(materialBorder, 1);
            materialIcon->addChild(selfMaterialNumLabel, 2);
            materialIcon->addChild(needMaterialNumLabel, 3);
            
            selfMaterialNumLabel->setPosition(ccpAdd(ccp(materialIcon->getContentSize().width / 2 - (selfMaterialNumLabel->getContentSize().width + needMaterialNumLabel->getContentSize().width) / 2, selfMaterialNumLabel->getContentSize().height / 1.5), ccp(0, 0)));
            
            needMaterialNumLabel->setPosition(ccpAdd(selfMaterialNumLabel->getPosition(), ccp(selfMaterialNumLabel->getContentSize().width, 0)));
            
            
            color->addChild(materialName, 3);
            materialIcon->addChild(cardBg, -1);
            color->addChild(nameBg, -1);
            
            materialIcon->setPosition(ccpAdd(ccp(0, 0), ccp(materialIcon->getContentSize().width / 2 +
                                                            5 + i *  (materialIcon->getContentSize().width + 10) + 40 - materialIcon->getContentSize().width / 2, materialIcon->getContentSize().height / 2 + 10)));
            
            colorContentSize.width += (materialIcon->getContentSize().width + 10);
            colorContentSize.height = materialIcon->getContentSize().height + 10;
            
            //materialBorder->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0, 0)));
            materialName->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0,-materialIcon->getContentSize().height / 2 + 15)));
            

            
            materialName->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0, -materialName->getContentSize().height * 2)));
            nameBg->setPosition(ccpAdd(materialName->getPosition(), ccp(0, -2)));
//            cardBg->setPosition(ccpAdd(materialIcon->getPosition(), ccp(0, 0)));
            cardBg->setPosition(ccpAdd(ccp(0, 0), ccp(cardBg->getContentSize().width / 2, cardBg->getContentSize().height / 2)));

        }
    }
    color->setContentSize(colorContentSize);
    return color;
}




void SGEquipInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);

    // 0 正常 1 进阶 2 抽奖结果 3 强化 4 图鉴 5首页进阶  6转生进入材料选择 10积分兑换进入 10000强化 CHATSHOWENTRYTAG聊天展示  20 炼丹秘境
    //24   武将信息界面  25 累计登陆进入
    if (enterType == 0) {
        SGMainManager::shareMain()->showEquipsLayer(NULL,0,0, true,0 ,_currentPage,_currentRow);
    }
    else if (enterType == 1)
    {
        showLayerByTag(sg_advanceLayer);
    }
    else if(enterType == 2)
    {
        showLayerByTag(sg_visitShowCard);
    }
    else if(enterType == 3)
    {
        showLayerByTag(sg_strengLayer);
    }
    else if(enterType == 4)
    {
        showLayerByTag(sg_cardsLayer);
    }
    else if(enterType == 5)
    {
        SGMainManager::shareMain()->showEquipsLayer(NULL,0,1);
    }
    else if (enterType == 6)
    {
        showLayerByTag(sg_advancechooselayer);
    }
    else if(enterType == 10)
    {
        showLayerByTag(sg_creditCardLayer);
    }
    else if(enterType == 20)
    {
        showjitian();
    }
    else if(enterType == 24)
    {
        showLayerByTag(sg_generalInfoLayer);
    }
    else if (enterType==25)
    {
        showLayerByTag(sg_totalLoginReward);
    }
    
    else if(enterType == 101)
    {
        SGVisitLayer *layer = (SGVisitLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_visitShowCard);
        if (layer)
        {
            SGMainManager::shareMain()->showLayer(layer);
        }
        else
        {
            showLayerByTag(sg_visitLayer);
        }
    }
    else if(enterType == 10000 || enterType==10001)
    {
        SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        layer->setIsCanTouch(true);
        SGMainManager::shareMain()->showStrengLayer(_card, 2,isfromfirst);
    }
	else if (enterType == 11000)//由碎片进入
	{
		SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
		SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
		SGEquipPiecesLayer *layer = (SGEquipPiecesLayer *)mainScene->getChildByTag(sg_equipPiecesLayer);
		if (layer)
		{
            layer->removeFromParent();
            layer = NULL;
		}
        layer = SGEquipPiecesLayer::create();

		mainLayer->showblack();
		mainLayer->setPortraitMenuCanTouch(false);
		mainScene->addShowLayer(layer);
		SNSTableView::showScrollItemMultiMoveFromRight(layer->getSNSTavleView());//1126效果优化
	}
    else if (enterType == CHATSHOWENTRYTAG)//聊天展示
	{
        SGMainManager::shareMain()->closeChatShowLayer();
	}
    //由邀请好友进入
    else if (enterType == 27)
    {
        main::RewardSalaryEntryRequest *request = new main::RewardSalaryEntryRequest();
        SGSocketClient::sharedSocketClient()->send(MSG_REWARD_INTIVER, request);
    }
    else if (enterType == 20001)
    {
        SGMainManager::shareMain()->getMainLayer()->entryTongQueShow();
    }
}
void SGEquipInfoLayer::showLayerByTag(LayerTag layerTag)
{
    //从内存中找到这个layer
    //使用的前提是在显示 SGGeneralInfoLayer 时不能把上一个layer释放
    
    SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(layerTag);
    if(NULL == layer)
    {
        return;
    }
    
    SGMainManager::shareMain()->showLayer(layer);
    
    SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_equipInfoLayer, true);
    
}

void SGEquipInfoLayer::strengHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_card)
    {
//        SGMainManager::shareMain()->showStrengLayer(_card, 2,isfromfirst);
        SGMainManager::shareMain()->showStrengLayer(_card, 2,-10);
    }
}
void SGEquipInfoLayer::advanceHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGEquipmentDataModel * edm = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    if (edm->getEquipNextId() <= 0) //无转生nextId
    {
        SGMainManager::shareMain()->showMessage(str_zhuansheng_no_need);
    }
    else
    {
        SGMainManager::shareMain()->showEquipAdvancedLayer(_card);
    }

}
void SGEquipInfoLayer::protectHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::EquipmentProtectionRequest *request = new main::EquipmentProtectionRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_PROTECT, request);
}
void SGEquipInfoLayer::sellHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    SGMainManager::shareMain()->getMainLayer()->sendChatShowContent(_card->getOfficerName(), 2,temp->getEquipStarPinzhi(), _card->getSsid());
}

void SGEquipInfoLayer::showEquipsInfo()
{
    SGEquipBox *equipsBox = SGEquipBox::create(this);
    SGMainManager::shareMain()->showBox(equipsBox);
}

void SGEquipInfoLayer::showBoxCall(cocos2d::CCObject *obj)
{
}
void SGEquipInfoLayer::disprotectListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::EquipmentUnProtectionResponse *response = (main::EquipmentUnProtectionResponse *)sr->m_msg;
    const char * msg = str_Equipslayer_str4;
    if (sr)
    {
        if (response->state() == 1)
        {
            msg = str_Equipslayer_str5;
            _card->setIsProt(0);
        }
        else if(response->state() == 0)
        {
            msg = str_Equipslayer_str4;
            CCLOG("取消保护失败");
        }        
    }
    SGMainManager::shareMain()->showMessage(msg);
}

void SGEquipInfoLayer::disProtectHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::EquipmentUnProtectionRequest *request = new main::EquipmentUnProtectionRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_DISPROTECT, request);
}
void SGEquipInfoLayer::aaaa()
{
    //leftTimes--;
    int temp=0;/////中间处理

    if(leftTimes==0)
    {
        temp=0;
        if(_card->getCurrLevel()==_card->getMaxLevel())
        {
            temp=1;
        }
        else
        {
            temp=0;
        }
    }
    else{
        temp=leftTimes;
    }
    

    int times = _card->getCurrLevel() - _before->getCurrLevel()-1;
    if (times)
    {
        int lvlchange = _before->getCurrLevel() + times-temp+1;
        int change = _before->getAtk() + ((_card->getAtk() - _before->getAtk())/times)*(times - leftTimes);
        int defchange =_before->getDef()+ ((_card->getDef() - _before->getDef())/times)*(times - leftTimes);
        int spdchange =_before->getSpeed()+ ((_card->getSpeed() - _before->getSpeed())/times)*(times - leftTimes);
        int morchange =_before->getMorale()+ ((_card->getMorale() - _before->getMorale())/times)*(times - leftTimes);
        
        if (leftTimes  == 0)
        {
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGEquipInfoLayer::removeSpX));
            this->runAction(CCSequence::create(CCDelayTime::create(1),call,NULL));
        }
        this->labAnimation(stuff_equipLevel, lvlchange);
        if (change >0)
        {
            this->labAnimation(stuff_atk,change);
        }
        if (defchange>0)
        {
            this->labAnimation(stuff_def, defchange);
        }
        if (spdchange>0)
        {
            this->labAnimation(stuff_speed, spdchange);
        }
        if (morchange>0)
        {
            this->labAnimation(stuff_mor, morchange);
        }
        
    }
    else
    {
        if (leftTimes==0 &&  (_card->getCurrLevel() >_before->getCurrLevel()))
        {
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGEquipInfoLayer::removeSpX));
            this->runAction(CCSequence::create(CCDelayTime::create(1),call,NULL));
        }
    }
    
    //地圈
    CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/qh_000.scml", true, true);
    fermEffect2->setanimaID(0);
    fermEffect2->setisloop(false);
    fermEffect2->setPosition(pos);
    //光圈
    CCSpriterX *fermEffect3 = CCSpriterX::create("animationFile/qh_001.scml", true, true);
    fermEffect3->setanimaID(0);
    fermEffect3->setisloop(false);
    fermEffect3->setPosition(pos);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    fermEffect2->setScale(s.width/768);
    fermEffect3->setScale(s.width/768);
    addChild(fermEffect2, 1, 2);
    addChild(fermEffect3, 4, 3);
    
    fermEffect2->play();
    fermEffect3->play();
    
}
void SGEquipInfoLayer::setcantouch()
{
    this->setIsCanTouch(true);
}
void SGEquipInfoLayer::setcanttouch()
{
    this->setIsCanTouch(false);
    runAction(CCSequence::create(CCDelayTime::create(lvlupAct->getAnimaTimeLength(0, 0)),CCCallFunc::create(this, callfunc_selector(SGEquipInfoLayer::playAction)),NULL));
}
void SGEquipInfoLayer::removeSpX()
{
    if (!isback)
    {
        isbreak = true;
        if (_card->getCurrLevel() > _before->getCurrLevel())
        {
            SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(this, _card,_before,2,isfromfirst);
            SGMainManager::shareMain()->showBox(LvlUpBox);
        }
        else
        {
            SGMainManager::shareMain()->showStrengLayer(_card, 2,isfromfirst);
        }
    }
}
bool SGEquipInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (enterType == 101)
    {
        backHandler();
    }
    if (isbreak)
    {
        return false;
    }
    if (enterType== 10000)
    {
        if (!m_break)
        {
            isback = true;
            this->setbreakAction(true);
            this->removeChildByTag(2, true);
            this->removeChildByTag(3, true);
            this->setlabelValue(stuff_equipLevel, _card->getCurrLevel(),_card->getMaxLevel());
            this->setlabelValue(stuff_exp, _card->getCurrExp(),_card->getMaxExp());
            this->setlabelValue(stuff_atk, _card->getAtk());
            this->setlabelValue(stuff_def, _card->getDef());
            this->setlabelValue(stuff_round, _card->getRound());
            this->setlabelValue(stuff_speed, _card->getSpeed());
            this->setlabelValue(stuff_mor, _card->getMorale());

        if (_card->getCurrLevel() > _before->getCurrLevel())
        {
            SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(this, _card,_before,2,isfromfirst);
            SGMainManager::shareMain()->showBox(LvlUpBox);
        }
        return true;
    }
    else
    {
        if (isback && (_card->getCurrLevel() == _before->getCurrLevel()))
        {
            backHandler();
        }
        return false;
    }
  }
    return false;
}

void SGEquipInfoLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, true);
}

void SGEquipInfoLayer::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void SGEquipInfoLayer::playAction()
{
    CCString* skillstate = (CCString*)Direc->objectForKey("skillup");
    SGMainManager::shareMain()->showMessage(skillstate->getCString());
}
void SGEquipInfoLayer::equipprotectListener(CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::EquipmentProtectionResponse *response = (main::EquipmentProtectionResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            _card->setIsProt(1);

        }
        }else
        {
            CCLOG("设为保护失败");
        }
        SGMainManager::shareMain()->showMessage(str_Equipslayer_str6);

}




//////////////////////////////////////////////////// 原出处 sggeneralinfolayer.cpp
void SGEquipInfoLayer::changeexplabel()
{
//    
//      labelExp->setVisible(true);
//    //currexp为当前经验值应该长到哪  exp为正在不断增加的经验条经验值
//    if (exp<=currexp) {
//        CCString *str = CCString::createWithFormat("%d/%d",exp,max);
//        labelExp->setString(str->getCString());
//        float rate = exp/(float)max;
//        expprog->setPercentage(rate*100);
//        
//        exp+=max*0.1;
//    
//        
//        CCSequence *seq = CCSequence::create(CCDelayTime::create(.000000000001),CCCallFuncN::create(this,callfuncN_selector(SGEquipInfoLayer::changeexplabel)),NULL);
//        labelExp->runAction(seq);
//        
//    }
//    else
//    {     ///最后一次
//        
//        if(leftTimes==0)
//        {
//            float rate = _card->getCurrExp()/(float)max;
//            expprog->setPercentage(rate*100);
//            
//            CCString *str = CCString::createWithFormat("%d/%d",_card->getCurrExp(),max);
//            labelExp->setString(str->getCString());
//            if(_card->getCurrLevel()==_card->getMaxLevel())
//            {
//                this->setlabelValue(stuff_equipLevel, _card->getCurrLevel(), 0);///
//            }
//            
//        }
//        else
//        {
//            //  float rate = _card->getCurrExp()/(float)max;
//            expprog->setPercentage(100);
//            
//            CCString *str = CCString::createWithFormat("%d/%d",max,max);
//            labelExp->setString(str->getCString());
//            
//            if(leftTimes==0)
//            {
//                if(_card->getCurrLevel()==_card->getMaxLevel())
//                {
//                    this->setbreakAction(true);
//                    this->setlabelValue(stuff_equipLevel, _card->getCurrLevel(), 0);
//                }
//            }
//            
//        }
//        
//    }
    
}

//定制的经验条动画启动
void SGEquipInfoLayer::progressAction(CCProgressTimer *m_progress,int times,SGBaseMilitaryCard *card,CCCallFuncN* call)
{
    int type=0;////
    _call = call;
    leftTimes = times;
    float per = m_progress->getPercentage();
    float time = 1;
    if (per != 100)
    {
        time = 0.01*(100 - per);
    }
    if (leftTimes == 0)
    {
        max = _card->getMaxExp();
        currexp = (float)_card->getCurrExp();
        
        if (_card->getCurrLevel()-_before->getCurrLevel()==0) {  ////单条第一次
            exp = _before->getCurrExp();
        }
        else    //多条最后一次
        {
            exp=0;
        }
        m_progress->runAction(CCCallFuncN::create(this,callfuncN_selector(SGEquipInfoLayer::changeexplabel)));
    }
    else
    {    ////多条第一次
        if (_card->getCurrLevel()-_before->getCurrLevel() == leftTimes)
        {
            exp = _before->getCurrExp();
            currexp = max = _before->getMaxExp();
        }
        else  ////多条其他次
        {
            exp = 0;
            type = SGStaticDataManager::shareStatic()->getStrengEquipType(_before->getItemId());
            int currLvl = _card->getCurrLevel();
            SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl-leftTimes);//比如currLvl为5级 此句话获得5级升6级所需经验值
            max = num->getExpValue();
            currexp = num->getExpValue();
        }
        CCAction *action =  CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(SGEquipInfoLayer::changeexplabel)),CCDelayTime::create(1),CCCallFuncND::create(this, callfuncND_selector(SGEquipInfoLayer::progressCallback),card),call,NULL);
        m_progress->runAction(action);
    }
    
}

void SGEquipInfoLayer::progressCallback(CCNode*node,SGBaseMilitaryCard *card)
{
    
    if (m_break) {
        return;
    }
    leftTimes--;
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_equipInfoLayer);
    CCSprite *waifaguang = CCSprite::createWithSpriteFrameName("waifaguang.png");
    this->addChild(waifaguang,9);
    CCFadeOut *fade = CCFadeOut::create(0.2);
    waifaguang->setAnchorPoint(ccp(.5,.5));
    waifaguang->setScaleX(180/210.0);
    waifaguang->runAction(CCSequence::create(fade,CCDelayTime::create(0.2),CCCallFuncN::create(this, callfuncN_selector(SGEquipInfoLayer::callBack)),NULL));
    waifaguang->setPosition(ccpAdd(node->getPosition(), ccp(-2,0)));
}

void SGEquipInfoLayer::showjitian()
{
    main::RewardGodPrayEntryRequest *request = new main::RewardGodPrayEntryRequest();
    SGSocketClient::sharedSocketClient()->send(MSG_REWARD_GODENTER, request);
}


void SGEquipInfoLayer::onLootSourceBtnClicked(CCObject * obj)
{
    
    if (SGGuideManager::shareManager()->limitlayer && SGGuideManager::shareManager()->isGuide)
    {
        if (SGGuideManager::shareManager()->limitlayer->getModel())
        {
            //对话的时候不再响应下面的按钮
            if (SGGuideManager::shareManager()->limitlayer->getModel()->getModelType() == 1)
                return;
        }
    }
    
    int needNum[12] = {0};
    CCArray *materialArray = CCArray::create();
    //获取卡牌的转生条件
    materialArray = this->getMaterialDependency(needNum);
    
    int tag = ( (SGButton *)obj)->getTag();
    int mataerialId = ( (SGMaterialDataModel *)materialArray->objectAtIndex(tag) )->getMaterialId();
    

    SGLootWiKiMainLayer * lootWiKiMainLayer = SGLootWiKiMainLayer::create(mataerialId , 1);
    SGMainManager::shareMain()->showBox(lootWiKiMainLayer);
}
                        
                        
                        
