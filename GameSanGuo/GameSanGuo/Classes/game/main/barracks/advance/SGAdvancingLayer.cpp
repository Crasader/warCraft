//
//  SGAdvancingLayer.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-7-22.
//
//

#include "SGAdvancingLayer.h"
#include "SGMainManager.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGStaticDataManager.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
//bool SGInfoLayer::init()
//{
//    if (!SGBaseShow::init(NULL, sg_advancetexiao)) {
//        return false;
//    }
//    return true;
//}


SGAdvancingLayer::SGAdvancingLayer():
isfromfirst(false),
isbreakdown(false),
cardType(0),
_card(NULL),
jinbian(NULL),
beforeitemid(0),
beforestar(0)
{
//    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animationFile/qhtexiao.plist");
//     CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("animationFile/wjzs.plist");
	
	ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/wjzs.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
}
SGAdvancingLayer::~SGAdvancingLayer()
{
    CCLOG("~SGAdvancingLayer");
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("animationFile/qhtexiao.plist");
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("animationFile/wjzs.plist");
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_advanceLayer);

    CC_SAFE_RELEASE(_card);
}
SGAdvancingLayer* SGAdvancingLayer::create(int itemid, SGBaseMilitaryCard *card_, int type,int star,bool isfromf)
{
    SGAdvancingLayer *generalLayer = new SGAdvancingLayer();
    generalLayer->_card = card_;
    generalLayer->_card->retain();
    generalLayer->cardType = type;
    generalLayer->isfromfirst = isfromf;
    generalLayer->beforeitemid = itemid;
    generalLayer->beforestar = star;
    if (generalLayer && generalLayer->init(NULL, sg_advancetexiao))
    {
        generalLayer->initView(itemid);
        generalLayer->autorelease();
        return generalLayer;
    }
    CC_SAFE_DELETE(generalLayer);
    return NULL;
}
void SGAdvancingLayer::initView(int itemid)
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();

	ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer2.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);

    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGAdvancingLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
	this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    menu->setZOrder(9);
 
    if (cardType == 1) {

        //12.05
        SGOfficerDataModel *of = SGStaticDataManager::shareStatic()->getOfficerById(itemid);
        
        SGMainManager::shareMain()->addOfficerPng(of->getIconId(),sg_advanceLayer);
    CCString *str = CCString::createWithFormat("officer_%d.png",of->getIconId());
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    item->setAnchorPoint(ccp(0.5,0));
    this->addChild(item,2,77);
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
    
    CCSprite *bigbg = NULL;
    if (s.height == 960) {
        frameSize = CCRectMake(0, 0 , 768, 466);
        this->setItemID("pad武将底图.png",true);
        fermEffect->setScale(s.width/768);
        fermEffect1->setScale(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -232)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -232)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,150)));
    }
    else if (s.height == 1136) {
        
        CCSprite* itemID = CCSprite::createWithSpriteFrameName("generalcardbg_ip5.png");
        this->addChild(itemID,-1);
        itemID->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - itemID->getContentSize().height/2)));
        fermEffect->setScaleX(s.width/768);
        fermEffect1->setScaleX(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -368 +26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -368+26)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,120)));
    }else
    {
        bigbg = CCSprite::createWithSpriteFrameName("pad武将底图.png");
        this->addChild(bigbg,-100);
        bigbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - bigbg->getContentSize().height*.45)));
        fermEffect->setPosition(ccpAdd(bigbg->getPosition(), ccp(-bigbg->getContentSize().width*.41, bigbg->getContentSize().height*.075)));
        fermEffect1->setPosition(ccpAdd(bigbg->getPosition(), ccp(bigbg->getContentSize().width*.41, bigbg->getContentSize().height*.075)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,155)));
        
    }
    fermEffect->play();
    fermEffect1->play();
       
            SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(itemid);
            //    this->setstar(_card->getCurrStar()-1);//传入的star为转生后的星级，应该减1得到原来的
            this->setstar(temp->getOfficerCurrStarLevel(),temp->getUpgradestar());
        if (_card->getCurrStar() !=beforestar) {
            if (_card->getOfficerName()->getCString()) {
                this->setTitle(_card->getOfficerName()->getCString());
            }
        }
        else
        {
                SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(beforeitemid);
                this->setTitle(temp->getOfficerName()->getCString());
        }
    
    float hgt = 0;
    if (s.height == 1136) {
        hgt = skewing(238);
    } else {
        hgt = skewingY(238);
    }
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);

    CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
    //CCRect r = CCRectMake(0, 1136/2 - (hgt*1.07)/2, bg1->getContentSize().width, hgt*1.07);
    bg1->setScaleX(s.width / bg1->getContentSize().width * 1.01);
    bg1->setScaleY(s.height/2 / bg1->getContentSize().height);

    //bg1->setTextureRect(r);
    bg1->setAnchorPoint(ccp(0.5, 0));
    bg1->setPosition(SGLayout::getPoint(kBottomCenter));
    this->addChild(bg1);
        
        jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
        jinbian->setPosition(ccp(s.width/2, bg1->getContentSize().height));
        jinbian->setScaleX(s.width/jinbian->getContentSize().width);
        this->addChild(jinbian,5);
    
    CCSpriterX *wjzs = CCSpriterX::create("animationFile/wjzs.scml", true, true);
    wjzs->setanimaID(0);
    wjzs->setisloop(false);
    addChild(wjzs, 10, 0);
    wjzs->setPosition(item->getPosition());
    wjzs->play();
        this->runAction(CCSequence::create(CCDelayTime::create(.48),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::action5)),CCDelayTime::create(0.75 - 0.48),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::action1)),NULL));
    }
    
    else
    {
        float hgt=skewingY(233);
        menu->setZOrder(10);
        if (s.height == 1136) {

            hgt = skewing(233);
        }
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);

        CCSprite *bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        //CCRect r = CCRectMake(0, 1136/2 - (hgt*1.18)/2, s.width, hgt*1.18);
        bg1->setScaleY(s.height/2 / bg1->getContentSize().height);
        bg1->setScaleX(s.width / bg1->getContentSize().width * 1.01);
        //bg1->setTextureRect(r);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1);

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
        
       
		
		SGEquipmentDataModel *eq = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
		SGMainManager::shareMain()->addEquipPng(eq->getIconId(),sg_advanceLayer);
        CCString *str = CCString::createWithFormat("equip%d.png",eq->getIconId());
		
        CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
        this->addChild(item,2,77);
        CCSprite *bigbg = NULL;
        item->setAnchorPoint(ccp(0.5, 0));
        if (s.height == 960) {
            frameSize = CCRectMake(0, 0 , 768, 413);
            this->setItemID("pad装备底图.png",true);
            fermEffect->setScale(s.width/768);
            fermEffect1->setScale(s.width/768);
            fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287 +26)));
            fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287 +26)));
            item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -365)));
            
        }
        else if (s.height == 1136) {
            frameSize = CCRectMake(0, 520 -(s.height - hgt*1.18 - title_bg->getContentSize().height), 768, (s.height - hgt*1.18 - title_bg->getContentSize().height));
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
            
            item->setPosition(ccpAdd(bigbg->getPosition(), ccp(0, -bigbg->getContentSize().height*.362)));
        }
        
        fermEffect->play();
        fermEffect1->play();
        
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(itemid);
            //    this->setstar(_card->getCurrStar()-1);//传入的star为转生后的星级，应该减1得到原来的
            this->setstar(temp->getEquipCurrStarLevel(),temp->getUpgradestar());
        
        if (_card->getCurrStar()!= beforestar) {
            this->setTitle(_card->getOfficerName()->getCString());
        }
        else
        {
            SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(beforeitemid);
            this->setTitle(temp->getEquipName()->getCString());

        }

        jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
        jinbian->setPosition(ccp(s.width/2, hgt*1.181));
        jinbian->setScaleX(s.width/jinbian->getContentSize().width);
        this->addChild(jinbian,5);
        
        CCSpriterX *wjzs = CCSpriterX::create("animationFile/wjzs.scml", true, true);
        wjzs->setanimaID(0);
        wjzs->setisloop(false);
        addChild(wjzs, 10, 0);
        wjzs->setPosition(item->getPosition());
        wjzs->play();
        
        this->runAction(CCSequence::create(CCDelayTime::create(.48),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::action5)),CCDelayTime::create(0.75 - 0.48),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::action1)),NULL));
    }
}
void SGAdvancingLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true);
    if (isfromfirst) {
        if (cardType == 1) {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard*)_card, 911);
        }else
        {
            SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)_card, 5);
        }
        
    }else
    {
        if (cardType == 1) {
            SGMainManager::shareMain()->showGeneralInfoLayer((SGOfficerCard*)_card, 911);
        }else
        {
            SGMainManager::shareMain()->showEquipInfoLayer((SGEquipCard *)_card, 0);
        }
    }
}
bool SGAdvancingLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    isbreakdown = true;
    if (isbreakdown)
    {
        backHandler();
        return true;
    }
    return false;
}

void SGAdvancingLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, true);
}

void SGAdvancingLayer::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void SGAdvancingLayer::showinfo()
{
	ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float frameheight1 = 0;
    float hgt = 0;
    if (s.height == 1136) {
        //frameheightt = skewing(54);
        frameheight1 = skewing(96);
        hgt = skewing(238);
    } else {
        //frameheightt = skewingY(54);
        frameheight1 = skewingY(96);
        hgt = skewingY(238);
    }
    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame1->setPreferredSize(CCSizeMake(568 +20, 93));
    
    CCSprite *frame1bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    addChild(frame1bg,6);
    
    frame1bg->setScaleX(590/frame1bg->getContentSize().width);
    frame1bg->setScaleY(93/frame1bg->getContentSize().height);
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+20, 197));
    
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    addChild(frame2bg,6);
    frame2bg->setScaleX(590/frame2bg->getContentSize().width);
    frame2bg->setScaleY(197/frame2bg->getContentSize().height);
    
    addChild(frame1,7);
    
    frame1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,hgt + 22 - frame1->getContentSize().height*.5 - 9)));
    addChild(frame2,7);
    frame2->setPosition(ccpAdd(frame1->getPosition(), ccp(0, -frame1->getContentSize().height*.5f - frame2->getContentSize().height*.5f - 10)));
    
    frame1bg->setPosition(frame1->getPosition());
    frame2bg->setPosition(frame2->getPosition());
    
    CCSprite *guang1l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    addChild(guang1l,6);
    guang1l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang1r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    addChild(guang1r,6);
    guang1r->setFlipX(true);
    guang1r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang1m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    addChild(guang1m,6);
    guang1m->setScaleX(468/guang1m->getContentSize().width);
    guang1m->setAnchorPoint(ccp(0.5, 1));
    
    guang1m->setPosition(ccpAdd(frame1->getPosition(), ccp(0, frame1->getContentSize().height*.5)));
    guang1r->setPosition(ccpAdd(guang1m->getPosition(), ccp(284+10, 0)));
    guang1l->setPosition(ccpAdd(guang1m->getPosition(), ccp(-284-10, 0)));
    
    CCSprite *guang2l = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    addChild(guang2l,6);
    guang2l->setAnchorPoint(ccp(0, 1));
    CCSprite *guang2r = CCSprite::createWithSpriteFrameName("barrack_kuang_guangl.png");
    addChild(guang2r,6);
    guang2r->setFlipX(true);
    guang2r->setAnchorPoint(ccp(1, 1));
    
    CCSprite *guang2m = CCSprite::createWithSpriteFrameName("barrack_kuang_guangm.png");
    addChild(guang2m,6);
    guang2m->setScaleX(468/guang2m->getContentSize().width);
    guang2m->setAnchorPoint(ccp(0.5, 1));
    
    guang2m->setPosition(ccpAdd(frame2->getPosition(), ccp(0, frame2->getContentSize().height*.5)));
    guang2r->setPosition(ccpAdd(guang2m->getPosition(), ccp(284+10, 0)));
    guang2l->setPosition(ccpAdd(guang2m->getPosition(), ccp(-284-10, 0)));
    
    float move = skewing(27);
    addLabel(stuff_lvl,ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move*1.2-6,frame1->getPosition().y + frame1->getContentSize().height/5.5));
    addLabel(stuff_exp, ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move*1.2-6,frame1->getPosition().y - frame1->getContentSize().height/5.5));
    addLabel(stuff_round,ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move*1.2-6,frame2->getPosition().y));
    addLabel(stuff_atk, ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move*1.2-6,frame2->getPosition().y + frameheight1 * 2/5));
    addLabel(stuff_def,ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move*1.2-6,frame2->getPosition().y + frameheight1 * 1/5));
    addLabel(stuff_speed, ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move*1.2-6,frame2->getPosition().y - frameheight1 * 1/5));
    addLabel(stuff_mor, ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move*1.2-6,frame2->getPosition().y - frameheight1 * 2/5));
    
    CCSprite *expbg = CCSprite::createWithSpriteFrameName("card_jingyan_bg.png");
    addChild(expbg,7);
    expbg->setPosition(ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move*1.2 + expbg->getContentSize().width*.66,frame1->getPosition().y - frame1->getContentSize().height/5.5 +1));
    CCProgressTimer* expprog = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("card_jingyan.png"));
    expprog->setType(kCCProgressTimerTypeBar);
    expprog->setMidpoint(ccp(0,0));
    expprog->setBarChangeRate(ccp(1, 0));
    expprog->setPosition(expbg->getPosition());
    addChild(expprog,8);
    
    SGCCLabelTTF *font_zjji = SGCCLabelTTF::create(str_zhujiangji, FONT_BOXINFO, 28);
    //font_zjji->setColor(ccc3(0xff, 0xf0, 0x00));
    font_zjji->setPosition(ccp(frame1->getPosition().x +95, frame1->getPosition().y + frame1->getContentSize().height/5.5));
    addChild(font_zjji,100);
    
    SGCCLabelTTF *font_wjji = SGCCLabelTTF::create(str_beidongji, FONT_BOXINFO, 28 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_wjji->setColor( ccc3(0xff, 0xf0, 0x00));
    font_wjji->setPosition(ccp(font_zjji->getPosition().x, frame2->getPosition().y + 27));
    addChild(font_wjji,100);
    
//    SGButton *zhuj_skill = SGButton::create("chakan_bg.png","font_chakan.png",this,//hehehe
//                                            NULL,
//                                            ccp(0, 0),
//                                            false,
//                                            true);
//    zhuj_skill->setPosition(ccpAdd(font_zjji->getPosition(), ccp(font_zjji->getContentSize().width*.5 + 70, 0)));
//    addBtn(zhuj_skill);
//    
//    SGButton *wuj_skill = SGButton::create("chakan_bg.png","font_chakan.png", this,
//                                           NULL,
//                                           ccp(0, 0),
//                                           false,
//                                           true);
//    wuj_skill->setPosition(ccpAdd(font_wjji->getPosition(), ccp(font_wjji->getContentSize().width*.5 +70, 0)));
//    addBtn(wuj_skill);
    
    //从静态数据读出主将计
    SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(((SGOfficerCard*)_card)->getLordId());
    int skillId = ((SGOfficerCard*)_card)->getOfficerSkil();
    SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(skillId);
    
    CCLabelTTF *font_zhujiang = CCLabelTTF::create(str_No,FONT_BOXINFO, 24);
    if (((SGOfficerCard*)_card)->getLordId()) {
        font_zhujiang->setString(CCString::createWithFormat(str_Format_round_class,lordSkill->getSkillName()->getCString(),lordSkill->getRound(),lordSkill->getLevel())->getCString());
    }
    
    addChild(font_zhujiang);
    font_zhujiang->setPosition(ccpAdd(font_zjji->getPosition(), ccp(92,- frame1->getContentSize().height*2/5.5)));
    
    if (((SGOfficerCard*)_card)->getLordId()) {
    }else
    {
        font_zhujiang->setPosition(ccpAdd(font_zhujiang->getPosition(), ccp(20, 0)));
    }
    
    
    CCLabelTTF *font_wujiang = CCLabelTTF::create(str_No, FONT_BOXINFO, 24);
    if (((SGOfficerCard*)_card)->getOfficerSkil()) {
        
        font_wujiang->setString(CCString::createWithFormat(str_Format_s_s_class,officerSkill->getSkillName()->getCString(),officerSkill->getSkillClientLevel()->getCString())->getCString());
    }
    addChild(font_wujiang);
    font_wujiang->setPosition(ccpAdd(font_wjji->getPosition(), ccp(92, -frameheight1 * 1/5)));
    if (((SGOfficerCard*)_card)->getOfficerSkil()) {
        
    }else
    {
        font_wujiang->setPosition(ccpAdd(font_wujiang->getPosition(), ccp(20, 0)));
    }
    
    CCSprite *font_bg = CCSprite::createWithSpriteFrameName("font_tyl_bg.png");
    font_bg->setAnchorPoint(ccp(0, 0.5));
    font_bg->setScaleX(245/font_bg->getContentSize().width);
    font_bg->setPosition(ccp(s.width/2 , frame2->getPosition().y - frame2->getContentSize().height/2 + font_bg->getContentSize().height*.9));
    addChild(font_bg,10);
    
    SGCCLabelTTF *font_szxtyl = SGCCLabelTTF::create(str_shangzheng_need_tongyuli, FONT_BOXINFO, 24 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //font_szxtyl->setColor(ccc3(0xff, 0xf0, 0x00));
    font_szxtyl->setAnchorPoint(ccp(0, 0.5));
    font_szxtyl->setPosition(ccpAdd(font_bg->getPosition(), ccp(6, 0)));
    addChild(font_szxtyl,11);
    
    //9 wuqi 10 yifu 11 jiezhi 12zuoji
    int atk = 0;
    int def = 0;
    int round = 0;
    float speed= 0;
    int mor = 0;
    int gov = _card->getGoverning();
    CCObject *obj = NULL;
    CCARRAY_FOREACH(((SGOfficerCard*)_card)->getOfficerEquips(), obj)
    {
        SGEquipCard *card = (SGEquipCard *)obj;
        atk += card->getAtk();
        def += card->getDef();
        round += card->getRound();
        speed += card->getSpeed();
        mor += card->getMorale();
        gov += card->getGoverning();
    }
    if (gov<0) {
        gov = 0;
    }
    
    CCString *str1 = CCString::createWithFormat("%d",gov);
    SGCCLabelTTF* governing = SGCCLabelTTF::create(str1->getCString(), FONT_BOXINFO, 28 , ccWHITE);
    //modify by:zyc. merge into create.
    //governing->setColor(ccWHITE);
    governing->setPosition(ccpAdd(font_szxtyl->getPosition(), ccp(210, 0)));
    addChild(governing,12);
    
    SGOfficerDataModel *general = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
    this->setCardType(general->getOfficerType());
    
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(false);
    
    addValueLabel(stuff_lvl, _card->getCurrLevel(), 65, _card->getMaxLevel());
    addValueLabel(stuff_atk, _card->getAtk(), move*2.1,atk);
    addValueLabel(stuff_def, _card->getDef(), move*2.1,def);
    addValueLabel(stuff_round, _card->getRound(), move*2.1,round);
    addValueLabel(stuff_speed, _card->getSpeed(), move*2.1,speed);
    addValueLabel(stuff_mor, _card->getMorale(), move*2.1,mor);
    
    if (_card->getCurrExp() == _card->getMaxExp()) {
        expprog->setPercentage(100);
    }else
    {
        expprog->setPercentage(100*((float)_card->getCurrExp()/_card->getMaxExp()));
    }
}
void SGAdvancingLayer::action1()
{
	
	ResourceManager::sharedInstance()->bindTexture("sgadvancelayer/sgadvancelayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);

    CCSprite *a = (CCSprite *)this->getChildByTag(77);

    CCSprite *sp = CCSprite::createWithSpriteFrameName("advance_baiquan.png");
    this->addChild(sp,1000);
    sp->setOpacity(0);
    sp->setPosition(ccpAdd(a->getPosition(), ccp(0, 100)));
    
    float time = (float)752/504;
    
    sp->runAction(CCSequence::create(CCSpawn::create(CCFadeIn::create(0.3),CCScaleTo::create(0.3,time),NULL),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::action2)),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::action3)),CCFadeOut::create(0.3),CCDelayTime::create(1),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::action4)),CCDelayTime::create(4.0),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::backHandler)),NULL));
}
void SGAdvancingLayer::action2()
{
    CCSprite *a = (CCSprite *)this->getChildByTag(77);
    
	//ResourceManager::sharedInstance()->bindTexture("officerIcon/officerIcon1099-1113.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
	SGMainManager::shareMain()->addOfficerPng(_card->getHeadSuffixNum(),sg_advanceLayer);

    if (cardType == 1) {
        //12.05
        SGOfficerDataModel * temp = SGStaticDataManager::shareStatic()->getOfficerById(_card->getItemId());
        SGMainManager::shareMain()->addOfficerPng(temp->getIconId(), sg_advanceLayer);
        a->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("officer_%d.png",temp->getIconId())->getCString())->displayFrame());
    }else
    {
        //12.05
        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        SGMainManager::shareMain()->addEquipPng(temp->getIconId(), sg_advanceLayer);
        a->setDisplayFrame(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("equip%d.png",temp->getIconId())->getCString())->displayFrame());
        
    }
    a->setVisible(true);
}
void SGAdvancingLayer::showequipinfo()
{
    //ResourceManager::sharedInstance()->bindTexture("public/public.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_advanceLayer, LIM_PNG_AS_PNG);
     ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    //float frameheight= skewingY(54);
    float frameheight1 = skewingY(81);

    menu->setZOrder(10);
    if (s.height == 1136) {
        CCLOG("1111");
        //frameheight = skewing(54);
        frameheight1 = skewing(81);
    }

    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame1->setPreferredSize(CCSizeMake(568 +20, 93));
    
    CCSprite *frame1bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame1bg,6);
    frame1bg->setScaleX(590/frame1bg->getContentSize().width);
    frame1bg->setScaleY(93/frame1bg->getContentSize().height);
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+20, 220));
    
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg,6);
    frame2bg->setScaleX(590/frame2bg->getContentSize().width);
    frame2bg->setScaleY(225/frame2bg->getContentSize().height);
    
    this->addChild(frame1,7);
    frame1->setPosition(ccpAdd(jinbian->getPosition(), ccp(0, - frame1->getContentSize().height*.5 - 25)));
    this->addChild(frame2,7);
    frame2->setPosition(ccpAdd(frame1->getPosition(), ccp(0, -frame1->getContentSize().height*.5f - frame2->getContentSize().height*.5f - 12)));
    
    frame1bg->setPosition(frame1->getPosition());
    frame2bg->setPosition(frame2->getPosition());
    
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
    
    CCLabelTTF* label = CCLabelTTF::create("", FONT_PANGWA, 28);
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccpAdd(font_bg->getPosition(), ccp(- font_bg->getContentSize().width*.5+15, 0)));
    this->addChild(label,6);
    
    int itemid = _card->getItemId();
    if (itemid == 4007 || itemid ==4028 || itemid==4029 ||itemid == 4030 ||itemid == 4032||itemid == 4034||itemid == 4036||itemid == 4038) {
        
        SGSuitDataModel *suit = SGStaticDataManager::shareStatic()->getSuitById(itemid);
        if (suit ) {
            label->setString(CCString::createWithFormat("%s (1/%d)",suit->getSuitName()->getCString(),suit->getSuitCount())->getCString());
        }
    }
    else
    {
        label->setString(str_No_zhuangbei_shuxing);
    }
    
    float move = skewing(46);
    
    CCSprite *expbg = CCSprite::createWithSpriteFrameName("card_jingyan_bg.png");
    this->addChild(expbg,7);
    expbg->setPosition(ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move + expbg->getContentSize().width*.5+12,frame1->getPosition().y - frame1->getContentSize().height/5.5 +1));
    CCProgressTimer* expprog = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("card_jingyan.png"));
    //    expprog->setAnchorPoint(ccp(0, 0.5));
    expprog->setType(kCCProgressTimerTypeBar);
    expprog->setMidpoint(ccp(0,0));
    expprog->setBarChangeRate(ccp(1, 0));
    expprog->setPosition(expbg->getPosition());
    this->addChild(expprog,8);
    
    this->addLabel(stuff_lvl,ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move- 30,frame1->getPosition().y + frame1->getContentSize().height/5.5));
    this->addLabel(stuff_exp, ccp(frame1->getPosition().x - frame1->getContentSize().width/2 + move- 30,frame1->getPosition().y - frame1->getContentSize().height/5.5));
    this->addLabel(stuff_def,ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 30,23+frame2->getPosition().y));
    this->addLabel(stuff_atk, ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 30,23+frame2->getPosition().y + frameheight1 * .25));
    this->addLabel(stuff_round,ccp(frame2->getPosition().x - frame2->getContentSize().width/2 + move- 30,23+frame2->getPosition().y - frameheight1 * .25));
    this->addLabel(stuff_speed, ccp(s.width/2 + move*.6+1,23+frame2->getPosition().y + frameheight1 * .25));
    this->addLabel(stuff_mor, ccp(s.width/2 + move*.6+1,23+frame2->getPosition().y));
    this->addLabel(stuff_gov, ccp(s.width/2 + move*.6-13, 23+frame2->getPosition().y - frameheight1 * .25));
    
    SGEquipmentDataModel *general = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    this->setCardType(general->getEquipType());
    
    this->addValueLabel(stuff_lvl, _card->getCurrLevel(), 50, _card->getMaxLevel());
    
    this->addValueLabel(stuff_atk, _card->getAtk(), move*1.3,NULL);
    this->addValueLabel(stuff_def, _card->getDef(), move*1.3,NULL);
    this->addValueLabel(stuff_round, _card->getRound(), move*1.3,NULL);
    this->addValueLabel(stuff_speed, _card->getSpeed(), move*1.3,NULL);
    this->addValueLabel(stuff_mor, _card->getMorale(), move*1.3,NULL);
    this->addValueLabel(stuff_gov, _card->getGoverning(), move*1.3 +13,NULL);
    
    if (_card->getMaxExp() == -1) {
        expprog->setPercentage(100);
    }else
    {
        expprog->setPercentage(100*((float)_card->getCurrExp()/_card->getMaxExp()));
        
    }

}
void SGAdvancingLayer::action3()
{
	
    
        if (_card->getCurrStar() != beforestar) {
        ResourceManager::sharedInstance()->bindTexture("animationFile/bsxx.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
        setadvancestar();
    }
    else
    {
//        SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
        CCSize s = CCDirector::sharedDirector()->getWinSize();
        if (_card->getAdNum()>0) {
            CCSprite* item = (CCSprite*)this->getChildByTag(77);
            CCString *strnum = CCString::createWithFormat("gener+%d.png",_card->getAdNum());
            CCSprite *advancenum = CCSprite::createWithSpriteFrameName(strnum->getCString());
            //    SGCCLabelTTF *lab = SGCCLabelTTF::create("+1", FONT_PANGWA, 36);
            advancenum->setAnchorPoint(ccp(0.5,0));
            advancenum->setPosition(ccpAdd(ccp(item->getContentSize().width/2, item->getContentSize().height/2),ccp(s.width*0.2 ,item->getContentSize().height*0.2)));
            advancenum->setScale(5);
            advancenum->runAction(CCScaleTo::create(.5, 1));
            item->addChild(advancenum);
            
        }

//        ResourceManager::sharedInstance()->bindTexture("animationFile/Changename.plist", RES_TYPE_LAYER_UI, sg_advanceLayer);
//        
//        CCSpriterX *nameeffect = CCSpriterX::create("animationFile/Changename.scml", true, true);
//        nameeffect->setanimaID(0);
//        nameeffect->setisloop(false);
//        addChild(nameeffect, 10, 0);
//        nameeffect->setPosition(labelTitle->getPosition());
//        nameeffect->play();
//        this->runAction(CCSequence::create(CCDelayTime::create(.45),CCCallFunc::create(this, callfunc_selector(SGAdvancingLayer::action6)),NULL));
        
    }
    
}
void SGAdvancingLayer::action6()
{
   labelTitle->setString(_card->getOfficerName()->getCString());
}
void SGAdvancingLayer::action4()
{
    if (cardType == 1) {
       // showinfo();
    }else
    {
        showequipinfo();
    }
}
void SGAdvancingLayer::action5()
{
     CCSprite *a = (CCSprite *)this->getChildByTag(77);
    a->setVisible(false);
}
