//
//  SGSkillInfoBox.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-2-21.
//
//

#include "SGSkillInfoBox.h"
#include "SGFont.h"
#include "SGMainManager.h"
#include "SGStaticDataManager.h"
#include "SGSkillDataModel.h"
#include "SGRegisterLayer.h"
#include "SGCantAdvanceBox.h"
#include "SGServerListLoginGame.h"
#include "SGStrengLayer.h"
#include "SGMainLayer.h"
#include "ResourceManager.h"
#include "SGServerListLoginGame.h"
#include "SGStringConfig.h"


SGSkillInfoBox::SGSkillInfoBox():
_dele(NULL),
_card(NULL),
enterType(0)
{
}
SGSkillInfoBox::~SGSkillInfoBox()
{
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_skillInfoBox);

}

SGSkillInfoBox *SGSkillInfoBox::create(SGBoxDelegate *bdg, SGOfficerCard *card,int type)
{
    SGSkillInfoBox *skillInfoBox = new SGSkillInfoBox();
    skillInfoBox->_card = card;
    skillInfoBox->_dele = bdg;
    if (skillInfoBox && skillInfoBox->init(bdg,card,type))
    {              
        skillInfoBox->autorelease();
        return skillInfoBox;
    }
    CC_SAFE_DELETE(skillInfoBox);
    return NULL;
}
bool SGSkillInfoBox::init(SGBoxDelegate *bdg, SGOfficerCard *card,int type)
{
    _card = card;
    
    if (_card) {
        _card->retain();
        _card->autorelease();
    }
    
 
    
    ResourceManager::sharedInstance()->bindTexture("sgsoldierslayer/sgsoldierslayer.plist", RES_TYPE_LAYER_UI, sg_skillInfoBox, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgskillcardslayer/sgskillcardslayer.plist", RES_TYPE_LAYER_UI, sg_skillInfoBox);
    ResourceManager::sharedInstance()->bindTexture("sgcantadvancebox/sgcantadvancebox.plist", RES_TYPE_LAYER_UI, sg_skillInfoBox, LIM_PNG_AS_PNG);
   
    if (type == 5 || type == 6) {
        ResourceManager::sharedInstance()->bindTexture("sgloginingbox/sgloginingbox.plist", RES_TYPE_LAYER_UI, sg_skillInfoBox);
    }
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    enterType = type;
    float wid = 535;
    float hgt = 340;
 
    if (type == 1 || type == 3 || type == 8 ||type == 9)
    {
        if (!SGBaseBox::init(bdg,box_skillInfo,CCSizeMake(535, 480),false,true,CCSizeMake(535, 135)))
        {
            return false;
        }
     
        fontbg = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("box_fontbg.png");
        this->addChild(fontbg,0);
        fontbg->setPreferredSize(CCSizeMake(535-55, 280));
        fontbg->setPosition(ccpAdd(center, ccp(0, 90)));
        
    }
    else
    {
        if (type == 4)
        {
            hgt = 500;
        }
        if ( (enterType != 616) && ( !SGBaseBox::init(bdg,box_skillInfo,CCSizeMake(wid, hgt)) ) )
        {
            return false;
        }
        else if (616 == enterType )//facebook  进来  超框。
        {
            SGBaseBox::init(bdg,box_skillInfo,CCSizeMake(535, 400) ) ;
        }
    }
        
    if (enterType == 1 || enterType == 8) {
        
        if (enterType == 1) {
            SGButton *ok = SGButton::create("box_btn_newbg.png",
                                            "font_skill_streng.png",
                                            this,
                                            menu_selector(SGSkillInfoBox::strengHandler),
                                            ccp(0, 0),
                                            false,
                                            true);
            this->addBtn(ok);
            SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                                "public_font_quxiao.png",
                                                this,
                                                menu_selector(SGSkillInfoBox::boxClose),
                                                ccp(0, 0),
                                                false,
                                                true);
            this->addBtn(cancel);
            ok->setPosition(ccpAdd(center, ccp(-145, -529/2+5 -20)));
            cancel->setPosition(ccpAdd(center, ccp(145, -529/2+5-20)));
        }
        else if (enterType == 8)
        {
            SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                                "public_font_queding.png",
                                                this,
                                                menu_selector(SGSkillInfoBox::boxClose),
                                                ccp(0, 0),
                                                false,
                                                true);
            this->addBtn(cancel);
            cancel->setPosition(ccpAdd(center, ccp(0, -529/2+5-28)));
        }

           //读取主将计
        SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(_card->getLordId());
        SGCCLabelTTF *title = SGCCLabelTTF::create("", FONT_PANGWA, 36);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        if (lordSkill) {
            title->setString(lordSkill->getSkillName()->getCString());
            SGCCLabelTTF* a = SGCCLabelTTF::create(lordSkill->getSkillInfo()->getCString(), FONT_BOXINFO, 32, CCSizeMake(32*13,350));
            a->setAnchorPoint(ccp(0.5, 1));
            this->addChild(a);
            a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        }else{
            title->setString(str_EmbattleLayer_str15);
            SGCCLabelTTF*a = SGCCLabelTTF::create(str_GeneralInfoEffect_str1, FONT_BOXINFO, 32);
            this->addChild(a);
            a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -200)));
        }
        
        CCSprite *huawenl = CCSprite::createWithSpriteFrameName("boxhuawen.png");
        this->addChild(huawenl,100);
        huawenl->setPosition(ccpAdd(title->getPosition(), ccp(-title->getContentSize().width*.5 - 50, 0)));
        huawenl->setFlipX(true);
        
        CCSprite *huawenr = CCSprite::createWithSpriteFrameName("boxhuawen.png");
        this->addChild(huawenr,100);
        huawenr->setPosition(ccpAdd(title->getPosition(), ccp(title->getContentSize().width*.5 +50, 0)));

        SGCCLabelTTF*c = SGCCLabelTTF::create(str_SkillInfoBox_str1, FONT_BOXINFO, 28  , ccc3(0xc4, 0xc4, 0xc4));
        this->addChild(c);
        c->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,-165)));
        SGCCLabelTTF*d = SGCCLabelTTF::create(str_SkillInfoBox_str2, FONT_BOXINFO, 28 , ccc3(0xc4, 0xc4, 0xc4));
        this->addChild(d);
        d->setPosition(ccpAdd(c->getPosition(), ccp(-c->getContentSize().width/2 + d->getContentSize().width/2,-c->getContentSize().height*1.5)));
        //modify by:zyc. merge into create.
        //c->setColor(ccc3(0xc4, 0xc4, 0xc4));
        //d->setColor(ccc3(0xc4, 0xc4, 0xc4));
        
    }
    if (enterType == 3 || enterType == 9) {
        if (enterType == 3) {
            SGButton *ok = SGButton::create("box_btn_newbg.png",
                                            "font_skill_streng.png",
                                            this,
                                            menu_selector(SGSkillInfoBox::strengHandler),
                                            ccp(0, 0),
                                            false,
                                            true);
            this->addBtn(ok);
            
            if (_card->getOfficerSkil() == 0) {
                ok->setEnabled(false);
                ok->setImage("quexiao_btn_bg.png");
            }
            
            SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                                "public_font_quxiao.png",
                                                this,
                                                menu_selector(SGSkillInfoBox::boxClose),
                                                ccp(0, 0),
                                                false,
                                                true);
            this->addBtn(cancel);
            ok->setPosition(ccpAdd(center, ccp(-145, -529/2+5 -20)));
            cancel->setPosition(ccpAdd(center, ccp(145, -529/2+5-20)));
        }
        else if (enterType == 9)
        {
            SGButton *cancel = SGButton::create("box_btn_newbg.png",
                                                "public_font_queding.png",
                                                this,
                                                menu_selector(SGSkillInfoBox::boxClose),
                                                ccp(0, 0),
                                                false,
                                                true);
            this->addBtn(cancel);
            cancel->setPosition(ccpAdd(center, ccp(0, -529/2+5-20)));
        }
        
        //读取被动技
        
        SGSkillDataModel *officerSkill = SGStaticDataManager::shareStatic()->getGeneralSkillById(_card->getOfficerSkil());
        SGCCLabelTTF *title = SGCCLabelTTF::create( "", FONT_PANGWA, 34);
        title->setPosition(biao->getPosition());
        this->addChild(title);
        if (officerSkill) {
            title->setString(officerSkill->getSkillName()->getCString());
            SGCCLabelTTF* a = SGCCLabelTTF::create(officerSkill->getSkillInfo()->getCString(), FONT_BOXINFO, 32, CCSizeMake(32*13,350));
            this->addChild(a);
            a->setAnchorPoint(ccp(0.5, 1));
            a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -90)));
        }else{
            title->setString(str_GeneralInfoEffect_str2);
            SGCCLabelTTF*a = SGCCLabelTTF::create(str_GeneralInfoEffect_str3, FONT_BOXINFO, 32);
            this->addChild(a);
            a->setPosition(ccpAdd(biao->getPosition(), ccp(0, -200)));
        }
        
        CCSprite *huawenl = CCSprite::createWithSpriteFrameName("boxhuawen.png");
        this->addChild(huawenl,100);
        huawenl->setPosition(ccpAdd(title->getPosition(), ccp(-title->getContentSize().width*.5 - 50, 0)));
        huawenl->setFlipX(true);
        
        CCSprite *huawenr = CCSprite::createWithSpriteFrameName("boxhuawen.png");
        this->addChild(huawenr,100);
        huawenr->setPosition(ccpAdd(title->getPosition(), ccp(title->getContentSize().width*.5 +50, 0)));
        
        SGCCLabelTTF*c = SGCCLabelTTF::create(str_SkillInfoBox_str3, FONT_BOXINFO, 28 , ccc3(0xc4, 0xc4, 0xc4));
        this->addChild(c);
        c->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0,-155)));
        SGCCLabelTTF*d = SGCCLabelTTF::create(str_SkillInfoBox_str4, FONT_BOXINFO, 28 , ccc3(0xc4, 0xc4, 0xc4));
        this->addChild(d);
        d->setPosition(ccpAdd(c->getPosition(), ccp(-c->getContentSize().width/2 + d->getContentSize().width/2,-c->getContentSize().height*1.5)));
        //modify by:zyc. merge into create.
        //c->setColor(ccc3(0xc4, 0xc4, 0xc4));
        //d->setColor(ccc3(0xc4, 0xc4, 0xc4));
    }

    if (enterType == 2)
    {
        SGCCLabelTTF* title = SGCCLabelTTF::create(str_CantAdvance_tips28, FONT_PANGWA, 40 , ccWHITE , ccc3(143, 87, 0));

        title->setPosition(biao->getPosition());
        this->addChild(title);
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_SkillInfoBox_str5, FONT_BOXINFO, 30);
        this->addChild(a);
        a->setPosition(ccpAdd(biao->getPosition(),ccp(0, -80)));
        SGCCLabelTTF*b = SGCCLabelTTF::create(str_SkillInfoBox_str6, FONT_BOXINFO,30);
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2, -50)));
        
        SGCCLabelTTF*c = SGCCLabelTTF::create(str_SkillInfoBox_str7, FONT_BOXINFO, 30);
        this->addChild(c);
        c->setPosition(ccpAdd(b->getPosition(), ccp(-b->getContentSize().width/2 + c->getContentSize().width/2, -50)));
        
        
        SGButton *ok = SGButton::create("box_btn_newbg.png",
                                        "public_font_queding.png",
                                        this,
                                        menu_selector(SGSkillInfoBox::confirmHandler),
                                        ccp(0, 0),
                                        false,
                                        true);
        this->addBtn(ok);

        ok->setPosition(ccpAdd(center, ccp(0,-ok->getContentSize().height*2.5)));
    }
    else if(enterType == 4)
    {
    //   士兵卡介绍
        CCSprite *title = CCSprite::createWithSpriteFrameName("barrack_font_sbkjs.png");
        title->setPosition(biao->getPosition());
        this->addChild(title);
        SGButton *ok = SGButton::create("box_btn_newbg.png",
                                        "public_font_queding.png",
                                        this,
                                        menu_selector(SGSkillInfoBox::boxClose),
                                        ccp(0, 0),
                                        false,
                                        true);
        this->addBtn(ok);
        ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -hgt/2+40)));
        
        SGCCLabelTTF* a = SGCCLabelTTF::create(str_SkillInfoBox_str8, FONT_PANGWA, 30, CCSizeMake(32*13,80));
        this->addChild(a);
        a->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, hgt*.25)));
            
        SGCCLabelTTF* b = SGCCLabelTTF::create(str_SkillInfoBox_str9, FONT_PANGWA, 30, CCSizeMake(32*13,180));
        this->addChild(b);
        b->setPosition(ccpAdd(a->getPosition(), ccp(-a->getContentSize().width/2 + b->getContentSize().width/2,- a->getContentSize().height*2)));
            
        SGCCLabelTTF* c = SGCCLabelTTF::create(str_SkillInfoBox_str10, FONT_PANGWA, 30, CCSizeMake(32*13,180));
        this->addChild(c);
        c->setPosition(ccpAdd(b->getPosition(), ccp(-b->getContentSize().width/2 + c->getContentSize().width/2, - a->getContentSize().height*1.3)));
    }
    else if(enterType == 5 ||enterType == 6 ||enterType == 616)
    {        
        
        SGButton *ok = NULL;
        SGButton *cancel = NULL;
        if (enterType == 616) {
            
            ok =  SGButton::createFromLocal("box_btn_newbg.png",
                                            str_SkillInfoBox_str11,
                                            this,
                                            menu_selector(SGSkillInfoBox::bindFbHandler),
                                            CCPointZero,FONT_PANGWA,ccWHITE,32,
                                            false,
                                            true);
            
//            cancel = SGButton::createFromLocal("box_btn_newbg.png",
//                                               str_SkillInfoBox_str14,//login_font_qczh.png
//                                               this,
//                                               menu_selector(SGSkillInfoBox::cleanFbHandler),
//                                               CCPointZero,FONT_PANGWA,ccWHITE,32,
//                                               false,
//                                               true);
            
            ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.2, -hgt*.65+90 -18)));
//            cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.33, -hgt*.65+90 -18)));
            
            
            
        }
        else
        {
            ok =  SGButton::createFromLocal("box_btn_newbg.png",
                                                 str_CantAdvance_tips49,
                                                 this,
                                                 menu_selector(SGSkillInfoBox::bindHandler),
                                                 CCPointZero,FONT_PANGWA,ccWHITE,32,
                                                 false,
                                                 true);
            

            
            
            cancel = SGButton::createFromLocal("box_btn_newbg.png",
                                      str_SkillInfoBox_str12,//login_font_qczh.png
                                      this,
                                      menu_selector(SGSkillInfoBox::cleanHandler),
                                      CCPointZero,FONT_PANGWA,ccWHITE,32,
                                      false,
                                      true);
            ok->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.2, -hgt*.65+90 +4)));
            
            cancel->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.33, -hgt*.65+90 +4)));
        }

        this->addBtn(ok);
        //this->addBtn(cancel);


        SGButton *buttonGoback = SGButton::createFromLocal("box_btn_newbg.png",
                                                     str_back,//login_font_qczh.png
                                                     this,
                                                     menu_selector(SGSkillInfoBox::backHandler),
                                                     CCPointZero,FONT_PANGWA,ccWHITE,32,
                                                     false,
                                                     true);
        this->addBtn(buttonGoback);
        if (enterType == 616)
        {
            buttonGoback->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.2, -hgt*.65+90 -18 )));
        }
        else
        {
            buttonGoback->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(wid*.2, -hgt*.65+90 +4 )));
        }
        
        
//        SGButton *buttonGoback = SGButton::create("public_btn_6.png","public_fanhui.png" , this, menu_selector(SGSkillInfoBox::backHandler));
//        this->addBtn(buttonGoback);
//        buttonGoback->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(-wid*.38, hgt*.41)));
        
//        CCSprite *title = CCSprite::createWithSpriteFrameName("login_font_notice.png");
        SGCCLabelTTF * title =  SGCCLabelTTF::create(str_CantAdvance_tips55, FONT_PANGWA, 36 , ccWHITE , ccc3(143, 87, 0));
        //modify by:zyc . merge into create.
        //title->setOutSideColor(ccc3(143,87,0));
        title->setPosition(biao->getPosition());
        this->addChild(title);
   
        SGCCLabelTTF* test = NULL;
        CCSize testTtfSize = CCSizeMake(470, 32*6);
        if (enterType == 616)
        {
            test = SGCCLabelTTF::create(str_SkillInfoBox_str15, FONT_PANGWA, 32, testTtfSize);
        }
        else
        {
            test = SGCCLabelTTF::create(str_SkillInfoBox_str13, FONT_PANGWA, 32, testTtfSize);
        }
#if (PLATFORM == IOS)
        test->setPosition(ccpAdd(title->getPosition(),ccp(2, -32*5)));
#else
        test->setPosition(ccpAdd(title->getPosition(),ccp(2, -32*3.4)));
#endif
        this->addChild(test);
    }    
    return true;
}

void SGSkillInfoBox::confirmHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    this->boxClose();
   SGMainManager::shareMain()->showStrengLayer(_card,1);
}
void SGSkillInfoBox::bindFbHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGWelComeLayer* welcomeLayer = (SGWelComeLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_welComeLayer);
    if (welcomeLayer) {
        welcomeLayer->doFacebookBind();
        welcomeLayer->setIsCanTouch(true);
    }
   
    this->boxClose();

}

void SGSkillInfoBox::bindHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showregisterLayer(2,false);
    data = CCString::createWithFormat("%d",-1);
    
#if (PLATFORM == IOS)
    this->boxClose();
#else
    this->setVisible(false);
    this->boxClose();
#endif
    
//    SGRegisterBox *registerBox = SGRegisterBox::create(_dele,2);
//    this->boxClose();
//    SGMainManager::shareMain()->showBox(registerBox);
}


void SGSkillInfoBox::cleanFbHandler()
{
//    EFFECT_PLAY(MUSIC_BTN);
//    SGMainManager::shareMain()->cleanAccount();
//    SGWelComeLayer* welcomeLayer = (SGWelComeLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_welComeLayer);
//    if (welcomeLayer) {
//        welcomeLayer->setIsCanTouch(true);
//        welcomeLayer->refreshRegisterBtn();
//    }
//    
//    this->boxClose();
}

void SGSkillInfoBox::cleanHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGServerListLoginGame* loginLayer = (SGServerListLoginGame*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_listLoginLayer);
    if (loginLayer)
    {
        loginLayer->setIsNewAccount(true);
        this->boxClose();
    }
    else
    {
        SGCantAdvanceBox *Box = SGCantAdvanceBox::create(_dele, NULL, 22, NULL);
        this->boxClose();
        SGMainManager::shareMain()->showBox(Box);
    }
}
void SGSkillInfoBox::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGServerListLoginGame* loginLayer = (SGServerListLoginGame*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_listLoginLayer);
    if (loginLayer) {
        loginLayer->setIsNewAccount(false);
    }
    this->boxClose();
}
void SGSkillInfoBox::strengHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (enterType == 1)
    {
        if (_card->getLordId()) {
            this->boxClose();
            
            SGMainManager::shareMain()->showStrengLayer(_card, 1,false,true);
        }
    }else
    {
    this->boxClose();
    
    SGMainManager::shareMain()->showStrengLayer(_card, 4,false);
    }
}
void SGSkillInfoBox::boxClose()
{
 EFFECT_PLAY(MUSIC_BTN);
    SGBaseBox::boxClose();
    
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,false);
    }
}
