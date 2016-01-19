//
//  SGCountrysInfo.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-4-12.
//
//

#include "SGCountrysInfo.h"
#include "SGMainManager.h"
#include "ResourceManager.h"
#include "SGRankCell.h"
#include "SGRankLayer.h"
#include "SGStringConfig.h"
#include "SGStaticDataManager.h"

SGCountrysInfo::SGCountrysInfo()
:
btn_wei(NULL),
btn_shu(NULL),
btn_wu(NULL),
label_1(NULL),
label_2(NULL),
label_3(NULL),
countrytitle(NULL),
entertype(0),
_countrytype(0)

{
    entertype=0;
    _countrytype=-10;

}

SGCountrysInfo::~SGCountrysInfo()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_countrysInfo);
    
}
SGCountrysInfo *SGCountrysInfo::create(int type,int countrytype)
{
    SGCountrysInfo *countryLayer = new SGCountrysInfo();
    if (countryLayer && countryLayer->init(NULL, sg_countrysInfo))
    {   countryLayer->entertype=type;
        countryLayer->_countrytype=countrytype;
        countryLayer->initView();
        countryLayer->autorelease();
        return countryLayer;
    }
    CC_SAFE_DELETE(countryLayer);
    return NULL;
}
void SGCountrysInfo::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgcountrylayer/sgcountrylayer.plist",RES_TYPE_LAYER_UI ,sg_countrysInfo);
    ResourceManager::sharedInstance()->bindTexture("sgsearchfriend/sgsearchfriend.plist",RES_TYPE_LAYER_UI ,sg_countrysInfo);
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist",RES_TYPE_LAYER_UI ,sg_countrysInfo, LIM_PNG_AS_PNG);
    
    float bottmoheight = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();
   
    
    CCSprite* title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(SGLayout::getPoint(kUpCenter));
    this->addChild(title_bg,-1);
    
    CCSprite* titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(title_bg->getPosition(), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(titlecenter,2);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight), ccp(0,  - title_bg->getContentSize().height - 10)));
    this->addChild(title_bg_r,10);
    
//    CCSprite *texing = CCSprite::createWithSpriteFrameName("font_country_tx.png");
    SGCCLabelTTF *texing = SGCCLabelTTF::create(str_CountrysInfo_str1, FONT_XINGKAI, 36 , ccc3(0xe7, 0xb6, 0x7b));
    //modify by:zyc. merge into create.
	//texing->setColor(ccc3(0xe7, 0xb6, 0x7b));
    this->addChild(texing);
    texing->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height*.52)));

//    SGButton *backBtn = SGButton::create("chatbtn.png",
//                                         "public_font_fanhui.png",
//                                         this,
//                                         menu_selector(SGCountrysInfo::backHandler),
//                                         ccp(0, 0),
//                                         false,
//                                         true);
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGCountrysInfo::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft), ccp( backBtn->getContentSize().width*.55, - backBtn->getContentSize().height*.55)));

//    float hgt = skewingY(238);
//    if (s.height == 1136) {       
//        hgt = skewing(238);
//    }
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/storyBg.plist", RES_TYPE_LAYER_UI, sg_countrysInfo);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("storyBg.png");
    //CCRect r = CCRectMake(0, 1136/2 - (s.height -bottmoheight - title_bg->getContentSize().height)/2,
                          //s.width, s.height -bottmoheight - title_bg->getContentSize().height);
    //bg->setTextureRect(r);
    bg->setScaleX(CCDirector::sharedDirector()->getWinSize().width / bg->getContentSize().width * 1.02);
    
    int shift = -20;
    if (CCDirector::sharedDirector()->getWinSize().height == 1136)
    {
        bg->setScaleY(1.1);
    }
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,bottmoheight + shift - title_bg->getContentSize().height/2)));
    this->addChild(bg,-1);
    
    CCSize size123 = CCSizeMake(470, 80);
    
    label_1 = SGCCLabelTTF::create("", FONT_BOXINFO, 26, size123);
    label_2 =  SGCCLabelTTF::create("", FONT_BOXINFO, 26, size123);
    label_3 =  SGCCLabelTTF::create("", FONT_BOXINFO, 26, size123);
    this->addChild(label_1,10);
    this->addChild(label_2,10);
    this->addChild(label_3,10);

    for (int i = 0; i<3 ; i++) {
        CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
        frame1->setPreferredSize(CCSizeMake(598, 172));
        frame1->setPosition(ccpAdd(SGLayout::getPoint(kMiddleCenter), ccp(0, -21+skewingY(95) - skewingY(95)*i)));
        this->addChild(frame1,7);
        
        CCSprite *frame1bg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
        this->addChild(frame1bg,2);
        frame1bg->setScaleX(600/frame1bg->getContentSize().width);
        frame1bg->setScaleY(172/frame1bg->getContentSize().height);
        frame1bg->setPosition(frame1->getPosition());
        
        CCSprite *officerframeup = CCSprite::createWithSpriteFrameName("embatte_red.png");
        this->addChild(officerframeup,4);
        officerframeup->setPosition(ccpAdd(frame1->getPosition(), ccp(0, frame1->getContentSize().height*.5f- officerframeup->getContentSize().height*.5f)));
        CCScale9Sprite *btou1 = CCScale9Sprite::createWithSpriteFrameName("bantou.png");
        this->addChild(btou1,-1);
        btou1->setPreferredSize(CCSize(615, 21));
        btou1->setAnchorPoint(ccp(0.5, 1));
        btou1->setPosition(ccpAdd(frame1->getPosition(), ccp(15, -frame1->getContentSize().height*.5f)));
        if (i == 0) {
            label_1->setPosition(ccpAdd(frame1bg->getPosition(), ccp(0, -officerframeup->getContentSize().height/2 - label_1->getContentSize().height/2)));
        }else if (i == 1) {
            label_2->setPosition(ccpAdd(frame1bg->getPosition(), ccp(0, -officerframeup->getContentSize().height/2 - label_2->getContentSize().height/2)));
        }else if (i == 2) {
            label_3->setPosition(ccpAdd(frame1bg->getPosition(), ccp(0, -officerframeup->getContentSize().height/2 - label_3->getContentSize().height/2)));
        }

    }
    
    btn_wei = SGButton::create("store_exchangebtnbg.png", "font_wei.png", this, menu_selector(SGCountrysInfo::showInfo),CCPointZero,false,true);
    btn_wei->setTag(2);
    btn_shu = SGButton::create("store_exchangebtnbg.png", "font_shu.png", this, menu_selector(SGCountrysInfo::showInfo),CCPointZero,false,true);
    btn_shu->setTag(1);
    btn_wu = SGButton::create("store_exchangebtnbg.png", "font_wu.png", this, menu_selector(SGCountrysInfo::showInfo),CCPointZero,false,true);
    btn_wu->setTag(3);
    SGButton *btn_quny = SGButton::create("store_exchangebtnbg.png", "font_qunying.png", this, menu_selector(SGCountrysInfo::showInfo),CCPointZero,false,true);
    btn_quny->setTag(4);
    
    this->addBtn(btn_wei);
    this->addBtn(btn_shu);
    this->addBtn(btn_wu);
    this->addBtn(btn_quny);

    countrytitle = SGCCLabelTTF::create("", FONT_BOXINFO, 32 , COLOR_YELLOW);
    //modify by:zyc. merge into create.
    //countrytitle->setColor(COLOR_YELLOW);
//    int sender = SGPlayerInfo::sharePlayerInfo()->getPlayerCountry();
    int sender=-10;
    if(entertype==2)
        sender= _countrytype;
    else
        sender = SGPlayerInfo::sharePlayerInfo()->getPlayerCountry();

    CCDictionary* dict = SGStaticDataManager::shareStatic()->getNationListPlist();
    
    CCDictionary *data = (CCDictionary*)dict->objectForKey(CCString::createWithFormat("%d",sender)->getCString());
    
    CCString *tip = (CCString*)data->objectForKey("tip_text");
    CCString *tip1 = (CCString*)data->objectForKey("tip_text1");
    CCString *tip2 = (CCString*)data->objectForKey("tip_text2");
//    label_1 = SGCCLabelTTF::create(tip->getCString(), FONT_FZXK, 24);
//    label_2 =  SGCCLabelTTF::create(tip1->getCString(), FONT_FZXK, 24);
//    label_3 =  SGCCLabelTTF::create(tip2->getCString(), FONT_FZXK, 24);
    label_1->setString(tip->getCString());
    label_2->setString(tip1->getCString());
    label_3->setString(tip2->getCString());

    
    if(sender == 2)
    {
        btn_wei->setImage("barrack_anniu_hui.png");
        btn_wei->setEnabled(false);
        countrytitle->setString(str_CountrysInfo_str2);
      
    }
    else if(sender == 1)
    {
        btn_shu->setImage("barrack_anniu_hui.png");
        btn_shu->setEnabled(false);
        countrytitle->setString(str_CountrysInfo_str3);
        }
    else if(sender == 3)
    {
        btn_wu->setImage("barrack_anniu_hui.png");
        btn_wu->setEnabled(false);
        countrytitle->setString(str_CountrysInfo_str4);
      
    }

    //    countryId 蜀 1   魏 2   吴 3
        
    float wid = (s.width - btn_shu->getContentSize().width*4)/7;
    btn_wei->setPosition(ccpAdd(title_bg->getPosition(), ccp( - wid*1.5 - btn_wei->getContentSize().width*1.5,-120)));
    btn_shu->setPosition(ccpAdd(title_bg->getPosition(), ccp( - wid*.5 - btn_wei->getContentSize().width*.5,-120)));
    btn_wu->setPosition(ccpAdd(title_bg->getPosition(), ccp( wid*.5 + btn_wei->getContentSize().width*.5, -120)));
    btn_quny->setPosition(ccpAdd(title_bg->getPosition(), ccp( wid*1.5 + btn_wei->getContentSize().width*1.5,-120)));
    
    CCSprite *infobg = CCSprite::createWithSpriteFrameName("country_infobg.png");
    this->addChild(infobg);
    infobg->setPosition(ccpAdd(label_1->getPosition(), ccp(0,skewingY(88))));
    countrytitle->setPosition(infobg->getPosition());
    this->addChild(countrytitle,10);
}
void SGCountrysInfo::showInfo(CCNode * psender)
{
    EFFECT_PLAY(MUSIC_BTN);
    
    int sender = psender->getTag();

    
    CCDictionary* dict = SGStaticDataManager::shareStatic()->getNationListPlist();
    
    CCDictionary *data = (CCDictionary*)dict->objectForKey(CCString::createWithFormat("%d",sender)->getCString());
    if (data) {
        CCString *tip = (CCString*)data->objectForKey("tip_text");
        CCString *tip1 = (CCString*)data->objectForKey("tip_text1");
        CCString *tip2 = (CCString*)data->objectForKey("tip_text2");
        //    label_1 = SGCCLabelTTF::create(tip->getCString(), FONT_FZXK, 24);
        //    label_2 =  SGCCLabelTTF::create(tip1->getCString(), FONT_FZXK, 24);
        //    label_3 =  SGCCLabelTTF::create(tip2->getCString(), FONT_FZXK, 24);
        label_1->setString(tip->getCString());
        label_2->setString(tip1->getCString());
        label_3->setString(tip2->getCString());

    }
       if(sender == 2)
    {
        btn_wei->setImage("barrack_anniu_hui.png");
        btn_wei->setEnabled(false);
        btn_shu->setEnabled(true);
        btn_shu->setImage("store_exchangebtnbg.png");
        btn_wu->setEnabled(true);
        btn_wu->setImage("store_exchangebtnbg.png");
        countrytitle->setString(str_CountrysInfo_str2);
    }
    else if(sender == 1)
    {
        btn_shu->setImage("barrack_anniu_hui.png");
        btn_shu->setEnabled(false);
        btn_wei->setEnabled(true);
        btn_wei->setImage("store_exchangebtnbg.png");
        btn_wu->setEnabled(true);
        btn_wu->setImage("store_exchangebtnbg.png");
        countrytitle->setString(str_CountrysInfo_str3);
    }
    else if(sender == 3)
    {
        btn_wu->setImage("barrack_anniu_hui.png");
        btn_wu->setEnabled(false);
        btn_wei->setEnabled(true);
        btn_wei->setImage("store_exchangebtnbg.png");
        btn_shu->setEnabled(true);
        btn_shu->setImage("store_exchangebtnbg.png");
        countrytitle->setString(str_CountrysInfo_str4);
    }else if(sender == 4)
    {
        SGMainManager::shareMain()->showMessage(str_CountrysInfo_str5);
    }

}
void SGCountrysInfo::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    
    if(entertype==2)
    {
//        main::FriendInfoRequest *infoinfo = new main::FriendInfoRequest();
//        SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
//        
//        int typetype=player->getrankType();
//        int playerid=player->getplayerID();
//        if(typetype==0)
//        {
//            infoinfo->set_type(1);
//        }
//        else
//        {
//            infoinfo->set_type(0);
//        }
//        infoinfo->set_friendid(playerid);
//        
//        SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
//        sgnc->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGRankCell::receivedetail));
//        SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, infoinfo);
//        SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_countrysInfo, true);
//        SGBaseLayer *layer = (SGBaseLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_rank);
//        SGMainManager::shareMain()->showLayer(layer);
        
        
        //  SGMainManager::shareMain()->getMainScene()->removeChildByTag(sg_soldierInfoLayer, true);
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_rank);
        SGBaseLayer*layer2 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_myFriend);
        SGBaseLayer*layer3 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_friendRequest);
        SGBaseLayer*layer4 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_searchFriendFinish);
        SGBaseLayer*layer5 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_chatLayer);
        
        SGBaseLayer*layer6 = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_bossbattleranklayer);
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
        
        else
        {
            main::FriendInfoRequest *infoinfo = new main::FriendInfoRequest();
            SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
            int playerid=player->getplayerID();
            infoinfo->set_type(0);
            infoinfo->set_friendid(playerid);
            
            //        SGNotificationCenter *sgnc = SGNotificationCenter::sharedNotificationCenter();
            //        sgnc->addObserver(MSG_FRIEND_PLAYERINFO, this, callfuncO_selector(SGRankCell::receivedetail));
            SGSocketClient::sharedSocketClient()->send(MSG_FRIEND_PLAYERINFO, infoinfo);
        }

        
        
        
        
        
        
    
    }
    else
    {
    SGMainManager::shareMain()->showPlayerInfo();
    }
}
