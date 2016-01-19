//
//  SGInviteNum.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-12.
//
//

#include "SGInviteNum.h"
#include "SGMainManager.h"
#include "SGCantAdvanceBox.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGInviteNum::SGInviteNum()
:editNum(NULL)
{
    
}

SGInviteNum::~SGInviteNum()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_inviteNum);
   
}

SGInviteNum* SGInviteNum::create()
{
    SGInviteNum *invite = new SGInviteNum();
    if(invite && invite->init(NULL, sg_inviteNum))
    {        
        invite->initView();
        invite->autorelease();
        return invite;
    }
    
    CC_SAFE_DELETE(invite);
    return NULL;
}
void SGInviteNum::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgrewardlayer/sgrewardlayer.plist",RES_TYPE_LAYER_UI ,sg_inviteNum);
    ResourceManager::sharedInstance()->bindTexture("sgsystemmaillayer/sgsystemmaillayer.plist",RES_TYPE_LAYER_UI ,sg_inviteNum);
    ResourceManager::sharedInstance()->bindTexture("sgserverlistlayer/sgserverlistlayer.plist",RES_TYPE_LAYER_UI ,sg_inviteNum);

    
    float headhgt = SGMainManager::shareMain()->getTotleHdHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_inviteNum);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(384- s.width/2, 1136/2 - (s.height - headhgt - btmhgt)/2, s.width, s.height - headhgt - btmhgt);
    //bg->cocos2d::CCNode::setScale(r.size.width / bg->getContentSize().width, r.size.height / bg->getContentSize().height);

    bg->setTextureRect(r);
    this->addChild(bg,-5);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt)));
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -headhgt + title_bg->getContentSize().height-2)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(title_bg->getPosition(), ccp(-s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(title_bg->getPosition(), ccp(s.width/2,  - title_bg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGInviteNum::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGInviteNum::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.55,title_bg->getPosition().y -title_bg->getContentSize().height*.5f));

    CCSprite *title = CCSprite::createWithSpriteFrameName("help_font_txyqm.png");
    title->setPosition(ccp(s.width/2, backBtn->getPosition().y));
    this->addChild(title);

    CCSprite *kuang1 = CCSprite::createWithSpriteFrameName("server_BlueBG.png");
    kuang1->setScaleX(SGLayout::getSkewing(280)/kuang1->getContentSize().width);
    kuang1->setScaleY(200/kuang1->getContentSize().height);
    this->addChild(kuang1);       
    kuang1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -79-150-72-120)));
    CCScale9Sprite *spriteBorder1 = CCScale9Sprite::createWithSpriteFrameName("public_jiao.png");
    spriteBorder1->setPreferredSize(CCSizeMake(SGLayout::getSkewing(280), 200));
    spriteBorder1->setPosition(kuang1->getPosition());
    this->addChild(spriteBorder1);
    
    CCLabelTTF *labelNike = CCLabelTTF::create(str_InviteNum_str1, FONT_BOXINFO, FONTSIZE(12));
    labelNike->setAnchorPoint(ccp(0, 1));
    labelNike->setPosition(ccpAdd(kuang1->getPosition(), ccp(-kuang1->getContentSize().width/2+30, kuang1->getContentSize().height/2+10)));
    this->addChild(labelNike);
    editNum = CCEditBox::create(CCSizeMake(SGLayout::getSkewing(250),50), CCScale9Sprite::createWithSpriteFrameName("public_kuang_input.png"));
    editNum->setAnchorPoint(ccp(0.5, 0.5));
    editNum->setPosition(ccpAdd(kuang1->getPosition(),ccp(0, 10)));
    editNum->setFontColor(ccBLACK);
    this->addChild(editNum);
    SGButton *buttonSearch1 = SGButton::create("public_btn_1.png", "public_font_queding.png", this, menu_selector(SGInviteNum::okHandler),CCPointZero,false,true);
    buttonSearch1->setAnchorPoint(ccp(0.5, 1));
    buttonSearch1->setPosition(ccpAdd(kuang1->getPosition(), ccp(0, -35)));
    this->addBtn(buttonSearch1);

    CCSprite *hd = CCSprite::createWithSpriteFrameName("hd.png");
    hd->setScaleY(200/hd->getContentSize().height);
    this->addChild(hd);
    hd->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -170)));
    
    CCLabelTTF *label1 = CCLabelTTF::create(str_InviteNum_str2, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label2 = CCLabelTTF::create(str_InviteNum_str3, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label3 = CCLabelTTF::create(str_InviteNum_str1, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label4 = CCLabelTTF::create(str_InviteNum_str4, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label5 = CCLabelTTF::create(str_InviteNum_str5, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label6 = CCLabelTTF::create(str_InviteNum_str6, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label7 = CCLabelTTF::create(str_InviteNum_str7, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label8 = CCLabelTTF::create(str_InviteNum_str8, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label9 = CCLabelTTF::create(str_InviteNum_str9, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label10 = CCLabelTTF::create(str_InviteNum_str10, FONT_BOXINFO, FONTSIZE(12));
    CCLabelTTF *label11 = CCLabelTTF::create(".", FONT_BOXINFO, FONTSIZE(12));
    label3->setColor(ccRED);
    label6->setColor(ccRED);
    label8->setColor(ccRED);
    label10->setColor(ccRED);
    label1->setAnchorPoint(ccp(0, 0.5));
    label2->setAnchorPoint(ccp(0, 0.5));
    label3->setAnchorPoint(ccp(0, 0.5));
    label4->setAnchorPoint(ccp(0, 0.5));
    label5->setAnchorPoint(ccp(0, 0.5));
    label6->setAnchorPoint(ccp(0, 0.5));
    label7->setAnchorPoint(ccp(0, 0.5));
    label8->setAnchorPoint(ccp(0, 0.5));
    label9->setAnchorPoint(ccp(0, 0.5));
    label10->setAnchorPoint(ccp(0, 0.5));
    label11->setAnchorPoint(ccp(0, 0.5));
    label2->setPosition(ccpAdd(hd->getPosition(), ccp(-s.width*.45, 25)));
    label3->setPosition(ccpAdd(label2->getPosition(), ccp(label2->getContentSize().width, 0)));
    label4->setPosition(ccpAdd(label3->getPosition(), ccp(label3->getContentSize().width, 0)));
    label1->setPosition(ccpAdd(label2->getPosition(), ccp(0 ,50)));
    label5->setPosition(ccpAdd(label2->getPosition(), ccp(0, -50)));
    label6->setPosition(ccpAdd(label5->getPosition(), ccp(label5->getContentSize().width, 0)));
    label7->setPosition(ccpAdd(label5->getPosition(), ccp(0, -50)));
    label8->setPosition(ccpAdd(label7->getPosition(), ccp(label7->getContentSize().width, 0)));
    label9->setPosition(ccpAdd(label8->getPosition(), ccp(label8->getContentSize().width, 0)));
    label10->setPosition(ccpAdd(label9->getPosition(), ccp(label9->getContentSize().width, 0)));
    label11->setPosition(ccpAdd(label10->getPosition(), ccp(label10->getContentSize().width, 0)));
    this->addChild(label1);
    this->addChild(label2);
    this->addChild(label3);
    this->addChild(label4);
    this->addChild(label5);
    this->addChild(label6);
    this->addChild(label7);
    this->addChild(label8);
    this->addChild(label9);
    this->addChild(label10);
    this->addChild(label11);
    
}
void SGInviteNum::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGMainManager::shareMain()->showHelpLayer();
}
void SGInviteNum::okHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    SGCantAdvanceBox *succeed = SGCantAdvanceBox::create(this, NULL,7,NULL);
    SGMainManager::shareMain()->showBox(succeed);
}
