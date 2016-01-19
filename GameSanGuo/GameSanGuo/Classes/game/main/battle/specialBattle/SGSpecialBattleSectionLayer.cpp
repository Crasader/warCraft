//
//  SGSpecialBattleSectionLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-12-12.
//
//

#include "SGSpecialBattleSectionLayer.h"
#include "SGSpecialBattleLayer.h"
#include "SGSpecialBattleSecionCell.h"
#include "SpecialBattleModule.pb.h"
#include "SGBattleManager.h"


SGSpecialBattleSectionLayer::SGSpecialBattleSectionLayer()
:selectIndex(-1),
chapInfo(NULL),
residueTimesTTF(NULL),
points(NULL)
{
    
}

SGSpecialBattleSectionLayer::~SGSpecialBattleSectionLayer()
{
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_specialBattleSecionLayer);
    SGNotificationCenter *center = SGNotificationCenter::sharedNotificationCenter();
    center->removeObserver(this, MSG_SPECIAL_BATTLE_ENTRY);
    center->removeObserver(this, MSG_SPECIAL_BATTLE_BUY_COUNT);
    center->removeObserver(this, MSG_SPECIAL_BATTLE_BUY);
    
    CC_SAFE_DELETE(datas);
}

SGSpecialBattleSectionLayer *SGSpecialBattleSectionLayer::create(cocos2d::CCArray *sArray, SpecialBattleData *chapInfo)
{
    SGSpecialBattleSectionLayer *sLayer = new SGSpecialBattleSectionLayer();
    
    if (sLayer && sLayer->init(NULL, sg_specialBattleSecionLayer))
    {
        sLayer->autorelease();
        sLayer->chapInfo = new SpecialBattleData;
        sLayer->chapInfo->storyId = chapInfo->storyId;
        sLayer->chapInfo->isOpen = chapInfo->isOpen;
        sLayer->chapInfo->itemId = chapInfo->itemId;
        sLayer->chapInfo->itemType = chapInfo->itemType;
        sLayer->chapInfo->chapName = chapInfo->chapName;
        sLayer->chapInfo->chapOpenDesc = chapInfo->chapOpenDesc;
        sLayer->chapInfo->chapCanBuyTimes = chapInfo->chapCanBuyTimes;
        sLayer->chapInfo->chapClosedDesc = chapInfo->chapClosedDesc;
        sLayer->chapInfo->chapDesc = chapInfo->chapDesc;
        sLayer->chapInfo->bannerBgId = chapInfo->bannerBgId;
        sLayer->chapInfo->freePlayTimes = chapInfo->freePlayTimes;
        sLayer->chapInfo->maxPlayTimes = chapInfo->maxPlayTimes;
        sLayer->chapInfo->pos = chapInfo->pos;
        
        if (!sLayer->datas)
        {
            sLayer->datas = CCArray::create();
            sLayer->datas->retain();
        }
        sLayer->datas->initWithArray(sArray);
        
        sLayer->initView();

        return sLayer;
    }
    
    CC_SAFE_DELETE(sLayer);
    return NULL;
}


//购买次数确定消息
void SGSpecialBattleSectionLayer::buyCountEntryListener(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::SpecialBattleBuyCountEntryResponse *res = (main::SpecialBattleBuyCountEntryResponse *)sr->m_msg;
        if (res)
        {
            int buyPrice = res->cost();
            int canBuyCount = res->canbuycount();
            int maxBuyCount = res->maxbuycount();
            
            SGBuySpBattleTimes *buyBox = SGBuySpBattleTimes::create(NULL, buyPrice, canBuyCount, maxBuyCount);
            SGMainManager::shareMain()->showBox(buyBox);
        }
        
    }
}

//刷新显示购买次数
void SGSpecialBattleSectionLayer::freshBuyCount(SpecialBattleData *info)
{
    if (residueTimesTTF)
    {
        residueTimesTTF->setString(CCString::createWithFormat(str_SpecialBattleResidue, info->freePlayTimes, info->maxPlayTimes)->getCString());
    }
}


//购买消息
void SGSpecialBattleSectionLayer::buyCountListener(cocos2d::CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::SpecialBattleBuyCountResponse *bRes = (main::SpecialBattleBuyCountResponse *)sr->m_msg;
        if (bRes)
        {
            if (bRes->state() == 1)
            {
                SG_SHOW_WINDOW(str_CantAdvance_tips132);
                int storyId = SGMainManager::shareMain()->getSpBattleChapId();
                main::SpecialBattlePlotListRequest *plotDetail = new main::SpecialBattlePlotListRequest;
                plotDetail->set_storyid(storyId);
                
                SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_SEC, plotDetail);
            }
            else if (bRes->state() == -1)//可購買次數不足
            {
                SG_SHOW_WINDOW(str_SpecialBattleNotCanBuy);
            }
            else if (bRes->state() == -2)//元寶不夠
            {
                SG_SHOW_WINDOW(str_CantAdvance_tips46);
            }
            else
            {
                SG_SHOW_WINDOW(str_BossBattleDetailLayer_str15);
            }
        }
    }
}


void SGSpecialBattleSectionLayer::initView()
{
    ResourceManager::sharedInstance()->bindTexture("sgvisitlayer/sgvisitlayer2.plist",RES_TYPE_LAYER_UI ,sg_specialBattleSecionLayer);
    ResourceManager::sharedInstance()->bindTexture("sgspecialbattlelayer/sgspecialbattlelayer.plist", RES_TYPE_LAYER_UI, sg_specialBattleSecionLayer);

    SGNotificationCenter *nCenter = SGNotificationCenter::sharedNotificationCenter();
    
    nCenter->addObserver(MSG_SPECIAL_BATTLE_ENTRY, this,
                          callfuncO_selector(SGSpecialBattleSectionLayer::specialBattlePlotCheckValid));
    nCenter->addObserver(MSG_SPECIAL_BATTLE_BUY_COUNT, this,
                         callfuncO_selector(SGSpecialBattleSectionLayer::buyCountEntryListener));
    nCenter->addObserver(MSG_SPECIAL_BATTLE_BUY, this,
                         callfuncO_selector(SGSpecialBattleSectionLayer::buyCountListener));
    
    CCSprite *heitiao = CCSprite::createWithSpriteFrameName("visit_heitiao.png");
    if (!heitiao)
    {
        return;
    }
    this->addChild(heitiao);
    heitiao->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter),ccp(0,-73)));
    
    //显示个人积分， 元宝，铜钱
    CCSprite *jewels = CCSprite::createWithSpriteFrameName("gouyuIcon.png");
    this->addChild(jewels, 1);
    jewels->setPosition(ccpAdd(heitiao->getPosition(), ccp(-CCDirector::sharedDirector()->getWinSize().width / 2 + 60, 0)));
    
    SGCCLabelTTF *jewelsCount = SGCCLabelTTF::create(CCString::createWithFormat("%d", SGPlayerInfo::sharePlayerInfo()->getPlayerJewels())->getCString(), FONT_BOXINFO, 23);
    this->addChild(jewelsCount);
    jewelsCount->setPosition(ccpAdd(jewels->getPosition(), ccp(jewels->getContentSize().width / 2 + jewelsCount->getContentSize().width, 0)));
    
    //coin 可能还用到 只是隐藏
    CCSprite *coin = CCSprite::createWithSpriteFrameName("coinicon.png");
    this->addChild(coin, 1);
    coin->setPosition(ccpAdd(jewelsCount->getPosition(), ccp(170, 0)));
    SGCCLabelTTF *coinCount = SGCCLabelTTF::create(CCString::createWithFormat("%d", SGPlayerInfo::sharePlayerInfo()->getPlayerCoins())->getCString(), FONT_BOXINFO, 23);
    this->addChild(coinCount, 1);
    coinCount->setPosition(ccpAdd(coin->getPosition(), ccp(coin->getContentSize().width +  coinCount->getContentSize().width / 2, 0)));
    coin->setVisible(false);
    coinCount->setVisible(false);
    
    //yb
    CCSprite *yb = CCSprite::createWithSpriteFrameName("goldicon.png");
    this->addChild(yb, 1);
    yb->setPosition(ccpAdd(coinCount->getPosition(), ccp(180, 0)));
    SGCCLabelTTF *ybCount = SGCCLabelTTF::create(CCString::createWithFormat("%d", SGPlayerInfo::sharePlayerInfo()->getPlayerGold())->getCString(), FONT_BOXINFO, 23);
    this->addChild(ybCount, 1);
    ybCount->setPosition(ccpAdd(yb->getPosition(), ccp(yb->getContentSize().width / 2 +  ybCount->getContentSize().width / 2, 0)));
    

    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(heitiao->getPosition(), ccp(0,  heitiao->getContentSize().height / 2 - 5)));
    this->addChild(titlecenter,10);
    titlecenter->setScaleX(CCDirector::sharedDirector()->getWinSize().width / titlecenter->getContentSize().width);
    titlecenter->setScaleY(0.7);
    
    
    CCSprite *gonggao_bg_3_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    gonggao_bg_3_l->setAnchorPoint(ccp(0, 0));
    gonggao_bg_3_l->setFlipY(true);
    gonggao_bg_3_l->setPosition(ccpAdd(heitiao->getPosition(), ccp(-heitiao->getContentSize().width / 2, -heitiao->getContentSize().height / 1.18)));
    this->addChild(gonggao_bg_3_l,13);
    
    CCSprite *titlecenter1 = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter1->setAnchorPoint(ccp(0.5, 0));
    titlecenter1->setPosition(ccpAdd(gonggao_bg_3_l->getPosition(), ccp(gonggao_bg_3_l->getContentSize().width + titlecenter->getContentSize().width / 2,7)));
    this->addChild(titlecenter1,11);
    titlecenter1->setScaleX(4);
    
    CCSprite *gonggao_bg_3_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    gonggao_bg_3_r->setFlipX(true);
    gonggao_bg_3_r->setFlipY(true);
    gonggao_bg_3_r->setAnchorPoint(ccp(1, 0));
    gonggao_bg_3_r->setPosition(ccpAdd(heitiao->getPosition(), ccp(heitiao->getContentSize().width / 2, -heitiao->getContentSize().height / 1.18)));
    this->addChild(gonggao_bg_3_r,13);
    
    
    //下半部分的花边
    CCSprite *gonggao_bg_3_bl = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    gonggao_bg_3_bl->setAnchorPoint(ccp(0, 0));
    gonggao_bg_3_bl->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft),ccp(0,150)));
    this->addChild(gonggao_bg_3_bl,13);
    
    CCSprite *titleBottomcenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titleBottomcenter->setAnchorPoint(ccp(0.5, 0));
    titleBottomcenter->setPosition(ccpAdd(gonggao_bg_3_bl->getPosition(), ccp(gonggao_bg_3_bl->getContentSize().width + titleBottomcenter->getContentSize().width / 2,0)));
    this->addChild(titleBottomcenter,11);
    titleBottomcenter->setScaleX(4);
    
    CCSprite *gonggao_bg_3_br = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    gonggao_bg_3_br->setFlipX(true);
    gonggao_bg_3_br->setAnchorPoint(ccp(1, 0));
    gonggao_bg_3_br->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(0,150)));
    this->addChild(gonggao_bg_3_br,13);

    
    //banner
    CCSprite *bannerBg = CCSprite::createWithSpriteFrameName("sp_battle_title_bar.png");
    bannerBg->setScaleX(CCDirector::sharedDirector()->getWinSize().width / bannerBg->getContentSize().width);
    this->addChild(bannerBg);
    bannerBg->setPosition(ccpAdd(heitiao->getPosition(), ccp(0, -heitiao->getContentSize().height / 2 - bannerBg->getContentSize().height / 2)));
    
    //banner前景
    
    int bannerBgId = chapInfo->bannerBgId;
    SGMainManager::shareMain()->addSpTopHeadBgPlistByNum(bannerBgId, sg_specialBattleSecionLayer);
    
    CCSprite *preBanner = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sptophead%d.png", bannerBgId)->getCString());
    //preBanner->setScaleX(CCDirector::sharedDirector()->getWinSize().width / bannerBg->getContentSize().width);

    bannerBg->addChild(preBanner, 1);
    preBanner->setPosition(ccpAdd(ccp(bannerBg->getContentSize().width / 2, bannerBg->getContentSize().height /2), ccp(0, 0)));
    
    
    //章名称背景
    CCSprite *chapNameBg = CCSprite::createWithSpriteFrameName("sp_battle_namebg.png");
    this->addChild(chapNameBg, 2);
    chapNameBg->setPosition(ccpAdd(bannerBg->getPosition(), ccp(-bannerBg->getContentSize().width / 2 + chapNameBg->getContentSize().width / 2,
                                                                bannerBg->getContentSize().height / 5.6)));
    
    //章名称 254	 209	11 //108	0	11
    SGCCLabelTTF *chapName = SGCCLabelTTF::create(chapInfo->chapName.c_str(), FONT_PANGWA, 30);
    this->addChild(chapName, 3);
    chapName->setInsideColor(ccc3(254, 209, 11));
    chapName->setOutSideColor(ccc3(108, 0, 11));
    chapName->setPosition(ccpAdd(chapNameBg->getPosition(), ccp(0, 0)));
    
    
    //章描述
//    CCSize desSize = CCSizeMake(bannerBg->getContentSize().width / 4,bannerBg->getContentSize().height / 2)
    CCSize desSize = CCSize(180, 60);
    SGCCLabelTTF *chapDesc = SGCCLabelTTF::create(chapInfo->chapDesc.c_str(), FONT_PANGWA, 18,desSize, kCCTextAlignmentCenter);
    chapDesc->setInsideColor(ccGREEN);
    this->addChild(chapDesc);
    chapDesc->setPosition(ccpAdd(chapNameBg->getPosition(), ccp(0, -chapNameBg->getContentSize().height / 2 - chapDesc->getContentSize().height / 2.5)));
    
    
    //剩余次数
    residueTimesTTF = SGCCLabelTTF::create(CCString::createWithFormat(str_SpecialBattleResidue, chapInfo->freePlayTimes, chapInfo->maxPlayTimes)->getCString(),
                                           FONT_PANGWA, 21);
    residueTimesTTF->setAnchorPoint(ccp(0, 0.5));
    this->addChild(residueTimesTTF);
    residueTimesTTF->setPosition(ccpAdd(chapName->getPosition(), ccp(bannerBg->getContentSize().width / 2.2, 20)));
    
    //返回按钮
    SGButton *backBtn = SGButton::create("sp_battle_return.png", NULL, this, menu_selector(SGSpecialBattleSectionLayer::backHandler));
    this->addBtn(backBtn);
    backBtn->setPosition(ccpAdd(bannerBg->getPosition(), ccp(bannerBg->getContentSize().width / 2 - backBtn->getContentSize().width / 2 - 10,
                                                             -bannerBg->getContentSize().height / 8)));
    
    //购买按钮
    SGButton *buyButton = SGButton::createFromLocal("btn_fenbg.png", str_SpecialBattleBuyTips, this,
                                                    menu_selector(SGSpecialBattleSectionLayer::buyPlayTimes), CCPointZero, FONT_PANGWA, ccWHITE, 20);
    this->addBtn(buyButton);
    buyButton->setPosition(ccpAdd(backBtn->getPosition(), ccp(-backBtn->getContentSize().width / 2 - buyButton->getContentSize().width,
                                                              -buyButton->getContentSize().height / 3)));
    
    buyButton->setVisible(false);
    
    tableViewHeight = 200;
    tableView->setDown(-10);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    float headhgt = SGMainManager::shareMain()->getVisitHgt();
    float btmhgt = SGMainManager::shareMain()->getBottomHeight();
    
    tableView->setFrame(CCRectMake(0, 0, winSize.width, winSize.height - btmhgt*.68 -heitiao->getContentSize().height - bannerBg->getContentSize().height - headhgt +20));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp( - winSize.width/2, btmhgt)));
    
    //记录本章id
    //SGMainManager::shareMain()->setSpBattleChapId(chapInfo->storyId);
}

//进入战斗消息处理
void SGSpecialBattleSectionLayer::specialBattlePlotCheckValid(CCObject *obj)
{
    SGSocketRequest *sr = (SGSocketRequest *)obj;
    
    if (sr)
    {
        main::SpecialBattleCheckFightResponse *response = (main::SpecialBattleCheckFightResponse *)sr->m_msg;
        if (response)
        {
            if (response->state() == 1)//成功
            {
                //直接进入战斗逻辑开始
                SGBattleManager::sharedBattle()->pveStart(SGMainManager::shareMain()->getSpBattleChapId(), SGMainManager::shareMain()->getSpBattlePlotId(), -1);
            }
            else if (response->state() == -2)//阵容不对，不能参加
            {
                SG_SHOW_WINDOW(str_SpecialBattleNoneCondition);
            }
            else if (response->state() == -1)//次数不足
            {
                SG_SHOW_WINDOW(str_SpecialBattleNoneCondition1);
            }
            else//默认显示阵容不符合
            {
                SG_SHOW_WINDOW(str_SpecialBattleNoneCondition);
            }
        }
    }
}

void SGSpecialBattleSectionLayer::backHandler(CCObject *obj)
{
    CCLOG("return last");
    main::SpecialBattleStoryListRequest *req = new main::SpecialBattleStoryListRequest;
    SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_CHAP, req);
}

void SGSpecialBattleSectionLayer::buyPlayTimes(CCObject *obj)
{
    CCLOG("Buy Play Times");
    
    if (chapInfo->freePlayTimes)
    {
        SG_SHOW_WINDOW(str_SpecialBattleCanUseCount);
        return ;
    }
    
    //弹窗购买
    main::SpecialBattleBuyCountEntryRequest *buyEntryReq = new main::SpecialBattleBuyCountEntryRequest;
    buyEntryReq->set_storyid(SGMainManager::shareMain()->getSpBattleChapId());
    SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_BUY_COUNT, buyEntryReq);
}


SNSTableViewCellItem *SGSpecialBattleSectionLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	int index = tableView->getItemPointerWithIndexPath(indexPath);
    
    SpecialBattleSecData *temp = (SpecialBattleSecData *)(datas->objectAtIndex(index));
    SGSpecialBattleSecionCell *sectCell = (SGSpecialBattleSecionCell*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
    
    if (!sectCell)
    {
        sectCell = SGSpecialBattleSecionCell::create(temp);
    }
    else if (index < datas->count())
    {
        sectCell->updateSpCell(temp);
    }

    return sectCell;
}

unsigned int SGSpecialBattleSectionLayer::tableViewAllItemsNumber(SNSTableView* tableView, int section)
{
	return datas->count();
}
void SGSpecialBattleSectionLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	selectIndex = tableView->getItemPointerWithIndexPath(indexPath);
}








SGBuySpBattleTimes::SGBuySpBattleTimes():
canBuyTimes(0),
price(0),
maxBuyTimes(0)
{
	
}
SGBuySpBattleTimes::~SGBuySpBattleTimes()
{
	
}

SGBuySpBattleTimes *SGBuySpBattleTimes::create(SGBoxDelegate *del, float price, int canBuyTimes, int maxBuyTimes)
{
	SGBuySpBattleTimes *cbb = new SGBuySpBattleTimes();
	
	if (cbb->init(del, box_buySpBattleTimes, CCSizeMake(535, 324)))
	{
        cbb->price = price;
        cbb->canBuyTimes = canBuyTimes;
        cbb->maxBuyTimes = maxBuyTimes;
		cbb->autorelease();
		cbb->initView();
		return cbb;
	}
	CC_SAFE_DELETE(cbb);
	return NULL;
}

void SGBuySpBattleTimes::initView()
{
    SGCCLabelTTF *tips;
    CCSize tipLabelSize = CCSizeMake(32*13, 220);

    tips = SGCCLabelTTF::create(CCString::createWithFormat(str_SpecialBattleBuyCount,
                                                           canBuyTimes, maxBuyTimes, (int)price)->getCString(), FONT_PANGWA, 28, tipLabelSize);
    tips->setAnchorPoint(ccp(.5, 1));
    tips->setPosition(ccpAdd(biao->getPosition(), ccp(0, -50)));
	
	this->addChild(tips);
        
    SGButton *okButton = SGButton::create("box_btn_newbg.png", "public_font_queding.png", this,
                                          menu_selector(SGBuySpBattleTimes::pressOKButton), CCPointZero, false, true);
    SGButton *cancelButton = SGButton::create("box_btn_newbg.png", "public_font_quxiao.png", this,
                                              menu_selector(SGBuySpBattleTimes::pressCancelButton), CCPointZero, false, true);
    
    this->addBtn(okButton);
    this->addBtn(cancelButton);
    CCPoint center = SGLayout::getPoint(kMiddleCenter);
    okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+40)));
    cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+40)));

    
}
void SGBuySpBattleTimes::boxClose()
{
	SGBaseBox::boxClose();
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
    SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
    SGBaseLayer *nowLayer = (SGBaseLayer *)mainScene->getChildByTag(mainScene->nowLayerTag);
    if (mainLayer) {
        mainLayer->setIsCanTouch(true,true);
    }
    if (nowLayer) {
        nowLayer->setIsCanTouch(true);
    }
    
}

void SGBuySpBattleTimes::pressOKButton()
{
	CCLOG("Press OK Button");
    if (canBuyTimes == 0)
    {
        SG_SHOW_WINDOW(str_SpecialBattleNotCanBuy);
        return ;
    }
    
    main::SpecialBattleBuyCountRequest *buyReq = new main::SpecialBattleBuyCountRequest;
    buyReq->set_storyid(SGMainManager::shareMain()->getSpBattleChapId());
    SGSocketClient::sharedSocketClient()->send(MSG_SPECIAL_BATTLE_BUY, buyReq);
    this->pressCancelButton();
}

void SGBuySpBattleTimes::pressCancelButton()
{
	CCLOG("Press Cancel Button");
	this->boxClose();
	
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
	SGMainManager::shareMain()->closeBox();
}


void SGBuySpBattleTimes::closebox()
{
    this->boxClose();
    SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
	SGMainManager::shareMain()->closeBox();
}










