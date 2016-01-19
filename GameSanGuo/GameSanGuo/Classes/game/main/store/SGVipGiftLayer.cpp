//
//  SGVipGiftLayer.cpp
//  GameSanGuo
//
//  Created by 陈雪龙 on 13-3-13.
//
//

#include "SGVipGiftLayer.h"
#include "SGMainManager.h"
#include "SGBaseShow.h"
#include "SGNotificationCenter.h"
#include "SGMsgId.h"
#include "SGSocketClient.h"
#include "SGPlayerInfo.h"
#include "ShopModule.pb.h"
#include "SGCantAdvanceBox.h"
#include "SGMenu.h"
#include "SGStaticDataManager.h"
#include "SGRichLabel.h"
#include "SimpleAudioEngine.h"
#include "SGGiftInfoBox.h"
#include "ResourceManager.h"
#include "SGStringConfig.h"
#define MSGTITLEHEIGHT 20
#define LABELHEIGHT    -30

#define VIPINFOSIZE    24
#define VIPINFONUM     15
#define VIPRIGHTTAG    1200
#define LABLESCAL     0.54
SGVipGiftLayer::SGVipGiftLayer():currVipLvl(0)
,_buyGift(NULL)
,vipGifts(NULL)
,giftArray(NULL)
, _touch(true)
,expprog(NULL)
,label1(NULL)
,label2(NULL)
,label_exp(NULL)
,fontvip(NULL)
,currvip(NULL)
, _need(0)
, _curr(0)
,_max(0)
{    
}
SGVipGiftLayer::~SGVipGiftLayer()
{
    CC_SAFE_RELEASE(vipGifts);
    CC_SAFE_RELEASE(giftArray);
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->removeObserver(this, MSG_VIPGIFT_BUY);
//    notification->removeObserver(this, MSG_STORE_ENTER);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_vipGiftLayer);

    //退出页面时  重置值 字段意义请参看函数定义。
    SGMainManager::shareMain()->setFromShopLayer(false);
}
SGVipGiftLayer *SGVipGiftLayer::create(int lvl, CCArray *_array,int curr,int max)
{
    SGVipGiftLayer *vipLayer = new SGVipGiftLayer();
    if (vipLayer && vipLayer->init(NULL, sg_vipGiftLayer))
    {
        vipLayer->currVipLvl = lvl;
        vipLayer->_need = max-curr;
        vipLayer->_curr = curr;
        vipLayer->_max = max;
        vipLayer->vipGifts = CCArray::create();
        CCObject * obj = NULL;
        CCARRAY_FOREACH(_array, obj)
        {
            vipLayer->vipGifts->addObject(obj);
        }
        vipLayer->vipGifts->retain();
        vipLayer->initView();
        vipLayer->autorelease();
        return vipLayer;
    }
    CC_SAFE_DELETE(vipLayer);
    return NULL;
}
void SGVipGiftLayer::initView()
{
    
    SGMainManager::shareMain()->getMainLayer()->setPortraitMenuCanTouch(false);
    
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist",RES_TYPE_LAYER_UI ,sg_vipGiftLayer);
    ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist",RES_TYPE_LAYER_UI ,sg_vipGiftLayer);
    ResourceManager::sharedInstance()->bindTexture("sgbattlemainlayer/sgbattlemainlayer.plist",RES_TYPE_LAYER_UI ,sg_vipGiftLayer);
    
    SGNotificationCenter *notification = SGNotificationCenter::sharedNotificationCenter();
    notification->addObserver(MSG_VIPGIFT_BUY,
                              this,
                              callfuncO_selector(SGVipGiftLayer::giftBuyListener));
//    notification->addObserver(MSG_STORE_ENTER,
//                              this,
//                              callfuncO_selector(SGVipGiftLayer::storeEnterListener));
    tableViewHeight = skewing(196);
    giftArray = CCArray::create();
    giftArray->retain();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    CCSize s = CCDirector::sharedDirector()->getWinSize();

    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_vipGiftLayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    
    CCSprite* titlebg = CCSprite::createWithSpriteFrameName("title_bg.png");
    titlebg->setAnchorPoint(ccp(0.5, 1));
    titlebg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -titlebg->getContentSize().height/2-MSGTITLEHEIGHT)));//
    this->addChild(titlebg,-1);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(titlebg->getPosition(), ccp(0,  - titlebg->getContentSize().height -10)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(titlebg->getPosition(), ccp(-s.width/2,  - titlebg->getContentSize().height -10)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(titlebg->getPosition(), ccp(s.width/2,  - titlebg->getContentSize().height -10)));
    this->addChild(title_bg_r,10);
    
    CCRect r = CCRectMake(0, 30, bg->getContentSize().width, bg->getContentSize().height - titlebg->getContentSize().height * 1.4);
    bg->setTextureRect(r);
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    if (CCDirector::sharedDirector()->getWinSize().height != 1136)
    {
        bg->setScaleY(0.8);
    }
    
    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0.5, 0));
    bg->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0,btmhgt-50)));
    
    SGCCLabelTTF *title = SGCCLabelTTF::create(str_StoreLayer_str10, FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    this->addChild(title);
    title->setPosition(ccp(titlebg->getPosition().x,titlebg->getPosition().y-title->getContentSize().height/1.3));
    
	SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this,
                                                  menu_selector(SGVipGiftLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32);
	backBtn->setScale(1.05);
	
    this->addBtn(backBtn);
    backBtn->setPosition(ccp(backBtn->getContentSize().width*.65, title->getPosition().y/*-backBtn->getContentSize().height/8*/));
    
    CCSprite *redbg = CCSprite::createWithSpriteFrameName("store_redbg.png");
    redbg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -titlebg->getContentSize().height - titlebg->getContentSize().height/2- redbg->getContentSize().height/2-MSGTITLEHEIGHT)));
    redbg->setScaleX(s.width/redbg->getContentSize().width);
    this->addChild(redbg);
    
    CCSprite *font_vip = CCSprite::createWithSpriteFrameName("font_storevip.png");
    this->addChild(font_vip,1);
    font_vip->setPosition(ccpAdd(redbg->getPosition(), ccp(-s.width*.4, 0)));

    tableView->setFrame(CCRectMake(0, 0, s.width, s.height- btmhgt - titlebg->getContentSize().height - redbg->getContentSize().height - 10));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2,
                                                                       btmhgt -titlebg->getContentSize().height/2- 16)));
    tableViewHeight=400;
    tableView->iscreateall=1;
    CCSprite *expbg = CCSprite::createWithSpriteFrameName("store_jindutiao.png");
    this->addChild(expbg);
    expbg->setPosition(ccpAdd(redbg->getPosition(), ccp(10, 0)));
    expprog = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("store_jindutiao_li.png"));
    expprog->setType(kCCProgressTimerTypeBar);
    expprog->setMidpoint(ccp(0,0));
    expprog->setBarChangeRate(ccp(1, 0));
    expprog->setPosition(expbg->getPosition());
    this->addChild(expprog);
    
    SGButton *special = SGButton::create("anniu.png", NULL, this, menu_selector(SGVipGiftLayer::showVipInfo),CCPointZero,false,true);
    this->addBtn(special);
    special->setPosition(ccpAdd(expprog->getPosition(), ccp(253, 0)));
    
    //您当前是vip12 享受最尊崇的服务
    int curr = currVipLvl;
//    int need = _need;//SGStaticDataManager::shareStatic()->getVipById(currVipLvl+1)->getneedRMB();
    int next = currVipLvl+1;
    if (curr == 0) {
        label1 = SGCCLabelTTF::create(str_StoreLayer_str11, FONT_BOXINFO, 20);
        label1->setAnchorPoint(ccp(0, 0.5));
        this->addChild(label1);
        label1->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, 24)));
        
        CCString *str_ = CCString::createWithFormat(str_StoreLayer_str2,_need);
        label2 = CCLabelTTF::create(str_->getCString(), FONT_BOXINFO, 20);
        label2->setAnchorPoint(ccp(0, 0.5));
        label2->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, -26)));
        this->addChild(label2);
        label2->setColor(ccc3(0xe7, 0xc4, 0x6a));
//        int currLevelExperienceMax = SGStaticDataManager::shareStatic()->getVipById(next)->getneedRMB();//当前等级最大经验
        float rate = (float)(_curr)/_max;
        expprog->setPercentage(rate*100);
        
        label_exp = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",_curr,_max)->getCString(), FONT_BOXINFO, 20);
        this->addChild(label_exp);
        label_exp->setPosition(ccpAdd(redbg->getPosition(), ccp(redbg->getContentSize().width*.2, 24)));
    }else if(curr == 12)
    {
        label1 = SGCCLabelTTF::create(str_StoreLayer_str3, FONT_BOXINFO, 20);
        label1->setAnchorPoint(ccp(0, 0.5));
        label1->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, 24)));
        this->addChild(label1);
        
        fontvip = CCSprite::createWithSpriteFrameName("font_store_vip.png");
        this->addChild(fontvip);
        fontvip->setPosition(ccpAdd(label1->getPosition(), ccp(label1->getContentSize().width + fontvip->getContentSize().width*.5, 0)));
        
        currvip = CCLabelAtlas::create(CCString::createWithFormat("%d",curr)->getCString(), "sanguobigpic/store_shuzi_z.png", 18, 23, '0');
        currvip->setAnchorPoint(ccp(0, 0.5));
        currvip->setPosition(ccpAdd(fontvip->getPosition(), ccp(fontvip->getContentSize().width*.5, 0)));
        this->addChild(currvip);
        
        label2 = CCLabelTTF::create(str_StoreLayer_str4, FONT_BOXINFO, FONTSIZE(12));
        this->addChild(label2);
        label2->setAnchorPoint(ccp(0, 0.5));
        label2->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, -26)));
        label2->setColor(ccc3(0xe7, 0xc4, 0x6a));
        
//        int currLevelExperienceMax = SGStaticDataManager::shareStatic()->getVipById(curr)->getneedRMB();//当前等级最大经验
        float rate = (float)(_curr)/_max;
        expprog->setPercentage(rate*100);
        
        label_exp = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",_curr,_max)->getCString(), FONT_BOXINFO, 20);
        this->addChild(label_exp);
        label_exp->setPosition(ccpAdd(redbg->getPosition(), ccp(redbg->getContentSize().width*.2, 24)));
        
        return;
    }
    else{
        
        label1 = SGCCLabelTTF::create(str_StoreLayer_str3, FONT_BOXINFO, 20);
        label1->setAnchorPoint(ccp(0, 0.5));
        label1->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, 24)));
        this->addChild(label1);
        
        fontvip = CCSprite::createWithSpriteFrameName("font_store_vip.png");
        this->addChild(fontvip);
        fontvip->setPosition(ccpAdd(label1->getPosition(), ccp(label1->getContentSize().width + fontvip->getContentSize().width*.5, 0)));
        
        currvip = CCLabelAtlas::create(CCString::createWithFormat("%d",curr)->getCString(), "sanguobigpic/store_shuzi_z.png", 18, 23, '0');
        currvip->setAnchorPoint(ccp(0, 0.5));
        currvip->setPosition(ccpAdd(fontvip->getPosition(), ccp(fontvip->getContentSize().width*.5-2, 0)));
        this->addChild(currvip);
        CCString *str_ = CCString::createWithFormat(str_StoreLayer_str5,_need,next);
        label2 = CCLabelTTF::create(str_->getCString(), FONT_BOXINFO, FONTSIZE(12));
        this->addChild(label2);
        label2->setAnchorPoint(ccp(0, 0.5));
        label2->setPosition(ccpAdd(redbg->getPosition(), ccp(-redbg->getContentSize().width*.285, -26)));
        label2->setColor(ccc3(0xe7, 0xc4, 0x6a));
        
        float rate = (float)(_curr)/_max;
        expprog->setPercentage(rate*100);
        
        label_exp = SGCCLabelTTF::create(CCString::createWithFormat("%d/%d",_curr,_max)->getCString(), FONT_BOXINFO, 20);
        this->addChild(label_exp);
        label_exp->setPosition(ccpAdd(redbg->getPosition(), ccp(redbg->getContentSize().width*.2, 24)));
        
    }
}

void SGVipGiftLayer::addGift(SGGift *gift)
{
    giftArray->addObject(gift);
}

SNSTableViewCellItem *SGVipGiftLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
    SGVipGiftItem *item = (SGVipGiftItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);

    SGVipGift *gift = (SGVipGift *)vipGifts->objectAtIndex(indexPath->getRow());
    if (NULL == item)
    {
        item = SGVipGiftItem::create(this,num,gift,currVipLvl,gift->getHaveBuy() == 0?false:true,_need);
    }
    else
    {
        item->updateInfo(gift,indexPath->getRow(),num,currVipLvl,gift->getHaveBuy() == 0?false:true,_need);
    }
    
    return item;
}
unsigned int  SGVipGiftLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
    int num = vipGifts->count();
    return num;
}
void SGVipGiftLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    
    EFFECT_PLAY(MUSIC_BTN);
//    int index = tableView->getItemPointerWithIndexPath(indexPath);
}

void SGVipGiftLayer::backHandler()
{
    //从储值页面进入
    if (false == SGMainManager::shareMain()->getFromShopLayer() )
    {
        main::ShopChargeEntryRequest *request = new main::ShopChargeEntryRequest;
        SGSocketClient::sharedSocketClient()->send(MSG_STORE_ENTER, request);
    }
    else
    {
        EFFECT_PLAY(MUSIC_BTN);
        SGMainManager::shareMain()->showShopLayer();
    }
}
void SGVipGiftLayer::giftBuyListener(cocos2d::CCObject *obj)
{
    SGPlayerInfo *player = SGPlayerInfo::sharePlayerInfo();
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    main::ShopVipGiftBagPurchResponse *response = (main::ShopVipGiftBagPurchResponse *)sr->m_msg;
    if (sr){
        if (response->state() == 0)
        {
            if (_buyGift)
            {                
                player->setPlayerGold(player->getPlayerGold() - _buyGift->getGiftDiscount());
                if (_buyGift->getGiftCoin()) {
                    player->setPlayerCoins(player->getPlayerCoins() + _buyGift->getGiftCoin());
                }
//                if (_buyGift->getGiftAward()) {
//                    player->setplayerAward(player->getplayerAward() + _buyGift->getGiftAward());
//                }
//                if (_buyGift->getGiftSpeak()) {
//                    player->setSpeakerCount(player->getSpeakerCount() + _buyGift->getGiftSpeak());
//                }
                if (_buyGift->getGiftLiandan()) {
                    player->setPlayerliandan(player->getPlayerliandan() + _buyGift->getGiftLiandan());
                }
                int len = response->officercards_size();
                for (int i = 0 ; i <len; i++) {
                    SGOfficerCard *officerCard = new SGOfficerCard();
                    main::OfficerCardItemProto proto = response->officercards(i);
                    CCDictionary *dicc = SGCardAttribute::getValue(proto.currlevel(), proto.itemid());
                    SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(proto.itemid());
                    int type = SGStaticDataManager::shareStatic()->getStrengOfficerType(proto.itemid());
                    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, proto.currlevel()+1);
                    
                    officerCard->setSsid(proto.ssid());
                    officerCard->setItemId(proto.itemid());
                    officerCard->setCurrExp(proto.currexp());
                    officerCard->setLordId(proto.lordskillid());
                    officerCard->setIsProt(proto.isprotection());
//                    officerCard->setIsExped(proto.isexpedition());
                    officerCard->setCurrLevel(proto.currlevel());
                    
                    if (temp->getOfficerNextId()) {
                        officerCard->setCost(temp->getOfficerCost());

                    }
                    officerCard->setExpRate(temp->getOfficerExpRate());
                    officerCard->setMaxStar(temp->getOfficerNextId());
                    officerCard->setMaxExp(expdata->getExpValue());
                    officerCard->setCurrStar(temp->getOfficerCurrStarLevel());
                    officerCard->setOfficerName(temp->getOfficerName());
                    officerCard->setMaxLevel(temp->getOfficerMaxLevel());
                    officerCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                    officerCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                    officerCard->setRound(temp->getOfficerRound());
                    officerCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                    officerCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                    officerCard->setGoverning(temp->getOfficerGoverning());
                    officerCard->setRace(temp->getOfficerRace());
                    officerCard->setItemType(temp->getOfficerType());
					
					officerCard->setHeadSuffixNum(temp->getIconId());//12.05
                    
                    if (strcmp(temp->getOfficerGeneralsSkill()->getCString(), "0") &&temp->getOfficerGeneralsSkill()->length()>3)
                    {
                        std::string generalskill(temp->getOfficerGeneralsSkill()->getCString(),3,temp->getOfficerGeneralsSkill()->length() - 3);
                        officerCard->setOfficerSkil(atoi(generalskill.c_str()));
                    }
                    if (temp->getOfficerLordSkill() &&temp->getOfficerLordSkill()->length()>3)
                    {
                        std::string lordskill(temp->getOfficerLordSkill()->getCString(),3,temp->getOfficerLordSkill()->length() - 3);
                        officerCard->setLordSkill(atoi(lordskill.c_str()));            
                    }
                    //转生等级
                    officerCard->setUpgradelevel(temp->getUpgradelevel());
                    //转生最大星级
                    officerCard->setUpgradestar(temp->getUpgradestar());
                    //武将计最大等级
                    officerCard->setSkillMaxLevel(temp->getSkillMaxLevel());
                    //主将计最大等级
                    officerCard->setLordMaxLevel(temp->getLordMaxLevel());
                    //性别和缘分
                    officerCard->setGender((OfficerGender) temp->getOfficerGender());
                    officerCard->setFateList(temp->getOfficerFateList());
                    //武将原型ID
                    officerCard->setProtoId(temp->getOfficerProtoId());
                    //速度修正系数
                    officerCard->setSpeedFactor(temp->getOfficerSpeedFactor());
                    //转生次数
                    officerCard->setAdNum(temp->getAdvNum());
                    //player->addOfficerCard(officerCard);
                    officerCard->autorelease();
                    
                }
                
                int equiplen = response->equipmentcards_size();
                for (int i = 0 ; i <equiplen; i++) {
                    SGEquipCard *equipCard = new SGEquipCard();
                    main::EquipmentCardItemProto cardInfo = response->equipmentcards(i);
                    CCDictionary *dicc = SGCardAttribute::getValue(cardInfo.currlevel(), cardInfo.itemid());
                    SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(cardInfo.itemid());
                    int type = SGStaticDataManager::shareStatic()->getStrengEquipType(cardInfo.itemid());
                    SGExpDataModel *expdata = SGStaticDataManager::shareStatic()->getExpByTypeAndId(type, cardInfo.currlevel()+1);
                    
                    equipCard->setSsid(cardInfo.ssid());
                    //装备原型ID
                    equipCard->setProtoId(temp->getEquipProtoId());
                    equipCard->setItemId(cardInfo.itemid());
                    //            equipCard->setSetNum(cardInfo.setnum());
                    //转生等级
                    equipCard->setUpgradelevel(temp->getUpgradelevel());
                    //转生次数
                    equipCard->setAdNum(temp->getAdvNum());
                    //转生最大星级
                    equipCard->setUpgradestar(temp->getUpgradestar());
                    equipCard->setCurrLevel(cardInfo.currlevel());
                    //equipCard->setCurrExp(cardInfo.currexp());
                    equipCard->setOfficerCardId(cardInfo.officercarditemid());
                    equipCard->setIsProt(cardInfo.isprotection());
                    if (cardInfo.officercarditemid() >0)
                    {
                        SGOfficerCard *officer = SGPlayerInfo::sharePlayerInfo()->getOfficerCard(cardInfo.officercarditemid());
                        officer->addEquip(equipCard);
                    }
                    
                    equipCard->setMorale(((CCString *)dicc->objectForKey("mor"))->intValue());
                    equipCard->setAtk(((CCString *)dicc->objectForKey("atk"))->intValue());
                    equipCard->setDef(((CCString *)dicc->objectForKey("def"))->intValue());
                    equipCard->setMaxExp(expdata->getExpValue());
                    equipCard->setRound(temp->getEquipRound());
                    equipCard->setSpeed(((CCString *)dicc->objectForKey("speed"))->floatValue());
                    equipCard->setExpRate(temp->getEquipExpRate());
                    equipCard->setCurrStar(temp->getEquipCurrStarLevel());
                    equipCard->setGoverning(temp->getEquipGoverning());
                    equipCard->setItemType(temp->getEquipType());
                    equipCard->setMaxStar(temp->getEquipNextId());
                    equipCard->setOfficerName(temp->getEquipName());
                    equipCard->setMaxLevel(temp->getEquipMaxLevel());
                    if (temp->getFateDesc())
                    {
                        equipCard->setFateDesc(temp->getFateDesc());
                    }					equipCard->setHeadSuffixNum(temp->getIconId());//12.06
					
                    if (temp->getEquipNextId()) {
                        equipCard->setCost(temp->getEquipCost());
                        
                    }

                    //player->addEquipCard(equipCard);
                    equipCard->autorelease();
                }
                int proplen = response->propscards_size();
                for (int i = 0 ; i <proplen; i++)
                {
                    SGPropsCard *propCard = new SGPropsCard();
                    main::PropsCardItemProto cardInfo = response->propscards(i);
                    SGPropsDataModel *prop = SGStaticDataManager::shareStatic()->getPropsById(cardInfo.itemid());
                    
                    propCard->setSsid(cardInfo.ssid());
                    propCard->setItemId(cardInfo.itemid());
                    propCard->setIsProt(cardInfo.isprotection());
                    
                    propCard->setSell(prop->getpropsSell());
                    propCard->setValue(prop->getpropsValue());
                    propCard->setRace(prop->getPropsCountry());
                    propCard->setOfficerName(CCString::create(prop->getPropsName()->getCString()));
                    propCard->setType(prop->getPropsPropsType());
                    propCard->setItemType(prop->getPropsType());
                    propCard->setCurrStar(prop->getPropsCurrStarLevel());
					propCard->setHeadSuffixNum(prop->getIconId());//12.05
    
                    //player->addPropCard(propCard);
                    propCard->autorelease();
                }              
   
                SGMainManager::shareMain()->updataUserMsg();
                _buyGift->setHaveBuy(1);
                int giftNum = vipGifts->count();
                int num = 0;
                for (int i=0; i<giftNum; i++) {
                    if ((SGVipGift *)vipGifts->objectAtIndex(i) == _buyGift) {
                        num = i;
                        break;
                    }
                }
                SGVipGiftItem *item = (SGVipGiftItem*)tableView->dequeueReusableCellItemForIndexPath(SNSIndexPath::create(num, 0, 1));
                if (item) {
                    item->updateInfo(_buyGift, num, num, currVipLvl, true, _need);
                }
                _buyGift = NULL;
            }
            SGMainManager::shareMain()->showMessage(str_ShouJingZhou_str12);
            CCLOG("购买成功");
        }else{
            SGMainManager::shareMain()->showMessage(str_StoreLayer_str12);
        }
    }
}

void SGVipGiftLayer::showInfoHandler(SGVipGiftItem *giftItem)
{
    EFFECT_PLAY(MUSIC_BTN);
    _buyGift = giftItem->getGift();
    SGGiftInfoBox *GiftInfoBox = SGGiftInfoBox::create(this, _buyGift);
    SGMainManager::shareMain()->showBox(GiftInfoBox);

}

void SGVipGiftLayer::buyHandler(SGVipGift *giftItem)
{
    _buyGift = giftItem;
    if (currVipLvl < _buyGift->getGiftVipLvl()) {
        EFFECT_PLAY(MUSIC_BTN);
       // CCLOG("%d   %d",currVipLvl,_buyGift->getGiftVipLvl() );
        SGCantAdvanceBox *cantexchangeBox = SGCantAdvanceBox::create(this, NULL,11, _buyGift->getGiftVipLvl());
        SGMainManager::shareMain()->showBox(cantexchangeBox);
    }
    else if (SGPlayerInfo::sharePlayerInfo()->getPlayerGold()< giftItem->getGiftDiscount())
    {
        EFFECT_PLAY(MUSIC_BTN);
        SGCantAdvanceBox *cantexchangeBox = SGCantAdvanceBox::create(this, NULL,10, _buyGift->getGiftDiscount());
        SGMainManager::shareMain()->showBox(cantexchangeBox);
    }
    else
    {
        EFFECT_PLAY(MUSIC_34);
        main::ShopVipGiftBagPurchRequest *request = new main::ShopVipGiftBagPurchRequest();
        request->set_ssid(giftItem->getGiftID());
        SGSocketClient::sharedSocketClient()->send(MSG_VIPGIFT_BUY, request);
    }

}
void SGVipGiftLayer::setIsCanTouch(bool isTouch)
{
    _touch = isTouch;
    SGBaseTableLayer::setIsCanTouch(isTouch);
    //tableView->reloadData(false);
    SGMainLayer *layer = (SGMainLayer*)(SGMainManager::shareMain()->getMainScene()->getChildByTag(sg_mainLayer));
    layer->setIsCanTouch(isTouch,isTouch);
}

SGVipGiftItem::SGVipGiftItem():vipTiao(NULL)
,store_k(NULL)
,_gift(NULL)
,menu1(NULL)
,_yuan(NULL)
,_te(NULL)
,delegate(NULL)
,vipLevel(NULL)
,itembg(NULL)
,vip1(NULL)
,_playerLev(0)
,_haveGot(false)
,infoPos(CCPointZero)
{
}
SGVipGiftItem::~SGVipGiftItem()
{
    CCLOG("~SGShopItem");
}
bool SGVipGiftItem::initWithSize(int num,SGVipGift *gift,int need)
{
    if (!SNSTableViewCellItem::init()) {
		return false;
	}

    CCSprite* itembgUp = CCSprite::createWithSpriteFrameName("banUp.png");
    itembgUp->setPosition(ccp(0,-30));
    this->addChild(itembgUp);
    bgbgbg = NULL;
    itembg = CCSprite::createWithSpriteFrameName("banDown1.png");
    itembg->setAnchorPoint(ccp(0.5, 1));
    this->addChild(itembg);
    itembg->setPosition(ccpAdd(itembgUp->getPosition(), ccp(0,-itembgUp->getContentSize().height/2)));
    
    cocos2d::extension::CCScale9Sprite *fontbg = cocos2d::extension::CCScale9Sprite::create("sanguobigpic/tipsinside.png");
    this->addChild(fontbg);
    
//#if (PLATFORM == IOS)
    fontbg->setPreferredSize(CCSizeMake(itembgUp->getContentSize().width*.7 , itembgUp->getContentSize().height*1.1));
    fontbg->setPosition(ccp(-itembgUp->getContentSize().width * 0.2 +34, -itembg->getContentSize().height/2 - 30));
//#else
//    fontbg->setPreferredSize(CCSizeMake(itembgUp->getContentSize().width*.63 , itembgUp->getContentSize().height*.92));
//    fontbg->setPosition(ccp(-itembgUp->getContentSize().width * 0.2 +14, -itembg->getContentSize().height/2 - 20));
//#endif
    
    store_k = SGButton::create("equipPinzhi_5_3.png", "mall_gift_pack.png", this, menu_selector(SGVipGiftItem::showGiftInfo),CCPointZero,false,true);
    store_k->setPosition(ccpAdd(ccp(0,itembgUp->getPosition().y), ccp(itembg->getContentSize().width *0.3,store_k->getContentSize().height/2 - 20)));
    this->addChild(store_k);
    store_k->setEnabled(false);
    //addBtn(store_k);
    
    CCSprite *cardBg = CCSprite::createWithSpriteFrameName("card_bj.png");
    cardBg->setPosition(ccp(store_k->getContentSize().width/2, store_k->getContentSize().height/2));
    store_k->addChild(cardBg,-1,-1);
    
    infoPos =ccpAdd(ccp(0,itembg->getPosition().y), ccp(-itembg->getContentSize().width/2 +20, itembgUp->getContentSize().height/2+store_k->getContentSize().height *0.75));
    
    
      setInfoLabel(num);
    
    vipTiao = CCSprite::createWithSpriteFrameName("vipKuang.png");
    vipTiao->setPosition(ccpAdd(infoPos, ccp(-18,42)));//60
    vipTiao->cocos2d::CCNode::setAnchorPoint(ccp(0,0.5));
    vipTiao->cocos2d::CCNode::ignoreAnchorPointForPosition(false);
    this->addChild(vipTiao);
    
    setVipBuyTag(num,need);
    
    vipLevel = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("bvip%d.png",num+1)->getCString());
    vipLevel->setPosition(ccpAdd(infoPos, ccp(0,60)));//80
    this->addChild(vipLevel);
    
    CCLabelTTF *yuanjia = CCLabelTTF::create(str_SystemMailLayer_str6, FONT_PANGWA, 20);
    yuanjia->setPosition(ccpAdd(store_k->getPosition(), ccp(-store_k->getContentSize().width/2+10,  - yuanjia->getContentSize().height - 50)));
    this->addChild(yuanjia,10);
    
    CCSprite *yb1 = CCSprite::createWithSpriteFrameName("store_yb.png");
    this->addChild(yb1,10);
    yb1->setPosition(ccpAdd(yuanjia->getPosition(), ccp(yuanjia->getContentSize().width + yb1->getContentSize().width/2 - 10, 0)));//-5
    
    CCSprite *tejia = CCSprite::createWithSpriteFrameName("font_tj.png");
    this->addChild(tejia,10);
    tejia->setScale(0.7);
    tejia->setPosition(ccpAdd(store_k->getPosition(), ccp(-store_k->getContentSize().width/2+10,  -tejia->getContentSize().height*2 -40)));
    
    CCSprite *yb2 = CCSprite::createWithSpriteFrameName("store_yb.png");
    this->addChild(yb2,10);
    yb2->setPosition(ccp(yb1->getPosition().x, tejia->getPosition().y));    
    
    CCSprite *font_bg = CCSprite::createWithSpriteFrameName("labelbg.png");
    font_bg->setScaleX(LABLESCAL);
    font_bg->setPosition(ccpAdd(yb1->getPosition(), ccp(font_bg->getContentSize().width/2 *LABLESCAL,0)));
    this->addChild(font_bg,8);
    
    font_bg = CCSprite::createWithSpriteFrameName("labelbg.png");
    font_bg->setScaleX(LABLESCAL);
    font_bg->setPosition(ccpAdd(yb2->getPosition(), ccp(font_bg->getContentSize().width/2 *LABLESCAL,0)));
    this->addChild(font_bg,8);
    
    
    SGButton *storeInfo = SGButton::createFromLocal("box_btn_newbg.png", str_vipgiftinfo, this, menu_selector(SGVipGiftItem::showGiftInfo),CCPointZero,FONT_PANGWA,ccWHITE,24);
    
    storeInfo->setPosition(ccpAdd(yb2->getPosition(), ccp(16,-yb2->getContentSize().height -storeInfo->getContentSize().height/2 )));
    this->addChild(storeInfo);
    addBtn(storeInfo);
    
    
    _yuan = SGCCLabelTTF::create(CCString::createWithFormat(" %d",_gift->getGiftCost())->getCString(), FONT_PANGWA, 20);
    _te = SGCCLabelTTF::create(CCString::createWithFormat(" %d",_gift->getGiftDiscount())->getCString(), FONT_PANGWA, 20 , ccYELLOW);
    this->addChild(_yuan,10);
    this->addChild(_te,10);
    _yuan->setAnchorPoint(ccp(0, 0.5));
    _te->setAnchorPoint(ccp(0, 0.5));
    _yuan->setPosition(ccpAdd(yb1->getPosition(), ccp(yb1->getContentSize().width/2, 0)));
    _te->setPosition(ccpAdd(yb2->getPosition(), ccp(yb2->getContentSize().width/2, 0)));
    //modify by:zyc. merge into create.
    //_te->setColor(ccYELLOW);
    
    createSubLine();
    return true;
}
SGVipGiftItem* SGVipGiftItem::create(SGBuyDelegate *delegate_, int num,SGVipGift *gift,int playerlvl,bool haveGot,int need)
{
	SGVipGiftItem *instance = new SGVipGiftItem();
    instance->_gift=gift;
    instance->delegate = delegate_;
    instance->_playerLev = playerlvl;
     instance->_haveGot = haveGot;
	if (instance && instance->initWithSize(num,gift, need)) {
		instance->autorelease();
		return instance;
	}
	CC_SAFE_DELETE(instance);
	return instance;
}
SGVipGift *SGVipGiftItem::getGift()
{
    return _gift;
}
void SGVipGiftItem::updateInfo(SGVipGift *gift, int index,int num,int playerlvl,bool haveGot,int need)
{
    ResourceManager::sharedInstance()->bindTexture("sgvipinfolayer/sgvipinfolayer.plist",RES_TYPE_LAYER_UI ,sg_vipGiftLayer);
    _gift = gift;
    _playerLev = playerlvl;
    
    _yuan->setString(CCString::createWithFormat("%d",gift->getGiftCost())->getCString());
    _te->setString(CCString::createWithFormat("%d",gift->getGiftDiscount())->getCString());
    
    _haveGot = haveGot;
    vipLevel->setDisplayFrame((CCSprite::createWithSpriteFrameName(CCString::createWithFormat("bvip%d.png",num+1)->getCString()))->displayFrame());
    
    createSubLine();
    
    if (num+1 == 12) {
        CCSprite *tempSprite = CCSprite::createWithSpriteFrameName("banDown2.png");
        itembg->setDisplayFrame(tempSprite->displayFrame());
    }
    else
    {
        CCSprite *tempSprite = CCSprite::createWithSpriteFrameName("banDown1.png");
        itembg->setDisplayFrame(tempSprite->displayFrame());
    }
    

    setVipBuyTag(num,need);
    setInfoLabel(num);
}
void SGVipGiftItem::createSubLine()
{
    if (_yuan == NULL) {
        return;
    }
    removeChildByTag(-300, true);

#if (PLATFORM == IOS)
    char *spritData = new char[(int)_yuan->getContentSize().width * 2 *4];
    memset(spritData,0,(int)_yuan->getContentSize().width * 2 *4);
    int spriteDataSize = (int)_yuan->getContentSize().width * 2*4;
    for (int j=0; j<spriteDataSize; j+=4) {
        spritData[j] = 255;
        spritData[j+1] = 0;
        spritData[j+2] = 0;
        spritData[j+3] = 255;
    }
    CCTexture2D *pTexture = new CCTexture2D();
    pTexture->initWithData(spritData, kCCTexture2DPixelFormat_Default, (int)_yuan->getContentSize().width, 2, CCSizeMake(_yuan->getContentSize().width, 2));
    delete [] spritData;
    pTexture->autorelease();
    
    CCSprite *redline = CCSprite::createWithTexture(pTexture);
    redline->setPosition(ccpAdd(_yuan->getPosition(), ccp(_yuan->getContentSize().width/2, 0)));

#else
    CCLayerColor *redline = CCLayerColor::create(ccc4(255, 0, 0, 255), _yuan->getContentSize().width, 3);
    redline->setPosition(ccpAdd(_yuan->getPosition(), ccp(_yuan->getContentSize().width/8, 0)));

#endif
    this->addChild(redline,10,-300);

    redline->setScaleX(1.1);
}

void SGVipGiftItem::showGiftInfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (_haveGot) {
        SGMainManager::shareMain()->showMessage(str_StoreLayer_str14);
    }
    else
    {
        if (delegate) {
            delegate->showInfoHandler(this);
        }
    }

}
void SGVipGiftItem::buyHandler()
{
    if (delegate) {
        delegate->buyHandler(this->getGift());
    }
}
void SGVipGiftItem::setVipBuyTag(int num,int need)
{
	ResourceManager::sharedInstance()->bindTexture("sgshoplayer/sgshoplayer.plist", RES_TYPE_LAYER_UI, sg_vipGiftLayer);
    if (num+1<=_playerLev)
    {
        if (vipTiao) {
            if (vipTiao->getChildByTag(10))
            {
                vipTiao->getChildByTag(10)->setVisible(true);
                CCSprite *vipLevelTitle = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("%ddc.png",num+1)->getCString());
                ((CCSprite *)vipTiao->getChildByTag(10))->setDisplayFrame(vipLevelTitle->displayFrame());
            }
            else
            {
                CCSprite *vipLevelTitle = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("%ddc.png",num+1)->getCString());
                vipLevelTitle->setPosition(ccp(vipTiao->getContentSize().width/2,vipTiao->getContentSize().height/2+10));
                vipTiao->addChild(vipLevelTitle,10,10);
            }
            if (vipTiao->getChildByTag(20)) {
                vipTiao->getChildByTag(20)->setVisible(false);
            }
        }
        
    }
    else
    {
        if (vipTiao) {
            int next= num+1 >12? 12 :num+1;
            CCString *tagMsg = NULL;
            if (num == _playerLev) {
                tagMsg = CCString::createWithFormat(str_StoreLayer_str5,need,next);
            }
            else
            {
                tagMsg = CCString::createWithFormat(str_StoreLayer_str15,next);
            }
            if (vipTiao->getChildByTag(20))
            {
                vipTiao->getChildByTag(20)->setVisible(true);
                ((SGCCLabelTTF *)vipTiao->getChildByTag(20))->setString(tagMsg->getCString());
            }
            else
            {
                SGCCLabelTTF *vipLevelTitle = SGCCLabelTTF::create(tagMsg->getCString(), FONT_BOXINFO, 35);
                vipLevelTitle->setPosition(ccp(vipTiao->getContentSize().width/2,vipTiao->getContentSize().height/2+10));
                vipTiao->addChild(vipLevelTitle,20,20);
            }
            
            if (vipTiao->getChildByTag(10)) {
                vipTiao->getChildByTag(10)->setVisible(false);
            }
        }
    }
    
    
    store_k->removeChildByTag(-100, true);
    if (_haveGot)
    {
        CCSprite *vipBuyTag = CCSprite::createWithSpriteFrameName("vipGuiftBuy.png");
        vipBuyTag->setPosition(ccp(store_k->getContentSize().width/2,store_k->getContentSize().height/2));
        vipBuyTag->setAnchorPoint(ccp(0.5,0.5));
        store_k->addChild(vipBuyTag,10,-100);
    }
    else
    {
        CCSprite *vipBuyTag = CCSprite::createWithSpriteFrameName("font_cz.png");
        vipBuyTag->setPosition(ccp(-2,store_k->getContentSize().height-10));
        vipBuyTag->setAnchorPoint(ccp(0,1));
        store_k->addChild(vipBuyTag,10,-100);
    }
}

#if (PLATFORM == IOS)

void SGVipGiftItem::setInfoLabel(int num)
{
    if (vip1)
    {
        int lastTag = vip1->getTag();
        for (int rightTag = lastTag; rightTag >= VIPRIGHTTAG; rightTag--)
        {
            if (getChildByTag(rightTag))
            {
                removeChildByTag(rightTag);
            }
        }
        vip1 = NULL;
    }
    
    SGVipDataModel *info = SGStaticDataManager::shareStatic()->getVipById(num +1);
    
    int rightCount = info->getVipCount();
    CCArray *rightList = info->getVipRightsList();
    CCPoint rightPos = infoPos;
    
    
    for (int i =0; i< rightCount; i++)
    {
       
        char strbuff[50];
        sprintf(strbuff,"%d.%s" ,i+1,((CCString*)rightList->objectAtIndex(i))->getCString());
        vip1 = SGRichLabel::getFomLabel(strbuff, CCSizeMake(VIPINFOSIZE * VIPINFONUM,60), FONT_BOXINFO, VIPINFOSIZE,ccYELLOW);
        this->addChild(vip1,VIPRIGHTTAG + i);
        vip1->setAnchorPoint(ccp(0, 1));
        vip1->setPosition(ccpAdd(rightPos, ccp(0, LABELHEIGHT)));
        rightPos = vip1->getPosition();
        
    }

}
#else
void SGVipGiftItem::setInfoLabel(int num)
{
    if (vip1)
    {
        int lastTag = vip1->getTag();
        for (int rightTag = lastTag; rightTag >= VIPRIGHTTAG; rightTag--)
        {
            if (getChildByTag(rightTag))
            {
                removeChildByTag(rightTag);
            }
        }
        vip1 = NULL;
    }
    
    SGVipDataModel *info = SGStaticDataManager::shareStatic()->getVipById(num +1);
    
    int rightCount = info->getVipCount();
    CCArray *rightList = info->getVipRightsList();
    CCPoint rightPos = infoPos;
    
    for (int i = 0; i< rightCount; i++)
    {
        vip1 = SGCCLabelTTF::create(CCString::createWithFormat("%d.%s",i+1,((CCString*)rightList->objectAtIndex(i))->getCString())->getCString(), FONT_BOXINFO, VIPINFOSIZE, CCSizeMake(VIPINFOSIZE * VIPINFONUM, 60), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
        this->addChild(vip1,VIPRIGHTTAG + i);
        vip1->setAnchorPoint(ccp(0, 1));
        vip1->setPosition(ccpAdd(rightPos, ccp(0, LABELHEIGHT)));
        rightPos = vip1->getPosition();
    }
}
#endif

float SGVipGiftItem::getSGLabelHeight(CCSprite *sgRichLabel)
{
    if (sgRichLabel == NULL) {
        return 0;
    }
    CCObject *child = NULL;
    float height = 0;
    int count = sgRichLabel->getChildrenCount();
    CCARRAY_FOREACH(sgRichLabel->getChildren(), child)
    {
        height += ((SGCCLabelTTF*)child)->getContentSize().height;
    }
    return count==1?(height/2):height;
}
///////////////////////////////////////////////////////////////////////
void SGVipGiftLayer::showVipInfo()
{
    EFFECT_PLAY(MUSIC_BTN);
    if (SGMainManager::shareMain()->getMainLayer())
    {
        SGMainManager::shareMain()->getMainLayer()->requestStoreLayer();
        SGMainManager::shareMain()->setFromShopLayer(false);
    }
}

