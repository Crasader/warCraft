//
//  SGSoldierInfoLayer.cpp
//  GameSanGuo
//  
//  Created by 陈雪龙 on 13-1-9.
//
//

#include "SGSoldierInfoLayer.h"
#include "SGMainManager.h"
#include "SGButton.h"
#include "cocos-ext.h"
#include "SGSkillInfoBox.h"
#include "SGStaticDataManager.h"
#include "SGStrengLvlUpBox.h"
#include "SGMainLayer.h"
//#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGSoldierinfoEffect.h"
#include "SGRankCell.h"
#include "SGRankLayer.h"
#include "SGStringConfig.h"

SGSoldierInfoLayer::SGSoldierInfoLayer()
:
isfromfirst(false),
_before(NULL),
_card(NULL),
enterType(0),
pos(CCPointZero),
expprog(NULL),
leftTimes(0),
isback(false),
isbreak(false),
admsBkg(NULL),
skillLayer(NULL),
strengBtn(NULL),
advanceBtn(NULL),
sellBtn(NULL)
,soilderLimitLevel(0)
{
    isback = false;
    isbreak = false;
}
SGSoldierInfoLayer::~SGSoldierInfoLayer()
{
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_soldierInfoLayer);

    CCLOG("~SGGeneralInfoLayer");
    CC_SAFE_RELEASE(_card);
    //CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("animationFile/qhtexiao.plist");
    if (_before) {
        _before->release();
    }
    if (Direc) {
        Direc->release();
    }
}
SGSoldierInfoLayer *SGSoldierInfoLayer::create(SGSoldierCard *card_, int type,SGSoldierCard *before,bool isfirst,CCDictionary *dic)
{
    SGSoldierInfoLayer *soldierInfoLayer = new SGSoldierInfoLayer();
    if (soldierInfoLayer && soldierInfoLayer->init(NULL, sg_soldierInfoLayer))
    {
        soldierInfoLayer->Direc = dic;
        soldierInfoLayer->_before = before;
        soldierInfoLayer->isfromfirst = isfirst;
        soldierInfoLayer->enterType = type;
        soldierInfoLayer->_card = card_;
        soldierInfoLayer->_card->retain();
        soldierInfoLayer->initView();
        soldierInfoLayer->autorelease();
        return soldierInfoLayer;
    }
    CC_SAFE_DELETE(soldierInfoLayer);
    return NULL;
}
void SGSoldierInfoLayer::initView()
{
    if (enterType == 10000 || enterType==10001) {
        SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        layer->setIsCanTouch(false);
        _before->retain();
    }
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float btmheight= SGMainManager::shareMain()->getBottomHeight();
    //float frameheight = skewingY(54);
    float frameheight1 = skewingY(81);
    float hgt = skewingY(244);
    if (s.height == 1136) {
        CCLOG("1111");
        //frameheight = skewing(54);
        frameheight1 = skewing(81);
        hgt = skewing(244);
    }
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-3303.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist" , RES_TYPE_LAYER_UI, sg_soldierInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/ofcNewUi.plist", RES_TYPE_LAYER_UI, sg_soldierInfoLayer);//提供四属性值图
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_soldierInfoLayer); //提供横条图

    
    soilderLimitLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
    
    int type = SGStaticDataManager::shareStatic()->getStrengCardType(CCString::create(_card->getClassName()), _card->getItemId());
    int cardLimitExp = 0;
    if (_card->getCurrLevel() >= soilderLimitLevel)
    {
        cardLimitExp  = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, _card->getCurrLevel())->getExpValue();
    }
    else
    {
       cardLimitExp  = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, _card->getCurrLevel()+1)->getExpValue();
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
    
//    //地圈
//    CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/qh_000.scml", true, true);
//    fermEffect2->setanimaID(0);
//    fermEffect2->setisloop(true);
//    addChild(fermEffect2, 1, 2);
//    //光圈
//    CCSpriterX *fermEffect3 = CCSpriterX::create("animationFile/qh_001.scml", true, true);
//    fermEffect3->setanimaID(0);
//    fermEffect3->setisloop(true);
//    addChild(fermEffect3, 4, 3);
    
    CCString *str = CCString::createWithFormat("%d.png",_card->getItemId());
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    this->addChild(item,2);
    item->setAnchorPoint(ccp(0.5, 0));
    CCSprite *bigbg = NULL;
    if (s.height == 960) {
        frameSize = CCRectMake(0, 0 , 768, 413);
        this->setItemID("pad装备底图.png",true);
        fermEffect->setScale(s.width/768);
        fermEffect1->setScale(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287+26)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -365)));
    }
    else if (s.height == 1136) {
        frameSize = CCRectMake(0, 520 -(s.height - hgt*1.13 - title_bg->getContentSize().height), 768, (s.height - hgt*1.13 - title_bg->getContentSize().height));
        this->setItemID("carditembg.png",true);

        fermEffect->setScaleX(s.width/768);
        fermEffect1->setScaleX(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -361+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -361+26)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -450)));
     }else
    {
        bigbg = CCSprite::createWithSpriteFrameName("pad装备底图.png");
        this->addChild(bigbg,-100);
        bigbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - bigbg->getContentSize().height*.45)));
        fermEffect->setPosition(ccpAdd(bigbg->getPosition(), ccp(-bigbg->getContentSize().width*.41, bigbg->getContentSize().height*.02 -bigbg->getContentSize().height*.098)));
        fermEffect1->setPosition(ccpAdd(bigbg->getPosition(), ccp(bigbg->getContentSize().width*.41, bigbg->getContentSize().height*.02-bigbg->getContentSize().height*.098)));
        item->setPosition(ccpAdd(bigbg->getPosition(), ccp(0, -bigbg->getContentSize().height*.33)));
    }
    fermEffect->play();
    fermEffect1->play();
    this->setstar(_card->getCurrStar());
       //    SGSoldierDataModel *oCard = SGStaticDataManager::shareStatic()->getSoliderById(card->getItemId());

    this->setTitle(_card->getOfficerName()->getCString());
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGSoldierInfoLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSoldierInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));

    CCSprite* jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
    jinbian->setPosition(ccp(s.width/2, hgt*1.13+1));
    jinbian->setScaleX(s.width/jinbian->getContentSize().width);
    this->addChild(jinbian,5);
    
//    SGButton *strengBtn = SGButton::create("store_exchangebtnbg.png",
//                                           "font_streng.png",
//                                           this,menu_selector(SGSoldierInfoLayer::strengHandler),
//                                           ccp(0, 0),
//                                           false,
//                                           true);
	//hotBtnBg.png
	strengBtn = SGButton::createFromLocal("hotBtnBg.png", str_qianghua, this, menu_selector(SGSoldierInfoLayer::strengHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(strengBtn);
    strengBtn->setPosition(ccp(s.width*.295f, btmheight*.48 + strengBtn->getContentSize().height +2));
    
    float framewidth = skewing(283.5);
    
    menu->setZOrder(7);
    
//    SGButton *advanceBtn = SGButton::create("store_exchangebtnbg.png",
//                                           "font_jieshao.png",
//                                           this,menu_selector(SGSoldierInfoLayer::showinfo),
//                                           ccp(0, 0),
//                                           false,
//                                           true);
	advanceBtn = SGButton::createFromLocal("hotBtnBg.png", str_jieshao, this, menu_selector(SGSoldierInfoLayer::showinfo),CCPointZero,FONT_PANGWA,ccWHITE,32);
    this->addBtn(advanceBtn);
    advanceBtn->setVisible(false);
    //advanceBtn->setPosition(ccpAdd(strengBtn->getPosition(), ccp(framewidth * 0.35, 0)));
    
    sellBtn = SGButton::createFromLocal("hotBtnBg.png", str_Show, this, menu_selector(SGSoldierInfoLayer::sellHandler), CCPointZero, FONT_PANGWA, ccWHITE, 32, false, true);

    sellBtn->setPosition(ccpAdd(strengBtn->getPosition(), ccp(framewidth * 0.5, 0)));
    this->addBtn(sellBtn);
    
    
    
    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame1->setPreferredSize(CCSizeMake(568 +20, 93));
    
    CCSprite *frame1bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame1bg,6);
    frame1bg->setScaleX(590/frame1bg->getContentSize().width);
    frame1bg->setScaleY(93/frame1bg->getContentSize().height);
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+20, 197));
    
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg,6);
    frame2bg->setScaleX(590/frame2bg->getContentSize().width);
    frame2bg->setScaleY(197/frame2bg->getContentSize().height);
    
    this->addChild(frame1,7);
    frame1->setPosition(ccpAdd(jinbian->getPosition(), ccp(0, - frame1->getContentSize().height*.5 - 30)));
    this->addChild(frame2,7);
    frame2->setPosition(ccpAdd(frame1->getPosition(), ccp(0, -frame1->getContentSize().height*.5f - frame2->getContentSize().height*.5f - 11)));
    
    frame1bg->setPosition(frame1->getPosition());
    frame2bg->setPosition(frame2->getPosition());
    
    CCSprite* zwMore = CCSprite::createWithSpriteFrameName("ofc_moreinfo.png");
    CCPoint zwPos = ccp(frame2->getContentSize().width - 32, 30);
    zwMore->setPosition(zwPos);
    frame2->addChild(zwMore, 10);
    
    CCPoint czjFontPos = ccp(30, 161);
    SGCCLabelTTF *font_czji = SGCCLabelTTF::create(str_chengzhangji, FONT_BOXINFO, 35 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_czji->setColor(ccc3(0xff, 0xf0, 0x00));
    font_czji->setAnchorPoint(ccp(0,0.5));
    font_czji->setPosition(czjFontPos);
    frame2->addChild(font_czji,100);
    
    CCPoint czjnameName = ccp(30+60, 121);
    SGCCLabelTTF *font_czjn = SGCCLabelTTF::create(str_wu_chengzhangji, FONT_BOXINFO, 30);
//    font_czjn->setColor(ccc3(0xff, 0xf0, 0x00));
    font_czjn->setAnchorPoint(ccp(0,0.5));
    font_czjn->setPosition(czjnameName);
    frame2->addChild(font_czjn,100);
    
    CCPoint czjlevelPos = ccp(30+60+150, 121);
    SGCCLabelTTF *font_czjl = SGCCLabelTTF::create("Lv1", FONT_BOXINFO, 30);
    //    font_czjn->setColor(ccc3(0xff, 0xf0, 0x00));
    font_czjl->setAnchorPoint(ccp(0,0.5));
    font_czjl->setPosition(czjlevelPos);
//    frame2->addChild(font_czjl,100);
    
    CCPoint tfjFontPos = ccp(30, 81);
    SGCCLabelTTF *font_tfji = SGCCLabelTTF::create(str_tianfuji, FONT_BOXINFO, 35 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_tfji->setColor(ccc3(0xff, 0xf0, 0x00));
    font_tfji->setAnchorPoint(ccp(0,0.5));
    font_tfji->setPosition(tfjFontPos);
    frame2->addChild(font_tfji,100);
    
    CCPoint tfjNameFontPos = ccp(30+60, 41);
    SGCCLabelTTF *font_tfjn = SGCCLabelTTF::create(str_wu_tianfuji, FONT_BOXINFO, 30);
//    font_tfjn->setColor(ccc3(0xff, 0xf0, 0x00));
    font_tfjn->setAnchorPoint(ccp(0,0.5));
    font_tfjn->setPosition(tfjNameFontPos);
    frame2->addChild(font_tfjn,100);
    
    if (_card->getsoldiertalentskill()!=0) {
        //CCLOG("%d",_card->getsoldiertalentskill());
        SGSoldierSkillDataModel *model = SGStaticDataManager::shareStatic()->getSoldierSkillById(_card->getsoldiertalentskill());
        if (model) {
            font_tfjn->setString(model->getSkillName()->getCString());
        }
    }
    int growid = SGStaticDataManager::shareStatic()->getSoldierGrowSkillid(_card->getCurrLevel(),_card->getItemId());
    if (growid!=0) {
        SGSoldierSkillDataModel *model = SGStaticDataManager::shareStatic()->getSoldierSkillById(growid);
        if (model) {
            font_czjn->setString(model->getSkillName()->getCString());
            CCString *str = CCString::createWithFormat("Lv%d",model->getSkillLevel());
            font_czjl->setString(str->getCString());
            font_czjl->setPosition(ccpAdd(font_czjn->getPosition(),ccp(font_czjn->getContentSize().width+20, 0)));
        }
    }

    
    
    skillLayer = SGTouchLayer::create(frame2->getContentSize().width, frame2->getContentSize().height, 0 /*, ccc4(0, 255, 0, 255)*/);
    skillLayer->ignoreAnchorPointForPosition(false);
    skillLayer->setButtonMode( this, pf_callback_selector(SGSoldierInfoLayer::showSkillInfo) );
    this->addChild(skillLayer, 10);
    skillLayer->setPosition(frame2->getPosition());
    
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
    
    admsBkg = CCSprite::createWithSpriteFrameName("store_redbg.png");//四属性红条
    admsBkg->setScaleY(44.0 / 87.0);
    this->addChild(admsBkg, 5);
    
    int po = -387;
    if (s.height == 1136) {
        po = - 530-35;
    }
    else if (s.height!=960)
    {
        po =-387-27;
        admsBkg->setScaleX(s.width/640);
    }
    admsBkg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, po)));
    
    
    
    float move = skewing(46);
    float move1 = skewing(15);
    this->addLabel(stuff_lvl,ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move - 25,frame1->getPosition().y + frame1->getContentSize().height/5.5));
    this->addLabel(stuff_exp, ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move- 25,frame1->getPosition().y - frame1->getContentSize().height/5));
    
    float admsOffX = admsBkg->getContentSize().width /5;
    float admsOffY = 0; //全幅和半幅偏移
    CCPoint atkPos = ccpAdd( admsBkg->getPosition(), ccp(-admsOffX*2, admsOffY) );
    CCPoint defPos = ccpAdd( admsBkg->getPosition(), ccp(-admsOffX+30, admsOffY) );
    CCPoint morPos = ccpAdd( admsBkg->getPosition(), ccp(admsOffX-70, admsOffY) );
    CCPoint speedPos = ccpAdd( admsBkg->getPosition(), ccp(admsOffX*2-40, admsOffY) );
    CCPoint roundPos = ccpAdd(SGLayout::getPoint(kUpRight), ccp(-100, -title_bg->getContentSize().height - 100));
    CCPoint racePos = ccpAdd(roundPos, ccp(0, -30));
    this->addLabel(stuff_atk, atkPos);
    this->addLabel(stuff_def, defPos);
    this->addLabel(stuff_mor, morPos);
    this->addLabel(stuff_speed, speedPos);
    this->addLabel(stuff_round,roundPos);
    this->addLabel(stuff_area,racePos);
    
//    this->addLabel(stuff_def,ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 25,frame2->getPosition().y));
//    this->addLabel(stuff_atk, ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 25,frame2->getPosition().y + frameheight1 * .3));
//    this->addLabel(stuff_round,ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 25,frame2->getPosition().y - frameheight1 * .3));
//    this->addLabel(stuff_speed, ccp(s.width/2 + move*.6-10,frame2->getPosition().y + frameheight1 * .3));
//    this->addLabel(stuff_mor, ccp(s.width/2 + move*.6-10,frame2->getPosition().y));
    
    CCSprite *expbg = CCSprite::createWithSpriteFrameName("card_jingyan_bg.png");
    this->addChild(expbg,7);
    expbg->setPosition(ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move*1.2 + expbg->getContentSize().width*.5-1,frame1->getPosition().y - frame1->getContentSize().height/5.5 +1));

    expprog = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("card_jingyan.png"));
    //    expprog->setAnchorPoint(ccp(0, 0.5));
    expprog->setType(kCCProgressTimerTypeBar);
    expprog->setMidpoint(ccp(0,0));
    expprog->setBarChangeRate(ccp(1, 0));
    expprog->setPosition(expbg->getPosition());
    this->addChild(expprog,8);
    
    //////////////////////kanata
    labelExp=SGCCLabelTTF::create(CCString::createWithFormat("hehe")->getCString(),FONT_BOXINFO,32 , ccWHITE);
    labelExp->setPosition(expprog->getPosition());
    //modify by:zyc. merge into create.
    //labelExp->setColor(ccWHITE);
    labelExp->setVisible(false);
    this->addChild(labelExp,9999);
    /////////////////////
    
    if (enterType == 10) {
        strengBtn->setVisible(false);
        advanceBtn->setVisible(false);
    }
    if(enterType==24)
    {
        strengBtn->setVisible(false);
        advanceBtn->setVisible(false);
        sellBtn->setVisible(false);
    }
    if(enterType==25)//竞技场
    {
        strengBtn->setVisible(false);
        advanceBtn->setVisible(false);
        sellBtn->setVisible(false);
    }
    
    this->setCardType(99);
    
    if (enterType == 10000)
    {
        strengBtn->setVisible(false);
        advanceBtn->setVisible(false);
        sellBtn->setVisible(false);
        if (s.height == 960) {

            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -430));
            
        }
        else if (s.height == 1136) {
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(1.5, -530));
            
        }else
        {
            pos = ccpAdd(bigbg->getPosition(), ccp(1.5, -bigbg->getContentSize().height*.365-bigbg->getContentSize().height*.195));
        }
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_soldierInfoLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.13)/2, s.width, hgt*1.13);
        
        //bg1->cocos2d::CCNode::setScale(r.size.width / bg1->getContentSize().width, r.size.height / bg1->getContentSize().height);
        bg1->setTextureRect(r);
        
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
        
        
        this->addValueLabel(stuff_lvl, _card->getCurrLevel(), 50, soilderLimitLevel,0,_before->getCurrLevel(),1);///kanata
        this->addValueLabel(stuff_atk, _before->getAtk(), move1,NULL,1);
        this->addValueLabel(stuff_def, _before->getDef(), move1,NULL,1);
        this->addValueLabel(stuff_round, _before->getRound(), move1+10,NULL,1);
        this->addValueLabel(stuff_speed, _before->getSpeed(), move1,NULL,1);
        this->addValueLabel(stuff_mor, _before->getMorale(), move1,NULL,1);
        this->addValueLabel(stuff_area, 1, move1+10,NULL,1);
        
        Direc->retain();
        int strengstate = ((CCString*)Direc->objectForKey("upstate"))->intValue();
        lvlupAct = CCSpriterX::create(CCString::createWithFormat("animationFile/success%d.scml",strengstate)->getCString(), true, true);
        lvlupAct->setanimaID(0);
        lvlupAct->setisloop(false);
        addChild(lvlupAct,100);
        lvlupAct->setPosition(ccpAdd(pos, ccp(0, 200)));
        lvlupAct->play();
        
        EFFECT_PLAY(MUSIC_35);

        int type = SGStaticDataManager::shareStatic()->getStrengCardType(CCString::create(_before->getClassName()), _before->getItemId());
        int beforeLimitExp  = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, soilderLimitLevel)->getexpAll();
        
        leftTimes = _card->getCurrLevel() - _before->getCurrLevel();
        if (beforeLimitExp == -1)
        {
            expprog->setPercentage(100);
        }
        else
        {
           float time = (_card->getCurrLevel() - _before->getCurrLevel())*0.9 + lvlupAct->getAnimaTimeLength(0, 0);
            this->runAction(CCSequence::create(CCCallFuncN::create(this, callfuncN_selector(SGSoldierInfoLayer::setcanttouch)),CCDelayTime::create(time + 0.012*(100 - expprog->getPercentage())),CCCallFuncN::create(this, callfuncN_selector(SGSoldierInfoLayer::setcantouch)),NULL));
            
            expprog->setPercentage(100*((float)_before->getCurrExp()/beforeLimitExp));
        }
    }else if (enterType==10001){
    
        strengBtn->setVisible(false);
        advanceBtn->setVisible(false);
        if (s.height == 960) {
            
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -430));
            
        }
        else if (s.height == 1136) {
            pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(1.5, -530));
            
        }else
        {
            pos = ccpAdd(bigbg->getPosition(), ccp(1.5, -bigbg->getContentSize().height*.365-bigbg->getContentSize().height*.195));
        }
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_soldierInfoLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        CCRect r = CCRectMake(384- s.width/2, 1136/2 - (hgt*1.13)/2, s.width, hgt*1.13);
        
        bg1->setTextureRect(r);
        //bg1->cocos2d::CCNode::setScale(r.size.width / bg1->getContentSize().width, r.size.height / bg1->getContentSize().height);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,5);
        
        this->addValueLabel(stuff_lvl, _before->getCurrLevel(), 50, soilderLimitLevel);
        
//        this->addValueLabel(stuff_atk, _before->getAtk(), move*1.3,NULL);
//        this->addValueLabel(stuff_def, _before->getDef(), move*1.3,NULL);
//        this->addValueLabel(stuff_round, _before->getRound(), move*1.3,NULL);
//        this->addValueLabel(stuff_speed, _before->getSpeed(), move*1.3,NULL);
//        this->addValueLabel(stuff_mor, _before->getMorale(), move*1.3,NULL);
        
   
        this->addValueLabel(stuff_atk, _before->getAtk(), move1,NULL,1);
        this->addValueLabel(stuff_def, _before->getDef(), move1,NULL,1);
        this->addValueLabel(stuff_round, _before->getRound(), move1+10,NULL,1);
        this->addValueLabel(stuff_speed, _before->getSpeed(), move1,NULL,1);
        this->addValueLabel(stuff_mor, _before->getMorale(), move1,NULL,1);
        this->addValueLabel(stuff_area, 1, move1+10,NULL,1);

    }
    else
    {
        
        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("info_redbg.png");
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setScaleX(s.width/bg1->getContentSize().width);
        bg1->setScaleY((hgt*1.13-btmheight*.68)/bg1->getContentSize().height);
        bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmheight*.68 )));
        this->addChild(bg1,-1);

        this->addValueLabel(stuff_lvl, _card->getCurrLevel(), 50,soilderLimitLevel);
        if (_card->getCurrLevel() >= soilderLimitLevel)
        {
            this->addValueLabel(stuff_exp, cardLimitExp, 50,cardLimitExp);
        }
        else
        {
            this->addValueLabel(stuff_exp, _card->getCurrExp(), 50,cardLimitExp);
        }
        this->addValueLabel(stuff_atk, _card->getAtk(), move1,NULL,1);
        this->addValueLabel(stuff_def, _card->getDef(), move1,NULL,1);
        this->addValueLabel(stuff_round, _card->getRound(), move1+10,NULL,1);
        this->addValueLabel(stuff_speed, _card->getSpeed(), move1,NULL,1);
        this->addValueLabel(stuff_mor, _card->getMorale(), move1,NULL,1);
        this->addValueLabel(stuff_area, 1, move1+10,NULL,1);

        if (_card->getCurrLevel() >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())
        {
            expprog->setPercentage(100);
        }
        else
        {
            expprog->setPercentage(100*((float)_card->getCurrExp()/cardLimitExp));
        }
        
        
        if (enterType == CHATSHOWENTRYTAG)
        {
            strengBtn->setVisible(false);
            advanceBtn->setVisible(false);
            sellBtn->setVisible(false);
        }

    }

     
}

void SGSoldierInfoLayer::showSkillInfo()
{
    CCLOG("<><><>显示技能!");
    EFFECT_PLAY(MUSIC_BTN);
    //    SGSkillInfoBox *skillInfoBox = SGSkillInfoBox::create(this, _card,isFriendGeneral==false?1:8);
    //    SGMainManager::shareMain()->showBox(skillInfoBox);
    
    SGSoldierinfoEffect *effect = SGSoldierinfoEffect::create(this, _card,enterType);
    SGMainManager::shareMain()->showBox(effect);
    
}

void SGSoldierInfoLayer::setIsCanTouch(bool iscan)
{
    strengBtn->setEnabled(iscan);
    advanceBtn->setEnabled(iscan);
    sellBtn->setEnabled(iscan);
    skillLayer->setIsWorking(iscan);

}



void SGSoldierInfoLayer::addLabel(stuffType type, cocos2d::CCPoint point)
{
    
    switch (type) {
        case stuff_lvl: //等级-文字
        {
            SGCCLabelTTF *lvl = SGCCLabelTTF::create(str_Class, FONT_BOXINFO, 28 , ccYELLOW);
            //modify by:zyc. merge into create.
            //lvl->setColor(ccc3(0xff, 0xf0, 0x00));
            this->addChild(lvl,9);
            lvl->setPosition(point);
//            lvl->setAnchorPoint(ccp(0, 0.5));
            lvl->setTag((int)type);
        }
            break;
        case stuff_exp: //经验-文字
        {
            SGCCLabelTTF *exp = SGCCLabelTTF::create(str_jingyan, FONT_BOXINFO, 28 , ccYELLOW);
            //modify by:zyc. merge into create.
            //exp->setColor(ccc3(0xff, 0xf0, 0x00));
            this->addChild(exp,9);
            exp->setPosition(point);
//            exp->setAnchorPoint(ccp(0, 0.5));
            exp->setTag((int)type);
        }
            break;
        case stuff_atk: //攻击-图标
        {
            CCSprite* atk = CCSprite::createWithSpriteFrameName("ofc_icon_atk.png");
            //            SGCCLabelTTF *atk = SGCCLabelTTF::create("攻击:", FONT_BOXINFO, 28, ccc3(0xff, 0xf0, 0x00));
            this->addChild(atk,9);
            atk->setPosition(point);
            atk->setTag((int)type);
            //            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            //            this->addChild(bg,8);
            //            bg->setPosition(ccpAdd(atk->getPosition(), ccp(130, 0)));
        }
            break;
        case stuff_def: //防御-图标
        {
            CCSprite* def = CCSprite::createWithSpriteFrameName("ofc_icon_def.png");
            //            SGCCLabelTTF *def = SGCCLabelTTF::create("防御:", FONT_BOXINFO, 28, ccc3(0xff, 0xf0, 0x00));
            this->addChild(def,9);
            def->setPosition(point);
            def->setTag((int)type);
            //            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            //            this->addChild(bg,8);
            //            bg->setPosition(ccpAdd(def->getPosition(), ccp(130, 0)));
        }
            break;
        case stuff_mor: //血量-图标
        {
            CCSprite* mor = CCSprite::createWithSpriteFrameName("ofc_icon_morale.png");
            //            SGCCLabelTTF *mor = SGCCLabelTTF::create("血量:", FONT_BOXINFO, 28, ccc3(0xff, 0xf0, 0x00));
            this->addChild(mor,9);
            mor->setPosition(point);
            mor->setTag((int)type);
            //            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            //            this->addChild(bg,8);
            //            bg->setPosition(ccpAdd(mor->getPosition(), ccp(130, 0)));
        }
            break;
        case stuff_speed: //速度-图标
        {
            CCSprite* speed = CCSprite::createWithSpriteFrameName("ofc_icon_speed.png");
            //            SGCCLabelTTF *speed = SGCCLabelTTF::create("速度:", FONT_BOXINFO, 28, ccc3(0xff, 0xf0, 0x00));
            this->addChild(speed,9);
            speed->setPosition(point);
            speed->setTag((int)type);
            //            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            //            this->addChild(bg,8);
            //            bg->setPosition(ccpAdd(speed->getPosition(), ccp(130, 0)));
        }
            break;
        case stuff_gov: //统御-文字
        {
            SGCCLabelTTF *gov = SGCCLabelTTF::create(str_GeneralInfoLayer_str2, FONT_BOXINFO, 28 , ccYELLOW);
            //modify by:zyc. merge into create.
            //gov->setColor(ccc3(0xff, 0xf0, 0x00));
            this->addChild(gov,1000);
            gov->setPosition(point);
            gov->setTag((int)type);
            //            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            //            this->addChild(bg,8);
            //            bg->setPosition(ccpAdd(gov->getPosition(), ccp(144, 0)));
            
        }
            break;
        case stuff_round: //蓄力-文字
        {
            SGCCLabelTTF *round = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 28 , ccYELLOW);
            //modify by:zyc. merge into create.
            //round->setColor(ccc3(0xff, 0xf0, 0x00));
            this->addChild(round,1000);
            round->setPosition(point);
            round->setTag((int)type);
            //            CCSprite *bg = CCSprite::createWithSpriteFrameName("labelbg.png");
            //            this->addChild(bg,8);
            //            bg->setPosition(ccpAdd(round->getPosition(), ccp(130, 0)));
        }
            break;
		case stuff_area: //武将占几格
		{
			SGCCLabelTTF *cell = SGCCLabelTTF::create(str_GeneralInfoLayer_str3, FONT_BOXINFO, 28 , ccYELLOW);
            //modify by:zyc. merge into create.
			//cell->setColor(ccc3(0xff, 0xf0, 0x00));
			this->addChild(cell, 1000);
			cell->setPosition(point);
			cell->setTag((int)type);
		}
			break;
        default:
            break;
    }
}



void SGSoldierInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (enterType == 1) {
        SGMainManager::shareMain()->showEmbattleLayer(0);
    }else if (enterType == 10) {
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_cardsLayer);
        SGMainManager::shareMain()->showLayer(layer);
        SGBaseLayer*mainlayer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        
        mainlayer->setIsCanTouch(true);
        
        
    }
    else if (enterType == 24) {//排行榜相关过来
        
        
        
      //  SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_soldierInfoLayer, true);
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_rank);
        SGBaseLayer*layer2 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_myFriend);
        SGBaseLayer*layer3 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_friendRequest);
        SGBaseLayer*layer4 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_searchFriendFinish);
        SGBaseLayer*layer5 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatLayer);
        SGBaseLayer*layer6 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bossbattleranklayer);
         SGBaseBox*layer7 = (SGBaseBox *)SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank);
        
        if(layer)
        {
            SGMainManager::shareMain()->showLayer(layer);
//            SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
//            sgnc->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGRankLayer::receivedetail));
        }
        if(layer2)
        {
            SGMainManager::shareMain()->showLayer(layer2);
        }
        if(layer3)
        {
            SGMainManager::shareMain()->showLayer(layer3);
        }
        if(layer4)
        {
            SGMainManager::shareMain()->showLayer(layer4);
        }
        if(layer5)
        {
            SGMainManager::shareMain()->showLayer(layer5);
        }
        if(layer6)
        {
            SGMainManager::shareMain()->showLayer(layer6);
        }
        if(layer7)
        {
            layer7->retain();
            layer7->removeFromParent();
            //layer5->addChild(layer7);
            
            SGMainManager::shareMain()->showBox(layer7);
            layer7->setVisible(true);
        }
        else
        {
        main::FriendInfoRequest *infoinfo = new main::FriendInfoRequest();
        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
        player->retain();
        int playerid=player->getplayerID();
        infoinfo->set_type(0);
        infoinfo->set_friendid(playerid);
        
//        SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
//        sgnc->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGRankCell::receivedetail));
        SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, infoinfo);
        
        }
        
        
        
        
        
        
        
        
    }
    else if(enterType == 25)//竞技场进来
    {
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_areanLayer);
        
        SGBaseBox*layer2 = (SGBaseBox *)SGMainManager::shareMain()->getMainScene()->getChildByTag(box_rank);
        if(layer)
        {
            SGMainManager::shareMain()->showLayer(layer);
        }
        if(layer2)
        {
            layer2->setVisible(true);
            if(layer)
                layer->setIsCanTouch(false);
        }

    }
    else if (enterType == 10000 || enterType==10001) {
        SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
        layer->setIsCanTouch(true);
        SGMainManager::shareMain()->showStrengLayer(_card,3,isfromfirst);
    }
    else if (enterType == CHATSHOWENTRYTAG ) {
        SGMainManager::shareMain()->closeChatShowLayer();
    }
    else{
    SGMainManager::shareMain()->showSoldierslayer();
    }
}
void SGSoldierInfoLayer::showinfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGSkillInfoBox *cantstrengBox = SGSkillInfoBox::create(this, NULL,4);
    SGMainManager::shareMain()->showBox(cantstrengBox);
}
void SGSoldierInfoLayer::strengHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_card->getCurrLevel() >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())
    {
        SGMainManager::shareMain()->showMessage(str_qianghua_tips1);
    }
    else
    {
        SGMainManager::shareMain()->showStrengLayer(_card,3,isfromfirst);
    }
}
void SGSoldierInfoLayer::aaaa()
{
//    leftTimes--;
//    int times = _card->getCurrLevel() - _before->getCurrLevel();
    
    
    int temp=0;/////中间处理
    if(leftTimes==0)
    {
        temp=0;
    }
    else{
        temp=leftTimes;
    }
    


    int times = _card->getCurrLevel() - _before->getCurrLevel()-1;
    if (times) {
//        int lvlchange = _before->getCurrLevel() + ((_card->getCurrLevel() - _before->getCurrLevel())/times)*(times - leftTimes);
        int lvlchange = _before->getCurrLevel() + times-temp+1;
        int change = _before->getAtk() + ((_card->getAtk() - _before->getAtk())/times)*(times - leftTimes);
        int defchange =_before->getDef()+ ((_card->getDef() - _before->getDef())/times)*(times - leftTimes);
        //int spdchange =_before->getSpeed()+ ((_card->getSpeed() - _before->getSpeed())/times)*(times - leftTimes);
        int morchange =_before->getMorale()+ ((_card->getMorale() - _before->getMorale())/times)*(times - leftTimes);
        
        if (leftTimes  == 0) {
            change = _card->getAtk();
            defchange = _card->getDef();
            //spdchange = _card->getSpeed();
            morchange = _card->getMorale();

            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGSoldierInfoLayer::removeSpX));
            this->runAction(CCSequence::create(CCDelayTime::create(1),call,NULL));
            
        }
        this->labAnimation(stuff_lvl, lvlchange);
        this->labAnimation(stuff_atk,change);
        this->labAnimation(stuff_def, defchange);
//        this->labAnimation(stuff_speed, spdchange);
        this->labAnimation(stuff_mor, morchange);
    }else
    {
        if (leftTimes==0 &&  (_card->getCurrLevel() >_before->getCurrLevel())) {
            CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGSoldierInfoLayer::removeSpX));
            this->runAction(CCSequence::create(CCDelayTime::create(1),call,NULL));
        }
        int type = SGStaticDataManager::shareStatic()->getStrengCardType(CCString::create(_card->getClassName()), _card->getItemId());
        int cardLimitExp  = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, soilderLimitLevel)->getExpValue();
        
        this->addValueLabel(stuff_exp, _card->getCurrExp(), expprog->getContentSize().width*.55,cardLimitExp);
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
void SGSoldierInfoLayer::setcantouch()
{
    this->setIsCanTouch(true);
}
void SGSoldierInfoLayer::setcanttouch()
{
    this->setIsCanTouch(false);
    runAction(CCSequence::create(CCDelayTime::create(lvlupAct->getAnimaTimeLength(0, 0)),CCCallFunc::create(this, callfunc_selector(SGSoldierInfoLayer::playAction)),NULL));
}
void SGSoldierInfoLayer::removeSpX()
{
    if (!isback) {
        isbreak = true;
    if (_card->getCurrLevel() > _before->getCurrLevel())
    {
        SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(this, _card,_before,3,isfromfirst);
        SGMainManager::shareMain()->showBox(LvlUpBox);
    }else
    {
        SGMainManager::shareMain()->showStrengLayer(_card,3,isfromfirst);
    }
}
}
bool SGSoldierInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (isbreak) {
        return false;
    }
    if (enterType== 10000) {
        if (!m_break)
        {
            isback = true;
            this->setbreakAction(true);
            this->removeChildByTag(2, true);
            this->removeChildByTag(3, true);
            
            int type = SGStaticDataManager::shareStatic()->getStrengCardType(CCString::create(_card->getClassName()), _card->getItemId());
            int cardLimitExp  = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, soilderLimitLevel)->getExpValue();
            
            this->setlabelValue(stuff_lvl, _card->getCurrLevel(),soilderLimitLevel);
            this->setlabelValue(stuff_exp, _card->getCurrExp(),cardLimitExp);
            this->setlabelValue(stuff_atk, _card->getAtk());
            this->setlabelValue(stuff_def, _card->getDef());
            this->setlabelValue(stuff_round, _card->getRound());
            this->setlabelValue(stuff_speed, _card->getSpeed());
            this->setlabelValue(stuff_mor, _card->getMorale());
            
            expprog->stopAllActions();
            if (_card->getCurrLevel() >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel()) {
                expprog->setPercentage(100);
            }
            else
            {

                int type = SGStaticDataManager::shareStatic()->getStrengCardType(CCString::create(_card->getClassName()), _card->getItemId());
                int cardLimitExp  = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, soilderLimitLevel)->getExpValue();
                    
                expprog->setPercentage(100*((float)_card->getCurrExp()/cardLimitExp));
                
            }
            if (_card->getCurrLevel() > _before->getCurrLevel())
            {
                SGStrengLvlUpBox *LvlUpBox = SGStrengLvlUpBox::create(this, _card,_before,3,isfromfirst);
                SGMainManager::shareMain()->showBox(LvlUpBox);
            }
            return true;
        }else
        {
            if (isback && (_card->getCurrLevel() == _before->getCurrLevel())) {
                SGMainManager::shareMain()->showStrengLayer(_card,3,isfromfirst);
            }
            
            return false;
        }
    }
    return false;
}
void SGSoldierInfoLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, true);
}

void SGSoldierInfoLayer::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void SGSoldierInfoLayer::playAction()
{
    CCCallFuncN *call = CCCallFuncN::create(this, callfuncN_selector(SGSoldierInfoLayer::aaaa));
    this->progressAction(expprog, leftTimes, _card, call);
    
    CCString* skillstate = (CCString*)Direc->objectForKey("skillup");
    
    SGMainManager::shareMain()->showMessage(skillstate->getCString());

}


///////////////////////////////////////////////////  摘自sggeneralinfolayer.cpp
void SGSoldierInfoLayer::changeexplabel()
{
    
    labelExp->setVisible(true);
    //currexp为当前经验值应该长到哪  exp为正在不断增加的经验条经验值
    if (exp<=currexp) {
        CCString *str = CCString::createWithFormat("%d/%d",exp,max);
        labelExp->setString(str->getCString());
        float rate = exp/(float)max;
        expprog->setPercentage(rate*100);
        
        exp+=max*0.1;
        
        
        CCSequence *seq = CCSequence::create(CCDelayTime::create(.000000000001),CCCallFuncN::create(this,callfuncN_selector(SGSoldierInfoLayer::changeexplabel)),NULL);
        labelExp->runAction(seq);
        
    }
    else
    {     ///最后一次
        
        if(leftTimes==0)
        {
            float rate = _card->getCurrExp()/(float)max;
            expprog->setPercentage(rate*100);
            
            CCString *str = CCString::createWithFormat("%d/%d",_card->getCurrExp(),max);
            labelExp->setString(str->getCString());
            if(_card->getCurrLevel() >= soilderLimitLevel)
            {
                this->setlabelValue(stuff_lvl, _card->getCurrLevel(), 0);///
            }
            
        }
        else
        {
            //  float rate = _card->getCurrExp()/(float)max;
            expprog->setPercentage(100);
            
            CCString *str = CCString::createWithFormat("%d/%d",max,max);
            labelExp->setString(str->getCString());
            if(leftTimes==0)
            {
                if(_card->getCurrLevel() >= soilderLimitLevel)
                {
                    this->setbreakAction(true);
                    this->setlabelValue(stuff_lvl, _card->getCurrLevel(), 0);
                }
            }
            
        }
        
    }
    
}

//定制的经验条动画启动
void SGSoldierInfoLayer::progressAction(CCProgressTimer *m_progress,int times,SGBaseMilitaryCard *card,CCCallFuncN* call)
{
    
    int type=0;////
    _call = call;
    leftTimes = times;
    //    float max = 100;
    float per = m_progress->getPercentage();
    float time = 1;
    if (per != 100) {
        time = 0.01*(100 - per);
    }
    //    CCAction *action2 = CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(SGGeneralInfoLayer::changeexplabel)),NULL);
    if (leftTimes == 0) {
        
        int type = SGStaticDataManager::shareStatic()->getStrengCardType(CCString::create(_card->getClassName()), _card->getItemId());
        max = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, soilderLimitLevel)->getexpAll();
        currexp = (float)_card->getCurrExp();
        
        if (_card->getCurrLevel()-_before->getCurrLevel()==0) {  ////单条第一次
            exp = _before->getCurrExp();
        }
        else    //多条最后一次
        {
            exp=0;
        }
        
        //        CCAction *action1 = CCSpawn::create(CCProgressTo::create(time, max),action2);
        m_progress->runAction(CCCallFuncN::create(this,callfuncN_selector(SGSoldierInfoLayer::changeexplabel)));
    }
    else
    {    ////多条第一次
        if (_card->getCurrLevel()-_before->getCurrLevel() == leftTimes) {
            exp = _before->getCurrExp();
            
            int type = SGStaticDataManager::shareStatic()->getStrengCardType(CCString::create(_before->getClassName()), _before->getItemId());
            int beforeLimitExp = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, soilderLimitLevel)->getexpAll();
            
            currexp = max = beforeLimitExp;
            
        }
        else  ////多条其他次
        {
            exp = 0;

            type = SGStaticDataManager::shareStatic()->getStrengSoldierType(_card->getItemId());
            
//            SGSoldierDataModel *officer = SGStaticDataManager::shareStatic()->getSoliderById(_card->getItemId());
            
//            if (officer->getSoldierExpType()->isEqual(CCString::create("A"))) {
//                type = 0;
//            }
//            else if (officer->getSoldierExpType()->isEqual(CCString::create("B"))) {
//                type = 1;
//            }
//            else if (officer->getSoldierExpType()->isEqual(CCString::create("C"))) {
//                type = 2;
//            }
//            else if (officer->getSoldierExpType()->isEqual(CCString::create("D"))) {
//                type = 3;
//            }

            int currLvl = _card->getCurrLevel();
            SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, currLvl-leftTimes);//比如currLvl为5级 此句话获得5级升6级所需经验值
            max = num->getExpValue();
            currexp = num->getExpValue();
            
        }
        CCAction *action =  CCSequence::create(CCCallFuncN::create(this,callfuncN_selector(SGSoldierInfoLayer::changeexplabel)),CCDelayTime::create(1),CCCallFuncND::create(this, callfuncND_selector(SGSoldierInfoLayer::progressCallback),card),call,NULL);
        m_progress->runAction(action);
    }
    
}

void SGSoldierInfoLayer::progressCallback(CCNode*node,SGBaseMilitaryCard *card)
{
    
    if (m_break) {
        return;
    }
    leftTimes--;
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_stuffLayer);
    CCSprite *waifaguang = CCSprite::createWithSpriteFrameName("waifaguang.png");
    this->addChild(waifaguang,9);
    CCFadeOut *fade = CCFadeOut::create(0.2);
    waifaguang->setAnchorPoint(ccp(.5,.5));
    waifaguang->setScaleX(180/210.0);
    waifaguang->runAction(CCSequence::create(fade,CCDelayTime::create(0.2),CCCallFuncN::create(this, callfuncN_selector(SGSoldierInfoLayer::callBack)),NULL));
    waifaguang->setPosition(ccpAdd(node->getPosition(), ccp(-2,0)));
    
    if((leftTimes==1)&&(_card->getCurrLevel() >= soilderLimitLevel))
    {
        progressAction((CCProgressTimer*)node, leftTimes-1, card, _call);
        
    }
    else
    {
        progressAction((CCProgressTimer*)node, leftTimes, card, _call);
    }
}

void SGSoldierInfoLayer::sellHandler()
{
    SGMainManager::shareMain()->getMainLayer()->sendChatShowContent(_card->getOfficerName(), 3, 1, _card->getSsid());
}

























