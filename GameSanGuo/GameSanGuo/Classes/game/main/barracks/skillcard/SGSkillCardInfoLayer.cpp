//
//  SGSkillCardInfoLayer.cpp
//  GameSanGuo
//
//  Created by 陈 雪龙 on 13-6-17.
//
//

#include "SGSkillCardInfoLayer.h"
#include "SGMainManager.h"
#include "cocos-ext.h"
#include "SGSellBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "itemmodule.pb.h"
#include "SGStaticDataManager.h"
#include "SGBattleScoreLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#include "SGGuideManager.h"

SGSkillCardInfoLayer::SGSkillCardInfoLayer()
: sellBtn(NULL)
, enterType(0)
, _card(NULL)
, protectBtn(NULL)
, btnwidth(0)
, btnwid(0)
, lock(NULL)
{
    time = 1;
    issuccess = false;
    
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(false);
}
SGSkillCardInfoLayer::~SGSkillCardInfoLayer()
{
  
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_skillcardInfoLayer);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_PROP_DISPROTECT);
    notification->removeObserver(this, MSG_PROP_PROTECT);
    
    CCLOG("~SGPropInfoLayer");
    CC_SAFE_RELEASE(_card);
//    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("animationFile/qhtexiao.plist");
    SGMainManager::shareMain()->getMainLayer()->setIsCanTouch(true , false);
}
SGSkillCardInfoLayer *SGSkillCardInfoLayer::create(SGBaseMilitaryCard *card_,int type,bool state)
{
    SGSkillCardInfoLayer *propInfoLayer = new SGSkillCardInfoLayer();
    if (propInfoLayer && propInfoLayer->init(NULL, sg_skillcardInfoLayer))
    {
        propInfoLayer->issuccess = state;
        propInfoLayer->enterType = type;
        propInfoLayer->_card = card_;
        propInfoLayer->_card->retain();
        propInfoLayer->initView();
        propInfoLayer->autorelease();
        return propInfoLayer;
    }
    CC_SAFE_DELETE(propInfoLayer);
    return NULL;
}
void SGSkillCardInfoLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer2.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);
 
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_PROP_DISPROTECT,
                              this,
                              callfuncO_selector(SGSkillCardInfoLayer::disprotectListener));
    notification->addObserver(MSG_PROP_PROTECT,
                              this,
                              callfuncO_selector(SGSkillCardInfoLayer::propprotectListener));
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    float btmheight = SGMainManager::shareMain()->getBottomHeight();
    
    //float frameheight = skewingY(155);
    float hgt = skewingY(244);
    if (s.height == 1136) {
        //frameheight = skewing(155);
        hgt = skewing(244);
    }

	ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);
    
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
    
    SGMainManager::shareMain()->addHeadIconPlistByNum(_card->getHeadSuffixNum(),sg_skillcardInfoLayer);
    CCString *str = CCString::createWithFormat("head%d.png",_card->getHeadSuffixNum());
    if (enterType == 3  || enterType == 4 || enterType==10) {
        SGOfficerCard *card = (SGOfficerCard*)_card;
        SGSkillDataModel *skill = SGStaticDataManager::shareStatic()->getGeneralSkillById(card->getOfficerSkil());
        if (enterType == 4) {
            SGMainManager::shareMain()->addHeadIconPlistByNum(skill->getIconId(),sg_skillcardInfoLayer);
            str = CCString::createWithFormat("head%d.png",skill->getSkillItemId());
        }
        else
        {
            SGMainManager::shareMain()->addHeadIconPlistByNum(skill->getIconId(),sg_skillcardInfoLayer);
            str = CCString::createWithFormat("head%d.png",skill->getIconId());
        }

        
//        str->setString(CCString::createWithFormat("head%d.png",skill->getSkillItemId())->getCString());
//        this->setstar(skill->getSkillStarLvl());
        this->setTitle(skill->getSkillName()->getCString());
    }
    
    else if (enterType ==11 ||enterType ==12)
    {
        SGOfficerCard *card = (SGOfficerCard*)_card;
        SGSkillDataModel *lordskill = SGStaticDataManager::shareStatic()->getLordSkillById(card->getLordId());
        SGMainManager::shareMain()->addHeadIconPlistByNum(lordskill->getIconId(),sg_skillcardInfoLayer);
        str = CCString::createWithFormat("head%d.png",lordskill->getIconId());
        //        str->setString(CCString::createWithFormat("head%d.png",skill->getSkillItemId())->getCString());
//        this->setstar(lordskill->getSkillStarLvl());
        this->setTitle(lordskill->getSkillName()->getCString());

    }
    
    else
    {
//        this->setstar(_card->getCurrStar());
        this->setTitle(_card->getOfficerName()->getCString());
    }
    CCSprite *item = CCSprite::createWithSpriteFrameName("propskill.png");
//    CCSprite *item = CCSprite::createWithSpriteFrameName("an.png");
    this->addChild(item,2);
    item->setAnchorPoint(ccp(0.5, 0));
    
    CCSprite *bigbg =NULL;
    
    if (s.height == 960) {
         pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0.5, -430));
        frameSize = CCRectMake(0, 0 , 768, 413);
        this->setItemID("pad装备底图.png",true);
        fermEffect->setScale(s.width/768);
        fermEffect1->setScale(s.width/768);
        
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287+26)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-19, -365)));
    }
    else if (s.height == 1136) {
        frameSize = CCRectMake(0, 520 -(s.height - hgt*1.13 - title_bg->getContentSize().height), 768, (s.height - hgt*1.13 - title_bg->getContentSize().height));
        this->setItemID("carditembg.png",true);
        
        fermEffect->setScaleX(s.width/768);
        fermEffect1->setScaleX(s.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -361+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -361+26)));
        
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-19, -450)));
        pos = ccpAdd(SGLayout::getPoint(kUpCenter), ccp(1.5, -530));
        
    }
    else
    {
        bigbg = CCSprite::createWithSpriteFrameName("pad装备底图.png");
        this->addChild(bigbg,-100);
        bigbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - bigbg->getContentSize().height*.45)));
        fermEffect->setPosition(ccpAdd(bigbg->getPosition(), ccp(-bigbg->getContentSize().width*.41, bigbg->getContentSize().height*.02 -bigbg->getContentSize().height*.09)));
        fermEffect1->setPosition(ccpAdd(bigbg->getPosition(), ccp(bigbg->getContentSize().width*.41, bigbg->getContentSize().height*.02-bigbg->getContentSize().height*.09)));
        
        item->setPosition(ccpAdd(bigbg->getPosition(), ccp(-19, -bigbg->getContentSize().height*.362)));
        
        pos = ccpAdd(bigbg->getPosition(), ccp(1.5, -bigbg->getContentSize().height*.365-bigbg->getContentSize().height*.195));

    }
    fermEffect->play();
    fermEffect1->play();
    menu->setZOrder(7);
    
    CCSprite *icon = CCSprite::createWithSpriteFrameName(str->getCString());
    this->addChild(icon,2);
    icon->setPosition(ccpAdd(item->getPosition(), ccp(22, 168)));

//    CCSprite *kuang = CCSprite::createWithSpriteFrameName("jinengkuang.png");
//    this->addChild(kuang,2);
//    kuang->setPosition(icon->getPosition());
    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGSkillCardInfoLayer::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGSkillCardInfoLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));
    
    CCSprite *bg1 = NULL;
    float scaleRateheight = 0.66;
    if (s.height == 960)
    {
        scaleRateheight = 0.9;
    }
    if (enterType == 3  ||enterType == 4 || enterType==10 ||enterType ==11 ||enterType ==12)
    {
        
        ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);

        bg1 = CCSprite::createWithSpriteFrameName("storyBg.png");
        //CCRect r = CCRectMake(0, 1136/2 - (hgt*1.13)/2, s.width, hgt*1.13);

        
        bg1->setScaleY( (s.height + btmheight * scaleRateheight)/ 2 / bg1->getContentSize().height);
        bg1->setScaleX(s.width / bg1->getContentSize().width);
        //bg1->setTextureRect(r);
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setPosition(SGLayout::getPoint(kBottomCenter));
        this->addChild(bg1,-2);
        
    //    //地圈
        CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/qh_000.scml", true, true);
        fermEffect2->setanimaID(0);
        fermEffect2->setisloop(false);
        addChild(fermEffect2, 1, 2);
    //    //光圈
        CCSpriterX *fermEffect3 = CCSpriterX::create("animationFile/qh_001.scml", true, true);
        fermEffect3->setanimaID(0);
        fermEffect3->setisloop(false);
        addChild(fermEffect3, 4, 3);
        fermEffect2->setScale(s.width/768);
        fermEffect3->setScale(s.width/768);
        fermEffect2->setPosition(pos);
        fermEffect3->setPosition(pos);
        fermEffect3->play();
        fermEffect2->play();
        
            runAction(CCSequence::create(CCDelayTime::create(fermEffect2->getAnimaTimeLength(0, 0) ),CCCallFunc::create(this, callfunc_selector(SGSkillCardInfoLayer::playAct)),NULL));

    }
    else
    {
        bg1 = CCSprite::createWithSpriteFrameName("info_redbg.png");
        bg1->setAnchorPoint(ccp(0.5, 0));
        bg1->setScaleX(s.width/bg1->getContentSize().width);
        bg1->setScaleY((hgt*1.13-btmheight*.68)/bg1->getContentSize().height);
        bg1->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, btmheight*.68 )));
        this->addChild(bg1,5);

    }
    int addjustY = 0;
    if (s.height > 1024 && enterType == 4)
    {
        addjustY = -80;
    }
    CCSprite* jinbian = CCSprite::createWithSpriteFrameName("jinbian.png");
    jinbian->setPosition(ccp(s.width/2, bg1->getPosition().y + addjustY + bg1->getContentSize().height * bg1->getScaleY()));
    jinbian->setScaleX(s.width/jinbian->getContentSize().width);
    this->addChild(jinbian,6);
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    frame2->setPreferredSize(CCSizeMake(568+20, 320));
    this->addChild(frame2,7);
    frame2->setPosition(ccpAdd(jinbian->getPosition(), ccp(0, -frame2->getContentSize().height/2 - 30)));
    
    CCSprite *frame2bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(frame2bg,6);
    frame2bg->setScaleX(590/frame2bg->getContentSize().width);
    frame2bg->setScaleY(320/frame2bg->getContentSize().height);
    frame2bg->setPosition(frame2->getPosition());
    
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
    
    CCScale9Sprite *fontbg = CCScale9Sprite::createWithSpriteFrameName("box_fontbg.png");
    fontbg->setPreferredSize(CCSizeMake(538, 205));
    this->addChild(fontbg,6);
    fontbg->setPosition(ccpAdd(frame2->getPosition(), ccp(0, 26)));
    
    CCSprite* hd = CCSprite::createWithSpriteFrameName("barrack_huibg.png");
    this->addChild(hd,6);
    hd->setScaleX(538/hd->getContentSize().width);
    hd->setPosition(ccpAdd(frame2->getPosition(), ccp(0, -112)));
    
    if (enterType <3 ||enterType == CHATSHOWENTRYTAG) {
        SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
        SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getPropsInfo()->getCString(), FONT_BOXINFO, 28, CCSizeMake(504,400));
         a->setAnchorPoint(ccp(0.5, 1));
        this->addChild(a,6);
        a->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, fontbg->getContentSize().height*.33)));
        
        if(_card->getIsProt() == 1)
        {
            protectBtn = SGButton::create("store_exchangebtnbg.png",
                                          "font_jiechu.png",
                                          this,menu_selector(SGSkillCardInfoLayer::disprotectHandler),
                                          ccp(0, 0),
                                          false,
                                          true);
            
        }
        else if(_card->getIsProt() == 0)
        {
            protectBtn = SGButton::create("store_exchangebtnbg.png",
                                          "font_protect.png",
                                          this,menu_selector(SGSkillCardInfoLayer::protectHandler),
                                          ccp(0, 0),
                                          false,
                                          true);
            
        }
        this->addBtn(protectBtn);
        sellBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_Show_, this, menu_selector(SGSkillCardInfoLayer::sellHandler), CCPointZero, FONT_PANGWA, ccWHITE, 32, false, true);

        this->addBtn(sellBtn);
        btnwidth = skewing(5);
        btnwid = skewing(33);
        
        
        protectBtn->setPosition(ccp(s.width/2 + btnwid + btnwidth, (frame2->getPosition().y -frame2->getContentSize().height/2  - btmheight*.68)/2 + btmheight*.68));
        sellBtn->setPosition(ccp(s.width/2 + btnwid*3 + btnwidth * 3,protectBtn->getPosition().y));
        
        SGCCLabelTTF *tishi = SGCCLabelTTF::create(str_SkillCardsInfoLayer_str1, FONT_BOXINFO,26 , ccc3(0xc4, 0xc4, 0xc4));
        tishi->setPosition(hd->getPosition());
        this->addChild(tishi,6);
        //modify by:zyc. merge into create.
        //tishi->setColor(ccc3(0xc4, 0xc4, 0xc4));
        
        lock = CCSprite::createWithSpriteFrameName("card_suo.png");
        this->addChild(lock,10);
        lock->setAnchorPoint(ccp(0.8, 0.8));
        lock->setPosition(ccpAdd(sellBtn->getPosition(), ccp(sellBtn->getContentSize().width/2, sellBtn->getContentSize().height/2)));
        lock->setVisible(false);
        if (_card->getIsProt()) {
            lock->setVisible(true);
        }
        
        if (enterType == 1 ||enterType == -1 ||enterType == -2 || enterType == CHATSHOWENTRYTAG) {
            protectBtn->setVisible(false);
            sellBtn->setVisible(false);
            lock->setVisible(false);
        }
        
        SGPropsDataModel *general = SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
        this->setCardType(general->getPropsType());
        
    }
    else if(enterType < 11)
    {
        SGOfficerCard *card = (SGOfficerCard *)_card;
        SGSkillDataModel *temp = SGStaticDataManager::shareStatic()->getGeneralSkillById(card->getOfficerSkil());
        temp->getSkillItemId();
        temp->getSkillInfo();
        SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getSkillInfo()->getCString(), FONT_BOXINFO, 28, CCSizeMake(504,400));
        a->setAnchorPoint(ccp(0.5, 1));
        this->addChild(a,6);
        a->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, fontbg->getContentSize().height*.33)));
        SGCCLabelTTF *tishi = SGCCLabelTTF::create(str_SkillCardsInfoLayer_str2, FONT_BOXINFO,26 , ccc3(0xc4, 0xc4, 0xc4));
        tishi->setPosition(hd->getPosition());
        this->addChild(tishi,6);
    }
    else
    {
        SGOfficerCard *card = (SGOfficerCard *)_card;
        SGSkillDataModel *temp = SGStaticDataManager::shareStatic()->getLordSkillById(card->getLordId());
        SGCCLabelTTF* a = SGCCLabelTTF::create(temp->getSkillInfo()->getCString(), FONT_BOXINFO, 28, CCSizeMake(504,400));
        a->setAnchorPoint(ccp(0.5, 1));
        this->addChild(a,6);
        a->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, fontbg->getContentSize().height*.33)));
        SGCCLabelTTF *tishi = SGCCLabelTTF::create(str_SkillCardsInfoLayer_str2, FONT_BOXINFO,26 , ccc3(0xc4, 0xc4, 0xc4));
        tishi->setPosition(hd->getPosition());
        this->addChild(tishi,6);
    }
    
}
void SGSkillCardInfoLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    // 0正常进入 1强化进入 2 武将进入 3 强化被动技能播放特效 4强化被动技能播放特效 -1图鉴进入 11 强化主将计播放特效 10武将计失败(服务器数据错误) 12 强化主将计播放特效(fromfirstlayer)
    switch (enterType) {
        case 0:
            SGMainManager::shareMain()->showSkillCardsLayer(0);
            break;
        case 1:
            showLayerByTag(sg_strengLayer);
            break;
        case 3:
        {
            SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(((SGOfficerCard*)_card)->getOfficerSkil());
            if (officerSkill && officerSkill->getNextId()==0)
            {
                SG_SHOW_WINDOW(str_GeneralInfoEffect_str4);
                 SGMainManager::shareMain()->showGeneralsLayer(5,true,0,1);
            }
            else if (officerSkill && officerSkill->getSkillMaxLevel()==_card->getSkillMaxLevel())
            {
                SG_SHOW_WINDOW(str_GeneralInfoEffect_str5);
                 SGMainManager::shareMain()->showGeneralsLayer(5,true,0,1);
            }
            else
            {
            SGMainManager::shareMain()->showStrengLayer(_card, 4,true);
                
            }
        }
            break;
        case 10:
        case 4:
        {
            SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(((SGOfficerCard*)_card)->getOfficerSkil());
            if (officerSkill && officerSkill->getNextId()==0)
            {
                SG_SHOW_WINDOW(str_GeneralInfoEffect_str4);
                SGMainManager::shareMain()->showGeneralsLayer(5,true,0,1);
            }
            else if (officerSkill && officerSkill->getSkillMaxLevel()==_card->getSkillMaxLevel())
            {
                SG_SHOW_WINDOW(str_GeneralInfoEffect_str5);
                SGMainManager::shareMain()->showGeneralsLayer(5,true,0,1);
            }
            else
            {
                SGMainManager::shareMain()->showStrengLayer(_card, 4,false);
                
            }

        }
            break;
        case -1:
            showLayerByTag(sg_cardsLayer);
            break;
        case -2:
//            SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_creditCardLayer);
        {
             showLayerByTag(sg_creditCardLayer);
//            this->removeFromParentAndCleanup(true);
//            SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//            SGBattleScoreLayer *cclayer = (SGBattleScoreLayer *)mainScene->getChildByTag(sg_creditCardLayer);
//            mainScene->addShowLayer(cclayer);
//            cclayer->getInfoByIndex(cclayer->_selectIndex);
        }

            break;
            
        case 11:
        {
            SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(((SGOfficerCard*)_card)->getLordId());
            if (lordSkill && lordSkill->getNextId()==0)
            {
                SG_SHOW_WINDOW(str_GeneralInfoEffect_str5);
            }
            else if (lordSkill && lordSkill->getLevel()==_card->getLordMaxLevel())
            {
                SG_SHOW_WINDOW(str_GeneralInfoEffect_str5);
            }
            else
            {
                SGMainManager::shareMain()->showStrengLayer(_card, 1,4,true);
            }
        }
            break;
        case 12:
        {
            SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(((SGOfficerCard*)_card)->getLordId());
            if (lordSkill && lordSkill->getNextId()==0)
            {
                SG_SHOW_WINDOW(str_GeneralInfoEffect_str4);
                SGMainManager::shareMain()->showGeneralsLayer(4,true,0,1);
            }
            else if (lordSkill && lordSkill->getLevel()==_card->getLordMaxLevel())
            {
                SG_SHOW_WINDOW(str_GeneralInfoEffect_str5);
                SGMainManager::shareMain()->showGeneralsLayer(4,true,0,1);
            }
            else
            {
                SGMainManager::shareMain()->showStrengLayer(_card, 1,4,true);
            }
        }

            break;
		case CHATSHOWENTRYTAG://铜雀台,技能卡返回
		{
            SGMainManager::shareMain()->closeChatShowLayer();
		}
			break;
        default:
            break;
    }
    
}
void SGSkillCardInfoLayer::showLayerByTag(LayerTag layerTag)
{
    
    SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(layerTag);
    if(NULL == layer)
    {
        return;
    }
    
    SGMainManager::shareMain()->showLayer(layer);
    
    SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_skillcardInfoLayer, true);
}

void SGSkillCardInfoLayer::getHandler()
{
}
void SGSkillCardInfoLayer::showBoxCall(cocos2d::CCObject *obj)
{
    if (obj)
    {
    }
}

void SGSkillCardInfoLayer::sellHandler()
{
    SGPropsDataModel *card =  SGStaticDataManager::shareStatic()->getPropsById(_card->getItemId());
    SGMainManager::shareMain()->getMainLayer()->sendChatShowContent(_card->getOfficerName(), 4,card->getPropsCurrStarLevel(), _card->getSsid());
}
void SGSkillCardInfoLayer::protectHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::PropsProtectionRequest *request = new main::PropsProtectionRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_PROP_PROTECT, request);
}
void SGSkillCardInfoLayer::disprotectHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::PropsUnProtectionRequest *request = new main::PropsUnProtectionRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_PROP_DISPROTECT, request);
    
}
void SGSkillCardInfoLayer::disprotectListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::PropsUnProtectionResponse *response = (main::PropsUnProtectionResponse *)sr->m_msg;
    const char * msg = str_Equipslayer_str4;
    if (sr)
    {
        if (response->state() == 1)
        {
            msg = str_Equipslayer_str5;
            _card->setIsProt(0);
            menu->removeChild(protectBtn, true);
            protectBtn = SGButton::create("store_exchangebtnbg.png",
                                          "font_protect.png",
                                          this,menu_selector(SGSkillCardInfoLayer::protectHandler),
                                          ccp(0, 0),
                                          false,
                                          true);
            protectBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(btnwid + btnwidth, sellBtn->getPosition().y)));
            this->addBtn(protectBtn);
            lock->setVisible(false);
        }
        else if(response->state() == 0)
        {
            msg = str_Equipslayer_str4;
            CCLOG("取消保护失败");
        }
    }
    SGMainManager::shareMain()->showMessage(msg);
}
bool SGSkillCardInfoLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (enterType== 3 ||enterType == 4 ||enterType ==11 ||enterType ==12) {
        if (!m_break)
        {
            this->setbreakAction(true);
            this->removeChildByTag(2, true);
            this->removeChildByTag(3, true);
                        
            return false;
        }
        else
        {
            if (m_break) {
                if (SGGuideManager::shareManager()->isGuide)
                {
                    SGGuideManager::shareManager()->startLimitGuide(-11);
                }
                backHandler();
            }
            
            return false;
        } 
    }
    return false;
}
void SGSkillCardInfoLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -INT32_MAX, true);
}

void SGSkillCardInfoLayer::onExit()
{
    SGBaseLayer::onExit();
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}
void SGSkillCardInfoLayer::playAct()
{
    if (issuccess) {
        ResourceManager::sharedInstance()->bindTexture("sgskillcardslayer/sgskillcardslayer.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);

       
        CCSprite *asdd = CCSprite::createWithSpriteFrameName("streng_fail.png");
        this->addChild(asdd,100,111);
        asdd->setPosition(ccpAdd(pos, ccp(0, 200)));
        asdd->setOpacity(0);
        asdd->runAction(CCSpawn::create(CCFadeIn::create(.75), CCShaky3D::create(.2, CCSizeMake(16, 11), 6, false), NULL));
//        asdd->runAction(CCSpawn::create(CCFadeIn::create(.75),CCShaky3D::create(6, false, ccg(16,11), .2),NULL));
        schedule(schedule_selector(SGSkillCardInfoLayer::updatewater), .2f);
    }else
    {
        CCSpriterX *lvlupAct = CCSpriterX::create("animationFile/success1.scml", true, true);
        lvlupAct->setanimaID(0);
        lvlupAct->setisloop(false);
        addChild(lvlupAct,100);
        lvlupAct->setPosition(ccpAdd(pos, ccp(0, 200)));
        lvlupAct->play();
    
    }
    
}
void SGSkillCardInfoLayer::updatewater()
{
    CCSprite *sp = (CCSprite*)this->getChildByTag(111);

    if (time <= 0.3f) {
        unschedule(schedule_selector(SGSkillCardInfoLayer::updatewater));
        this->removeChild(sp, true);
        return;
    }
    time -= 0.2f;
        CCAction *aaa = CCShaky3D::create(.2f, CCSizeMake(1+15*time,1+10*time),1 +5*time, false);
//        CCAction *aaa = CCShaky3D::create(1 +5*time, false, ccg(1+15*time,1+10*time), .2f);
    sp->runAction(aaa);
}
void SGSkillCardInfoLayer::propprotectListener(cocos2d::CCObject *obj)
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_skillcardInfoLayer);
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::PropsProtectionResponse *response = (main::PropsProtectionResponse *)sr->m_msg;
        
        if (response->state() == 1) {
            menu->removeChild(protectBtn, true);
       
                protectBtn = SGButton::create("store_exchangebtnbg.png",
                                              "font_jiechu.png",
                                              this,menu_selector(SGSkillCardInfoLayer::disprotectHandler),
                                              ccp(0, 0),
                                              false,
                                              true);
                protectBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(btnwid + btnwidth, sellBtn->getPosition().y)));
                this->addBtn(protectBtn);
                _card->setIsProt(1);
                lock->setVisible(true);
           
            
            SGMainManager::shareMain()->showMessage(str_Equipslayer_str6);
        }else
        {
            CCLOG("设为保护失败");
        }
    }
}