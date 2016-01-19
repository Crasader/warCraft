//
//  SGEquipStrengLayer.cpp
//  GameSanGuo
//
//  Created by gengshengcai on 13-1-11.
//
//

#include "SGEquipStrengLayer.h"
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
#include "SGEquipInfoLayer.h"


//彩色按钮背景
#define IMG_BTN_ON "an.png" //"hotBtnBg.png"
//灰色按钮背景
#define IMG_BTN_OFF "hotBtnBgGray.png"
#define STATUSBG   "ofc_100x100.png"

#define EQUIP_ATK "ofc_icon_atk.png" //攻击，剑
#define EQUIP_DEF "ofc_icon_def.png" //防御，盾
#define EQUIP_MOR "ofc_icon_morale.png" //血量，心
#define EQUIP_SPD "ofc_icon_speed.png" //速度，马
#define FONTBG    "labelbg.png" //速度，马
#define FONTBGSCALX 0.5
#define SHOWBOSTGAPTIME   0.12
#define HIDEBOSTCOUNT    7

SGEquipStrengLayer::SGEquipStrengLayer()
:isback(false)
,isbreak(false)
,adpt_wid(0)
,adpt_hei(0)
,shift_ipad(0)
,_card(NULL)
,equipItem(NULL)
,leftTimes(0)
,isfromfirst(0)
,equipStrengBtn(NULL)
,equipAutoStrengBtn(NULL)
,equipName(NULL)
,costCount(NULL)
,totalCount(NULL)
,equiplevel(NULL)
,currAtk(NULL)
,currDef(NULL)
,currMor(NULL)
,currSpd(NULL)
,nextAtk(NULL)
,nextDef(NULL)
,nextMor(NULL)
,nextSpd(NULL)
,equipLevels(NULL)
,bostLevel(NULL)
,finishLevel(NULL)
,finishNumLabel(NULL)
,bostNumLabel(NULL)
,m_fermEffect2(NULL)
,m_fermEffect3(NULL)
,enterType(0)
,_currentPage(0)
,equipStartLevel(0)
,totalCoin(0)
{
    ;
}

SGEquipStrengLayer::~SGEquipStrengLayer()
{
    showMainChat(true);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_EQUIP_NEWSTRENG);
    notification->removeObserver(this, MSG_EQUIP_NEWAUTOSTRENG);
    
	ResourceManager::sharedInstance()->unBindLayerTexture(sg_equipstrengLayer);
}
SGEquipStrengLayer *SGEquipStrengLayer::create(SGBaseMilitaryCard* card,int type,int isfirst,int currentPage)
{
    SGEquipStrengLayer *equipStrengLayer = new SGEquipStrengLayer();
    if (equipStrengLayer && equipStrengLayer->init(NULL, sg_equipstrengLayer))
    {
        equipStrengLayer->isfromfirst = isfirst;
        equipStrengLayer->enterType = type;
        equipStrengLayer->_card = (SGEquipCard*)card;
        equipStrengLayer->initView();
        equipStrengLayer->_currentPage = currentPage;
        equipStrengLayer->autorelease();
        return equipStrengLayer;
    }
    CC_SAFE_DELETE(equipStrengLayer);
    return NULL;
}


void SGEquipStrengLayer::refreshView(SGEquipCard *card)
{
    _card = card;
    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
	
    SGMainManager::shareMain()->addEquipPng(temp->getIconId(),sg_equipstrengLayer);
    CCString *str = CCString::createWithFormat("equip%d.png",temp->getIconId());
	
	
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());

    static_cast<CCSprite *>(this->getChildByTag(123))->setDisplayFrame(item->displayFrame());
    
    
    this->setstar(_card->getCurrStar(),_card->getUpgradestar());
    
    SGEquipmentDataModel *general = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    this->setCardType(general->getEquipType());
    
    
    equipName->setString(_card->getOfficerName()->getCString());
    int equipStar = _card->getCurrStar() - 2;
    equipStar = equipStar < 0 ? 0 : equipStar;
    if (equipStar >= 0 && equipStar < 5)
    {
        equipName->setInsideColor(ccStarLevelColor[equipStar]);
    }
    else
    {
        equipName->setInsideColor(ccWHITE);
    }
    
    equiplevel->setString(CCString::createWithFormat("Lv %d/%d",_card->getCurrLevel(), SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())->getCString());
    
    
    setEquipLevel(_card->getCurrLevel());
    setCostCount(_card->getCurrLevel());
    setCurrStatus(_card->getCurrLevel());
    
    creatBostLevel(SGLayout::getPoint(kMiddleCenter));
    
    showMainChat(false);
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    //更换背景图片
    //创建4中颜色武将背景
    std::vector<std::string> str_vec;
    if(winSize.height != 1136)
    {
        str_vec.push_back("greenBg.png");
        str_vec.push_back("blueBg.png");
        str_vec.push_back("purpleBg.png");
        str_vec.push_back("orangeBg.png");
    }
    //inphone 5  根据武将地图来
    else
    {
        str_vec.push_back("greenBgI5.png");
        str_vec.push_back("blueBgI5.png");
        str_vec.push_back("purpleBgI5.png");
        str_vec.push_back("orangeBgI5.png");
    }
    int starLev = _card->getCurrStar();
    starLev = starLev<3?3:starLev;
    starLev = starLev>6?6:starLev;
    //光效背景图片
    CCSprite * refreshBg = CCSprite::createWithSpriteFrameName(str_vec[starLev-3].c_str());
    ((CCSprite*)getChildByTag(1919))->setDisplayFrame(refreshBg->displayFrame());

}

void SGEquipStrengLayer::initView()
{

    ResourceManager::sharedInstance()->bindTexture("sgequipslayer/sgequipslayer.plist", RES_TYPE_LAYER_UI, sg_equipstrengLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist", RES_TYPE_LAYER_UI, sg_equipstrengLayer);
    ResourceManager::sharedInstance()->bindTexture("animationFile/wjzs.plist", RES_TYPE_LAYER_UI, sg_equipstrengLayer);
    ResourceManager::sharedInstance()->bindTexture("sgpropslayer/sgpropslayer.plist", RES_TYPE_LAYER_UI, sg_equipstrengLayer);
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist", RES_TYPE_LAYER_UI, sg_equipstrengLayer); //提供横条图
    ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/ofcNewUi.plist", RES_TYPE_LAYER_UI, sg_equipstrengLayer);

    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_EQUIP_NEWSTRENG,
                              this,
                              callfuncO_selector(SGEquipStrengLayer::equipStrenghListener));
    notification->addObserver(MSG_EQUIP_NEWAUTOSTRENG,
                              this,
                              callfuncO_selector(SGEquipStrengLayer::autoEquipStrenghListener));
	if (CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadResource.size.width
        ||CCEGLView::sharedOpenGLView()->getFrameSize().width == ipadhdResource.size.width)
	{
		adpt_wid = -40;
		adpt_hei = -20;
		shift_ipad = 70;
	}
	else if (CCEGLView::sharedOpenGLView()->getFrameSize().height == iphonehdResource.size.height
             ||CCEGLView::sharedOpenGLView()->getFrameSize().height == iphoneResource.size.height)
	{
        adpt_wid = -10;
		adpt_hei = -50;
	}
    else
    {
        adpt_wid = -20;
		adpt_hei = -50;
    }
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float btmheight = SGMainManager::shareMain()->getBottomHeight();
    float frameheight1 = skewingY(81);
    float hgt=skewingY(233);
    menu->setZOrder(10);
    if (winSize.height == 1136)
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
	
    SGMainManager::shareMain()->addEquipPng(temp->getIconId(),sg_equipstrengLayer);
    CCString *str = CCString::createWithFormat("equip%d.png",temp->getIconId());
	
	
    CCSprite *item = CCSprite::createWithSpriteFrameName(str->getCString());
    this->addChild(item,2, 123);
    //创建4中颜色武将背景
    std::vector<std::string> str_vec;
    if(winSize.height != 1136)
    {
        str_vec.push_back("greenBg.png");
        str_vec.push_back("blueBg.png");
        str_vec.push_back("purpleBg.png");
        str_vec.push_back("orangeBg.png");
    }
    //inphone 5  根据武将地图来
    else
    {
        str_vec.push_back("greenBgI5.png");
        str_vec.push_back("blueBgI5.png");
        str_vec.push_back("purpleBgI5.png");
        str_vec.push_back("orangeBgI5.png");
    }
    int starLev = _card->getCurrStar();
    starLev = starLev<3?3:starLev;
    starLev = starLev>6?6:starLev;
    //光效背景图片
    CCSprite * lightBg = CCSprite::createWithSpriteFrameName(str_vec[starLev-3].c_str());
    this->addChild(lightBg , 0  , 1919);
    
    float adjustY = 0;
    CCSprite *bigbg = NULL;
    item->setAnchorPoint(ccp(0.5, 0));
    if (winSize.height == 960)
    {
        adjustY = 40;
        frameSize = CCRectMake(0, 0 , 768, 413);
        this->setItemID("pad装备底图.png",true);
        fermEffect->setScale(winSize.width/768);
        fermEffect1->setScale(winSize.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -287 +26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -287 +26)));
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -365)));
        
        lightBg->setZOrder(0);
        lightBg->setScaleX(winSize.width / lightBg->getContentSize().width);
        lightBg->setScaleY(1.6);
        lightBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),
                               ccp(0, -title_bg->getContentSize().height - lightBg->getContentSize().height*0.5*lightBg->getScaleY())));;
        
    }
    else if (winSize.height == 1136)
    {
        adjustY=31;
        
        frameSize = CCRectMake(0, 520 -(winSize.height - hgt*1.18 - title_bg->getContentSize().height), 768, (winSize.height - hgt*1.18 - title_bg->getContentSize().height));
        this->setItemID("carditembg.png",true);
        fermEffect->setScaleX(winSize.width/768);
        fermEffect1->setScaleX(winSize.width/768);
        fermEffect->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(-264, -361+26)));
        fermEffect1->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(264, -361+26)));
        
        item->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -450)));
        
        CCSprite *temp = CCSprite::createWithSpriteFrameName("carditembg.png");
        lightBg->setScaleX(winSize.width / lightBg->getContentSize().width);
        lightBg->setScaleY(temp->getContentSize().height * 0.75 / lightBg->getContentSize().height);
        lightBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),
                               ccp(0, -title_bg->getContentSize().height - temp->getContentSize().height * 0.8 /2 + 16) ));
        
    }
    else
    {
        adjustY = 55;
        
        bigbg = CCSprite::createWithSpriteFrameName("pad装备底图.png");
        
        this->addChild(bigbg,-100);
        bigbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - title_bg->getContentSize().height - bigbg->getContentSize().height*.45)));
        fermEffect->setPosition(ccpAdd(bigbg->getPosition(), ccp(-bigbg->getContentSize().width*.41, bigbg->getContentSize().height*.02 -bigbg->getContentSize().height*.098)));
        fermEffect1->setPosition(ccpAdd(bigbg->getPosition(), ccp(bigbg->getContentSize().width*.41, bigbg->getContentSize().height*.02-bigbg->getContentSize().height*.098)));
        
        item->setPosition(ccpAdd(bigbg->getPosition(), ccp(0, -bigbg->getContentSize().height*.362)));
        
        lightBg->setScaleX(winSize.width / lightBg->getContentSize().width);
        lightBg->setScaleY(1.9);
        lightBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),
                               ccp(0, -title_bg->getContentSize().height-5 - lightBg->getContentSize().height*0.5*lightBg->getScaleY())));
    }
    equipItem = item;
    fermEffect->play();
    fermEffect1->play();
    
    this->setstar(_card->getCurrStar(),_card->getUpgradestar());
    this->setTitle(str_zhuangbeiqianghua);//_card->getOfficerName()->getCString()

    //////////////////////
    
    float btnwidth = skewing(5);
    float btnwid = skewing(33);
    
    equipStrengBtn = SGButton::createFromLocal(IMG_BTN_ON, str_qianghua_, this, menu_selector(SGEquipStrengLayer::equipStrengHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(equipStrengBtn);
    if(winSize.height==1136)
        equipStrengBtn->setPosition(ccp(winSize.width/2- btnwid*3 - btnwidth - adpt_wid, btmheight*.55 + equipStrengBtn->getContentSize().height*0.7+ adjustY*0.6));
    else if (winSize.height == 1024)
        equipStrengBtn->setPosition(ccp(winSize.width/2- btnwid*3 - btnwidth - adpt_wid, btmheight*.55 + equipStrengBtn->getContentSize().height*0.7));
    else
        equipStrengBtn->setPosition(ccp(winSize.width/2- btnwid*3 - btnwidth - adpt_wid, btmheight*.55 + equipStrengBtn->getContentSize().height*0.7 + 10));
    
    equipAutoStrengBtn = SGButton::createFromLocal(IMG_BTN_ON, str_autoqianghua, this, menu_selector(SGEquipStrengLayer::autoEquipStrengHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(equipAutoStrengBtn);
    equipAutoStrengBtn->setPosition(ccpAdd(equipStrengBtn->getPosition(), ccp( equipAutoStrengBtn->getContentSize().width - adpt_wid,0 )));
    
    int limittag = SGStaticDataManager::shareStatic()->getimitTagById(limitEquipGuide,9);
    equipAutoStrengBtn->setTag(limittag);
    
    SGButton *backBtn = SGButton::createFromLocal(IMG_BTN_ON, str_Back_, this, menu_selector(SGEquipStrengLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(equipAutoStrengBtn->getPosition(), ccp( backBtn->getContentSize().width - adpt_wid,0)));
    
    
    SGEquipmentDataModel *general = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    this->setCardType(general->getEquipType());
    
 
    ////////////////////////
    
    float bigFrmWidth = winSize.width - 40;
    float bigFrmHeight = (int)winSize.height/3 -20;
    
    CCScale9Sprite *bigFrm = CCScale9Sprite::createWithSpriteFrameName("barrack_kuang.png");
    bigFrm->setPreferredSize(CCSizeMake(bigFrmWidth, bigFrmHeight));
    this->addChild(bigFrm,7);
    if(winSize.height==1136)
        bigFrm->setPosition(ccpAdd(ccp(winSize.width/2,equipStrengBtn->getPosition().y), ccp(0, equipStrengBtn->getContentSize().height/2 + bigFrmHeight/2 + 4 + adjustY*0.6)));
    else if(winSize.height==1024)
        bigFrm->setPosition(ccpAdd(ccp(winSize.width/2,equipStrengBtn->getPosition().y), ccp(0, equipStrengBtn->getContentSize().height/2 + bigFrmHeight/2 + 4)));
    else
        bigFrm->setPosition(ccpAdd(ccp(winSize.width/2,equipStrengBtn->getPosition().y), ccp(0, equipStrengBtn->getContentSize().height/2 + bigFrmHeight/2 + 10 )));
    
    CCSprite *bigFrmBg = CCSprite::createWithSpriteFrameName("barrack_kuangbg.png");
    this->addChild(bigFrmBg,6);
    bigFrmBg->setScaleX(bigFrmWidth/bigFrmBg->getContentSize().width);
    bigFrmBg->setScaleY(bigFrmHeight/bigFrmBg->getContentSize().height);
    bigFrmBg->setPosition(bigFrm->getPosition());
    
    CCSprite* admsBkg = CCSprite::createWithSpriteFrameName("store_redbg.png");
    float scaleX = winSize.width / admsBkg->getContentSize().height;
    float scaleY = 60 / admsBkg->getContentSize().height;
    admsBkg->setScaleX(scaleX);
    admsBkg->setScaleY(scaleY);
    this->addChild(admsBkg, 9);
//    admsBkg->setPosition(ccpAdd(bigFrm->getPosition(), ccp(0,admsBkg->getContentSize().height*scaleY/2 + bigFrm->getContentSize().height/2) ));
    
    admsBkg->setPosition(ccpAdd(lightBg->getPosition(), ccp(0, -lightBg->getScaleY() *lightBg->getContentSize().height*0.5 - admsBkg->getContentSize().height*0.5*lightBg->getScaleY() + adjustY)) );
    /////////////////////////////////
    
    equipName = SGCCLabelTTF::create(_card->getOfficerName()->getCString(), FONT_PANGWA, 26);
    equipName->setPosition(ccpAdd(bigFrm->getPosition(), ccp(-40,bigFrm->getContentSize().height/2 - 26 )));
    int equipStar = _card->getCurrStar() - 2;
    equipStar = equipStar < 0 ? 0 : equipStar;
    if (equipStar >= 0 && equipStar < 5) {
        equipName->setInsideColor(ccStarLevelColor[equipStar]);
    }
    else
    {
        equipName->setInsideColor(ccWHITE);
    }
    addChild(equipName,11);
    
    equiplevel = SGCCLabelTTF::create(CCString::createWithFormat("Lv %d/%d",_card->getCurrLevel(),SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())->getCString(), FONT_PANGWA, 26);
    addChild(equiplevel,11);
    equiplevel->setPosition(ccpAdd(equipName->getPosition(), ccp(equipName->getContentSize().width/2 + equiplevel->getContentSize().width/2 + 20,0)));
    
    ///////////////////////////
    
    SGCCLabelTTF *costMoney = SGCCLabelTTF::create(str_zbqianghuancost, FONT_PANGWA, 24 , COLOR_UNKNOW_TAN);
    this->addChild(costMoney,11);
    costMoney->setPosition( ccp(costMoney->getContentSize().width/2 +20, admsBkg->getPosition().y));
    
    SGCCLabelTTF *totalMoney = SGCCLabelTTF::create(str_zbqianghuantotalMy, FONT_PANGWA, 24 , COLOR_UNKNOW_TAN);
    this->addChild(totalMoney,11);
    totalMoney->setPosition( ccp(winSize.width/2 + totalMoney->getContentSize().width/2 + 10 , costMoney->getPosition().y));
    
    CCSprite *font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    font_bg->setScaleY(1.4);
    this->addChild(font_bg,9);
    font_bg->setPosition(ccpAdd(costMoney->getPosition(),  ccp(costMoney->getContentSize().width/2 + font_bg->getContentSize().width/2+ 6,0)));
    
    font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    font_bg->setScaleY(1.4);
    this->addChild(font_bg,9);
    font_bg->setPosition(ccpAdd(totalMoney->getPosition(),  ccp(totalMoney->getContentSize().width/2 + font_bg->getContentSize().width/2+6,0)));
    
    CCSprite *coins = CCSprite::createWithSpriteFrameName("coinicon.png");
    coins->setPosition(ccpAdd(costMoney->getPosition(),ccp(costMoney->getContentSize().width/2 +coins->getContentSize().width/2+4,0)));
    addChild(coins,11);
    
    costCount = SGCCLabelTTF::create("999999", FONT_PANGWA, 24);
    costCount->setPosition(ccpAdd(coins->getPosition(),ccp(coins->getContentSize().width/2+costCount->getContentSize().width/2+8,0)));
    addChild(costCount,11);
    
    coins = CCSprite::createWithSpriteFrameName("coinicon.png");
    coins->setPosition(ccpAdd(totalMoney->getPosition(),ccp(totalMoney->getContentSize().width /2+coins->getContentSize().width/2+4,0)));
    addChild(coins,11);
    
    totalCount = SGCCLabelTTF::create("999999", FONT_PANGWA, 24);
    totalCount->setPosition(ccpAdd(coins->getPosition(),ccp(coins->getContentSize().width/2+totalCount->getContentSize().width/2 +8,0)));
    addChild(totalCount,11);
    
    /////////////////////////////////
    
    float f1Height = (int)bigFrmHeight*2/3;
    float f1Width = (int)f1Height* 0.85;
    
    CCScale9Sprite *frame1 = CCScale9Sprite::createWithSpriteFrameName(STATUSBG);
    frame1->setPreferredSize(CCSizeMake(f1Width, f1Height));
    this->addChild(frame1,11);
    frame1->setPosition(ccp(40 + f1Width/2, bigFrm->getPosition().y - 30));
    
    CCScale9Sprite *frame2 = CCScale9Sprite::createWithSpriteFrameName(STATUSBG);
    frame2->setPreferredSize(CCSizeMake(f1Width, f1Height));
    this->addChild(frame2,11);
    frame2->setPosition( ccp(winSize.width - frame1->getPosition().x, frame1->getPosition().y));
    
    
    SGCCLabelTTF *currentTitle = SGCCLabelTTF::create(str_zbqianghuacurstatus, FONT_PANGWA, 32);
    this->addChild(currentTitle,11);
    currentTitle->setPosition( ccp(frame1->getPosition().x, frame1->getPosition().y + frame1->getContentSize().height/2 + 30));

    SGCCLabelTTF *nextTitle = SGCCLabelTTF::create(str_zbqianghuanextstatus, FONT_PANGWA, 32 , ccGREEN);
    this->addChild(nextTitle,11);
    nextTitle->setPosition( ccp(frame2->getPosition().x, currentTitle->getPosition().y));
    //modify by:zyc. merge into create.
    //nextTitle->setColor(ccGREEN);
    
 
    CCSprite *equipArrow = CCSprite::createWithSpriteFrameName("equipArrow.png");
    this->addChild(equipArrow,20);
    equipArrow->setPosition(ccp(SGLayout::getPoint(kMiddleCenter).x, frame1->getPosition().y));

    this->addCurrStatus( ccpAdd(frame1->getPosition(),ccp( -f1Width/2,f1Height/2)),f1Width * 0.4,1.4*winSize.height/1140);
    this->addNextStatus( ccpAdd(frame2->getPosition(),ccp( -f1Width/2,f1Height/2)),f1Width * 0.4,1.4*winSize.height/1140);
    
    
    setEquipLevel(_card->getCurrLevel());
    setCostCount(_card->getCurrLevel());
    setCurrStatus(_card->getCurrLevel());

    creatBostLevel(SGLayout::getPoint(kMiddleCenter));
    
    showMainChat(false);
    
}

void SGEquipStrengLayer::showMainChat(bool isvisble)
{
    
    isvisble = false;   //added by cgp
    SGMainLayer* mainLayerT = SGMainManager::shareMain()->getMainLayer();
    SGButton *mainLayerChat = (SGButton *)mainLayerT->getBtnByTag(CHATBTNTAG);
    if (mainLayerChat) {
        mainLayerChat->setVisible(isvisble);
        mainLayerChat->setEnabled(isvisble);
        mainLayerT->getChildByTag(CHATBTNTAG + 101)->setVisible(isvisble);
        
        if (mainLayerT->chat) {
            mainLayerT->chat->setVisible(isvisble);
        }
        if (mainLayerT->chatMore) {
            mainLayerT->chatMore->setVisible(isvisble);
        }
        
    }
}
void SGEquipStrengLayer::setEquipLevel(int costLevel)
{
    equiplevel->setString(CCString::createWithFormat("Lv %d/%d",costLevel,SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())->getCString());
    equiplevel->setPosition(ccpAdd(equipName->getPosition(), ccp(equipName->getContentSize().width/2 + equiplevel->getContentSize().width/2 + 20,0)));
    
}

void SGEquipStrengLayer::setCostCount(int costLevel,bool isLastTime,int lastLevel)
{
    costCount->setString(CCString::createWithFormat("%d",getStrenghCost(costLevel))->getCString());
    
    if (isLastTime)
    {
        totalCoin = SGPlayerInfo::sharePlayerInfo()->getPlayerCoins();
    }
    else
    {
        totalCoin -= getStrenghCost(lastLevel);
        totalCoin = totalCoin > 0?totalCoin:0;
    }
   
    if(totalCoin>=10000000)
    {
        totalCount->setString(CCString::createWithFormat(str_wan_,(int)(totalCoin/10000))->getCString());
    }
    else
    {
       totalCount->setString(CCString::createWithFormat("%d",totalCoin)->getCString());
    }

}

void SGEquipStrengLayer::setCurrStatus(int level)
{
    CCDictionary *dicc = SGCardAttribute::getValue(level, _card->getItemId());
    if (dicc)
    {
        currAtk->setString(((CCString *)dicc->objectForKey("atk"))->getCString());
        currDef->setString(((CCString *)dicc->objectForKey("def"))->getCString());
        currMor->setString(((CCString *)dicc->objectForKey("mor"))->getCString());
        currSpd->setString(((CCString *)dicc->objectForKey("speed"))->getCString());
    }
    setNextStatus(level+1);
}

void SGEquipStrengLayer::setNextStatus(int level)
{
    CCDictionary *dicc = SGCardAttribute::getValue(level, _card->getItemId());
    if (dicc)
    {
        nextAtk->setString(((CCString *)dicc->objectForKey("atk"))->getCString());
        nextDef->setString(((CCString *)dicc->objectForKey("def"))->getCString());
        nextMor->setString(((CCString *)dicc->objectForKey("mor"))->getCString());
        nextSpd->setString(((CCString *)dicc->objectForKey("speed"))->getCString());
    }
}

void SGEquipStrengLayer::addCurrStatus(CCPoint startPos, float fontwid,float heightRate)
{
    CCSprite * atk = CCSprite::createWithSpriteFrameName(EQUIP_ATK);
    atk->setPosition(ccpAdd(startPos,  ccp(atk->getContentSize().width , -atk->getContentSize().height)));
    this->addChild(atk,12);
    
    CCSprite *font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    this->addChild(font_bg,12);
    font_bg->setScaleX(FONTBGSCALX);
    font_bg->setPosition(ccpAdd(atk->getPosition(),  ccp(fontwid,0)));
    currAtk = SGCCLabelTTF::create("00001", FONT_PANGWA, 20);
    currAtk->setPosition(font_bg->getPosition());
    this->addChild(currAtk,13);
  
    
    CCSprite * def = CCSprite::createWithSpriteFrameName(EQUIP_DEF);
    def->setPosition(ccpAdd(atk->getPosition(),  ccp(0,-atk->getContentSize().height * heightRate)));
    this->addChild(def,12);
    font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    font_bg->setScaleX(FONTBGSCALX);
    this->addChild(font_bg,12);
    font_bg->setPosition(ccpAdd(def->getPosition(),  ccp(fontwid,0)));
    currDef = SGCCLabelTTF::create("00001", FONT_PANGWA, 20);
    currDef->setPosition(font_bg->getPosition());
    this->addChild(currDef,13);
    
    CCSprite * mor = CCSprite::createWithSpriteFrameName(EQUIP_MOR);
    mor->setPosition(ccpAdd(def->getPosition(), ccp(0,-atk->getContentSize().height * heightRate)));
    this->addChild(mor,12);
    font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    this->addChild(font_bg,12);
    font_bg->setScaleX(FONTBGSCALX);
    font_bg->setPosition(ccpAdd(mor->getPosition(),  ccp(fontwid,0)));
    currMor = SGCCLabelTTF::create("00001", FONT_PANGWA, 20);
    currMor->setPosition(font_bg->getPosition());
    this->addChild(currMor,13);
    
    CCSprite * spd = CCSprite::createWithSpriteFrameName(EQUIP_SPD);
    spd->setPosition(ccpAdd(mor->getPosition(),  ccp(0,-atk->getContentSize().height * heightRate)));
    this->addChild(spd,12);
    font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    this->addChild(font_bg,12);
    font_bg->setScaleX(FONTBGSCALX);
    font_bg->setPosition(ccpAdd(spd->getPosition(),  ccp(fontwid,0)));
    currSpd = SGCCLabelTTF::create("00001", FONT_PANGWA, 20);
    currSpd->setPosition(font_bg->getPosition());
    this->addChild(currSpd,13);
    
}

void SGEquipStrengLayer::addNextStatus(CCPoint startPos, float fontwid,float heightRate)
{
    CCSprite * atk = CCSprite::createWithSpriteFrameName(EQUIP_ATK);
    atk->setPosition(ccpAdd(startPos,  ccp(atk->getContentSize().width , -atk->getContentSize().height)));
    this->addChild(atk,12);
    
    CCSprite *font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    this->addChild(font_bg,12);
    font_bg->setScaleX(FONTBGSCALX);
    font_bg->setPosition(ccpAdd(atk->getPosition(),  ccp(fontwid,0)));
    nextAtk = SGCCLabelTTF::create("00001", FONT_PANGWA, 20 , ccGREEN);
    nextAtk->setPosition(font_bg->getPosition());
    this->addChild(nextAtk,13);
    //modify by:zyc. merge into create.
    //nextAtk->setColor(ccGREEN);
    
    CCSprite * def = CCSprite::createWithSpriteFrameName(EQUIP_DEF);
    def->setPosition(ccpAdd(atk->getPosition(),  ccp(0,-atk->getContentSize().height * heightRate)));
    this->addChild(def,12);
    font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    font_bg->setScaleX(FONTBGSCALX);
    this->addChild(font_bg,12);
    font_bg->setPosition(ccpAdd(def->getPosition(),  ccp(fontwid,0)));
    nextDef = SGCCLabelTTF::create("00001", FONT_PANGWA, 20 , ccGREEN);
    nextDef->setPosition(font_bg->getPosition());
    this->addChild(nextDef,13);
    //modify by:zyc. merge into create.
    //nextDef->setColor(ccGREEN);
    
    CCSprite * mor = CCSprite::createWithSpriteFrameName(EQUIP_MOR);
    mor->setPosition(ccpAdd(def->getPosition(), ccp(0,-atk->getContentSize().height * heightRate)));
    this->addChild(mor,12);
    font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    this->addChild(font_bg,12);
    font_bg->setScaleX(FONTBGSCALX);
    font_bg->setPosition(ccpAdd(mor->getPosition(),  ccp(fontwid,0)));
    nextMor = SGCCLabelTTF::create("00001", FONT_PANGWA, 20 , ccGREEN);
    nextMor->setPosition(font_bg->getPosition());
    this->addChild(nextMor,13);
    //modify by:zyc. merge into create.
    //nextMor->setColor(ccGREEN);
    
    CCSprite * spd = CCSprite::createWithSpriteFrameName(EQUIP_SPD);
    spd->setPosition(ccpAdd(mor->getPosition(),  ccp(0,-atk->getContentSize().height * heightRate)));
    this->addChild(spd,12);
    font_bg = CCSprite::createWithSpriteFrameName(FONTBG);
    this->addChild(font_bg,12);
    font_bg->setScaleX(FONTBGSCALX);
    font_bg->setPosition(ccpAdd(spd->getPosition(),  ccp(fontwid,0)));
    nextSpd = SGCCLabelTTF::create("00001", FONT_PANGWA, 20 , ccGREEN);
    nextSpd->setPosition(font_bg->getPosition());
    this->addChild(nextSpd,13);
    //modify by:zyc. merge into create.
    //nextSpd->setColor(ccGREEN);
}

void SGEquipStrengLayer::creatBostLevel(CCPoint orignPos)
{
    equipLevels = CCSprite::createWithSpriteFrameName("equipLevels.png");
    addChild(equipLevels,20);
    equipLevels->setPosition(ccpAdd(orignPos,  ccp(0, equipLevels->getContentSize().height * 1.5)));
    
    bostLevel = CCSprite::createWithSpriteFrameName("equipBost.png");
    addChild(bostLevel,20);
    bostLevel->setPosition(ccpAdd(equipLevels->getPosition(),  ccp(0, bostLevel->getContentSize().height/2  )));
    
    finishLevel = CCSprite::createWithSpriteFrameName("equipFinish.png");
    addChild(finishLevel,20);
    finishLevel->setPosition(bostLevel->getPosition());
    
    
    
    finishNumLabel = CCLabelAtlas::create("1", "sanguobigpic/equipFinishNum.png", 91, 91, '0');
    finishNumLabel->setPosition( ccp(equipLevels->getContentSize().width/2 - 20, 0));
    equipLevels->addChild(finishNumLabel,20);
    
    bostNumLabel = CCLabelAtlas::create("2", "sanguobigpic/equipBostNum.png", 91, 91, '0');
    bostNumLabel->setPosition(ccp(equipLevels->getContentSize().width/2 - 20, 0));
    equipLevels->addChild(bostNumLabel,20);
    
    hideLables();
    
    createGuanquan(true);

}
void SGEquipStrengLayer::hideLables(bool hideFlag)
{
    equipLevels->setVisible(hideFlag);
    bostLevel->setVisible(hideFlag);
    finishLevel->setVisible(hideFlag);
    finishNumLabel->setVisible(hideFlag);
    bostNumLabel->setVisible(hideFlag);
}

void SGEquipStrengLayer::createGuanquan(bool isfirst)
{
    ResourceManager::sharedInstance()->bindTexture("animationFile/qhtexiao.plist", RES_TYPE_LAYER_UI, sg_equipstrengLayer);
    //    //地圈
    CCSpriterX *fermEffect2 = CCSpriterX::create("animationFile/qh_000.scml", true, true);
    fermEffect2->setanimaID(0);
    fermEffect2->setisloop(false);
    addChild(fermEffect2, 1);
    //    //光圈
    CCSpriterX *fermEffect3 = CCSpriterX::create("animationFile/qh_001.scml", true, true);
    fermEffect3->setanimaID(0);
    fermEffect3->setisloop(false);
    addChild(fermEffect3, 1);
    
    CCSize winS = CCDirector::sharedDirector()->getWinSize();
    fermEffect2->setScale(winS.width/768);
    fermEffect3->setScale(winS.width/768);
    fermEffect2->setPosition(ccpAdd(equipItem->getPosition(),ccp(0, -74)));
    fermEffect3->setPosition(fermEffect2->getPosition());
    fermEffect3->play();
    fermEffect2->play();
    
    if (isfirst)
    {
        m_fermEffect2 = fermEffect2;
        m_fermEffect2->stop();
        m_fermEffect2->setisloop(true);
        m_fermEffect2->setVisible(false);
        
        m_fermEffect3 = fermEffect3;
        m_fermEffect3->stop();
        m_fermEffect3->setisloop(true);
        m_fermEffect3->setVisible(false);
    }
}
void SGEquipStrengLayer::showBostLevel()
{
    if (processList.size() < 1)
    {
        m_fermEffect2->stop();
        m_fermEffect2->setVisible(false);

        m_fermEffect3->stop();
        m_fermEffect3->setVisible(false);

        hideLables();
        setIsCanTouch(true);
        return;
    }
    
    setIsCanTouch(false);
    int levels = processList[0];
    equipStartLevel += levels;
    setEquipLevel(equipStartLevel);
    setCostCount(equipStartLevel,processList.size() == 1,equipStartLevel - levels);
    setCurrStatus(equipStartLevel);
    
    bool isbost = levels > 1 ? true : false;
    processList.erase(processList.begin());
    
    bostLevel->setVisible(isbost);
    bostNumLabel->setVisible(isbost);
    finishLevel->setVisible(!isbost);
    finishNumLabel->setVisible(!isbost);
    
    equipLevels->setVisible(true);
    bostLevel->stopAllActions();
    finishLevel->stopAllActions();
    equipLevels->stopAllActions();
    if (isbost)
    {
        bostLevel->setOpacity(255*0.2);
        bostLevel->setScale(2.95);
        
        bostLevel->runAction(CCSequence::create(CCDelayTime::create(SHOWBOSTGAPTIME*2),
                                                   CCScaleTo::create(SHOWBOSTGAPTIME*2,0.89),
                                                   CCScaleTo::create(SHOWBOSTGAPTIME,1.13),
                                                   CCScaleTo::create(SHOWBOSTGAPTIME,1.06),
                                                   CCScaleTo::create(SHOWBOSTGAPTIME,1),
                                                   CCDelayTime::create(SHOWBOSTGAPTIME*HIDEBOSTCOUNT),NULL));
        bostLevel->runAction(CCSequence::create(CCDelayTime::create(SHOWBOSTGAPTIME*2),CCFadeTo::create(SHOWBOSTGAPTIME*2, 255),NULL));
    
        
        bostNumLabel->setOpacity(255*0.2);
        bostNumLabel->setString(CCString::createWithFormat("%d",levels)->getCString());
        bostNumLabel->runAction(CCSequence::create(CCDelayTime::create(SHOWBOSTGAPTIME*2),CCFadeTo::create(SHOWBOSTGAPTIME*2, 255),NULL));
    }
    else
    {
        finishLevel->setOpacity(255*0.2);
        finishLevel->setScale(2.95);
        
        finishLevel->runAction(CCSequence::create(CCDelayTime::create(SHOWBOSTGAPTIME*2),
                                                   CCScaleTo::create(SHOWBOSTGAPTIME*2,0.89),
                                                   CCScaleTo::create(SHOWBOSTGAPTIME,1.13),
                                                   CCScaleTo::create(SHOWBOSTGAPTIME,1.06),
                                                   CCScaleTo::create(SHOWBOSTGAPTIME,1),
                                                   CCDelayTime::create(SHOWBOSTGAPTIME*HIDEBOSTCOUNT),NULL));
        finishLevel->runAction(CCSequence::create(CCDelayTime::create(SHOWBOSTGAPTIME*2),CCFadeTo::create(SHOWBOSTGAPTIME*2, 255),NULL));
    
        finishNumLabel->setOpacity(255*0.2);
        finishNumLabel->setString(CCString::createWithFormat("%d",levels)->getCString());
        finishNumLabel->runAction(CCSequence::create(CCDelayTime::create(SHOWBOSTGAPTIME*2),CCFadeTo::create(SHOWBOSTGAPTIME*2, 255),NULL));
    }
    
    
    
    equipLevels->setOpacity(255*0.2);
    equipLevels->setScale(2.95);
    
    equipLevels->runAction(CCSequence::create(CCDelayTime::create(SHOWBOSTGAPTIME*2),
                                               CCScaleTo::create(SHOWBOSTGAPTIME*2,0.89),
                                               CCScaleTo::create(SHOWBOSTGAPTIME,1.13),
                                               CCScaleTo::create(SHOWBOSTGAPTIME,1.06),
                                               CCScaleTo::create(SHOWBOSTGAPTIME,1),
                                               CCDelayTime::create(SHOWBOSTGAPTIME*HIDEBOSTCOUNT),
                                               CCCallFunc::create(this, callfunc_selector(SGEquipStrengLayer::showBostLevel)),NULL));
    equipLevels->runAction(CCSequence::create(CCDelayTime::create(SHOWBOSTGAPTIME*2),CCFadeTo::create(SHOWBOSTGAPTIME*2, 255),NULL));
    
    if (processList.size() == 0)
    {
        createGuanquan();
    }
    else 
    {
        m_fermEffect2->stop();
        m_fermEffect2->play();
        m_fermEffect2->setVisible(true);
        m_fermEffect2->setanimaID(0);

        m_fermEffect3->stop();
        m_fermEffect3->play();
        m_fermEffect3->setVisible(true);
        m_fermEffect3->setanimaID(0);
    }
}

void SGEquipStrengLayer::equipStrengHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::EquipmentStrengthRequest *request = new main::EquipmentStrengthRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_NEWSTRENG, request);
    setIsCanTouch(false);
    equipStartLevel = _card->getCurrLevel();
    totalCoin = SGPlayerInfo::sharePlayerInfo()->getPlayerCoins();
}

void SGEquipStrengLayer::autoEquipStrengHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    main::EquipmentStrengthAutoRequest *request = new main::EquipmentStrengthAutoRequest();
    request->set_ssid(_card->getSsid());
    SGSocketClient::sharedSocketClient()->send(MSG_EQUIP_NEWAUTOSTRENG, request);
    setIsCanTouch(false);
    equipStartLevel = _card->getCurrLevel();
    totalCoin = SGPlayerInfo::sharePlayerInfo()->getPlayerCoins();
}

void SGEquipStrengLayer::equipStrenghListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::EquipmentStrengthResponse *response = (main::EquipmentStrengthResponse *)sr->m_msg;
        if (response->state() == 1)
        {
            processList.clear();
            processList.push_back(response->addlevel());
            showBostLevel();
        }
        else
        {
            if (_card->getCurrLevel() >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())
            {
                SGMainManager::shareMain()->showMessage(str_zbqianghuatoplevel);
            }
            else
            {
                SGMainManager::shareMain()->showMessage(str_zbqianghuaneedmoney);
            }
            setIsCanTouch(true);
        }
    }
    else
    {
        setIsCanTouch(true);
    }
}

void SGEquipStrengLayer::autoEquipStrenghListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    if (sr)
    {
        main::EquipmentStrengthAutoResponse *response = (main::EquipmentStrengthAutoResponse *)sr->m_msg;
        if (response->state() == 1)
        {

            processList.clear();
            int levelSize = response->process_size();
            for (int i=0; i<levelSize; i++)
            {
                processList.push_back(response->process(i));
            }
            
            showBostLevel();
        }
        else
        {
            if (_card->getCurrLevel() >= SGPlayerInfo::sharePlayerInfo()->getPlayerLevel())
            {
                SGMainManager::shareMain()->showMessage(str_zbqianghuatoplevel);
            }
            else
            {
                SGMainManager::shareMain()->showMessage(str_zbqianghuaneedmoney);
            }
            setIsCanTouch(true);
        }
    }
    else
    {
        setIsCanTouch(true);
    }
}


void SGEquipStrengLayer::setEquipData(int cardLevel,int sellPrice)
{
    //equipCard->setCurrLevel(cardLevel);
}

void SGEquipStrengLayer::backHandler()
{
    EFFECT_PLAY(MUSIC_BTN);
    
    SGMainLayer *layer = (SGMainLayer*)SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer);
    layer->setIsCanTouch(true);
	
    if (isfromfirst==1)
	{
        if (enterType == 2)
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL,0,1,true,0,_currentPage,0);
        }
    }
    else if(isfromfirst==3)
    {
        SGEquipCard * equip =(SGEquipCard*)_card;
        int id = equip->getOfficerCardId();
        SGMainManager::shareMain()->showGeneralInfoLayer(SGPlayerInfo::sharePlayerInfo()->getOfficerCard(id), 3,NULL,true,NULL,12);
    }
    else
    {
        //装备详情 强化  返回
        if (-10 == isfromfirst)
        {
            SGEquipInfoLayer *Layer = SGEquipInfoLayer::create(_card,0);
            SGMainManager::shareMain()->showLayer(Layer,false);
            return;
        }
        else if (enterType == 2)
        {
            SGMainManager::shareMain()->showEquipsLayer(NULL,0,0,true,0 ,_currentPage,0);
            
        }
        
    }
}

int SGEquipStrengLayer::getStrenghCost(int strenghLevel)
{
    int costMoney = 0;
    
    if (_card)
    {
        int type = -1;
        type = SGStaticDataManager::shareStatic()->getStrengEquipType(_card->getItemId());
        
//        SGEquipmentDataModel *equip = SGStaticDataManager::shareStatic()->getEquipById(_card->getItemId());
    
//        if (equip->getEquipExpType()->isEqual(CCString::create("A"))) {
//            type = 0;
//        }
//        else if (equip->getEquipExpType()->isEqual(CCString::create("B"))) {
//            type = 1;
//        }
//        else if (equip->getEquipExpType()->isEqual(CCString::create("C"))) {
//            type = 2;
//        }
//        else if (equip->getEquipExpType()->isEqual(CCString::create("D"))) {
//            type = 3;
//        }
        
        int nextLvl = strenghLevel + 1;
 
        SGExpDataModel *num = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, nextLvl);
        if (num) {
            costMoney = num->getEquipCost();
            (costMoney < 0) && (costMoney = 0);
        }
    }
    
    return costMoney;
}



bool SGEquipStrengLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    if (processList.size() > 1)
    {
        int level = processList[processList.size() -1];
        processList.clear();
        processList.push_back(level);
        equipStartLevel = _card->getCurrLevel() - level;
        showBostLevel();
    }
    return false;
}

void SGEquipStrengLayer::onEnter()
{
    SGBaseLayer::onEnter();
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -8888, true);
}

void SGEquipStrengLayer::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    SGBaseLayer::onExit();
}




