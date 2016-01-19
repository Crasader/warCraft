//
//  SGGeneralEquipBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-8.
//
//

#include "SGGeneralEquipBox.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "ArmyModule.pb.h"
#include "SGSocketClient.h"
#include "SGFont.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGExchangeEquipBox.h"
#include "SGMainLayer.h"
#include "ResourceManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGStringConfig.h"
SGGeneralEquipBox::SGGeneralEquipBox()
:officerCard(NULL),
equipCard(NULL),
state(0),
dlg(NULL),
enterType(0),
enterTypeOther(0)

{
    
}
SGGeneralEquipBox::~SGGeneralEquipBox()
{
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_EQUIP_OFF);    
    notification->removeObserver(this, MSG_EQUIP_WEAR);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_generalEquipBox);

    
}
SGGeneralEquipBox *SGGeneralEquipBox::create(SGBoxDelegate *delegate, SGOfficerCard *card,SGEquipCard * card_,int type,int enter,int enterTypeOther)
{
    SGGeneralEquipBox *instaceBox = new SGGeneralEquipBox();
    instaceBox->enterTypeOther= enterTypeOther;
    instaceBox->enterType = enter;
    instaceBox->dlg = delegate;
    instaceBox->state = type;
    instaceBox->officerCard = card;
    instaceBox->equipCard = card_;
    if (instaceBox && instaceBox->init(delegate, box_generalEquip,CCSizeMake(535, 634),true))
    {
        instaceBox->initView();
        instaceBox->autorelease();
        return instaceBox;
    }
    CC_SAFE_RELEASE(instaceBox);
    return NULL;
}

void SGGeneralEquipBox::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_generalEquipBox);
    ResourceManager::sharedInstance()->bindTexture("sgskillcardslayer/sgskillcardslayer.plist", RES_TYPE_LAYER_UI, sg_generalEquipBox);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_generalEquipBox);
    ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_generalEquipBox);
     ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_generalEquipBox, LIM_PNG_AS_PNG);
    
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EQUIP_OFF,
                              this,
                              callfuncO_selector(SGGeneralEquipBox::offEquipListener));   
 
    notification->addObserver(MSG_EQUIP_WEAR,
                              this,
                              callfuncO_selector(SGGeneralEquipBox::wearEquipListener));
    

    fontbg->setPreferredSize(CCSize(480,465));
    fontbg->setPosition(ccpAdd(biao->getPosition(), ccp(0,- fontbg->getContentSize().height*.5f-40)));
    //fontbg->setVisible(false);  //cgpUI
    
    SGCCLabelTTF* title = SGCCLabelTTF::create("", FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));
    //modify by:zyc . merge into create.
    //title->setOutSideColor(ccc3(143,87,0));
    
    if (enterType ==1) {
//        title = CCSprite::createWithSpriteFrameName("font_zbwp.png");
        title->setString(str_GeneralEquipBox_str1);
        
        SGButton *upBtn = SGButton::create("box_btn_newbg.png",
                                           "public_font_queding.png",
                                           this,
                                           menu_selector(SGGeneralEquipBox::boxCloseWithOutCallBack),
                                           ccp(0, 0),
                                           false,
                                           true);
        this->addBtn(upBtn);
        upBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -270)));
     
    }else{
    if (state == 1) {             
        
//    title = CCSprite::createWithSpriteFrameName("font_glyzbdbq.png");
       title->setString(str_GeneralEquipBox_str2);
    SGButton *upBtn = SGButton::create("box_btn_newbg.png",
                                       "font_xiex.png",
                                       this,
                                       menu_selector(SGGeneralEquipBox::offHandler),
                                       ccp(0, 0),
                                       false,
                                       true);
    this->addBtn(upBtn);
    upBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -270)));
        
//    SGCCLabelTTF* label1 = SGCCLabelTTF::create("要将这个装备卸下吗", FONT_BOXINFO, 30);
//    label1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -210)));
//    this->addChild(label1);
        
        
        SGButton *streng = SGButton::create("box_btn_newbg.png",
                                            "font_skill_streng.png",
                                            this,
                                            menu_selector(SGGeneralEquipBox::strengHandler),
                                            ccp(0, 0),
                                            false,
                                            true);
        this->addBtn(streng);
        
        int limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitEquipGuide,7);
        streng->setTag(limittag);
        
        streng->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-180, -270)));
        
        
        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_quxiao.png",
                                            this,
                                            menu_selector(SGGeneralEquipBox::boxCloseWithOutCallBack),
                                            ccp(0, 0),
                                            false,
                                            true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(180, -270)));

        
    }    
    
    else if (state == 2) {
           
           
        
//        title = CCSprite::createWithSpriteFrameName("font_zbwp.png");
        title->setString(str_GeneralEquipBox_str1);
        SGButton *upBtn = SGButton::create("box_btn_newbg.png",
                                           "font_zhuangb.png",
                                           this,
                                           menu_selector(SGGeneralEquipBox::upHandler),
                                           ccp(0, 0),
                                           false,
                                           true);
        this->addBtn(upBtn);
        upBtn->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-145, -270)));
        
//        SGCCLabelTTF* label1 = SGCCLabelTTF::create("让刚才选择的武将穿戴这个装备吗", FONT_BOXINFO, 30);
//        label1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -220)));
//        this->addChild(label1);
        
        
        SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                            "public_font_quxiao.png",
                                            this,
                                            menu_selector(SGGeneralEquipBox::boxCloseWithOutCallBack),
                                            ccp(0, 0),
                                            false,
                                            true);
        this->addBtn(cancel);
        cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(145, -270)));
        
        
    }
    
       
               
        
     }
    if (title) {
        title->setPosition(biao->getPosition());
        this->addChild(title,5);
    }

    char spriteName[256] = {0};
	//12.06
	SGMainManager::shareMain()->addHeadIconPlistByNum(equipCard->getHeadSuffixNum(),sg_generalEquipBox);
    sprintf(spriteName, "head%d.png",equipCard->getHeadSuffixNum());
    
  
    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(equipCard->getItemId());
    SGButton *portrait = SGButton::create(spriteName,CCString::createWithFormat("equipPinzhi_%d_0.png",temp->getEquipStarPinzhi())->getCString(), this,
                                    NULL,
                                    ccp(0, 0),
                                    false,
                                    true);
    portrait->setPosition(ccpAdd(biao->getPosition(),ccp(-portrait->getContentSize().width - portrait->getContentSize().width/2 - 30, -115)));
    if (temp->getAdvNum()>0) {
        SGCCLabelTTF *advancenumber = SGCCLabelTTF::create(CCString::createWithFormat("+%d",temp->getAdvNum())->getCString(), FONT_PANGWA, 24);
        advancenumber->setInsideColor(ccGREEN);
        advancenumber->setAnchorPoint(ccp(1,1));
        advancenumber->ignoreAnchorPointForPosition(false);
        advancenumber->setPosition(ccp(portrait->getContentSize().width,portrait->getContentSize().height - advancenumber->getContentSize().height*.1));
        portrait->addChild(advancenumber,50,250);
    }

    
    CCSprite *background = CCSprite::createWithSpriteFrameName("card_bj.png");
    this->addChild(background);
    background->setPosition(portrait->getPosition());
    this->addChild(portrait);

    SGCCLabelTTF* name = SGCCLabelTTF::create(equipCard->getOfficerName()->getCString(), FONT_PANGWA, 26);
    name->setAnchorPoint(ccp(0,1));
    name->setPosition(ccp(portrait->getPosition().x + portrait->getContentSize().width*.7, portrait->getPosition().y + portrait->getContentSize().height*.4));
    this->addChild(name);
    
    for(int i=0; i < equipCard->getUpgradestar();i++)
    {
        CCSprite* xingji = CCSprite::createWithSpriteFrameName("xx.png");
        xingji->setAnchorPoint(ccp(0.2, 0.5));
        xingji->setPosition(ccp(name->getPosition().x + xingji->getContentSize().width * i, name->getPosition().y - xingji->getContentSize().height*1.4));
        this->addChild(xingji,1,i);
        if (i>=equipCard->getCurrStar()) {
            xingji->setVisible(false);
        }
    }
    for(int i=equipCard->getCurrStar(); i < equipCard->getUpgradestar();i++)
    {
        CCSprite* xingji = CCSprite::createWithSpriteFrameName("bluestar.png");
        xingji->setAnchorPoint(ccp(0.2, 0.5));
        xingji->setPosition(ccpAdd(((CCSprite*)getChildByTag(i))->getPosition(), ccp(0,-0.5)));
        this->addChild(xingji,1,i);
        
    }

    
    CCString* str_lvl = CCString::createWithFormat(str_Format_ji,equipCard->getCurrLevel());
    SGCCLabelTTF* lvl = SGCCLabelTTF::create(str_lvl->getCString(), FONT_PANGWA,26 ,ccc3(0xff, 0xff, 0xff) );
    //modify by:zyc. merge into create.
    //lvl->setColor(ccc3(0xff, 0xff, 0xff));
    lvl->setAnchorPoint(ccp(0, 0.5));
    this->addChild(lvl);
    lvl->setPosition(ccp(name->getPosition().x + name->getContentSize().width*1.1, name->getPosition().y - name->getContentSize().height/2));

    CCSprite* tiao1 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    tiao1->setScaleX(480/tiao1->getContentSize().width);
    tiao1->setPosition(ccp(biao->getPosition().x, portrait->getPosition().y - 65));
    this->addChild(tiao1,5);

    CCSprite* tiao3 = CCSprite::createWithSpriteFrameName("login_name_line.png");
    tiao3->setScaleX(510/tiao1->getContentSize().width);
    tiao3->setPosition(ccpAdd(tiao1->getPosition(),ccp(0, -167)));
    this->addChild(tiao3,5);

    SGCCLabelTTF *atk_ = SGCCLabelTTF::create(str_attack, FONT_BOXINFO, 32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //atk_->setColor(COLOR_YELLOW);
    atk_->setPosition(ccpAdd(portrait->getPosition(), ccp(-portrait->getContentSize().width*.5f + atk_->getContentSize().width*.5f,-100)));
    this->addChild(atk_);
    
    SGCCLabelTTF *def_ = SGCCLabelTTF::create(str_defense, FONT_BOXINFO, 32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //def_->setColor(COLOR_YELLOW);
    def_->setPosition(ccpAdd(atk_->getPosition(), ccp(0,-45)));
    this->addChild(def_);
    
    SGCCLabelTTF *mor_ = SGCCLabelTTF::create(str_hp, FONT_BOXINFO, 32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //mor_->setColor(COLOR_YELLOW);
    mor_->setPosition(ccpAdd(def_->getPosition(), ccp(0,-45)));
    this->addChild(mor_);
    
    SGCCLabelTTF *xuli_ = SGCCLabelTTF::create(str_xuli, FONT_BOXINFO, 32 , COLOR_YELLOW);
    xuli_->setPosition(ccpAdd(atk_->getPosition(), ccp(230, 0)));
    this->addChild(xuli_);
    //modify by:zyc. merge into create.
    //xuli_->setColor(COLOR_YELLOW);
    
    SGCCLabelTTF *sudu_ = SGCCLabelTTF::create(str_speed, FONT_BOXINFO, 32 , COLOR_YELLOW);
    sudu_->setPosition(ccpAdd(xuli_->getPosition(), ccp(0,-45)));
    this->addChild(sudu_);
    //modify by:zyc. merge into create.
    //sudu_->setColor(COLOR_YELLOW);
    
    SGCCLabelTTF *gov_ = SGCCLabelTTF::create(str_tongyuli, FONT_BOXINFO, 32 , COLOR_YELLOW);
    gov_->setPosition(ccpAdd(sudu_->getPosition(), ccp(16,-45)));
    this->addChild(gov_);
    //modify by:zyc. merge into create.
    //gov_->setColor(COLOR_YELLOW);
    
    CCString* str_atk = CCString::createWithFormat("%d",equipCard->getAtk());
    SGCCLabelTTF* atk = SGCCLabelTTF::create(str_atk->getCString(), FONT_BOXINFO, 30);
    atk->setAnchorPoint(ccp(1, 0.5));
    atk->setPosition(ccpAdd(atk_->getPosition(), ccp(120, 0)));
    this->addChild(atk);
    
    CCString* str_def = CCString::createWithFormat("%d",equipCard->getDef());
    SGCCLabelTTF* def = SGCCLabelTTF::create(str_def->getCString(), FONT_BOXINFO, 30);
    def->setAnchorPoint(ccp(1, 0.5));
    def->setPosition(ccpAdd(def_->getPosition(), ccp(120, 0)));
    this->addChild(def);
    
    CCString* str_shiqi = CCString::createWithFormat("%d",equipCard->getMorale());
    SGCCLabelTTF* shiqi = SGCCLabelTTF::create(str_shiqi->getCString(), FONT_BOXINFO, 30);
    shiqi->setAnchorPoint(ccp(1, 0.5));
    shiqi->setPosition(ccpAdd(mor_->getPosition(), ccp(120,0)));
    this->addChild(shiqi);
    
    CCString* strxuli = CCString::createWithFormat("%d",equipCard->getRound());
    SGCCLabelTTF* xuli = SGCCLabelTTF::create(strxuli->getCString(), FONT_BOXINFO, 30);
    xuli->setAnchorPoint(ccp(1, 0.5));
    xuli->setPosition(ccp(xuli_->getPosition().x + 120, atk->getPosition().y));
    this->addChild(xuli);
    
    CCString* str_spd = CCString::createWithFormat("%d",(int)equipCard->getSpeed());
    SGCCLabelTTF* speed = SGCCLabelTTF::create(str_spd->getCString(), FONT_BOXINFO, 30);
    speed->setAnchorPoint(ccp(1, 0.5));
    speed->setPosition(ccp(xuli_->getPosition().x + 120, def->getPosition().y));
    this->addChild(speed);
    
    CCString* str_gov = CCString::createWithFormat("%d",equipCard->getGoverning());
    SGCCLabelTTF* govern = SGCCLabelTTF::create(str_gov->getCString(), FONT_BOXINFO, 30);
    govern->setAnchorPoint(ccp(1, 0.5));
    govern->setPosition(ccp(gov_->getPosition().x+ 120-16, shiqi->getPosition().y));
    this->addChild(govern);

    SGCCLabelTTF* font_zjji = SGCCLabelTTF::create(str_GeneralEquipBox_str3, FONT_BOXINFO, 32 , COLOR_YELLOW);
    font_zjji->setPosition(ccpAdd(atk_->getPosition(), ccp(font_zjji->getContentSize().width*.25f,-170)));
    //modify by:zyc. merge into create.
    //font_zjji->setColor(COLOR_YELLOW);
    this->addChild(font_zjji);
        
    SGCCLabelTTF* label = SGCCLabelTTF::create("", FONT_BOXINFO, FONTSIZE(13));
    label->setAnchorPoint(ccp(0, 0.5));
    label->setPosition(ccpAdd(font_zjji->getPosition(), ccp(3-font_zjji->getContentSize().width*.5f, -40)));
    this->addChild(label);

    int itemid = equipCard->getItemId();
    if (itemid == 4007 || itemid ==4028 || itemid==4029 ||itemid == 4030 ||itemid == 4032||itemid == 4034||itemid == 4036||itemid == 4038) {
        
        SGSuitDataModel *suit = SGStaticDataManager::shareStatic()->getSuitById(itemid);
        if (suit ) {
            label->setString(CCString::createWithFormat("%s (1/%d)",suit->getSuitName()->getCString(),suit->getSuitCount())->getCString());
        }
    }
    else
    {
        label->setString(str_No);
    }

  
}

void SGGeneralEquipBox::strengHandler()
{
  
    EFFECT_PLAY(MUSIC_BTN);
    if (equipCard->getCurrExp() == equipCard->getMaxExp())
    {
         this->boxClose();
        SGMainManager::shareMain()->showMessage(str_qianghua_tips1);
        
    }else{
        
        SGMainManager::shareMain()->showStrengLayer(equipCard, 2,enterType,false);//hehe
        this->boxClose();
    }

}
void SGGeneralEquipBox::upHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (equipCard->getOfficerCardId() != 0) {        
    this->boxCloseWithOutCallBack();
    SGExchangeEquipBox *exchangeBox = SGExchangeEquipBox::create(dlg, officerCard,equipCard,enterTypeOther);
    SGMainManager::shareMain()->showBox(exchangeBox);

    }else{
    main::WearEquipmentRequest *request = new main::WearEquipmentRequest();
    request->set_officerid(officerCard->getSsid());
    request->set_equipmentid(equipCard->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_WEAR, request);
    }

}
void SGGeneralEquipBox::offHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::OffEquipmentRequest *request = new main::OffEquipmentRequest();
    request->set_officerid(officerCard->getSsid());
    request->set_equipmentid(equipCard->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_OFF, request);
    
}
void SGGeneralEquipBox::offEquipListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::OffEquipmentResponse *response = (main::OffEquipmentResponse *)sr->m_msg;
    if (sr)
    {
        if (response->state() == 1)
        {
            equipCard->setOfficerCardId(0);
            
            CCDictionary *dict = CCDictionary::create();
            dict->setObject(CCString::createWithFormat("%d",2), "type");
            data = dict;                        
            CCString *str = CCString::createWithFormat(str_GeneralEquipBox_str4,officerCard->getOfficerName()->getCString(),equipCard->getOfficerName()->getCString());
            SGMainManager::shareMain()->showMessage(str->getCString());
             SGMainManager::shareMain()->showEffect(equipCard, 2);
            officerCard->getOfficerEquips()->removeObject(equipCard);
            
            //首页布阵提示
            //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
            
            //刷新武将页面文本
            SGMainManager::shareMain()->refreshGilOfficerInfo();
            this->boxClose();
            
        }
    }    
}
void SGGeneralEquipBox::wearEquipListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::WearEquipmentResponse *response = (main::WearEquipmentResponse *)sr->m_msg;
    if (sr)
    {
        if (response->state() == 1)
        {
            equipCard->setOfficerCardId(officerCard->getSsid());
            officerCard->addEquip(equipCard);
            
            //首页布阵提示
            //SGMainManager::shareMain()->getMainLayer()->checkEmbattleNotice();
            
            CCString *str = CCString::createWithFormat(str_SGExchangeEquipBox_str5,officerCard->getOfficerName()->getCString(),equipCard->getOfficerName()->getCString());
            SGMainManager::shareMain()->showMessage(str->getCString());
             //
//            equipCard->retain();
//            officerCard->retain();
//               SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
//                SGGeneralInfoLayer *mainLayer = (SGGeneralInfoLayer *)mainScene->getChildByTag(sg_generalInfoLayer);
//                if (mainLayer) {
//                    mainLayer->setIsCanTouch(true);
//                    mainLayer->setVisible(true);
//                }

//
            
//            SGMainManager::shareMain()->showEffect(equipCard, 1);
             SGMainManager::shareMain()->showEffect(equipCard, 1);
            int type = enterTypeOther;
            this->boxClose();
            if(type==11||type==12)
            {
                SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,enterType,NULL,false,NULL,type);
            }else{
                SGMainManager::shareMain()->showGeneralInfoLayer(officerCard,enterType,NULL,false);
            }
            
        }
    }
    
}
void SGGeneralEquipBox::boxClose()
{
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
        mainLayer->setPortraitMenuCanTouch(false);
    }
    
    SGBaseBox::boxClose();
    
//    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *arranglayer = (SGMainLayer *)mainScene->getChildByTag(sg_equipsLayer);
    if (arranglayer) {
        mainScene->removeChildByTag(sg_equipsLayer);
    }

	//this->boxCloseWithOutCallBack();
   }
void SGGeneralEquipBox::boxCloseWithOutCallBack()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true);
    }
    
    SGBaseBox::boxCloseWithOutCallBack();
}
