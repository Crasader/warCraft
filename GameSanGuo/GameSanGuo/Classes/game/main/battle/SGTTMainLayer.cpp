//
//  SGTTMainLayer.cpp
//  GameSanGuo
//
//  Created by zhibaoli on 13-4-8.
//
//

#include "SGMainLayer.h"
#include "SGTTMainLayer.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
SGTTMainLayer::SGTTMainLayer()
:
gonggao(NULL),
chat(NULL),
name(NULL),
experience(NULL),
tili(NULL),
jl(NULL),
CD(NULL),
gold(NULL),
copper(NULL),
level(NULL),
layer(NULL),
gonggao_bg_3(NULL),
gonggaobg(NULL),
tilicd(NULL),
junlcd(NULL),
tilicdbg(NULL),
junlcdbg(NULL)
{
    
}
SGTTMainLayer::~SGTTMainLayer()
{
//    ResourceManager::sharedInstance()->unBindLayerTexture(sg_ttMainLayer);
}

SGTTMainLayer* SGTTMainLayer::create()
{
    SGTTMainLayer *mainLayer = new SGTTMainLayer();
    if (mainLayer && mainLayer->init(NULL, sg_mainLayer))
    {
        mainLayer->initView();
        mainLayer->autorelease();
        return mainLayer;
    }
    CC_SAFE_DELETE(mainLayer);
    return NULL;
}

void SGTTMainLayer::initView()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    const char *str = CCString::create(str_TTMainLayer_str1)->getCString();
    const int tIVar = 10;

    //    首页
    SGButton *homeBtn = SGButton::create("homeBtn.png", NULL, this, NULL, ccp(0, 0), false, false);
    this->addBtn(homeBtn);
    
    //    故事
    SGButton *storyBtn = SGButton::create("storyBtn.png", NULL, this, NULL, ccp(0, 0), false, false);
    this->addBtn(storyBtn);
    
    //    战斗
    SGButton *battleBtn = SGButton::create("battleBtn.png", NULL,  this, NULL, ccp(0, 0), false, false);
    this->addBtn(battleBtn);
    
    //    军营
    SGButton *barracksBtn = SGButton::create("barrackBtn.png", NULL, this, NULL, ccp(0, 0), false, false);
    this->addBtn(barracksBtn);
    
    //    拜访
    SGButton *visitBtn = SGButton::create("visitBtn.png", NULL,  this, NULL, ccp(0, 0), false, false);
    this->addBtn(visitBtn);
    
    //    商城
    SGButton *shopBtn = SGButton::create("shopBtn.png", NULL, this, NULL, ccp(0, 0), false, false);
    this->addBtn(shopBtn);
    
    CCSprite *Btnbackground = CCSprite::createWithSpriteFrameName("mainBtn_background_1.png");
    
    Btnbackground->setAnchorPoint(ccp(0.5, 0));
    Btnbackground->setPosition(SGLayout::getPoint(kBottomCenter));
    Btnbackground->setScaleX(s.width/Btnbackground->getContentSize().width);
    this->addChild(Btnbackground,-1);
    
    homeBtn->setPosition(ccp(s.width*0.75/6.3,Btnbackground->getContentSize().height*.47));
    storyBtn->setPosition(ccp(s.width*1.75/6.3, Btnbackground->getContentSize().height*.47));
    battleBtn->setPosition(ccp(s.width*2.75/6.3, Btnbackground->getContentSize().height*.47));
    barracksBtn->setPosition(ccp(s.width*3.75/6.3, Btnbackground->getContentSize().height*.47));
    visitBtn->setPosition(ccp(s.width*4.75/6.3, Btnbackground->getContentSize().height*.47));
    shopBtn->setPosition(ccp(s.width*5.75/6.3, Btnbackground->getContentSize().height*.47));
    
    CCSprite *chatbackground = CCSprite::createWithSpriteFrameName("chat_background.png");
    chatbackground->setAnchorPoint(ccp(0.5, 0));
    chatbackground->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, Btnbackground->getContentSize().height)));
    this->addChild(chatbackground,-1);
    
    SGButton *chatBtn = SGButton::create("chatbtn.png", "public_font_chat.png", this, NULL, ccp(0, 0), false, false);
    this->addBtn(chatBtn);
    chatBtn->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(s.width/2.5, chatbackground->getPosition().y + chatbackground->getContentSize().height*.55)));
    
    gonggaobg = CCSprite::create("public/gonggao_bg.png");
    gonggaobg->setAnchorPoint(ccp(0.5,1));
    gonggaobg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(gonggaobg,2);
    
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-gonggaobg->getContentSize().height)));
    this->addChild(gonggao_3_2,2);
    
    CCSprite *player_bg_2 = CCSprite::createWithSpriteFrameName("playerinfo_bg_2.png");
    player_bg_2->setAnchorPoint(ccp(0.5, 1));
    
    player_bg_2->setPosition(ccpAdd(gonggao_3_2->getPosition(), ccp(0,  - gonggao_3_2->getContentSize().height)));
    
    this->addChild(player_bg_2, -2);
    
    gonggao_bg_3 = CCSprite::create("public/gonggao_3_1.png");
    gonggao_bg_3->setAnchorPoint(ccp(0.5, 1));
    gonggao_bg_3->setTag(999);
    gonggao_bg_3->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-gonggaobg->getContentSize().height)));
    this->addChild(gonggao_bg_3,3);
    
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-gonggaobg->getContentSize().height)));
    this->addChild(gonggao_bg_3_l,3);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-gonggaobg->getContentSize().height)));
    this->addChild(gonggao_bg_3_r,3);
    
    CCSprite *gonggao_1 = CCSprite::create("public/gonggao_1.png");
    gonggao_1->setAnchorPoint(ccp(0.5, 1));
    gonggao_1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -3.5)));
    this->addChild(gonggao_1,3);
    
    CCSprite *gonggao_2 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    gonggao_2->setAnchorPoint(ccp(0.5, 0));
    gonggao_2->setScaleX(6);
    gonggao_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -gonggao_1->getContentSize().height)));
    this->addChild(gonggao_2,2);
    
    CCSprite *gonggao_left = CCSprite::createWithSpriteFrameName("gonggao_LR.png");
    gonggao_left->setAnchorPoint(ccp(0, 0));
    gonggao_left->setPosition(ccp(0, gonggao_2->getPosition().y));
    this->addChild(gonggao_left,2);
    
    CCSprite *gonggao_right =  CCSprite::createWithSpriteFrameName("gonggao_LR.png");
    gonggao_right->setFlipX(true);
    gonggao_right->setAnchorPoint(ccp(1, 0));
    gonggao_right->setPosition(ccp(s.width, gonggao_2->getPosition().y));
    this->addChild(gonggao_right,2);
    
    CCSprite *gonggao_3 = CCSprite::createWithSpriteFrameName("gonggao_xian.png");
    gonggao_3->setAnchorPoint(ccp(0.5, 0.28));
    gonggao_3->setScaleX(6.5);
    gonggao_3->setPosition(ccp(s.width/2,gonggao_bg_3->getPosition().y));
    this->addChild(gonggao_3,2);
    
    CCSprite *rightcorner = CCSprite::create("public/playerinfo_Rcorner.png");
    rightcorner->setAnchorPoint(ccp(1, 0));
    rightcorner->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height - gonggao_3_2->getContentSize().height)));
    this->addChild(rightcorner);
    
    CCSprite *leftcorner = CCSprite::create("public/playerinfo_Rcorner.png");
    leftcorner->setFlipX(true);
    leftcorner->setAnchorPoint(ccp(0, 0));
    
    leftcorner->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height  - gonggao_3_2->getContentSize().height)));
    this->addChild(leftcorner);
    
    CCSprite *btmbian = CCSprite::createWithSpriteFrameName("new_xian.png");
    btmbian->setAnchorPoint(ccp(0.5, 0));
    btmbian->setScaleX((s.width - leftcorner->getContentSize().width*2)/btmbian->getContentSize().width);
    this->addChild(btmbian);
    btmbian->setPosition(ccp(s.width/2, rightcorner->getPosition().y));
    
    CCSprite *rightbian = CCSprite::createWithSpriteFrameName("public_bian.png");
    rightbian->setAnchorPoint(ccp(1, 1));
    rightbian->setFlipX(true);
    this->addChild(rightbian,-1);
    rightbian->setScaleY(1.5);
    rightbian->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0, -gonggaobg->getContentSize().height)));
    
    CCSprite *leftbian =  CCSprite::createWithSpriteFrameName("public_bian.png");
    leftbian->setAnchorPoint(ccp(0, 1));
    this->addChild(leftbian,-1);
    leftbian->setScaleY(1.5);
    leftbian->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0, -gonggaobg->getContentSize().height)));
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(player_bg_2->getPosition(), ccp(0, - player_bg_2->getContentSize().height + leftcorner->getContentSize().height/25)));
    this->addChild(title_bg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height)));
    this->addChild(titlecenter);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(0,  title_bg->getPosition().y - title_bg->getContentSize().height)));
    this->addChild(title_bg_l);
    
    CCSprite *title_bg_r =CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, title_bg->getPosition().y - title_bg->getContentSize().height)));
    this->addChild(title_bg_r);
    
    //  头像
    char spriteName[256] = {0};
    sprintf(spriteName, "headIcon/head100%d.png",1);
    SGButton *portrait_bg = SGButton::create("player_portrait.png", spriteName, this, NULL, ccp(0, 0), false, false);
    portrait_bg->setAnchorPoint(ccp(0, 0));
    portrait_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(s.width/70, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height - gonggao_3_2->getContentSize().height)));
    portraitMenu = CCMenu::create(portrait_bg, NULL);
    this->addChild(portraitMenu,-1);
    portraitMenu->setPosition(CCPointZero);
    
    //昵称
    name = CCLabelTTF::create(str, FONT_PANGWA, 26);
    name->setColor(ccc3(0xff, 0xff, 0xff));
    name->setAnchorPoint(ccp(0, 0.5));
    this->addChild(name);
    name->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(portrait_bg->getPosition().x + portrait_bg->getContentSize().width*1.07, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height/3.5)));
    //经验
    CCSprite *expprog = CCSprite::create("public/exp.png");
    expprog->setAnchorPoint(ccp(0, 0.5));
    expprog->setPosition(ccpAdd(name->getPosition(), ccp(0,-name->getContentSize().height*1.4)));
    this->addChild(expprog);
    CCSprite *exppower = CCSprite::createWithSpriteFrameName("font_exp_tili.png");
    this->addChild(exppower);
    exppower->setAnchorPoint(ccp(0, 0.5));
    exppower->setPosition(expprog->getPosition());
    
    //元宝  银子
    CCSprite *money_bg = CCSprite::create("public/money_bg.png");
    money_bg->setAnchorPoint(ccp(0, 0.5));
    money_bg->setPosition(ccpAdd(exppower->getPosition(), ccp(0,-money_bg->getContentSize().height*1.3)));
    this->addChild(money_bg,-1);
    
    CCSprite *money_bg1 = CCSprite::create("public/money_bg.png");
    money_bg1->setAnchorPoint(ccp(0, 0.5));
    money_bg1->setPosition(ccpAdd(money_bg->getPosition(),ccp(money_bg->getContentSize().width*1.08,0)));
    this->addChild(money_bg1,-1);
    
    CCSprite *goldicon = CCSprite::createWithSpriteFrameName("goldicon.png");
    goldicon->setPosition(ccpAdd(money_bg->getPosition(), ccp(goldicon->getContentSize().width*.5,0)));
    this->addChild(goldicon);
    //金子
    CCString* str_gold = CCString::createWithFormat("%d", tIVar);
    gold = CCLabelTTF::create(str_gold->getCString(), "Arial-BoldMT", 24);
    gold->setColor(ccc3(0xff, 0xff, 0xff));
    gold->setAnchorPoint(ccp(0, 0.5));
    this->addChild(gold);
    gold->setPosition(ccpAdd(goldicon->getPosition(), ccp(goldicon->getContentSize().width/2,0)));
    CCSprite *silvericon = CCSprite::createWithSpriteFrameName("coinicon.png");
    silvericon->setPosition(ccpAdd(money_bg1->getPosition(),ccp(goldicon->getContentSize().width*.55,0)));
    this->addChild(silvericon);
    //铜钱
    CCString* str_coins = CCString::createWithFormat("%d", tIVar);
    copper = CCLabelTTF::create(str_coins->getCString(), "Arial-BoldMT", 24);
    copper->setAnchorPoint(ccp(0, 0.5));
    this->addChild(copper);
    copper->setPosition(ccpAdd(silvericon->getPosition(),ccp(goldicon->getContentSize().width*.55,0)));
    //公告
    gonggao = CCLabelTTF::create(str_TTMainLayer_str2, "方正行楷繁体", 24);
    gonggao->setColor(ccc3(0xFF, 0xCC, 0x66));
    gonggao->setAnchorPoint(ccp(0, 0.5));
    this->addChild(gonggao,4);
    gonggao->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(s.width/10, - (gonggaobg->getContentSize().height - gonggao_1->getContentSize().height*0.75)/2 - gonggao_1->getContentSize().height*0.75)));
    //聊天
    chat = CCLabelTTF::create(str_TTMainLayer_str3, FONT_BOXINFO,30);
    this->addChild(chat);
    chat->setAnchorPoint(ccp(0,0.5));
    chat->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(s.width/40, Btnbackground->getContentSize().height + chatbackground->getContentSize().height/2)));
    //体力
    CCSprite *font_tili = CCSprite::createWithSpriteFrameName("font_tili.png");
    this->addChild(font_tili);
    font_tili->setAnchorPoint(ccp(0, 0.5));
    font_tili->setPosition(ccp(money_bg1->getPosition().x, name->getPosition().y));
    //体力
    tili = CCLabelTTF::create("", "Arial-BoldMT", FONTSIZE(12));
    tili->setColor(ccc3(0xff, 0xff, 0xff));
    tili->setAnchorPoint(ccp(0, 0.5));
    this->addChild(tili);
    tili->setPosition(ccpAdd(font_tili->getPosition(), ccp(font_tili->getContentSize().width*1.2, 0)));
    
    //军粮
    CCSprite *junliang = CCSprite::createWithSpriteFrameName("font_junliang.png");
    this->addChild(junliang);
    junliang->setAnchorPoint(ccp(0, 0.5));
    junliang->setPosition(ccp(font_tili->getPosition().x, exppower->getPosition().y));
    
    jl = CCLabelTTF::create("", "Arial-BoldMT", FONTSIZE(12));
    jl->setAnchorPoint(ccp(0, 0.5));
    this->addChild(jl);
    jl->setPosition(ccp(tili->getPosition().x, junliang->getPosition().y));
    //级别
    CCSprite *lvlbg = CCSprite::createWithSpriteFrameName("levelbg.png");
    this->addChild(lvlbg);
    lvlbg->setPosition(ccpAdd(portrait_bg->getPosition(), ccp(portrait_bg->getContentSize().width*.9, +portrait_bg->getContentSize().height*.18)));
    
    CCString* str_level = CCString::createWithFormat("%d", tIVar);
    level = CCLabelTTF::create(str_level->getCString(), "Arial-BoldMT", FONTSIZE(12));
    level->setColor(ccc3(0xff, 0xff, 0xff));
    //    CCRenderTexture *lelbg = SGFont::createStroke(level, 1, ccc3(0, 0, 0));
    //    this->addChild(lelbg);
    this->addChild(level);
    level->setPosition(lvlbg->getPosition());
    //    level->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(powerprog->getPosition().x + powerprog->getContentSize().width, -gonggaobg->getContentSize().height - player_bg_2->getContentSize().height/3.5)));
    //    lelbg->setPosition(ccpAdd(level->getPosition(), ccp(-level->getContentSize().width/2, 0)));
    
    tilicdbg = CCSprite::createWithSpriteFrameName("touming_kuang.png");
    this->addChild(tilicdbg);
    tilicdbg->setAnchorPoint(ccp(0.4, 0.5));
    tilicdbg->setPosition(ccpAdd(font_tili->getPosition(), ccp(money_bg->getContentSize().width, 0)));
    junlcdbg = CCSprite::createWithSpriteFrameName("touming_kuang.png");
    this->addChild(junlcdbg);
    junlcdbg->setAnchorPoint(ccp(0.4, 0.5));
    junlcdbg->setPosition(ccp(tilicdbg->getPosition().x, junliang->getPosition().y));
    
    tilicd = CCLabelTTF::create(str_TTMainLayer_str4, FONT_BOXINFO, FONTSIZE(11));
    tilicd->setAnchorPoint(ccp(0, 0.5));
    this->addChild(tilicd);
    tilicd->setPosition(ccpAdd(tilicdbg->getPosition(), ccp(-tilicdbg->getContentSize().width*.3, 0)));
    
    junlcd = CCLabelTTF::create(str_TTMainLayer_str4, FONT_BOXINFO, FONTSIZE(11));
    junlcd->setAnchorPoint(ccp(0, 0.5));
    this->addChild(junlcd);
    junlcd->setPosition(ccpAdd(junlcdbg->getPosition(), ccp(-tilicdbg->getContentSize().width*.3, 0)));
    
    //隐藏冷却时间
    junlcdbg->setVisible(false);
    junlcd->setVisible(false);
    tilicd->setVisible(false);
    tilicdbg->setVisible(false);
}
