//
//  SGEmbattleLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-1-4.
//
//

#include "SGEmbattleLayer.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGEmbattleBox.h"
#include "SGSoldierListBox.h"
#include "SGCardItem.h"
#include "ItemModule.pb.h"
#include "SGArrangeLayer.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "ArmyModule.pb.h"
#include "SGStaticDataManager.h"
#include "SGGuideManager.h"
#include "SGMainLayer.h"
#include "SGSoldierItem.h"
#include "ResourceManager.h"
#include "SGTeamgroup.h"
#include "SGAutoEquipBox.h"
#include "SGGeneralInfoLayer.h"
#include <algorithm>
#include "AppMacros.h"
#include "SGStringConfig.h"

#define   ATTRIBUTELABLETAG  110110
#define   SHINE_RING_EFFECT_TAG 1314
using namespace cocos2d;
//using namespace CocosDenshion;

/**
 * Used for sort
 */

CCRect SGEmbattleLayer::scrollviewRect = CCRectZero;

static int SGLess(const CCObject* p1, const CCObject* p2)
{
    return ((SGEmbattleItem*)p1)->index < ((SGEmbattleItem*)p2)->index;
}
SGEmbattleLayer::SGEmbattleLayer()
:cardArray(NULL),
isCanCrade(false),
selectSoldier(0),
canUsePosition(0),
m_speed(0),
m_moral(0)
,comBatle(0)
,m_gov(0)
,lorditem(NULL)
,m_noticeEquip(NULL),
colorId(0),
enterType(0),
totleGovern(0),
cards(NULL),
soldiers(NULL),
scrollView(NULL),
tradeIndex(0),
selectIndex(0),
sid(0),
titleBg(NULL),
officerframe(NULL),
em1(NULL),
em2(NULL),
em3(NULL),
pqsz(NULL),
zqzb(NULL),
cksb(NULL),
yqy(NULL),
m_noticeSoldier(NULL),
zongSqi(NULL),
zongSdu(NULL),
jineng(NULL),
tongyuli(NULL),
zhandouli(NULL),
lordskillinfo(NULL)
{
}
SGEmbattleLayer::~SGEmbattleLayer()
{
    scrollviewRect = CCRectZero;
    CC_SAFE_RELEASE(cardArray);
    CC_SAFE_RELEASE(cards);
    CC_SAFE_RELEASE(lorditem);
    CC_SAFE_RELEASE(soldiers);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_EMBATTLE_CHANG_OFFICER);
    notification->removeObserver(this, MSG_CHANGE_EMBATTLE);
    notification->removeObserver(this, MSG_EMBATTLE_SETLORD);
    notification->removeObserver(this, MSG_AUTO_ALLEQUIP);
    CCLOG("~SGEmbattleLayer");
 
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_embattleLayer);


}

SGEmbattleLayer *SGEmbattleLayer::create(int type,int em)
{
    SGEmbattleLayer *teamlayer = new SGEmbattleLayer();
    if (teamlayer  && teamlayer->init(NULL , sg_embattleLayer))
    {
        teamlayer->enterType = type;
        //        teamlayer->emtype = SGTeamgroup::shareTeamgroupInfo()->getIscurrent();
        teamlayer->emtype=em;
        teamlayer->initView();
        teamlayer->autorelease();
        return teamlayer;
    }
    CC_SAFE_DELETE(teamlayer);
    return NULL;
}


void SGEmbattleLayer::initMsg()
{
    cardArray = CCArray::create();
    cardArray->retain();
}
void SGEmbattleLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist", RES_TYPE_LAYER_UI, sg_embattleLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer.plist", RES_TYPE_LAYER_UI, sg_embattleLayer, LIM_PNG_AS_PNG);
    ResourceManager::sharedInstance()->bindTexture("sgembattlelayer/sgembattlelayer2.plist", RES_TYPE_LAYER_UI, sg_embattleLayer, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("soldier/soldier3101-1-3303-3.plist", RES_TYPE_LAYER_UI, sg_embattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist", RES_TYPE_LAYER_UI, sg_embattleLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/short.plist", RES_TYPE_LAYER_UI, sg_embattleLayer);
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_embattleLayer);

    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    this->setTouchEnabled(true);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EMBATTLE_CHANG_OFFICER,
                              this,
                              callfuncO_selector(SGEmbattleLayer::exchangPositionListener));
    notification->addObserver(MSG_CHANGE_EMBATTLE,
                              this,
                              callfuncO_selector(SGEmbattleLayer::exchangembattleListener));
    notification->addObserver(MSG_EMBATTLE_SETLORD,
                              this,
                              callfuncO_selector(SGEmbattleLayer::setLordOfficerListener));
    notification->addObserver(MSG_AUTO_ALLEQUIP,
                              this,
                              callfuncO_selector(SGEmbattleLayer::autoAllEquipListener));
    
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCSize s= CCDirector::sharedDirector()->getWinSize();
    
    CCSprite *gonggao_3_2 = CCSprite::createWithSpriteFrameName("gonggao_3_2.png");
    
    gonggao_3_2->setAnchorPoint(ccp(0.5, 1));
    gonggao_3_2->setScaleX(6);
    gonggao_3_2->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-47)));
    this->addChild(gonggao_3_2,2);
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 1));
    gonggao_bg_3_l->setPosition(ccpAdd(SGLayout::getPoint(kUpLeft),ccp(0,-47)));
    this->addChild(gonggao_bg_3_l,3);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 1));
    gonggao_bg_3_r->setPosition(ccpAdd(SGLayout::getPoint(kUpRight),ccp(0,-47)));
    this->addChild(gonggao_bg_3_r,3);
    
    CCSprite *title_bg = CCSprite::createWithSpriteFrameName("title_bg.png");
    title_bg->setAnchorPoint(ccp(0.5, 1));
    title_bg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0,  -12 -45 - gonggao_3_2->getContentSize().height)));  //cgpUI
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
    
    CCSprite *msgBg = CCSprite::createWithSpriteFrameName("playerinfo_bg_red.png");
    msgBg->setAnchorPoint(ccp(0.5, 1));
    msgBg->setPosition(ccpAdd(title_bg->getPosition(), ccp(0, -title_bg->getContentSize().height)));
    this->addChild(msgBg);
    
    CCSprite *rightcorner = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    rightcorner->setAnchorPoint(ccp(1, 0));
    rightcorner->setFlipX(true);
    rightcorner->setFlipY(true);
    rightcorner->setPosition(ccpAdd(msgBg->getPosition(), ccp(s.width*.5f, -msgBg->getContentSize().height)));
    this->addChild(rightcorner);
    
    CCSprite *leftcorner = CCSprite::createWithSpriteFrameName("gonggao_3_rl.png");
    leftcorner->setFlipY(true);
    leftcorner->setAnchorPoint(ccp(0, 0));
    leftcorner->setPosition(ccpAdd(msgBg->getPosition(), ccp(-s.width*.5f, -msgBg->getContentSize().height)));
    this->addChild(leftcorner);
//    CCSprite *btmbian = CCSprite::createWithSpriteFrameName("new_xian.png");
//    btmbian->setAnchorPoint(ccp(0.5, 0));
//    btmbian->setScaleX((s.width - leftcorner->getContentSize().width*2)/btmbian->getContentSize().width);
//    this->addChild(btmbian);
//    btmbian->setPosition(ccp(s.width/2, rightcorner->getPosition().y));
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGEmbattleLayer::goback),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.1);
    
    this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, title_bg->getPosition().y-title_bg->getContentSize().height*.5));
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_EmbattleLayer_str1, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    title->setPosition(ccp(s.width/2, title_bg->getPosition().y - title_bg->getContentSize().height*.52));
    this->addChild(title);
    
    SGCCLabelTTF *lab_speed = SGCCLabelTTF::create(str_EmbattleLayer_str2, FONT_BOXINFO, 30 , COLOR_YELLOW);
    this->addChild(lab_speed,2);
    lab_speed->setPosition(ccpAdd(msgBg->getPosition(), ccp(10-208, 10-msgBg->getContentSize().height*.5f-3)));
    
    SGCCLabelTTF *lab_mor = SGCCLabelTTF::create(str_EmbattleLayer_str3, FONT_BOXINFO, 30 , COLOR_YELLOW);
    this->addChild(lab_mor,2);
    lab_mor->setPosition(ccpAdd(lab_speed->getPosition(), ccp(0,40)));
    
    SGCCLabelTTF *lab_wjj = SGCCLabelTTF::create(str_zhujiangji, FONT_BOXINFO, 30 , COLOR_YELLOW);
    this->addChild(lab_wjj,2);
    lab_wjj->setPosition(ccpAdd(lab_speed->getPosition(), ccp(5, -40)));
    
    SGCCLabelTTF *lab_tyl = SGCCLabelTTF::create(str_tongyuli, FONT_BOXINFO, 30 , COLOR_YELLOW);
    lab_tyl->setPosition(ccp(s.width*.5 + 53 + 10, lab_mor->getPosition().y));
    this->addChild(lab_tyl,2);
    
    
    SGCCLabelTTF *lab_zdl = SGCCLabelTTF::create(str_EmbattleLayer_str4, FONT_BOXINFO, 30 , COLOR_YELLOW);
    lab_zdl->setPosition(ccp(s.width*.5 + 53 + 10, lab_speed->getPosition().y));
    this->addChild(lab_zdl,2);
    
//    CCScale9Sprite *font_bg_sq = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("embattle_infobg.png");//总气势等条背景
//    font_bg_sq->setPreferredSize(CCSize(220, 28));
//    font_bg_sq->setPosition(ccpAdd(lab_mor->getPosition(), ccp(-lab_mor->getContentSize().width*.5f-10, 0)));
//    font_bg_sq->setAnchorPoint(ccp(0, 0.5));
//    this->addChild(font_bg_sq);
    
//    CCScale9Sprite *font_bg_spd = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("embattle_infobg.png");
//    font_bg_spd->setPreferredSize(CCSize(220, 28));
//    font_bg_spd->setPosition(ccpAdd(lab_speed->getPosition(), ccp(-lab_speed->getContentSize().width*.5f-10, 0)));
//    font_bg_spd->setAnchorPoint(ccp(0, 0.5));
//    this->addChild(font_bg_spd);
    
//    CCScale9Sprite *font_bg_wjj = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("embattle_infobg.png");
//    font_bg_wjj->setPreferredSize(CCSize(545, 28));
//    font_bg_wjj->setPosition(ccpAdd(lab_wjj->getPosition(), ccp(-lab_wjj->getContentSize().width*.5f-10, 0)));
//    font_bg_wjj->setAnchorPoint(ccp(0, 0.5));
//    this->addChild(font_bg_wjj);
    
    
//    CCScale9Sprite *font_bg_zdl = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("embattle_infobg.png");
//    font_bg_zdl->setPreferredSize(CCSize(284, 28));
//    font_bg_zdl->setPosition(ccpAdd(lab_zdl->getPosition(), ccp(-lab_zdl->getContentSize().width*.5f-10, 0)));
//    font_bg_zdl->setAnchorPoint(ccp(0, 0.5));
//    this->addChild(font_bg_zdl);
    
//    CCScale9Sprite *font_bg_gov = cocos2d::extension::CCScale9Sprite::createWithSpriteFrameName("embattle_infobg.png");
//    font_bg_gov->setPreferredSize(CCSize(284, 28));
//    font_bg_gov->setPosition(ccpAdd(lab_tyl->getPosition(), ccp(-lab_tyl->getContentSize().width*.5f-10, 0)));
//    font_bg_gov->setAnchorPoint(ccp(0, 0.5));
//    this->addChild(font_bg_gov);
    
 
    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect r = CCRectMake(0, b,bg->getContentSize().width, s.height - b - msgBg->getContentSize().height - title_bg->getContentSize().height);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
 
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(0, b- 47));
    
    bg->setVisible(false);  //modified by cgp
    
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
    //总血量
    zongSqi = SGCCLabelTTF::create("0", FONT_BOXINFO, 26);
    zongSqi->setAnchorPoint(ccp(0, 0.5));
    this->addChild(zongSqi);
    zongSqi->setPosition(ccpAdd(lab_mor->getPosition(), ccp(60, 0)));
    
    //总速度
    zongSdu = SGCCLabelTTF::create("3000", FONT_BOXINFO, 26);
    zongSdu->setAnchorPoint(ccp(0, 0.5));
    this->addChild(zongSdu);
    zongSdu->setPosition(ccpAdd(lab_speed->getPosition(), ccp(60, 0)));
    
    //统御力
    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(playerInfo->getPlayerLevel());
    
    tongyuli = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    tongyuli->setAnchorPoint(ccp(1, 0.5));
    this->addChild(tongyuli);
    tongyuli->setPosition(ccpAdd(lab_tyl->getPosition(), ccp(120, 0)));
    
    SGCCLabelTTF *totlegov = SGCCLabelTTF::create(CCString::createWithFormat("/%d",player->getPlayerGoverning() + SGPlayerInfo::sharePlayerInfo()->getAddPlayerGovering())->getCString(), FONT_BOXINFO, 26);
    this->addChild(totlegov);
    totlegov->setAnchorPoint(ccp(0, 0.5));
    totlegov->setPosition(tongyuli->getPosition());
    
    //战斗力
    zhandouli = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    zhandouli->setAnchorPoint(ccp(1, 0.5));
    this->addChild(zhandouli);
    zhandouli->setPosition(ccpAdd(lab_zdl->getPosition(), ccp(150, 0)));
    
    //主将计
    jineng = SGCCLabelTTF::create("", FONT_BOXINFO, 26);
    jineng->setAnchorPoint(ccp(0, 0.5));
    this->addChild(jineng);
    jineng->setPosition(ccpAdd(lab_wjj->getPosition(), ccp(60, 0)));
    
    
    CCSprite *kuang = CCSprite::createWithSpriteFrameName("db.png");//中间的方框
    kuang->setPosition(ccpAdd(center,ccp(0,-110)));
    this->addChild(kuang);
    
//    officerframe = CCSprite::createWithSpriteFrameName("embattle_newbg.png");//武将的方框
//    officerframe->setPosition(ccpAdd(center,ccp(0,-200)));
//    this->addChild(officerframe);
    
    CCSprite *nowload = CCSprite::createWithSpriteFrameName("embattle_nowlord.png");
    nowload->setPosition(ccpAdd(center,ccp(-245,70)));
    this->addChild(nowload);
    
    SGCCLabelTTF* infomation = SGCCLabelTTF::create(str_EmbattleLayer_str5, FONT_BOXINFO, 24 , ccc3(0xff, 0xc7, 0x3));
    infomation->setPosition(ccpAdd(center,ccp(0,-60)));
    this->addChild(infomation);
    
    pqsz = SGButton::createFromLocal("qy.png", "", this, menu_selector(SGEmbattleLayer::embattle1),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    pqsz->setPosition(ccpAdd(center,ccp(180,35)));

    this->addChild(pqsz);
    pqsz->setVisible(false);
    
    em1 = SGButton::createFromLocal("bz.png", str_EmbattleLayer_str6, this, menu_selector(SGEmbattleLayer::embattle1),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    em1->setPosition(ccpAdd(center,ccp(-200,170 + 15)));
    this->addChild(em1);
    
    
    em2 = SGButton::createFromLocal("bz.png", str_EmbattleLayer_str7, this, menu_selector(SGEmbattleLayer::embattle2),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    em2->setPosition(ccpAdd(center,ccp(0,170 + 15)));
    this->addChild(em2);
    
    em3 = SGButton::createFromLocal("bz.png", str_EmbattleLayer_str8, this, menu_selector(SGEmbattleLayer::embattle3),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    em3->setPosition(ccpAdd(center,ccp(200,170 + 15)));
    this->addChild(em3);
    
    switch (emtype)
    {
        case 1:
        {
            em2->setFontColor(ccGRAY);
            em2->setColor(ccGRAY);
            em3->setFontColor(ccGRAY);
            em3->setColor(ccGRAY);

        }
            break;
        case 2:
        {
            em1->setFontColor(ccGRAY);
            em1->setColor(ccGRAY);
            em3->setFontColor(ccGRAY);
            em3->setColor(ccGRAY);
        }
            break;
        case 3:
        {
            em1->setFontColor(ccGRAY);
            em1->setColor(ccGRAY);
            em2->setFontColor(ccGRAY);
            em2->setColor(ccGRAY);
        }
            break;
        default:
            break;
    }
    
    
    
    zqzb = SGButton::createFromLocal("upwar_button_bbg.png", str_EmbattleLayer_str9, this, menu_selector(SGEmbattleLayer::equiphandler),CCPointZero,FONT_PANGWA,ccWHITE,28,false,true);
    zqzb->setPosition(ccpAdd(center,ccp(180,20)));
	this->addBtn(zqzb);//为显示阴影效果
    
    //叹号
    m_noticeEquip = CCSprite::createWithSpriteFrameName("publc_notice.png");
    m_noticeEquip->setPosition(ccpAdd(zqzb->getPosition(), ccp(zqzb->getContentSize().width*.37, zqzb->getContentSize().height*.38)));
    CCLog("\n\n\n\nbest equip: %s", SGPlayerInfo::sharePlayerInfo()->getBestEquip()?"true" : "false" );
    updateBestEquipBt(playerInfo->getBestEquip());
    this->addChild(m_noticeEquip,133);
    
    cksb = SGButton::createFromLocal("upwar_button_bbg.png", str_EmbattleLayer_str10, this, menu_selector(SGEmbattleLayer::soldiercheck),CCPointZero,FONT_PANGWA,ccWHITE,28,false,true);
    cksb->setPosition(ccpAdd(center,ccp(180,90)));
	this->addBtn(cksb);//为显示阴影效果
    
    
    m_noticeSoldier = CCSprite::createWithSpriteFrameName("publc_notice.png");
    m_noticeSoldier->setPosition(ccpAdd(ccp(0,0), ccp(cksb->getContentSize().width, cksb->getContentSize().height)));
    updateNoticeSoldierBt(false);
    cksb->addChild(m_noticeSoldier,144);

    //當前陣容已啟用
    yqy = SGCCLabelTTF::create(str_EmbattleLayer_str11, FONT_PANGWA, 25 , ccc3(0x0c,0x4b,0xff) , ccWHITE);
    yqy->setPosition(ccpAdd(center,ccp(0,110)));
    this->addChild(yqy);
    
    if (SGTeamgroup::shareTeamgroupInfo()->getiscurrent()!=emtype) {
        pqsz->setVisible(true);
        yqy->setVisible(false);
        zqzb->setVisible(false);
        cksb->setVisible(false);
        
    }
    else
    {
        pqsz->setVisible(false);
        yqy->setVisible(true);
        zqzb->setVisible(true);
        cksb->setVisible(true);
    }
    
     //三个选兵的按钮
    soldiers = CCArray::create();
    soldiers->retain();
    
    //主將計
    SGCCLabelTTF *lordskillzjj = SGCCLabelTTF::create(str_zhujiangji,FONT_PANGWA,36 ,ccc3(0xff, 0xc7, 0x03));
    lordskillzjj->setPosition(ccpAdd(center,ccp(-20,55)));
    this->addChild(lordskillzjj);
    
    //主將計名稱
    lordskillinfo = SGCCLabelTTF::create(str_EmbattleLayer_str12,FONT_PANGWA,26 ,ccc3(0x3c, 0xff, 0x00));
    lordskillinfo->setAnchorPoint(ccp(0,0.5));
    lordskillinfo->setPosition(ccpAdd(center,ccp(-82,8)));
    this->addChild(lordskillinfo);
    
   
    int modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_36);
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId > guide_tag_41)
        modelTag = SGStaticDataManager::shareStatic()->getGuideTagById(guide_tag_40);
    scrollView = SNSScrollView::create(CCRectMake(0, 0, kuang->getContentSize().width, kuang->getContentSize().height));
    
    scrollView->setPosition(ccpAdd(kuang->getPosition(), ccp(-kuang->getContentSize().width*.5,-kuang->getContentSize().height*0.5f)));
    
    
    this->addChild(scrollView,100);
    scrollView->setHorizontal(false);
    scrollView->setVertical(true);
    scrollView->setTag(modelTag);
    
    
    this->updateView();
    this->updateEmbattle();
}

void SGEmbattleLayer::equiphandler(cocos2d::CCObject *obj)
{
    //removed by cgp
//    SGOfficerCard *temp = NULL;
//    SGAutoEquipBox *autoequipbox = SGAutoEquipBox::create(this, temp,1);
//    SGMainManager::shareMain()->showBox(autoequipbox);
    
    CCLog("@@@@equip");
}

void SGEmbattleLayer::soldiercheck(cocos2d::CCObject *obj)
{
    //removed by cgp
    //SGMainManager::shareMain()->showSoldierslayer(true,3);//布阵进入
}
void SGEmbattleLayer::autoAllEquipListener(cocos2d::CCObject *obj)
{
    updateView();
}
void SGEmbattleLayer::exchangembattleListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::ChangeCurrentTeamResponse *response = (main::ChangeCurrentTeamResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            for (int i = 0;i<TEAMID;i++) {
                
                if (SGTeamgroup::shareTeamgroupInfo()->getteamindex(emtype)!=i) {
                    SGTeamgroup::shareTeamgroupInfo()->setiscurrent(i,0);
                }
                else
                {
                    SGTeamgroup::shareTeamgroupInfo()->setiscurrent(i,1);
                }
            }
            
            
            CCLog("@@@@@@@%d",SGTeamgroup::shareTeamgroupInfo()->getiscurrent());
            
            pqsz->setVisible(false);
            yqy->setVisible(true);
            zqzb->setVisible(true);
            cksb->setVisible(true);
        }
        else
		{
			SG_SHOW_WINDOW(str_EmbattleLayer_str13);
		}
        
    }
}

void SGEmbattleLayer::setLordOfficerListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    bool setState = false;
    if (sr)
    {
        main::SetLordResponse *response = (main::SetLordResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            SGOfficerCard  *pcard = NULL;
            CCObject *officercard = NULL;
            CCARRAY_FOREACH(cards, officercard)
            {
               pcard = (SGOfficerCard *)officercard;
               int index = SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,pcard);
                if (index==selectIndex) {
                    break;
                }
            
            }
                       CCDictionary *dict = CCDictionary::create();
            dict->setObject(CCString::createWithFormat("%d",pcard->getSsid()), "lordid");
            dict->setObject(CCString::create("lord"), "type");
            SGMainManager::shareMain()->showMessage(CCString::createWithFormat(str_EmbattleLayer_str14,pcard->getOfficerName()->getCString())->getCString());
            showBoxCall(dict);
     
            CCLOG("设为主将成功");
            setState = true;
        }
        else
        {
            CCLOG("设为主将失败");
        }
    }
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
    updateView();
    updateEmbattle();
    if (setState ) {
        SGGuideManager::shareManager()->checkIsDone(guide_tag_42);//跳过guide_tag_41
    }
    isCanCrade = false;
}

void SGEmbattleLayer::dispatch()
{
    main::ChangeCurrentTeamRequest *request = new main::ChangeCurrentTeamRequest();
    request->set_teamid(emtype-1);
    SGSocketClient::sharedSocketClient()->send(MSG_CHANGE_EMBATTLE, request);
    
}

void SGEmbattleLayer::embattle1()
{
    em1->setFontColor(ccWHITE);
    em1->setColor(ccWHITE);
    em2->setFontColor(ccGRAY);
    em2->setColor(ccGRAY);
    em3->setFontColor(ccGRAY);
    em3->setColor(ccGRAY);
    emtype=1;
    if (SGTeamgroup::shareTeamgroupInfo()->getiscurrent()!=1) {
        pqsz->setVisible(true);
        yqy->setVisible(false);
        zqzb->setVisible(false);
        cksb->setVisible(false);
        m_noticeEquip->setVisible(false);
    }
    else
    {
        pqsz->setVisible(false);
        yqy->setVisible(true);
        zqzb->setVisible(true);
        cksb->setVisible(true);
        if(!SGPlayerInfo::sharePlayerInfo()->getBestEquip())
            m_noticeEquip->setVisible(SGPlayerInfo::sharePlayerInfo()->getBestEquip());
    }
    this->updateView();
    //this->updateEmbattle();
}

void SGEmbattleLayer::embattle2()
{
    em1->setFontColor(ccGRAY);
    em1->setColor(ccGRAY);
    em3->setFontColor(ccGRAY);
    em3->setColor(ccGRAY);
    em2->setFontColor(ccWHITE);
    em2->setColor(ccWHITE);
    emtype=2;
    if (SGTeamgroup::shareTeamgroupInfo()->getiscurrent()!=2) {
        pqsz->setVisible(true);
        yqy->setVisible(false);
        zqzb->setVisible(false);
        cksb->setVisible(false);
        m_noticeEquip->setVisible(false);
    }
    else
    {
        pqsz->setVisible(false);
        yqy->setVisible(true);
        zqzb->setVisible(true);
        cksb->setVisible(true);
        if(!SGPlayerInfo::sharePlayerInfo()->getBestEquip())
            m_noticeEquip->setVisible(SGPlayerInfo::sharePlayerInfo()->getBestEquip());
    }
    
    this->updateView();
    //this->updateEmbattle();
}

void SGEmbattleLayer::embattle3()
{
    em1->setFontColor(ccGRAY);
    em1->setColor(ccGRAY);
    em2->setFontColor(ccGRAY);
    em2->setColor(ccGRAY);
    em3->setFontColor(ccWHITE);
    em3->setColor(ccWHITE);
    emtype=3;
    if (SGTeamgroup::shareTeamgroupInfo()->getiscurrent()!=3) {
        pqsz->setVisible(true);
        yqy->setVisible(false);
        zqzb->setVisible(false);
        cksb->setVisible(false);
        m_noticeEquip->setVisible(false);
    }
    else
    {
        pqsz->setVisible(false);
        yqy->setVisible(true);
        zqzb->setVisible(true);
        cksb->setVisible(true);
        if(!SGPlayerInfo::sharePlayerInfo()->getBestEquip())
            m_noticeEquip->setVisible(SGPlayerInfo::sharePlayerInfo()->getBestEquip());
    }
    
    this->updateView();
    //this->updateEmbattle();
}
void SGEmbattleLayer::initUsePosition()
{
    SGPlayerDataModel *player=SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    canUsePosition=player->getPlayerOfficerMax()-cards->count();
}

//新开启的武将空位,要加入光效
void SGEmbattleLayer::addBlinkEffect(CCNode *node)
{
	int shiftLeft = 0, shiftDown = 0;
	if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
        ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
	{
		shiftLeft = 60;
		shiftDown = -60;
	}
	else if (CCEGLView::sharedOpenGLView()->getFrameSize().height == iphonehdResource.size.height)
	{
		shiftLeft = -5;
		shiftDown = -90;
	}
	
	
    SGEmbattleItem *emItem = (SGEmbattleItem*)node;
//    CCSpriterX *newRewardEffect = CCSpriterX::create("animationFile/short.scml", true, true);
//    newRewardEffect->setanimaID(0);
//    newRewardEffect->setisloop(true);
//    newRewardEffect->setScale(0.85);
//    newRewardEffect->setPosition(ccpAdd(n->getPosition(), ccp(30 + shiftLeft, 210 + shiftDown)));
    //newRewardEffect->setPosition(ccp(0 , 0));
    CCSprite * redPoint = CCSprite::createWithSpriteFrameName("publc_notice.png");
    redPoint->setPosition(ccp(111/2-redPoint->getContentSize().width/2+20,111/2-redPoint->getContentSize().height/2+20));
    emItem->addChild(redPoint , 112 , SHINE_RING_EFFECT_TAG);
    //newRewardEffect->play();
}


void SGEmbattleLayer::updateView()
{
    cardArray->removeAllObjects();
    scrollView->m_body->removeAllChildrenWithCleanup(true);
    scrollView->setBodySize(scrollView->getContentSize());
    cards = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
    float wid = 592 - 115*4  ;
    if (cards) {
        cards->retain();
    }
    CCArray * allCards = NULL;
    allCards = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    int curr = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();//启用的当前阵容id
    bool isIdleCards = false;//是否有空闲武将
    if(allCards)
    {
        
        CCObject *obj = NULL;
        CCARRAY_FOREACH(allCards, obj)
        {
            SGOfficerCard  *pcard = (SGOfficerCard *)obj;
            int po = SGTeamgroup::shareTeamgroupInfo()->getPositionId(curr,pcard);
            if(po ==0)
            {
                isIdleCards = true;
                break;
            }
        }
    }
    
    
    initUsePosition();
    
    for (int i = 0; i<10; i++)
    {
        SGOfficerCard *card = NULL;
        SGEmbattleItem *item = NULL;
        
        CCObject *obj = NULL;
        CCARRAY_FOREACH(cards, obj)
        {
            SGOfficerCard  *pcard = (SGOfficerCard *)obj;
            int x=SGTeamgroup::shareTeamgroupInfo()->getPositionId(emtype,pcard);
            if (x-1 == i)
            {
                card = pcard;
                break;
            }
        }
        if (card)
        {
            item = SGEmbattleItem::create(this, card,emtype);
        }
        else
        {
            item = SGEmbattleItem::create(this,(i+1),emtype);
        }
        item->removeChildByTag(SHINE_RING_EFFECT_TAG);
        cardArray->addObject(item);
        scrollView->m_body->addChild(item);
        
        item->setPosition(ccp(wid/2 + (i%5) *115 + 15, -(i/5)*140 + 230 + 13));
		
		//设置位置之后再次设置光效 i<canUsePosition+cards->count()是保证只有启用的空位置才有光效 modify by:zyc  只要有空位一直显示
//		if(!card && SGMainManager::shareMain()->getEmbattleKey() && i < canUsePosition + cards->count())
        if( !card && i < (canUsePosition + cards->count()) )
		{
			//这里是没有武将的空位
            if(isIdleCards)
            {
                this->addBlinkEffect(item);
            }
		}
        
    }
	//进入布阵了,不再显示提醒
	if (SGMainManager::shareMain()->getEmbattleKey())
	{
		SGMainManager::shareMain()->setEmbattleKey(false);
		CCUserDefault::sharedUserDefault()->flush();
	}
    
    scrollView->setMoveEnable(false);
    scrollView->setTouchEnabled(false);
    
    if (lorditem!=NULL) {
        lorditem->removeFromParent();
        lorditem=NULL;
    }
    
    SGOfficerCard *card = SGTeamgroup::shareTeamgroupInfo()->getlordofficer(emtype);
    //CCLOG("%s",card->getOfficerName()->getCString());
    lorditem = SGEmbattleItem::create(this,card,emtype,1);
    lorditem->retain();
    lorditem->setPosition(ccpAdd(ccp(0,0),ccp(wid*1 + 11,435)));
    scrollView->m_body->addChild(lorditem);
    
    SGOfficerCard *lordcard = SGTeamgroup::shareTeamgroupInfo()->getlordofficer(emtype);
    SGSkillDataModel* lordskill =  SGStaticDataManager::shareStatic()->getLordSkillById(lordcard->getLordId());
    if (lordskill) {
        lordskillinfo->setString(CCString::createWithFormat("%s Lv%d",lordskill->getSkillName()->getCString(),lordskill->getLevel())->getCString());
    }
    else
    {
        lordskillinfo->setString(str_EmbattleLayer_str15);
    }
    
}

SGEmbattleItem* SGEmbattleLayer::getItemByIndex(int index)
{
    if (index < 0 || index >= cardArray->count()) return NULL;
    return (SGEmbattleItem *)cardArray->objectAtIndex(index);
}

void SGEmbattleLayer::updateBestEquipBt(bool isVisible)
{
//    if(STATE_YES == SGPlayerInfo::sharePlayerInfo()->getPlayerNoticeById(505 , false))
//        m_noticeEquip->setVisible(true);
//    else
//        m_noticeEquip->setVisible(false);
    if (m_noticeEquip)
    {
        m_noticeEquip->setVisible(isVisible);
    }
    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId") < UIG_MAX1)
    {
         m_noticeEquip->setVisible(false);
    }
}
void SGEmbattleLayer::updateNoticeSoldierBt(bool isVisible)
{
//    int soulNum = 0;
//    CCArray *soulCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
//    CCObject *obj_ = NULL;
//    CCARRAY_FOREACH(soulCards, obj_)
//    {
//        
//        SGPropsCard *card = (SGPropsCard *)obj_;
//        if (card && card->getType() == 3 )
//        {
//            soulNum++;
//        }
//    }
//    
//    int teamid = SGTeamgroup::shareTeamgroupInfo()->getiscurrent();
//    int soldierId = 0;
//    CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
//    CCObject *obj1 = NULL;
//    int playerLevel = SGPlayerInfo::sharePlayerInfo()->getPlayerLevel();
//    bool soldierLevelFlag = false;
//    
//    for (int i = 0; i<3; i++)
//    {
//        switch (i)
//        {
//            case 0:
//            {
//                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldierblue(teamid);
//            }
//                break;
//            case 1:
//            {
//                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldierred(teamid);
//            }
//                break;
//            case 2:
//            {
//                soldierId = SGTeamgroup::shareTeamgroupInfo()->getsoldiergreen(teamid);
//            }
//                
//                break;
//            default:
//            {
//                soldierId = 0;
//            }
//                break;
//        }
//        
//        CCARRAY_FOREACH(allSoldiers, obj1)
//        {
//            SGSoldierCard *card = (SGSoldierCard *)obj1;
//            if (card->getItemId() == soldierId)
//            {
//                if (card->getCurrLevel() < playerLevel)
//                {
//                    soldierLevelFlag = true;
//                    break;
//                }
//            }
//        }
//        
//        if (soldierLevelFlag)
//        {
//            break;
//        }
//        
//    }
    
    if(SGPlayerInfo::sharePlayerInfo()->canSoldierStreng())
        isVisible = true;
    else
        isVisible=false;
//    if(STATE_YES == SGPlayerInfo::sharePlayerInfo()->getPlayerNoticeById(503 , false))
//        m_noticeSoldier->setVisible(true);
//    else
//        m_noticeSoldier->setVisible(false)
    if (m_noticeSoldier)
    {
        m_noticeSoldier->setVisible(isVisible);
    }
    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId") < UIG_MAX1)
    {
        m_noticeSoldier->setVisible(isVisible);
    }
}

void SGEmbattleLayer::updateEmbattle(int type)
{
    CCArray *array = SGTeamgroup::shareTeamgroupInfo()->getEmbattleOfficers(emtype);
    CCObject *obj = NULL;
    int mor = 0;
    int speed = 0;
    totleGovern = 0;
    CCARRAY_FOREACH(array, obj)
    {
        SGOfficerCard *card = (SGOfficerCard *)obj;
        mor += card->getMorale();
        speed += card->getSpeed();
        totleGovern += card->getGoverning();
		
        if (card->getOfficerEquips())
        {
            for (int i = 0; i <card->getOfficerEquips()->count(); i++)
            {
                SGEquipCard *equip = (SGEquipCard *)card->getOfficerEquips()->objectAtIndex(i);
//                mor += equip->getMorale();
//                speed += equip->getSpeed();
                if ((card->getGoverning()+equip->getGoverning())<0)
                    totleGovern -= card->getGoverning();
                else
                    totleGovern += equip->getGoverning();
            }
        }
        if (SGTeamgroup::shareTeamgroupInfo()->islord(emtype,card))
        {
			//使用LordId读取升级的主将计对应的是g_times ^_-，而不是原来的lordSkill
            if (card->getLordId() /*&&!officer->getOfficerLordSkill()->isEqual(CCString::create(""))*/)
            {
                SGSkillDataModel *lordSkill = SGStaticDataManager::shareStatic()->getLordSkillById(card->getLordId());
                if (lordSkill)
                    jineng->setString(CCString::createWithFormat(str_EmbattleLayer_str22,lordSkill->getSkillName()->getCString(),lordSkill->getLevel(),lordSkill->getRound())->getCString());
                else
                    jineng->setString(str_EmbattleLayer_str17);
            }
            else
                jineng->setString(str_EmbattleLayer_str17);
        }
    }
    CCARRAY_FOREACH(SGPlayerInfo::sharePlayerInfo()->getSoldierCards(), obj)
    {
        SGSoldierCard *card = (SGSoldierCard *)obj;
        if (SGTeamgroup::shareTeamgroupInfo()->getsoldierred(emtype) == card->getItemId()) {
            mor += card->getMorale();
        }
        if (SGTeamgroup::shareTeamgroupInfo()->getsoldiergreen(emtype) == card->getItemId()) {
            mor += card->getMorale();
        }
        if (SGTeamgroup::shareTeamgroupInfo()->getsoldierblue(emtype) == card->getItemId()) {
            mor += card->getMorale();
        }
        
    }
    SGPlayerDataModel *player = SGStaticDataManager::shareStatic()->getPlayerByLevel(SGPlayerInfo::sharePlayerInfo()->getPlayerLevel());
    
    
   // CCLog("%d",atoi(zongSdu->getString()));
   // CCLog("%d", SGPlayerInfo::sharePlayerInfo()->getcombatValue());
    if (type==1) {
        showAddAttributeEffect(mor-atoi(zongSqi->getString()), speed-atoi(zongSdu->getString()), SGPlayerInfo::sharePlayerInfo()->getcombatValue()-atoi(zhandouli->getString()),totleGovern-atoi(tongyuli->getString()));
    }
    
    
    setcombattle(SGPlayerInfo::sharePlayerInfo()->getcombatValue());
    setspeedvalue(speed);
    setmorValue(mor);
    setgovValue(totleGovern);
    
     //CCLog("%d", SGPlayerInfo::sharePlayerInfo()->getcombatValue());
    zongSqi->setString(CCString::createWithFormat("%d",mor)->getCString());
    zongSdu->setString(CCString::createWithFormat("%d",speed)->getCString());
    tongyuli->setString(CCString::createWithFormat("%d",totleGovern)->getCString());
    zhandouli->setString(CCString::createWithFormat("%d",SGPlayerInfo::sharePlayerInfo()->getcombatValue())->getCString());
     //CCLog("%d", SGPlayerInfo::sharePlayerInfo()->getcombatValue());
    if (totleGovern > player->getPlayerGoverning() + SGPlayerInfo::sharePlayerInfo()->getAddPlayerGovering())
        tongyuli->setInsideColor(ccRED);
    else
        tongyuli->setInsideColor(ccWHITE);
}
void SGEmbattleLayer::setIsSelect(bool isSelect)
{
}

void SGEmbattleLayer:: showEmbattleGenerinfo(SGOfficerCard *_card,int islord){
    //获得当前阵营武将列表大全。
    SGTeamgroup* sgtg = SGTeamgroup::shareTeamgroupInfo();
    CCArray* ofcList = sgtg->getEmbattleOfficers(sgtg->getiscurrent());
    SGGeneralInfoLayer::setMyOfficerList(ofcList);
    //打开武将详情
    int speed=this->getspeedvalue();
    int combattleValue=SGPlayerInfo::sharePlayerInfo()->getcombatValue();
    int mor=this->getmorValue();
    int bufgov=this->getgovValue();
    if (islord)
       SGMainManager::shareMain()->showGeneralInfoLayer(_card, 500);
    else
        SGMainManager::shareMain()->showGeneralInfoLayer(_card, 22,NULL,true,NULL,12);
    SGMainScene *mainSence = SGMainManager::shareMain()->getMainScene();
    SGGeneralInfoLayer *generlMsgLayer = (SGGeneralInfoLayer *)mainSence->getChildByTag(sg_generalInfoLayer);
    generlMsgLayer->setGovAndType(totleGovern, emtype, 0,canUsePosition);
    generlMsgLayer->speedValue=speed ;
    generlMsgLayer->combatle=combattleValue;
    generlMsgLayer->morValue=mor;
    generlMsgLayer->GovValue=bufgov;
    
}

void SGEmbattleLayer::selectedIndex(SGEmbattleItem *item)
{
//    SGEmbattleItem *item = (SGEmbattleItem *)cardArray->objectAtIndex(index-1);
	//CCLOG("%d",item->_islord);
    SGOfficerCard *card = item->getOfficerCard();
    if (card)
    {
//        this->showEmbattleBox(item->getOfficerCard());
        SGGeneralInfoLayer::setEmType(emtype);
        this->showEmbattleGenerinfo(item->getOfficerCard(),item->_islord);
    }
    else
    {
        int speed=this->getspeedvalue();
        int morValue=this->getmorValue();
        int gov=this->getgovValue();
        SGArrangeLayer *layer = SGArrangeLayer::create(item->index,totleGovern,enterType,emtype,canUsePosition);
        SGMainManager::shareMain()->showLayer(layer);
        layer->setComBattle(SGPlayerInfo::sharePlayerInfo()->getcombatValue());
        layer->setSpeedValue( speed);
        layer->setMorValue(morValue);
        layer->setGovValue(gov);
    }
    int guideId = CCUserDefault::sharedUserDefault()->getIntegerForKey("guideId");
    if (guideId > guide_tag_37)
        SGGuideManager::shareManager()->checkIsDone(guide_tag_41);
    else if (guideId > guide_tag_0)
        SGGuideManager::shareManager()->checkIsDone(guide_tag_37);
    
}
void SGEmbattleLayer::moveToPoint(cocos2d::CCPoint point, int index)
{
   
    if (lorditem->testIsCanMove(point))
    {
        isCanCrade = true;
        lorditem->startRock();
        tradeIndex = 100;//换主将时的区分
        selectIndex = index;
        return;
    }
    else
    {
        CCObject *obj = NULL;
        CCARRAY_FOREACH(cardArray, obj)
        {
            SGEmbattleItem *item = (SGEmbattleItem *)obj;
            if (item->index != index)
            {
                if (item->testIsCanMove(point))
                {
                    isCanCrade = true;
                    tradeIndex = item->index;
                    selectIndex = index;
                    item->startRock();
                    break;
                }
                else
                {
                    item->stopRock();
                    isCanCrade = false;
                }
            }
        }

    }
    
    
}

void SGEmbattleLayer::moveToLordItemPoint(CCPoint point, int index)
{
    if (lorditem->testIsCanMove(point))
    {
        isCanCrade = true;
        lorditem->startRock();
        tradeIndex = 100;//换主将时的区分
        selectIndex = index;
    }
}
bool SGEmbattleLayer::trade()
{
    if (isCanCrade)
    { //从1开始
        if (tradeIndex ==100)//换主将
        {
            SGEmbattleItem *item = NULL;
            for (int i=0;i<cardArray->count();i++)
            {
                SGEmbattleItem *item1 = (SGEmbattleItem*)cardArray->objectAtIndex(i);
                if (item1->index == selectIndex)
                {
                    item = item1;
                }
            }
            //发消息,收消息后把头像换一下
            if (item)
            {
               SGOfficerCard *card = item->officerCard;
                main::SetLordRequest *request = new main::SetLordRequest();
                request->set_ssid(card->getSsid());
                request->set_teamid(emtype-1);
                SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_SETLORD, request);
            }
            
        }
       else if (tradeIndex != selectIndex)
       {
            main::ChangePositionRequest *request = new main::ChangePositionRequest();
            request->set_from(selectIndex);
            request->set_to(tradeIndex);
            request->set_teamid(emtype-1);
            SGSocketClient::sharedSocketClient()->send(MSG_EMBATTLE_CHANG_OFFICER, request);
        }
    }
    return isCanCrade;
}
void SGEmbattleLayer::exchangPositionListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::ChangePositionResponse *response = (main::ChangePositionResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            
            isCanCrade = false;
            SGEmbattleItem *firstItem = ((SGEmbattleItem *)cardArray->objectAtIndex(tradeIndex - 1));
            CCPoint tradeP = firstItem->getPosition();
            int tradeIndex = firstItem->index;
            
            if (tradeIndex <selectIndex)
            {
                
                int k = selectIndex - tradeIndex;
                for (int i = tradeIndex; i< selectIndex; i++)
                {
                    k--;
                    SGEmbattleItem *beforeItem = (SGEmbattleItem *)cardArray->objectAtIndex(i-1);
                    
                    beforeItem->stopRock();
                    SGEmbattleItem* afterItem = (SGEmbattleItem *)cardArray->objectAtIndex(i);
                    beforeItem->index = afterItem->index;
                    CCDelayTime *delay = CCDelayTime::create(k*.1+.1);
                    CCMoveTo *move = CCMoveTo::create(.1, afterItem->getPosition());
                    beforeItem->runAction(CCSequence::create(delay,move,NULL));
                }
            }
            else
            {
                
                int k = tradeIndex - selectIndex;
                for (int i = tradeIndex; i> selectIndex; i--)
                {
                    k--;
                    SGEmbattleItem *beforeItem = (SGEmbattleItem *)cardArray->objectAtIndex(i-1);
                    beforeItem->stopRock();
                    SGEmbattleItem* afterItem = (SGEmbattleItem *)cardArray->objectAtIndex(i-2);
                    beforeItem->index = afterItem->index;
                    CCDelayTime *delay = CCDelayTime::create(k*.1+.1);
                    CCMoveTo *move = CCMoveTo::create(.1, afterItem->getPosition());
                    beforeItem->runAction(CCSequence::create(delay, move,NULL));
                }
            }
            SGEmbattleItem *lastItem = (SGEmbattleItem *)cardArray->objectAtIndex(selectIndex -1);
            lastItem->index = tradeIndex;
            lastItem->setPosition(tradeP);
            std::sort(cardArray->data->arr, cardArray->data->arr + cardArray->data->num, SGLess);
        }
        
    }
    
}
void SGEmbattleLayer::showEmbattleBox(SGOfficerCard *card)
{

    SGEmbattleBox *embattleBox = SGEmbattleBox::create(this, card,totleGovern,emtype,0,535, 646);
    SGMainManager::shareMain()->showBox(embattleBox);
}
void SGEmbattleLayer::setSolider(int sender)
{
    colorId = sender;
    SGSoldierListBox *listBox = SGSoldierListBox::create(this, colorId , this, emtype);
    SGMainManager::shareMain()->showBox(listBox);
}

void SGEmbattleLayer::showAll()
{
    SGMainManager::shareMain()->showAllLayer();
}
void SGEmbattleLayer::goback()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (enterType == 0) {
//        SGMainManager::shareMain()->showBarrackslayer();
        SGMainManager::shareMain()->showFirstLayer();
    }else if (enterType == 1) {
        SGMainManager::shareMain()->showFirstLayer();
    }
    else if (enterType == 3) {
        
        
        SGBaseLayer*layer = (SGBaseLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_rank);
         SGMainManager::shareMain()->showLayer(layer);
    }
    else if(enterType == 4)
    {
        SGMainManager::shareMain()->goToArenaLayerRequest();
    }
}

//回调改变小兵的头像
void SGEmbattleLayer::callBackToSetSoldierSkin(CCSprite *spriteSkin)
{
    SGSoldierItem *btn = NULL;
    
    if(colorId == 3)
    {
        btn = (SGSoldierItem*)soldiers->objectAtIndex(0);
        
    }else if(colorId == 1)
    {
        btn = (SGSoldierItem*)soldiers->objectAtIndex(1);
        
    }else if(colorId == 2)
    {
        btn = (SGSoldierItem*)soldiers->objectAtIndex(2);
    }
    if (btn) {
        btn->setSoldierItem(spriteSkin);
    }
}

void SGEmbattleLayer::showBoxCall(cocos2d::CCObject *obj)
{
    if (obj)
    {
        CCDictionary *dict = (CCDictionary *)obj;
        CCString *type = (CCString *)dict->objectForKey("type");
        if (strcmp(type->getCString(), "lord") == 0)
        {
            CCString *sid = (CCString *)dict->objectForKey("lordid");
            
            CCObject *obj = NULL;
            CCARRAY_FOREACH(cardArray, obj)
            {
                
                SGEmbattleItem *item = (SGEmbattleItem *)obj;
                if (item->getOfficerCard())
                {
                    
                    int m = SGTeamgroup::shareTeamgroupInfo()->islord(emtype, item->getOfficerCard());
                    
                    if (m)
                    {
                        item->setIsLord(false);
                    }
                    if (item->getOfficerCard()->getSsid() == sid->intValue())
                    {
                        item->setIsLord(true);
                        SGTeamgroup::shareTeamgroupInfo()->changelord(emtype,item->getOfficerCard());
                        
                    }
                    
                }
                
                
            }
            this->updateView();
        }
		else if (strcmp(type->getCString(), "123") == 0)
		{
			CCLOG("WTF");
            
			this->updateEmbattle(1);
			this->updateView();
            
            
			
		}
        
    }
//    else
//    {
//        this->updateView();
//    }
	
	
    
}
void SGEmbattleLayer::setIsCanTouch(bool isTouch)
{
    this->setTouchEnabled(isTouch);
    SGBaseLayer::setIsCanTouch(isTouch);
    scrollView->setIsTouchEnabled(isTouch);
    CCObject *obj = NULL;
    CCARRAY_FOREACH(cardArray, obj)
    {
        SGEmbattleItem *item = (SGEmbattleItem *)obj;
        item->setTouchEnabled(isTouch);
    }
    if (lorditem) {
        lorditem->setTouchEnabled(isTouch);
    }
    SGMainLayer *layer = (SGMainLayer *)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(isTouch,false);
}

void SGEmbattleLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
}
bool SGEmbattleLayer::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCPoint point = convertTouchToNodeSpace(pTouch);
	CCObject *obj = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
        SGSoldierItem *item = (SGSoldierItem *)obj;
        
        CCRect faceRect = CCRectMake(item->getPosition().x - item->getContentSize().width * .5 - 40,
                                     item->getPosition().y - item->getContentSize().height * .5,
                                     item->getContentSize().width+40,
                                     item->getContentSize().height);
        if (faceRect.containsPoint(point))
        {
			CCLayerColor *bl = CCLayerColor::create(ccc4(0, 0, 0, 150), item->getContentSize().width	 - 40,item->getContentSize().height - 40);
			bl->setPosition(item->getPosition());
			bl->setAnchorPoint(ccp(0.5, 0.5));
			bl->ignoreAnchorPointForPosition(false);
			this->addChild(bl, 10, 321);
		}
    }
	
	
	CCRect pqszrect = CCRectMake(pqsz->getPosition().x-pqsz->getContentSize().width * .5,pqsz->getPosition().y - pqsz->getContentSize().height * .5, pqsz->getContentSize().width,pqsz->getContentSize().height);
    
    if (pqszrect.containsPoint(point) && pqsz->isVisible()) {
        //
		CCLayerColor *bl = CCLayerColor::create(ccc4(0, 0, 0, 150), pqszrect.size.width - 15,pqszrect.size.height - 20);
		bl->setPosition(ccpAdd(pqsz->getPosition(), ccp(0, 5)));
		bl->setAnchorPoint(ccp(0.5, 0.5));
		bl->ignoreAnchorPointForPosition(false);
		this->addChild(bl, 10, 322);
    }
	
    return true;
}

void SGEmbattleLayer::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	if (this->getChildByTag(322))
	{
		this->removeChildByTag(322);
	}
}

void SGEmbattleLayer::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point = convertTouchToNodeSpace(pTouch);
    CCRect emrect2 = CCRectMake(em2->getPosition().x - em2->getContentSize().width * .5, em2->getPosition().y - em2 ->getContentSize().height * .5, em2->getContentSize().width, em2->getContentSize().height);
    if (emrect2.containsPoint(point)) {
        embattle2();
    }
    
    CCRect emrect1 = CCRectMake(em1->getPosition().x - em1->getContentSize().width * .5, em1->getPosition().y - em1 ->getContentSize().height * .5, em1->getContentSize().width, em1->getContentSize().height);
    if (emrect1.containsPoint(point)) {
        embattle1();
    }
    CCRect emrect3 = CCRectMake(em3->getPosition().x - em3->getContentSize().width * .5, em3->getPosition().y - em3 ->getContentSize().height * .5, em3->getContentSize().width, em3->getContentSize().height);
    if (emrect3.containsPoint(point)) {
        embattle3();
    }
    CCRect pqszrect = CCRectMake(pqsz->getPosition().x-pqsz->getContentSize().width * .5,pqsz->getPosition().y - pqsz->getContentSize().height * .5, pqsz->getContentSize().width,pqsz->getContentSize().height);
    
    
    if (pqszrect.containsPoint(point) && pqsz->isVisible()) {
        dispatch();
		if (this->getChildByTag(322))
		{
			this->removeChildByTag(322);
		}
    }
    //直接使用button来做，为显示阴影效果
//    CCRect equiprect = CCRectMake(zqzb->getPosition().x-zqzb->getContentSize().width * .5,zqzb->getPosition().y - zqzb->getContentSize().height * .5, zqzb->getContentSize().width,zqzb->getContentSize().height);
//    
//    
//    if (equiprect.containsPoint(point)) {
//        equiphandler(NULL);
//    }
    
    CCObject *obj = NULL;
    CCARRAY_FOREACH(soldiers, obj)
    {
		
        SGSoldierItem *item = (SGSoldierItem *)obj;
        this->removeChildByTag(321);
        CCRect faceRect = CCRectMake(item->getPosition().x - item->getContentSize().width * .5 - 40,
                                     item->getPosition().y - item->getContentSize().height * .5,
                                     item->getContentSize().width+40,
                                     item->getContentSize().height);
        if (faceRect.containsPoint(point))
        {
			
//            CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
//            CCObject *obj1 = NULL;
//            CCARRAY_FOREACH(allSoldiers, obj1)
//            {
//				
//                SGSoldierCard *card = (SGSoldierCard *)obj1;
//                if (card->getItemId() == item->getCardId())
//                {
//                    setSolider(item->getColorId());
//                }
//            }
//            break;
            
            CCArray *allSoldiers = SGPlayerInfo::sharePlayerInfo()->getSoldierCards();
            CCObject *obj1 = NULL;
            CCARRAY_FOREACH(allSoldiers, obj1)
            {
                SGSoldierCard *card = (SGSoldierCard *)obj1;
                if (card->getItemId() == item->getCardId())
                {
                    SGMainManager::shareMain()->showSoldierInfoLayer(card,1);
                    break;
                    
                }
            }
            break;

        }
    }
}

void SGEmbattleLayer::showAddAttributeEffect(int blood, int speed, int compbatle,int gov)
{
    if (getChildByTag(ATTRIBUTELABLETAG)) {
        getChildByTag(ATTRIBUTELABLETAG)->stopAllActions();
        removeEffectLabel();
    }

    CCPoint center=SGLayout::getPoint(kMiddleCenter);
    ccColor3B lableColor = ccc3(0x0c, 0xff, 0x00);
    
    std::string attribute;
    CCString *buffer=NULL;
    if (blood>0) {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str18,blood);
        attribute.append(buffer->getCString());
        lableColor = ccc3(0x0c, 0xff, 0x00);
    }
    else if(blood<0)
    {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str181,-blood);
        attribute.append(buffer->getCString());
        lableColor = ccRED;
    }
    if (speed>0) {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str19,speed);
        attribute.append(buffer->getCString());
    }
    else if(speed<0)
    {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str191,-speed);
        attribute.append(buffer->getCString());
        lableColor =  ccRED;
    }
    if (compbatle>0) {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str20,compbatle);
        attribute.append(buffer->getCString());
    }
    else if (compbatle<0)
    {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str201,-compbatle);
        attribute.append(buffer->getCString());
        lableColor = ccRED;
    }
    if (gov>0) {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str21,gov);
        attribute.append(buffer->getCString());
    }
    else if (gov<0)
    {
        buffer=CCString::createWithFormat(str_EmbattleLayer_str211,-gov);
        attribute.append(buffer->getCString());
        lableColor = ccRED;
    }
    

    SGCCLabelTTF *label=SGCCLabelTTF::create(attribute.c_str(), FONT_BOXINFO, 30 , lableColor);
    label->setPosition(ccpAdd(center, ccp(0, 300)));
    label->setTag(ATTRIBUTELABLETAG);
    //modify by:zyc. merge into create.
    //label->setColor(lableColor);
    this->addChild(label,11111);
    label->setVisible(false);
    
    
    CCMoveTo *moveTo=CCMoveTo::create(0.5+1.0, ccpAdd(center, ccp(0, 500)));
    
    CCCallFunc *fun1=CCCallFunc::create(this, callfunc_selector(SGEmbattleLayer::showEffectLabel));
    CCCallFunc *fun2=CCCallFunc::create(this, callfunc_selector(SGEmbattleLayer::removeEffectLabel));
    
    CCMoveTo *moveTo2=CCMoveTo::create(0.5, ccpAdd(center, ccp(0,580)));
    CCActionInterval *easeIn=CCEaseIn::create(moveTo, 0.7+0.5);
    label->runAction(CCSequence::create(fun1,label->fadin(0.5),easeIn,CCDelayTime::create(1),label->fadout(0.5),moveTo2,fun2,NULL));
}
void SGEmbattleLayer::showEffectLabel(){

    SGCCLabelTTF *label=(SGCCLabelTTF *)this->getChildByTag(ATTRIBUTELABLETAG);
    label->setVisible(true);
}
void SGEmbattleLayer::removeEffectLabel(){
    
    SGCCLabelTTF *label=(SGCCLabelTTF *)this->getChildByTag(ATTRIBUTELABLETAG);
    label->removeFromParentAndCleanup(true);
}
