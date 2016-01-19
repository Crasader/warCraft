//
//  SGCardBatchSellLayer.cpp
//  GameSanGuo
//
//  Created by bugcode on 14-2-11.
//
//

#include "SGCardBatchSellLayer.h"
#include "ResourceManager.h"
#include "SGMainManager.h"
#include "SGPlayerInfo.h"
#include "SGCardItem.h"
#include "SGSortBox.h"
#include "SGStaticDataManager.h"
#include "SGSkillInfoBox.h"
#include "SGCantAdvanceBox.h"
#include "SimpleAudioEngine.h"
#include "SGFirstStrengBox.h"
#include "SGGuideManager.h"
#include "SGMainScene.h"
#include "SGBaseLayer.h"
#include"SGFirstAdvanceBox.h"
#include "SGFirstLayer.h"
#include "ResourceManager.h"
#include "SGGeneralInfoLayer.h"
#include "SGStringConfig.h"

#define TITLEBGWID  560
#define TITLEBGHT   180
#define STARBTGAP   145
#define TITLEXIAGAP -8
#define TITLESHANGAP 6
#define TITLEYOUGAP  10
#define TITLEZUOGAP  -6
#define TITLEROWSCAL  10.6
#define TITLELISTSCAL 1.0

#define SORTTYPE  (3)
//卡牌可卖出多少铜钱
int computeSellMoney(SGBaseMilitaryCard *_card)
{
	int sellCoin = 0;
	int base = 0;
	switch (_card->getCurrStar()) {
        case 1:
            base = 50;
            break;
        case 2:
            base = 250;
            break;
        case 3:
            base = 500;
            break;
        case 4:
            base = 750;
            break;
        case 5:
            base = 1500;
            break;
        case 6:
            base = 2500;
            break;
            
        default:
            break;
    }
	sellCoin = base * ((_card->getCurrLevel() - 1) *.1 + 1);
    
    CCString *str = CCString::create(_card->getClassName());
    
    if (str->isEqual(CCString::create("SGEquipCard")))
    {
        sellCoin = ((SGEquipCard*)_card)->getSellPrice();
    }
	return sellCoin;
}

SGCardBatchSellLayer::SGCardBatchSellLayer()
:enterType(0),
currPage(1000),
curRow(0),
selectNum(0),
totalMoney(0),
countChange(NULL),
isPressAutoSell(false)
,batch2StarShow(NULL)
,batch3StarShow(NULL)
,batch4StarShow(NULL)

, range(NULL)
, pageLabel(NULL)
, selCardCount(NULL)
, nextLine1(NULL)
, curCount(NULL)
, afterSoldCount(NULL)
, coinsCount(NULL)
, batchSellButton(NULL)
, isSelectArray(NULL)
{
	_totalPage = 0;
    _nowPage = 0;
    _allData = NULL;
	for (int i = 0; i < 300; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			selectItem[i][j] = 0;
		}
	}
}
SGCardBatchSellLayer::~SGCardBatchSellLayer()
{
    CC_SAFE_RELEASE(datas);
    CC_SAFE_RELEASE(_allData);
    CC_SAFE_RELEASE(isSelectArray);


	ResourceManager::sharedInstance()->unBindLayerTexture(sg_batchselllayer);
}

SGCardBatchSellLayer *SGCardBatchSellLayer::create(int enterType, int curPage, int curRow)
{
	SGCardBatchSellLayer *batchSellLayer = new SGCardBatchSellLayer();
	batchSellLayer->enterType = enterType;
	batchSellLayer->currPage = curPage;
	batchSellLayer->curRow = curRow;
	
	if (batchSellLayer && batchSellLayer->init(NULL, sg_batchselllayer))
	{
		batchSellLayer->initView();
		batchSellLayer->autorelease();
		return batchSellLayer;
	}
	CC_SAFE_DELETE(batchSellLayer);
    return NULL;
	
}

//初始化视图
void SGCardBatchSellLayer::initView()
{
	ResourceManager::sharedInstance()->bindTexture("sggeneralslayer/sggeneralslayer.plist", RES_TYPE_LAYER_UI, sg_batchselllayer);
    ResourceManager::sharedInstance()->bindTexture("sgcardslayer/sgcardslayer.plist", RES_TYPE_LAYER_UI, sg_batchselllayer);
	ResourceManager::sharedInstance()->bindTexture("sgmainlayer/sgmainlayer.plist",RES_TYPE_LAYER_UI ,sg_batchselllayer, LIM_PNG_AS_PNG);
    
	datas = CCArray::create();
	datas->retain();

	isSelectArray = CCArray::create();
	isSelectArray->retain();
	
	this->initMsg();//初始化各卡牌数据
	
	
	float h = SGMainManager::shareMain()->getHeadHeight();
    float b = SGMainManager::shareMain()->getBottomHeight();
    CCSize s= CCDirector::sharedDirector()->getWinSize();
	
	// 初始化数据
//	std::string str;
//	if (enterType == OFFICER)
//		str = CCUserDefault::sharedUserDefault()->getStringForKey("officersort");
//	else if (enterType == EQUIP)
//		str = CCUserDefault::sharedUserDefault()->getStringForKey("equipsort");
//	
//    CCString *type = CCString::create(str);
    SGSortBox::sortCards(_allData,SORTTYPE,1,false);//写死按星级低
    datas->removeAllObjects();
    int last = _allData->count();
    last > 8 && (last = 8);
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex(ii);
        datas->addObject(obj);
    }
	
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

	
	CCSprite *titleBg = CCSprite::createWithSpriteFrameName("title_bg.png");
    titleBg->setAnchorPoint(ccp(0.5, 1));
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, -45 - gonggao_3_2->getContentSize().height)));
    this->addChild(titleBg);
    
    CCSprite *titlecenter = CCSprite::createWithSpriteFrameName("title_bg_2.png");
    titlecenter->setAnchorPoint(ccp(0.5, 0));
    titlecenter->setPosition(ccpAdd(titleBg->getPosition(), ccp(0,  - titleBg->getContentSize().height -6)));
    this->addChild(titlecenter,10);
	titlecenter->setScaleX(4);
    
    CCSprite *title_bg_l = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_l->setAnchorPoint(ccp(0, 0));
    title_bg_l->setPosition(ccpAdd(titleBg->getPosition(), ccp(-s.width/2,  - titleBg->getContentSize().height -6)));
    this->addChild(title_bg_l,10);
    
    CCSprite *title_bg_r = CCSprite::createWithSpriteFrameName("title_bg_LR.png");
    title_bg_r->setFlipX(true);
    title_bg_r->setAnchorPoint(ccp(1, 0));
    title_bg_r->setPosition(ccpAdd(titleBg->getPosition(), ccp(s.width/2,  - titleBg->getContentSize().height -6)));
    this->addChild(title_bg_r,10);
	
	//    CCSprite *title = CCSprite::createWithSpriteFrameName("visit_title.png");
	std::string title_str = str_CardBatchSellLayer_str1;
	if (enterType == 6)
	{
		title_str = str_CardBatchSellLayer_str1;
	}
	else if (enterType == 7)//装备
	{
		title_str = str_CardBatchSellLayer_str2;
	}
	else if (enterType == 9)//技能
	{
		title_str = str_CardBatchSellLayer_str3;
	}
    SGCCLabelTTF *title = SGCCLabelTTF::create(title_str.c_str(), FONT_XINGKAI, 36 , COLOR_UNKNOW_TAN);
    //modify by:zyc. merge into create.
	//title->setColor(ccc3(0xff, 0x95, 0x0c));
    title->setPosition(ccpAdd(titleBg->getPosition(), ccp(0, -titleBg->getContentSize().height*.5)));
    this->addChild(title);
    
 
    titleBg->setPosition(ccpAdd(SGLayout::getPoint(kUpCenter), ccp(0, - h)));
    
    SGButton *backBtn = SGButton::createFromLocal("store_exchangebtnbg.png", str_back, this, menu_selector(SGCardBatchSellLayer::backHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    backBtn->setScale(1.01);
	this->addBtn(backBtn);
    backBtn->setAnchorPoint(ccp(0, 0.5));
    backBtn->setPosition(ccp(0, titleBg->getPosition().y-titleBg->getContentSize().height/2));
	
	range = SGButton::createFromLocal("store_exchangebtnbg.png", str_sort, this, menu_selector(SGCardBatchSellLayer::sortHandler),CCPointZero,FONT_PANGWA,ccWHITE,32,false,true);
    this->addBtn(range);
    range->setAnchorPoint(ccp(1, 1));
    range->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight),ccp(0, titleBg->getPosition().y - 10)));
	
    range->setFont(str_SortBox_str5);
    
	//标题横条
    char *spritData = new char[TITLEBGWID * TITLEBGHT *4];
    memset(spritData,0,TITLEBGWID * TITLEBGHT *4);
    int spriteDataSize = TITLEBGWID * TITLEBGHT*4;
    for (int j=0; j<spriteDataSize; j+=4) {
        spritData[j] = 0;
        spritData[j+1] = 0;
        spritData[j+2] = 0;
        spritData[j+3] = 255;
    }
    int  nOffset = 27;
    int nOffset2 = 56;
    CCTexture2D *pTexture = new CCTexture2D();
    pTexture->initWithData(spritData, kCCTexture2DPixelFormat_Default, TITLEBGWID, TITLEBGHT-nOffset2, CCSizeMake(TITLEBGWID, TITLEBGHT-nOffset2));

    CCSprite *batchSellTitle = CCSprite::createWithTexture(pTexture);
    batchSellTitle->setOpacity(255 *0.8);
    batchSellTitle->setPosition(ccpAdd(titleBg->getPosition(), ccp(0, 28-TITLEBGHT )));
    this->addChild(batchSellTitle);
    
    delete []spritData;
    pTexture->release();
    
    //标题横条边框
    CCSprite *zuoshangjiao = CCSprite::createWithSpriteFrameName("zuoshangjiao.png");
    CCSprite *shangbian = CCSprite::createWithSpriteFrameName("shangbian.png");
    CCSprite *youshangjiao = CCSprite::createWithSpriteFrameName("youshangjiao.png");
    CCSprite *zuobian = CCSprite::createWithSpriteFrameName("zuobian.png");
    CCSprite *youbian = CCSprite::createWithSpriteFrameName("youbian.png");
    CCSprite *zuoxiajiao = CCSprite::createWithSpriteFrameName("zuoxiajiao.png");
    CCSprite *xiabian = CCSprite::createWithSpriteFrameName("xia-bian.png");
    CCSprite *youxiajiao = CCSprite::createWithSpriteFrameName("youxiajiao.png");
    int nOffset3 = 26;
    nOffset = nOffset-nOffset3;
    nOffset2 = nOffset2-nOffset3;
    
    
    zuoshangjiao->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(zuoshangjiao->getContentSize().width/2 - TITLEBGWID/2  + TITLEZUOGAP, TITLEBGHT/2 - zuoshangjiao->getContentSize().height/2 + TITLESHANGAP-nOffset3)));
    zuobian->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(zuobian->getContentSize().width/2-TITLEBGWID/2  + TITLEZUOGAP, nOffset)));
    zuoxiajiao->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(zuoxiajiao->getContentSize().width/2 - TITLEBGWID/2 + TITLEZUOGAP,zuoshangjiao->getContentSize().height/2 - TITLEBGHT/2 + TITLEXIAGAP+(nOffset2))));
    
    youshangjiao->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(TITLEBGWID/2 - youshangjiao->getContentSize().width/2 + TITLEYOUGAP, TITLEBGHT/2 - youshangjiao->getContentSize().height/2 + TITLESHANGAP-nOffset3)));
    youbian->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(TITLEBGWID/2 - zuobian->getContentSize().width/2 + TITLEYOUGAP, nOffset)));
    youxiajiao->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(TITLEBGWID/2 - youxiajiao->getContentSize().width/2 + TITLEYOUGAP, youxiajiao->getContentSize().height/2 - TITLEBGHT/2 + TITLEXIAGAP+(nOffset2))));
    
    shangbian->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(3, TITLEBGHT/2 - shangbian->getContentSize().height/2 + TITLESHANGAP-nOffset3)));
    xiabian->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(3, xiabian->getContentSize().height/2 - TITLEBGHT/2  + TITLEXIAGAP+(nOffset2))));
    
    
    shangbian->setScaleX(TITLEROWSCAL);
    xiabian->setScaleX(TITLEROWSCAL);
    zuobian->setScaleY(TITLELISTSCAL);
    youbian->setScaleY(TITLELISTSCAL);

    addChild(zuoshangjiao);
    addChild(shangbian);
    addChild(youshangjiao);
    addChild(zuobian);
    addChild(youbian);
    addChild(zuoxiajiao);
    addChild(xiabian);
    addChild(youxiajiao);

	//标题中的文字
	SGCCLabelTTF *content = SGCCLabelTTF::create(str_Consumable_str5, FONT_BOXINFO, 28 , ccYELLOW);
    //modify by:zyc. merge into create.
	//content->setColor(ccYELLOW);
	coinsCount = SGCCLabelTTF::create(CCString::createWithFormat(" : %d", totalMoney)->getCString(), FONT_BOXINFO, 28);
	coinsCount->setAnchorPoint(ccp(0, 0.5));
	CCSprite *coinIcon = CCSprite::createWithSpriteFrameName("coinicon.png");
	this->addChild(content, 11);
	this->addChild(coinsCount, 11);
	this->addChild(coinIcon, 11);
	content->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(content->getContentSize().width/2 - TITLEBGWID/2 + 20, TITLEBGHT/2 - 50)));
	coinIcon->setPosition(ccpAdd(content->getPosition(), ccp(content->getContentSize().width / 1.2, 2)));
	coinsCount->setPosition(ccpAdd(coinIcon->getPosition(), ccp(coinsCount->getContentSize().width / 2, 2)));
	
	//"已选中N个武将卡"
	SGCCLabelTTF *nextLine = SGCCLabelTTF::create(str_CardBatchSellLayer_str4, FONT_BOXINFO, 28 , ccYELLOW);
	selCardCount = SGCCLabelTTF::create("0", FONT_BOXINFO, 28 , ccRED);
	selCardCount->setAnchorPoint(ccp(0, 0.5));
	nextLine1 = SGCCLabelTTF::create(str_CardBatchSellLayer_str5, FONT_BOXINFO, 28 , ccYELLOW);
	nextLine1->setAnchorPoint(ccp(0, 0.5));
	
	

	SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
	
	SGCCLabelTTF *containerCount = SGCCLabelTTF::create(str_CardBatchSellLayer_str6, FONT_BOXINFO, 28 , ccYELLOW);
//	int total = this->computeTotal();
	int total = playerInfo->getGoodsNumInBag();
	curCount = SGCCLabelTTF::create(CCString::createWithFormat("%d", total)->getCString(), FONT_BOXINFO, 23);

	
	curCount->setAnchorPoint(ccp(0, 0.5));
	SGCCLabelTTF *barrackCount1 = SGCCLabelTTF::create(CCString::createWithFormat("/%d", playerInfo->getPlayerBagSize())->getCString(), FONT_BOXINFO, 23);
	SGCCLabelTTF *barrackCount2 = SGCCLabelTTF::create(CCString::createWithFormat("/%d", playerInfo->getPlayerBagSize())->getCString(), FONT_BOXINFO, 23);
	barrackCount1->setAnchorPoint(ccp(0, 0.5));
	barrackCount2->setAnchorPoint(ccp(0, 0.5));
	SGCCLabelTTF *arrow = SGCCLabelTTF::create("→", FONT_BOXINFO, 23);
	arrow->setAnchorPoint(ccp(0, 0.5));
	afterSoldCount = SGCCLabelTTF::create(CCString::createWithFormat("%d", total)->getCString(), FONT_BOXINFO, 23);
	afterSoldCount->setAnchorPoint(ccp(0, 0.5));
	if (total >= playerInfo->getPlayerBagSize())
	{
		curCount->setInsideColor(ccRED);
		afterSoldCount->setInsideColor(ccRED);
	}
	

    this->addChild(nextLine, 11);
	this->addChild(selCardCount, 11);
	this->addChild(nextLine1, 11);
	this->addChild(containerCount, 11);
	//this->addChild(countChange, 11);
	
	this->addChild(curCount, 11);
	this->addChild(barrackCount1, 11);
	this->addChild(arrow, 11);
	this->addChild(afterSoldCount, 11);
	this->addChild(barrackCount2, 11);
	
	
	nextLine->setPosition(ccpAdd(content->getPosition(), ccp(nextLine->getContentSize().width / 32 - 15, -nextLine->getContentSize().height * 1.5)));
	selCardCount->setPosition(ccpAdd(nextLine->getPosition(), ccp(nextLine->getContentSize().width / 1.8, 0)));
	nextLine1->setPosition(ccpAdd(selCardCount->getPosition(), ccp(selCardCount->getContentSize().width + 6, 0)));
	containerCount->setPosition(ccpAdd(nextLine->getPosition(), ccp(nextLine->getContentSize().width +
									nextLine->getContentSize().width + selCardCount->getContentSize().width + 40, 0)));
	//countChange->setPosition(ccpAdd(containerCount->getPosition(), ccp(containerCount->getContentSize().width / 2, 0)));
	curCount->setPosition(ccpAdd(containerCount->getPosition(), ccp(containerCount->getContentSize().width / 2, 0)));
	barrackCount1->setPosition(ccpAdd(curCount->getPosition(),ccp(curCount->getContentSize().width, 0)));
	arrow->setPosition(ccpAdd(barrackCount1->getPosition(), ccp(barrackCount1->getContentSize().width, 0)));
	afterSoldCount->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width, 0)));
	barrackCount2->setPosition(ccpAdd(afterSoldCount->getPosition(), ccp(afterSoldCount->getContentSize().width, 0)));
	
	
	//自动卖出按钮
	SGButton *autoSell = SGButton::createFromLocal("advance_manzubg.png", str_CardBatchSellLayer_str7, this, menu_selector(SGCardBatchSellLayer::autoSellCard),ccpAdd(ccp(0, 3), ccp(-2, 0)),FONT_PANGWA,ccWHITE,16,false,true);
	autoSell->setScale(1.3);
	autoSell->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(TITLEBGWID/2 - autoSell->getContentSize().width*0.8, 36 - TITLEBGHT/2 )));
	this->addBtn(autoSell);
    autoSell->setVisible(false);
//	SGCCLabelTTF *tips = SGCCLabelTTF::create(str_CardBatchSellLayer_str8, FONT_BOXINFO, 24);
//	this->addChild(tips, 11);
//	tips->setPosition(ccpAdd(autoSell->getPosition(), ccp(tips->getContentSize().width / 2 + autoSell->getContentSize().width, 0)));
	
    //选择星级按钮
    SGButton *batch2StarBt = SGButton::create("batchSelectBg.png", NULL, this, menu_selector(SGCardBatchSellLayer::select2StarKind));
    batch2StarShow = CCSprite::createWithSpriteFrameName("batchSelect.png");

    SGButton * batch2StarLable = SGButton::create("batch2Start.png", NULL, this, menu_selector(SGCardBatchSellLayer::select2StarKind));
    batch2StarBt->setPosition(ccpAdd(batchSellTitle->getPosition(), ccp(batch2StarBt->getContentSize().width/2 - TITLEBGWID/2 +20, 36 - TITLEBGHT/2)));
    batch2StarShow->setPosition(batch2StarBt->getPosition());
    batch2StarLable->setPosition(ccpAdd(batch2StarBt->getPosition(), ccp(batch2StarBt->getContentSize().width/2 + batch2StarLable->getContentSize().width/2,-2)));
    
    addBtn(batch2StarBt);
    addChild(batch2StarShow,100);
    addBtn(batch2StarLable);
    batch2StarBt->setVisible(false);
    batch2StarShow->setVisible(false);
    batch2StarLable->setVisible(false);
    
    SGButton *batch3StarBt = SGButton::create("batchSelectBg.png", NULL, this, menu_selector(SGCardBatchSellLayer::select3StarKind));
    batch3StarShow = CCSprite::createWithSpriteFrameName("batchSelect.png");
    SGButton *batch3StarLable = SGButton::create("batch3Star.png", NULL, this, menu_selector(SGCardBatchSellLayer::select3StarKind));

    batch3StarBt->setPosition(ccpAdd(batch2StarBt->getPosition(), ccp(STARBTGAP + batch3StarBt->getContentSize().width/2+40, 0)));
    batch3StarShow->setPosition(batch3StarBt->getPosition());
    batch3StarLable->setPosition(ccpAdd(batch3StarBt->getPosition(), ccp(batch3StarLable->getContentSize().width/2 + batch3StarBt->getContentSize().width/2, -2)));
    
    addBtn(batch3StarBt);
    addChild(batch3StarShow,100);
    addBtn(batch3StarLable);
    
    batch3StarBt->setVisible(false);
    batch3StarShow->setVisible(false);
    batch3StarLable->setVisible(false);
//    SGButton *batch4StarBt = SGButton::create("batchSelectBg.png", NULL, this, menu_selector(SGCardBatchSellLayer::select4StarKind));
//    batch4StarShow = CCSprite::createWithSpriteFrameName("batchSelect.png");
//    SGButton *batch4StarLable = SGButton::create("batch4Star.png", NULL, this, menu_selector(SGCardBatchSellLayer::select4StarKind));
// 
//    
//    batch4StarBt->setPosition(ccpAdd(batch3StarBt->getPosition(), ccp(STARBTGAP *2/3+ batch4StarBt->getContentSize().width/2, 0)));
//    batch4StarShow->setPosition(batch4StarBt->getPosition());
//    batch4StarLable->setPosition(ccpAdd(batch4StarBt->getPosition(), ccp(batch4StarLable->getContentSize().width/2 + batch4StarBt->getContentSize().width/2, -2)));
//    
//    addBtn(batch4StarBt);
//    addChild(batch4StarShow,100);
//    addBtn(batch4StarLable);
    
    batch2StarShow->setVisible(false);
    batch3StarShow->setVisible(false);
    //batch4StarShow->setVisible(false);
    
    //无卡提示语
	if (_allData->count() == 0)
	{
		SGCCLabelTTF *tipInfo = NULL;
		if (enterType == OFFICER)
			tipInfo = SGCCLabelTTF::create(str_CardBatchSellLayer_str9, FONT_BOXINFO, 30);
		else if (enterType == EQUIP)
			tipInfo = SGCCLabelTTF::create(str_CardBatchSellLayer_str10, FONT_BOXINFO, 30);
		else if (enterType == SKILL)
			tipInfo = SGCCLabelTTF::create(str_CardBatchSellLayer_str11, FONT_BOXINFO, 30);
		this->addChild(tipInfo, 1);
		tipInfo->setPosition(ccpAdd(ccp(s.width / 2, s.height / 2), ccp(0, 0)));
	}
	
    ResourceManager::sharedInstance()->bindTexture("sanguobigpic/barrack_bg.plist", RES_TYPE_LAYER_UI, sg_batchselllayer);

    CCSprite *bg = CCSprite::createWithSpriteFrameName("barrack_bg.png");
    CCRect e = CCRectMake( 0, b,bg->getContentSize().width,s.height - h - b - titleBg->getContentSize().height - 20 );
    
    bg->setScaleX(s.width/bg->getContentSize().width * 1.01);
    bg->setScaleY(1.05);
    bg->setTextureRect(e);

    this->addChild(bg,-1);
    bg->setAnchorPoint(ccp(0, 0));
    bg->setPosition(ccp(-2, b-10));
	
	tableView->setFrame(CCRectMake(0, 0, 600, s.height-b - h  - titleBg->getContentSize().height - 270+nOffset2));
    tableView->setPosition(ccpAdd(SGLayout::getPoint(kBottomLeft), ccp(s.width/2-tableView->getContentSize().width/2, b + 80+nOffset2)));
    tableView->setDown(-15);
	
	//设置左右箭头
    float posY = tableView->getContentSize().height*.5f + tableView->getPositionY();
    CCSprite *spriteL = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    float widLR = spriteL->getContentSize().width*.5f;
    float hitLR = spriteL->getContentSize().height*.5f;
    spriteL->setPosition(ccp(widLR, posY-hitLR));
    spriteL->setVisible(false);
    spriteL->setTag(111);
    this->addChild(spriteL);
    CCSprite *spriteR = CCSprite::createWithSpriteFrameName("visit_jiantou.png");
    spriteR->setFlipX(true);
    spriteR->setTag(112);
    spriteR->setVisible(false);
    spriteR->setPosition(ccp(s.width-widLR, posY-hitLR));
    this->addChild(spriteR);
    
    if (_totalPage > 1) {
        spriteL->setVisible(true);
        spriteR->setVisible(true);
        tableView->setPagegate(this);
        tableView->setPageNEnable(true);
        tableView->setShowSlider(false);
    }

//	if (enterType == OFFICER)
//		SGSortBox::updateBtn(range,1);
//	else if (enterType == EQUIP)
//		SGSortBox::updateBtn(range,2);
    
    CCSprite *pagecountbg = CCSprite::createWithSpriteFrameName("barrack_cardcountbg.png");
	pagecountbg->setScaleX(0.75);
    pagecountbg->setAnchorPoint(ccp(1, 0));
    this->addChild(pagecountbg,40,110);
    pagecountbg->setPosition(ccpAdd(SGLayout::getPoint(kBottomRight), ccp(0, b)));
	
    CCString *strpg = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    pageLabel = SGCCLabelTTF::create(strpg->getCString(), FONT_BOXINFO, 28 , COLOR_YELLOW);
    this->addChild(pageLabel,42);
    //modify by:zyc. merge into create.
    //pageLabel->setColor(ccc3(0xff, 0xf0, 0x00));
    pageLabel->setPosition(ccpAdd(pagecountbg->getPosition(), ccp(-pagecountbg->getContentSize().width*.5f + 30, pagecountbg->getContentSize().height*.5f)));
	
	
	//下方卖出按钮

	batchSellButton = SGButton::createFromLocal("batch_sell.png", str_sale, this, menu_selector(SGCardBatchSellLayer::batchSellCard),ccp(0, 0),FONT_PANGWA,ccWHITE,32,false,true);
	batchSellButton->setPosition(ccpAdd(SGLayout::getPoint(kBottomCenter), ccp(0, 200)));
	this->addBtn(batchSellButton);
	if (selectNum == 0)
	{
		batchSellButton->setColor(ccc3(80, 80, 80));
	}
	
	
	if (_allData->count()>0)
	{
		 refreshPage(1);
	}
    
	
}
//算出现在营里所有的东西总数
int SGCardBatchSellLayer::computeTotal()
{
	//计算军营总数
	CCArray *officer = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
    CCArray *equip = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
    CCArray *propCards = SGPlayerInfo::sharePlayerInfo()->getPropCards();
    int generalProp = 0;
    int soulProp = SGPlayerInfo::sharePlayerInfo()->getSoldierCards()->count();
    int equipProp = 0;
    int otherProp = 0;
    int skillProp = 0;
    CCObject *obj = NULL;
    int total = 0;
    if (propCards) {
        CCARRAY_FOREACH(propCards, obj)
        {
            SGPropsCard *card = (SGPropsCard *)obj;
            if (card)
            {
                if (card->getType() == 1)
                {
                    generalProp += 1 ;
                }
                else if (card->getType() == 2)
                {
                    equipProp += 1 ;
                }
                else if (card->getType() == 3)
                {
                    soulProp += 1 ;
                }
                else if (card->getType() == 0)
                {
                    otherProp += 1 ;//转生道具
                }else if (card->getType() == 4 ||card->getType() == 5)
                {
                    skillProp+= 1;
                }
            }
        }
        
        total = officer->count() + equip->count() + propCards->count() + SGPlayerInfo::sharePlayerInfo()->getSoldierCards()->count();
    }
	return total;
}

SNSTableViewCellItem* SGCardBatchSellLayer::tableViewItemForCell(SNSTableView* tableView, SNSIndexPath* indexPath)
{
    int num = tableView->getItemPointerWithIndexPath(indexPath);
	SGBaseMilitaryCard  *card = (SGBaseMilitaryCard *)datas->objectAtIndex(num);
    SGCardItem *item = (SGCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);

    if(item == NULL)
    {
       item = SGCardItem::create(false,0,this,tableView,111);
    }
	
	if (num<datas->count())
	{
		if (enterType == OFFICER)
			item ->updataCard((SGBaseMilitaryCard *)datas->objectAtIndex(num), cardType_officer,true,false);
		else if (enterType == EQUIP)
			item ->updataCard((SGBaseMilitaryCard *)datas->objectAtIndex(num), cardType_equip,true,false);
		else if (enterType == SKILL)
			item ->updataCard((SGBaseMilitaryCard *)datas->objectAtIndex(num), cardType_skill,true,false);
	}
	if (item)
	{
		//item->setIsSelected(item->getIsSelected());
        //在数组存在的情况下再进行其他操作
        if (isSelectArray && isSelectArray->data && isSelectArray->data->arr && isSelectArray->containsObject(card)) {
            selectItem[_nowPage][indexPath->getRow()] = 1;//自动售出时更新
        }
		item->setIsSelected(selectItem[_nowPage][indexPath->getRow()] == 1);
	}
	
	//设置每个卡牌的钱数
	item->setCardCost(computeSellMoney(card));
	return item;
}



void SGCardBatchSellLayer::tableViewSelectItem(SNSTableView* tableView, SNSIndexPath* indexPath)
{
	int index = tableView->getItemPointerWithIndexPath(indexPath);
	SGBaseMilitaryCard  *card = (SGBaseMilitaryCard *)datas->objectAtIndex(index);
	
	SGCardItem *item = (SGCardItem*)tableView->dequeueReusableCellItemForIndexPath(indexPath);
	//设置选中或者非选中状态
	if (item != NULL)
	{
//		if (item->getIsSelected())
		if (selectItem[_nowPage][indexPath->getRow()] == 1)
		{
			selectItem[_nowPage][indexPath->getRow()] = 0;
			selectNum--;
			if (selectNum < 0)
				selectNum = 0;
			//改变数值显示
			totalMoney -= computeSellMoney(card);
			if (totalMoney < 0)
			{
				totalMoney = 0;
			}
			//如果数组中有东西才能做移除操作
			if (isSelectArray->data && isSelectArray->data->arr && isSelectArray->count())
			{
				isSelectArray->removeObject(card);
                if(selectNum == 0)
                {
                    this->clearSelectedArray(false);
                    isSelectArray->removeAllObjects();
                }
			}
			
		}
		else if (selectItem[_nowPage][indexPath->getRow()] == 0)
		{
            if(batch2StarShow->isVisible()||batch3StarShow->isVisible())
            {
                this->clearSelectedArray(false);
                isSelectArray->removeAllObjects();
            }
			selectItem[_nowPage][indexPath->getRow()] = 1;
			selectNum++;
			totalMoney += computeSellMoney(card);
			if (totalMoney < 0)
			{
				totalMoney = 0;
			}
           
			//这里的isSelectArray可能会出现提前被释放的情况，这里加入判空操作，如果为空，重新创建并做保留操作
            if (!isSelectArray || !isSelectArray->data || !isSelectArray->data->arr)
            {
                isSelectArray = CCArray::create();
                isSelectArray->retain();
            }
            //加入数组中
			isSelectArray->addObject(card);
		}
	}
	item->setIsSelected(selectItem[_nowPage][indexPath->getRow()] == 1);
	this->changeSelectCount();
	tableView->reloadData(false);
}


void SGCardBatchSellLayer::changeSellInfo(int sellCount)
{
    SGPlayerInfo *playerInfo = SGPlayerInfo::sharePlayerInfo();
	selCardCount->setString(CCString::createWithFormat("%d", sellCount)->getCString());
//	int total = this->computeTotal();
    int total = playerInfo->getGoodsNumInBag();
	curCount->setString(CCString::createWithFormat("%d", total)->getCString());
	afterSoldCount->setString(CCString::createWithFormat("%d", total - sellCount)->getCString());
   
	if (total >= playerInfo->getPlayerBagSize())
	{
		curCount->setInsideColor(ccRED);
	}
	else
	{
		curCount->setInsideColor(ccWHITE);
	}
	if (total - sellCount > playerInfo->getPlayerBagSize())
	{
		curCount->setInsideColor(ccRED);
	}
	else
	{
		curCount->setInsideColor(ccWHITE);
	}
	//更改钱数显示
	coinsCount->setString(CCString::createWithFormat(" : %d", totalMoney)->getCString()	);
    nextLine1->setPosition(ccpAdd(selCardCount->getPosition(), ccp(selCardCount->getContentSize().width + 6, 0)));
}


// 选择卡牌后更新上部数字显示
void SGCardBatchSellLayer::changeSelectCount(bool initShowSelect )
{
    if (initShowSelect) {
        if (batch2StarShow)
        {
            batch2StarShow->setVisible(false);
        }
        if (batch3StarShow)
        {
            batch3StarShow->setVisible(false);
        }
        
    }
    
    changeSellInfo(selectNum);
    
	//如果没有选择卡牌，卖出按钮为灰色不可用，点击提示未选择卡牌
	if (selectNum == 0)
	{
		batchSellButton->setColor(ccc3(80, 80, 80));
	}
	else
	{
		batchSellButton->setColor(ccWHITE);
	}
}

void SGCardBatchSellLayer::updateSelectStatus()
{
    autoSelectCard();
    changeSellInfo(isSelectArray->count());
}

void SGCardBatchSellLayer::select2StarKind()
{
    if (batch2StarShow)
    {
        batch2StarShow->setVisible(!batch2StarShow->isVisible());
        updateSelectStatus();
    }
}
void SGCardBatchSellLayer::select3StarKind()
{
    if (batch3StarShow)
    {
       batch3StarShow->setVisible(!batch3StarShow->isVisible());
       updateSelectStatus();
    }
}

void SGCardBatchSellLayer::select4StarKind()
{
//    if (batch4StarShow)
//    {
//        batch4StarShow->setVisible(!batch4StarShow->isVisible());
//        updateSelectStatus();
//    }
}

void SGCardBatchSellLayer::autoSelectCard()
{
    if (isSelectArray->data && isSelectArray->data->arr && isSelectArray->count())
	{
        
        this->clearSelectedArray(false);
	}
	m_ntwoStarCount = 0;
    m_nthreeStarCount = 0;
	CCObject *obj = NULL;
	this->isPressAutoSell = true;
	int starlvl = 0;
	if (enterType == OFFICER)
	{
		CCARRAY_FOREACH(_allData, obj)
		{
			SGOfficerCard *card = (SGOfficerCard *)obj;
			SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(card->getItemId());
            starlvl = temp->getUpgradestar();
			if (( batch2StarShow->isVisible() && starlvl ==2))
			{
				isSelectArray->addObject(card);
				totalMoney += computeSellMoney(card);
                m_ntwoStarCount++;
			}
            else if((batch3StarShow->isVisible() && starlvl == 3))
            {
                isSelectArray->addObject(card);
				totalMoney += computeSellMoney(card);
                m_nthreeStarCount++;
            }
		}
	}
	else if (enterType == EQUIP)
	{
		CCARRAY_FOREACH(_allData, obj)
		{
			SGEquipCard *card = (SGEquipCard *)obj;
			SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(card->getItemId());
			starlvl = temp->getEquipCurrStarLevel();
            
            if (( batch2StarShow->isVisible() && starlvl ==2))
			{
				isSelectArray->addObject(card);
				totalMoney += computeSellMoney(card);
                m_ntwoStarCount++;
			}
            else if((batch3StarShow->isVisible() && starlvl == 3))
            {
                isSelectArray->addObject(card);
				totalMoney += computeSellMoney(card);
                m_nthreeStarCount++;
            }

		}
	}
	else if (enterType == SKILL)
	{
		CCARRAY_FOREACH(_allData, obj)
		{
			SGPropsCard *card = (SGPropsCard *)obj;
			SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(card->getItemId());
			starlvl = temp->getPropsCurrStarLevel();
            if (( batch2StarShow->isVisible() && starlvl ==2))
			{
				isSelectArray->addObject(card);
				totalMoney += computeSellMoney(card);
                m_ntwoStarCount++;
			}
            else if((batch3StarShow->isVisible() && starlvl == 3))
            {
                isSelectArray->addObject(card);
				totalMoney += computeSellMoney(card);
                m_nthreeStarCount++;
            }

		}
	}
    tableView->reloadData(false);
}

void SGCardBatchSellLayer::autoSellCard()
{
	CCLOG("Auto Sell");

    if (batch2StarShow->isVisible() || batch3StarShow->isVisible() )
	{
        if (isSelectArray->count() == 0)
        {
            SG_SHOW_WINDOW(str_CardBatchSellLayer_str12);
            return ;
        }
        SGBatchSellBox_x *box = SGBatchSellBox_x::create(this, isSelectArray, totalMoney,m_ntwoStarCount,m_nthreeStarCount);
        SGMainManager::shareMain()->showBox(box);
	}
    else
    {
        SG_SHOW_WINDOW(str_CardBatchSellLayer_str20);
    }
}

void SGCardBatchSellLayer::batchSellCard()
{
	CCLOG("Batch Sell");
	if (selectNum == 0)
	{
		SG_SHOW_WINDOW(str_CardBatchSellLayer_str13);
		return ;
	}
	
	SGBatchSellBox *box = SGBatchSellBox::create(this, isSelectArray, totalMoney);
	SGMainManager::shareMain()->showBox(box);
}


//初始化卡牌数据, 根据不同的enterType
void SGCardBatchSellLayer::initMsg()
{
    //bind  资源。
    SGCardItem::addCardItemRes(sg_batchselllayer);
	switch (enterType) {
		case OFFICER:
		{
			CCLOG("Officer");
			this->initDatas(OFFICER);
		}
			break;
		case EQUIP:
		{
			CCLOG("Equip");
			this->initDatas(EQUIP);
		}
			break;
		case SKILL:
		{
			CCLOG("Skill");
			this->initDatas(SKILL);
		}
			break;
		default:
			CCLOG("Error >> ");
			break;
	}
	_totalPage = ceil(_allData->count()/8.0);
}

void SGCardBatchSellLayer::initDatas(SGCardBatchSellLayer::CardType ct)
{
	_allData = CCArray::create();
	_allData->retain();
	CCArray *temp = CCArray::create();
	//如果是武将卡
	if (ct == OFFICER)
	{
		temp = SGPlayerInfo::sharePlayerInfo()->getOfficerCards();
		
		for (int i = 0; i < temp->count(); i++)
		{
			SGOfficerCard *card = (SGOfficerCard *)temp->objectAtIndex(i);
			if (!SGTeamgroup::shareTeamgroupInfo()->isOnEmbattle(card) && !card->getIsProt())//不在阵上，并且没有保护
			{
				_allData->addObject(temp->objectAtIndex(i));
			}
		}
		
	}
	else if (ct == EQUIP)//如果是装备卡
	{
		temp = SGPlayerInfo::sharePlayerInfo()->getEquipCards();
		for (int i = 0; i < temp->count(); i++)
		{
			SGEquipCard *card = (SGEquipCard *)temp->objectAtIndex(i);
			if (!card->getIsProt() && !card->getOfficerCardId())
			{
				_allData->addObject(temp->objectAtIndex(i));
			}
		}
	}
	else if (ct == SKILL)//如果是技能卡
	{
		temp = SGPlayerInfo::sharePlayerInfo()->getPropCards();
		for (int i = 0; i < temp->count(); i++)
		{
			SGPropsCard *card = (SGPropsCard *)temp->objectAtIndex(i);
			if (card->getType() == 4 || card->getType() == 5)
			{
				_allData->addObject(temp->objectAtIndex(i));
			}
		}
	}
	else
	{
		
	}
}

//返回按钮
void SGCardBatchSellLayer::backHandler()
{
	CCLOG("Return");
	switch (enterType) {
		case OFFICER:
		{
			SGMainManager::shareMain()->showGeneralsLayer(0);
		}
			break;
		case EQUIP:
		{
			SGMainManager::shareMain()->showEquipsLayer(NULL,0,0);
		}
			break;
		case SKILL:
		{
			SGMainManager::shareMain()->showSkillCardsLayer(0);
		}
			break;
		default:
			break;
	}
	
}
//排序回调
void SGCardBatchSellLayer::sortHandler()
{
    SGSortBox *Box = SGSortBox::create(this, this, _allData, 1);
    SGMainManager::shareMain()->showBox(Box);
}
//是否点击自动卖出
bool SGCardBatchSellLayer::getIsPressAuto()
{
	return isPressAutoSell;
}
void SGCardBatchSellLayer::setIsPressAuto(bool isPress)
{
	isPressAutoSell = isPress;
}
//清空selArray
void SGCardBatchSellLayer::clearSelectedArray(bool clearCount)
{
	//重置所有
	if (isSelectArray->data && isSelectArray->data->arr && isSelectArray->count())
	{
		isSelectArray->removeAllObjects();
	}
	totalMoney = 0;//重置可获得钱数
	for (int i = 0; i < 300; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			selectItem[i][j] = 0;
		}
	}
	selectNum = 0;
    this->changeSelectCount(clearCount);
    if (clearCount)
    {
        this->initMsg();
        std::string str;
        if (enterType == OFFICER)
            str = CCUserDefault::sharedUserDefault()->getStringForKey("officersort");
        else if (enterType == EQUIP)
            str = CCUserDefault::sharedUserDefault()->getStringForKey("equipsort");

        CCString *type = CCString::create(str);
        SGSortBox::sortCards(_allData, type->intValue(),1);
        
        _nowPage = 0;
        this->refreshPage(1);
        
    }
    tableView->reloadData(clearCount);
}
int SGCardBatchSellLayer::getAutoSellStars()
{
    int starsValue = 0;

    
    if (batch3StarShow->isVisible())
    {
        starsValue += 3 *10;
    }
    if (batch2StarShow->isVisible())
    {
        starsValue += 2 ;
    }
    return starsValue;
}
//排序回调
void SGCardBatchSellLayer::sortLayerCallBack(cocos2d::CCArray *cardArray)
{
	//    this->datas = cardArray;
    _nowPage = 0;
    this->refreshPage(1);
    this->tableView->reloadData();
	if (enterType == OFFICER)
		SGSortBox::updateBtn(range,1);
	else if (enterType == EQUIP)
		SGSortBox::updateBtn(range,2);
}
void SGCardBatchSellLayer::refreshPage(int type)
{
	if (!_allData->data->arr || !_allData->count())
	{
		 datas->removeAllObjects();
		return ;
	}
		
   datas->removeAllObjects();
    int pageCount = 8; // 每页显示的个数
    
	//某些情况会出现_nowpage是很大的数情况，这里做一个防御
	if (_nowPage < 0)
		_nowPage = 1;
	
    if (type == 1) { // to next page
        if (_nowPage < _totalPage) {
            _nowPage++;
        }else { // _nowPage >= _totalPage
            _nowPage = 1;
        }
    }else if (type == 2) { // to pre page
        if (_nowPage > 1) {
            _nowPage--;
        }else {
            _nowPage = _totalPage;
        }
    }
    
    int last = pageCount;
    if (_nowPage == _totalPage) {
        last = _allData->count()-(_nowPage-1)*pageCount;
    }
    for (int ii = 0; ii < last; ii++) {
        CCObject *obj = _allData->objectAtIndex((_nowPage-1)*pageCount+ii);
        datas->addObject(obj);
    }
    
    CCString *str = CCString::createWithFormat(str_Consumable_str1, _nowPage, _totalPage);
    if(pageLabel)
        pageLabel->setString(str->getCString());
    tableView->reloadData();
}

void SGCardBatchSellLayer::itemselect(SNSTableView* tableView,SNSIndexPath* indexpath)
{
	
}

//批量卖出二次确认
SGBatchSellBox::SGBatchSellBox() : m_scrollview(NULL)
{
	selectedCard = CCArray::create();
	selectedCard->retain();
}
SGBatchSellBox::~SGBatchSellBox()
{
    CC_SAFE_RELEASE(selectedCard);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_AUTOBATCHSELL);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BATCHSELL);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_batchSell);

}


SGBatchSellBox *SGBatchSellBox::create(SGBoxDelegate *del, CCArray *array, int total)
{
	SGBatchSellBox *bfb = new SGBatchSellBox();
    if (bfb->init(del, box_batchSell, CCSizeMake(600,524)))
    {
		bfb->del = del;
		bfb->selectedCard->addObjectsFromArray(array);
        bfb->autorelease();
        bfb->initView(total);
        return bfb;
    }
    CC_SAFE_DELETE(bfb);
    return NULL;
}

void SGBatchSellBox::initView(int total)
{
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_AUTOBATCHSELL, this, callfuncO_selector(SGBatchSellBox::autoSellConfirmListener));
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_BATCHSELL, this, callfuncO_selector(SGBatchSellBox::sellConfirmListener));
	ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_box_batchSell);
	int curr = SGPlayerInfo::sharePlayerInfo()->getPlayerCoins();
	CCPoint center = SGLayout::getPoint(kMiddleCenter);
	fontbg->setPreferredSize(CCSize(480, 282));
	fontbg->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, -20)));
	SGButton *okButton = SGButton::create("gonggao_an.png", "public_font_queding.png", this, menu_selector(SGBatchSellBox::sellConfirm),CCPointZero,false,true);
    SGButton *cancelButton = SGButton::create("gonggao_an.png", "public_font_quxiao.png", this, menu_selector(SGBatchSellBox::sellCancel),CCPointZero,false,true);
	SGCCLabelTTF *label = SGCCLabelTTF::create(str_CardBatchSellLayer_str14, FONT_PANGWA, 32 , ccc3(230, 0, 0));
    //modify by:zyc. merge into create.
	//label->setColor(ccc3(230, 0, 0));
	CCLabelTTF *content = CCLabelTTF::create(str_Consumable_str5, FONT_BOXINFO, 26);
	content->setColor(ccc3(0, 0, 0));
	CCSprite *coins1 = CCSprite::createWithSpriteFrameName("coinicon.png");
	CCSprite *coins2 = CCSprite::createWithSpriteFrameName("coinicon.png");
	CCLabelTTF *totalCoin = CCLabelTTF::create(str_nowhave, FONT_BOXINFO, 26);
	totalCoin->setColor(ccc3(0, 0, 0));
	CCLabelTTF *curCoins = CCLabelTTF::create(CCString::createWithFormat("%d", curr)->getCString(), FONT_BOXINFO, 26);
	CCLabelTTF *arrow = CCLabelTTF::create("→", FONT_BOXINFO, 23);
	CCLabelTTF *getCoins = CCLabelTTF::create(CCString::createWithFormat("%d", total)->getCString(), FONT_BOXINFO, 26);
	CCLabelTTF *finalCoins = CCLabelTTF::create(CCString::createWithFormat("%d", curr + total)->getCString(), FONT_BOXINFO, 26);
	curCoins->setColor(ccc3(0, 0, 0));
	arrow->setColor(ccc3(0, 0, 0));
	getCoins->setColor(ccc3(0, 0, 0));
	finalCoins->setColor(ccc3(0, 0, 0));
	
	this->addChild(content, 1);
	this->addChild(coins1, 1);
	
	this->addChild(curCoins, 1);
	this->addChild(arrow, 1);
	this->addChild(getCoins, 1);
	this->addChild(finalCoins, 1);
	
    label->setPosition(ccpAdd(biao->getPosition(), ccp(20, 10)));
	content->setPosition(ccpAdd(center, ccp(-185, 183)));
	coins1->setPosition(ccpAdd(content->getPosition(), ccp(content->getContentSize().width / 1.2, 0)));
	totalCoin->setPosition(ccpAdd(content->getPosition(), ccp(-25, -totalCoin->getContentSize().height * 1.3)));
	coins2->setPosition(ccpAdd(totalCoin->getPosition(), ccp(totalCoin->getContentSize().width + 5, 0)));
	getCoins->setAnchorPoint(ccp(0, 0.5));
	getCoins->setPosition(ccpAdd(coins1->getPosition(), ccp(coins1->getContentSize().width, 0)));
	curCoins->setAnchorPoint(ccp(0, 0.5));
	curCoins->setPosition(ccpAdd(coins2->getPosition(), ccp(coins2->getContentSize().width * 1.2, 0)));
	arrow->setAnchorPoint(ccp(0, 0.5));
	arrow->setPosition(ccpAdd(curCoins->getPosition(), ccp(curCoins->getContentSize().width + 10, 0)));
	finalCoins->setAnchorPoint(ccp(0, 0.5));
	finalCoins->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width + 10, 0)));
	
    this->addChild(label);
	this->addBtn(okButton);
	this->addBtn(cancelButton);
	this->addChild(coins2);
	this->addChild(totalCoin);
	
	okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+5 -50)));
    cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+5-50)));
	// 加入scrollview
	m_scrollview = CCScrollView::create(CCSizeMake(480, 270));
    CCLayerColor *container = CCLayerColor::create(ccc4(0, 255, 0, 0));
    container->setContentSize(CCSizeMake(470, 265));
    m_scrollview->setContainer(container);
	m_scrollview->setPosition(ccpAdd(fontbg->getPosition(), ccp(-fontbg->getContentSize().width / 2 + 5,
																-fontbg->getContentSize().height / 2 + 5)));//设置scrollview位置
	
	m_scrollview->setDirection(kCCScrollViewDirectionVertical);
    addChild(m_scrollview);
	
    ResourceManager::sharedInstance()->bindTexture("sgstrenglayer/sgstrenglayer.plist", RES_TYPE_LAYER_UI, sg_box_batchSell, LIM_PNG_AS_PNG);
	ResourceManager::sharedInstance()->bindTexture("sggeneralinfolayer/sggeneralinfolayer1.plist", RES_TYPE_LAYER_UI, sg_box_batchSell);
    
	int finalLen = 0;
	for (int i = 0; i < selectedCard->count(); i++)
	{
		SGBaseMilitaryCard *card = (SGBaseMilitaryCard *)selectedCard->objectAtIndex(i);
		SGTableItem *item = SGTableItem::create(card);
		container->addChild(item);
		
		item->setPosition(ccpAdd(container->getPosition(), ccp(70, (95 + 12) * i + 50)));
		finalLen += (95 + 10);
	}
	m_scrollview->setContentOffset(ccp(0, -finalLen + m_scrollview->getContentSize().height));
	m_scrollview->setContentSize(CCSizeMake(470, finalLen));
	m_scrollview->getContainer()->setContentSize(CCSizeMake(470, finalLen));
	
/*
	SGTableItem *item = SGTableItem::create((SGBaseMilitaryCard *)selectedCard->objectAtIndex(0));
	container->addChild(item);
	item->setPosition(ccpAdd(container->getPosition(), ccp(item->getContentSize().width / 4, container->getContentSize().height - item->getContentSize().height / 2)));
*/
}
//自动卖出监听
void SGBatchSellBox:: autoSellConfirmListener(CCObject *sender)
{
	CCLOG("Receive !");
	SGSocketRequest *request = (SGSocketRequest *)sender;
	if (request)
	{
		main::SellRoleItemAllAutoResponse *autoSellResponce = (main::SellRoleItemAllAutoResponse *)request->m_msg;
		if(autoSellResponce->state() == 0)
		{
			CCLOG("Failed?");
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str15);
		}
		else if (autoSellResponce->state() == 1)
		{
			CCLOG("Success!");
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str16);
		}
		else if (autoSellResponce->state() == 2)
		{
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str17);
		}
	}
	

	this->sellCancel();
	//((SGCardBatchSellLayer *)del)->backHandler();
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
}
//手动选择监听
void SGBatchSellBox::sellConfirmListener(CCObject *sender)
{
	SGSocketRequest *request = (SGSocketRequest *)sender;
	if (request)
	{
		main::SellRoleItemAllResponse *sellRes = (main::SellRoleItemAllResponse *)request->m_msg;
		
		if(sellRes->state() == 0)
		{
			CCLOG("Failed?");
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str15);
		}
		else if (sellRes->state() == 1)
		{
			CCLOG("Success");
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str16);
		}
		else if (sellRes->state() == 2)
		{
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str17);
		}
	}
	this->sellCancel();
	//((SGCardBatchSellLayer *)del)->backHandler();
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
	
}

void SGBatchSellBox::sellConfirm()
{
	CCLOG("Sell Confirm");
	
	/*
	 officerCard(0), // 将领卡片
	 equipmentCard(1), // 装备卡片
	 soldierCard(2), // 士兵卡片
	 propsCard(3),// 道具卡片
	 officerFragmentCard(4)//武将卡碎片
	 */
	//自动卖出
	if (((SGCardBatchSellLayer *)del)->getIsPressAuto())
	{
		if (selectedCard->count() == 0)
		{
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str18);
			return ;
		}
        
        int sellstars = ((SGCardBatchSellLayer *)del)->getAutoSellStars();
        int star4 = sellstars / 100;
        int star3 = (sellstars - star4*100) / 10;
        int star2 = sellstars - star4*100 - star3*10;
        
		int type = ((SGCardBatchSellLayer *)del)->getItemType();//根据layer中的entertype确定卡牌类型
		main::SellRoleItemAllAutoRequest *autoSellRequest = new main::SellRoleItemAllAutoRequest();
		autoSellRequest->set_type(type);
        
        if (star4 > 0)
        {
            autoSellRequest->add_stars(star4);
        }
        if (star3 > 0)
        {
            autoSellRequest->add_stars(star3);
        }
        if (star2 > 0)
        {
            autoSellRequest->add_stars(star2);
        }
        
		SGSocketClient::sharedSocketClient()->send(MSG_AUTOBATCHSELL, autoSellRequest);
	}
	else//手动选择
	{
		if (selectedCard->count() == 0)
		{
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str19);
			return ;
		}
		main::SellRoleItemAllRequest *sellResq = new main::SellRoleItemAllRequest();
		int type = ((SGCardBatchSellLayer *)del)->getItemType();
		
		sellResq->set_type(type);
		for (int i = 0; i < selectedCard->count(); i++)
		{
			int itemId = ((SGBaseMilitaryCard *)selectedCard->objectAtIndex(i))->getSsid();
			sellResq->add_itemid(itemId);
		}
		
		SGSocketClient::sharedSocketClient()->send(MSG_BATCHSELL, sellResq);
	}
	
}
void SGBatchSellBox::sellCancel()
{
	CCLOG("Cancel Comfirm");
	//是否是点击了自动卖出按钮,  取消后要把对应的选择的卡牌取消掉
	if (((SGCardBatchSellLayer *)del)->getIsPressAuto())
	{
		CCLOG("is Press Auto");
		((SGCardBatchSellLayer *)del)->setIsPressAuto(false);
		((SGCardBatchSellLayer *)del)->clearSelectedArray();
	}
	else
	{
		((SGCardBatchSellLayer *)del)->clearSelectedArray();
	}
	this->boxClose();
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}
}

//
SGTableItem::SGTableItem()
{
	
}
SGTableItem::~SGTableItem()
{
	
}

SGTableItem *SGTableItem::create(SGBaseMilitaryCard *card)
{
	SGTableItem *item = new SGTableItem();
	if (item)
	{
		item->autorelease();
		item->initView(card);
		return item;
	}
	CC_SAFE_DELETE(item);
	return NULL;
}
//构建二次确认框中的信息
void SGTableItem::initView(SGBaseMilitaryCard *card)
{
	CCLayer *bg = CCLayer::create();
	CCSprite *cardbg = CCSprite::createWithSpriteFrameName("card_bj.png");
	bg->addChild(cardbg, 0);
	CCSprite *head = NULL;
	CCSprite *cardBorder = NULL;
	SGCCLabelTTF *level = NULL;
	CCLabelTTF *name = NULL;
	CCLabelTTF *money = CCLabelTTF::create(CCString::createWithFormat("%d", computeSellMoney(card))->getCString(), FONT_BOXINFO, 26);
	money->setColor(ccc3(90, 0, 0));
	
	CCSprite *coin = CCSprite::createWithSpriteFrameName("coinicon.png");
	char headIcon[256] = "\0";
	char border[256] = "\0";
	if (CCString::create(card->getClassName())->isEqual(CCString::create("SGOfficerCard")))
	{
		
		SGOfficerCard *tmp = (SGOfficerCard *)card;
		SGMainManager::shareMain()->addHeadIconPlistByNum(tmp->getHeadSuffixNum(),sg_batchselllayer);//12.05
		sprintf(headIcon, "head%d.png", tmp->getHeadSuffixNum());
		
		head = CCSprite::createWithSpriteFrameName(headIcon);
		bg->addChild(head, 0);
		//+1, +2, +3
		if (tmp->getAdNum())
		{
            SGCCLabelTTF *advance = SGCCLabelTTF::create(CCString::createWithFormat("+%d",tmp->getAdNum())->getCString(), FONT_PANGWA, 24);
            advance->setInsideColor(ccGREEN);
            bg->addChild(advance, 1);
			advance->setPosition(ccpAdd(head->getPosition(), ccp(head->getContentSize().width / 2 - advance->getContentSize().width / 1.5,
																 head->getContentSize().height / 2 - advance->getContentSize().height / 2)));
		}
		
		SGOfficerDataModel *temp = SGStaticDataManager::shareStatic()->getOfficerById(card->getItemId());
		int starlvl = temp->getOfficerCurrStarLevel();
		if (starlvl>1) {
			starlvl-=1;
		}
		sprintf(border, "officerPinzhi_%d_0.png",starlvl);
		cardBorder = CCSprite::createWithSpriteFrameName(border);
		bg->addChild(cardBorder, 1);
		
		CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName%d.png",card->getRace()))->getCString());
		bg->addChild(countryName, 2);
		countryName->setPosition(ccpAdd(cardBorder->getPosition(), ccp(-cardBorder->getContentSize().width / 2 + 37, cardBorder->getContentSize().height / 2 - 35)));
		
		level = SGCCLabelTTF::create(CCString::createWithFormat("LV%d", card->getCurrLevel())->getCString(), FONT_PANGWA, 24);
		bg->addChild(level, 2);
		level->setPosition(ccpAdd(head->getPosition(), ccp(0, -35)));
		
		name = CCLabelTTF::create(card->getOfficerName()->getCString(), FONT_BOXINFO, 30);
		name->setColor(ccc3(90, 0, 0));
		bg->addChild(name, 1);
		name->setAnchorPoint(ccp(0, 0.5));
		name->setPosition(ccpAdd(head->getPosition(), ccp(head->getContentSize().width, countryName->getPositionY() - name->getContentSize().height / 3)));
		
		bg->addChild(coin, 1);
		coin->setPosition(ccpAdd(name->getPosition(), ccp(coin->getContentSize().width / 2, -name->getContentSize().height * 1.5)));
		
		money->setAnchorPoint(ccp(0, 0.5));
		bg->addChild(money, 1);
		money->setPosition(ccpAdd(coin->getPosition(), ccp(coin->getContentSize().width / 1.5, 0)));
	}
	else if (CCString::create(card->getClassName())->isEqual(CCString::create("SGEquipCard")))
	{
		SGEquipCard *tmp = (SGEquipCard *)card;
		SGMainManager::shareMain()->addHeadIconPlistByNum(tmp->getHeadSuffixNum(),sg_batchselllayer);//12.05
		sprintf(headIcon, "head%d.png", tmp->getHeadSuffixNum());
		
		head = CCSprite::createWithSpriteFrameName(headIcon);
		bg->addChild(head, 0);
		//+1, +2, +3
		if (tmp->getAdNum())
		{
            SGCCLabelTTF *advance = SGCCLabelTTF::create(CCString::createWithFormat("+%d",tmp->getAdNum())->getCString(), FONT_PANGWA, 24);
            advance->setInsideColor(ccGREEN);
			bg->addChild(advance, 1);
			advance->setPosition(ccpAdd(head->getPosition(), ccp(head->getContentSize().width - advance->getContentSize().width / 2,
																 head->getContentSize().height - advance->getContentSize().height / 2)));
		}
		
		SGEquipmentDataModel *temp = SGStaticDataManager::shareStatic()->getEquipById(card->getItemId());
		int starlvl = temp->getEquipCurrStarLevel();
		if (starlvl>1) {
			starlvl-=1;
		}
		sprintf(border, "equipPinzhi_%d_0.png",starlvl);
		cardBorder = CCSprite::createWithSpriteFrameName(border);
		bg->addChild(cardBorder, 1);
		
		CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName1.png"))->getCString());
		countryName->setPosition(ccpAdd(cardBorder->getPosition(), ccp(-cardBorder->getContentSize().width / 2 + 37, cardBorder->getContentSize().height / 2 - 35)));
		
		level = SGCCLabelTTF::create(CCString::createWithFormat("LV%d", card->getCurrLevel())->getCString(), FONT_PANGWA, 24);
		bg->addChild(level, 2);
		level->setPosition(ccpAdd(head->getPosition(), ccp(0, -35)));
		
		name = CCLabelTTF::create(card->getOfficerName()->getCString(), FONT_BOXINFO, 30);
		name->setColor(ccc3(90, 0, 0));
		bg->addChild(name, 1);
		name->setAnchorPoint(ccp(0, 0.5));
		name->setPosition(ccpAdd(head->getPosition(), ccp(head->getContentSize().width, countryName->getPositionY() - name->getContentSize().height / 3)));
		
		bg->addChild(coin, 1);
		coin->setPosition(ccpAdd(name->getPosition(), ccp(coin->getContentSize().width / 2, -name->getContentSize().height * 1.5)));
		
		money->setAnchorPoint(ccp(0, 0.5));
		bg->addChild(money, 1);
		money->setPosition(ccpAdd(coin->getPosition(), ccp(coin->getContentSize().width / 1.5, 0)));
	}
	else if (CCString::create(card->getClassName())->isEqual(CCString::create("SGPropsCard")))
	{
		SGPropsCard *tmp = (SGPropsCard *)card;
		SGMainManager::shareMain()->addHeadIconPlistByNum(tmp->getHeadSuffixNum(),sg_batchselllayer);//12.05
		sprintf(headIcon, "head%d.png", tmp->getHeadSuffixNum());
		
		head = CCSprite::createWithSpriteFrameName(headIcon);
		bg->addChild(head, 0);
		
		
		SGPropsDataModel *temp = SGStaticDataManager::shareStatic()->getPropsById(card->getItemId());
		int starlvl = temp->getPropsCurrStarLevel();
		if (starlvl>1) {
			starlvl-=1;
		}
		sprintf(border, "equipPinzhi_%d.png",starlvl);
		cardBorder = CCSprite::createWithSpriteFrameName(border);
		bg->addChild(cardBorder, 1);
		
		CCSprite *countryName = CCSprite::createWithSpriteFrameName((CCString::createWithFormat("countryName1.png"))->getCString());
		countryName->setPosition(ccpAdd(cardBorder->getPosition(), ccp(-cardBorder->getContentSize().width / 2 + 37, cardBorder->getContentSize().height / 2 - 35)));
		
		
		name = CCLabelTTF::create(card->getOfficerName()->getCString(), FONT_BOXINFO, 30);
		name->setColor(ccc3(90, 0, 0));
		bg->addChild(name, 1);
		name->setAnchorPoint(ccp(0, 0.5));
		name->setPosition(ccpAdd(head->getPosition(), ccp(head->getContentSize().width, countryName->getPositionY() - name->getContentSize().height / 3)));
		
		bg->addChild(coin, 1);
		coin->setPosition(ccpAdd(name->getPosition(), ccp(coin->getContentSize().width / 2, -name->getContentSize().height * 1.5)));
		
		money->setAnchorPoint(ccp(0, 0.5));
		bg->addChild(money, 1);
		money->setPosition(ccpAdd(coin->getPosition(), ccp(coin->getContentSize().width / 1.5, 0)));
	}
	
	this->addChild(bg, 1);
	this->setContentSize(CCSizeMake(head->getContentSize().width * 3, head->getContentSize().height + 5));
	
}

////////////////////////
SGBatchSellBox_x::SGBatchSellBox_x()
{
    selectedCard = CCArray::create();
	selectedCard->retain();
}
SGBatchSellBox_x::~SGBatchSellBox_x()
{
    SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_AUTOBATCHSELL);
	SGNotificationCenter::sharedNotificationCenter()->removeObserver(this, MSG_BATCHSELL);
    ResourceManager::sharedInstance()->unBindLayerTexture(sg_box_batchSell);
}
SGBatchSellBox_x *SGBatchSellBox_x::create(SGBoxDelegate *del, CCArray *array, int total, int twoStar, int  threeStar )
{
    SGBatchSellBox_x *bfb = new SGBatchSellBox_x();
    if (bfb->init(del, box_batchSell, CCSizeMake(600,524)))
    {
		bfb->del = del;
		bfb->selectedCard ->addObjectsFromArray(array);
        bfb->autorelease();
        bfb->initView(total,twoStar,threeStar);
        return bfb;
    }
    CC_SAFE_DELETE(bfb);
    return NULL;

}
void SGBatchSellBox_x::initView(int total,int twoStar , int  threeStar )
{
    SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_AUTOBATCHSELL, this, callfuncO_selector(SGBatchSellBox_x::autoSellConfirmListener));
	SGNotificationCenter::sharedNotificationCenter()->addObserver(MSG_BATCHSELL, this, callfuncO_selector(SGBatchSellBox_x::sellConfirmListener));
	ResourceManager::sharedInstance()->bindTexture("sgannouncementlayer/sgannouncementlayer.plist", RES_TYPE_LAYER_UI, sg_box_batchSell);
	int curr = SGPlayerInfo::sharePlayerInfo()->getPlayerCoins();
	CCPoint center = SGLayout::getPoint(kMiddleCenter);
	fontbg->setPreferredSize(CCSize(480, 282));
	fontbg->setPosition(ccpAdd(fontbg->getPosition(), ccp(0, -20)));
	SGButton *okButton = SGButton::create("gonggao_an.png", "public_font_queding.png", this, menu_selector(SGBatchSellBox_x::sellConfirm),CCPointZero,false,true);
    SGButton *cancelButton = SGButton::create("gonggao_an.png", "public_font_quxiao.png", this, menu_selector(SGBatchSellBox_x::sellCancel),CCPointZero,false,true);
	SGCCLabelTTF *label = SGCCLabelTTF::create(str_CardBatchSellLayer_str7, FONT_PANGWA, 40 , ccc3(230, 0, 0));
    //modify by:zyc. merge into create.
	//label->setColor(ccc3(230, 0, 0));
	CCLabelTTF *content = CCLabelTTF::create(str_CardBatchSellLayer_str21, FONT_BOXINFO, 26);
	content->setColor(ccc3(0, 0, 0));
	CCSprite *coins1 = CCSprite::createWithSpriteFrameName("coinicon.png");
	CCSprite *coins2 = CCSprite::createWithSpriteFrameName("coinicon.png");
	CCLabelTTF *totalCoin = CCLabelTTF::create(str_nowhave, FONT_BOXINFO, 26);
	totalCoin->setColor(ccc3(0, 0, 0));
	CCLabelTTF *curCoins = CCLabelTTF::create(CCString::createWithFormat("%d", curr)->getCString(), FONT_BOXINFO, 26);
	CCLabelTTF *arrow = CCLabelTTF::create("→", FONT_BOXINFO, 23);
	CCLabelTTF *getCoins = CCLabelTTF::create(CCString::createWithFormat("%d", total)->getCString(), FONT_BOXINFO, 26);
	CCLabelTTF *finalCoins = CCLabelTTF::create(CCString::createWithFormat("%d", curr + total)->getCString(), FONT_BOXINFO, 26);
	curCoins->setColor(ccc3(0, 0, 0));
	arrow->setColor(ccc3(0, 0, 0));
	getCoins->setColor(ccc3(0, 0, 0));
	finalCoins->setColor(ccc3(0, 0, 0));
	
	this->addChild(content, 1);
	this->addChild(coins1, 1);
	
	this->addChild(curCoins, 1);
	this->addChild(arrow, 1);
	this->addChild(getCoins, 1);
	this->addChild(finalCoins, 1);
	
    label->setPosition(ccpAdd(biao->getPosition(), ccp(0, -30)));
    
	content->setPosition(ccpAdd(center, ccp(-155, -50)));
	coins1->setPosition(ccpAdd(content->getPosition(), ccp(content->getContentSize().width*1.2 , 0)));
	totalCoin->setPosition(ccpAdd(content->getPosition(), ccp(0, -totalCoin->getContentSize().height * 1.3)));
	coins2->setPosition(ccpAdd(totalCoin->getPosition(), ccp(totalCoin->getContentSize().width + 5, 0)));
	getCoins->setAnchorPoint(ccp(0, 0.5));
	getCoins->setPosition(ccpAdd(coins1->getPosition(), ccp(coins1->getContentSize().width, 0)));
	curCoins->setAnchorPoint(ccp(0, 0.5));
	curCoins->setPosition(ccpAdd(coins2->getPosition(), ccp(coins2->getContentSize().width * 1.2, 0)));
	arrow->setAnchorPoint(ccp(0, 0.5));
	arrow->setPosition(ccpAdd(curCoins->getPosition(), ccp(curCoins->getContentSize().width + 10, 0)));
	finalCoins->setAnchorPoint(ccp(0, 0.5));
	finalCoins->setPosition(ccpAdd(arrow->getPosition(), ccp(arrow->getContentSize().width + 10, 0)));
	
    this->addChild(label);
	this->addBtn(okButton);
	this->addBtn(cancelButton);
	this->addChild(coins2);
	this->addChild(totalCoin);
    
    SGCCLabelTTF *labelinfo = SGCCLabelTTF::create(CCString::createWithFormat(str_CardBatchSellLayer_str22, twoStar,threeStar)->getCString(), FONT_BOXINFO, 32 , CCSize(ccp(420,200)));
    labelinfo->setPosition(ccpAdd(center, ccp(0, 0)));
    this->addChild(labelinfo);
                           
	
	okButton->setPosition(ccpAdd(center, ccp(-145, -324/2+5 -50)));
    cancelButton->setPosition(ccpAdd(center, ccp(145, -324/2+5-50)));
}
void SGBatchSellBox_x::sellConfirm()
{
    CCLOG("Sell Confirm");
	
	/*
	 officerCard(0), // 将领卡片
	 equipmentCard(1), // 装备卡片
	 soldierCard(2), // 士兵卡片
	 propsCard(3),// 道具卡片
	 officerFragmentCard(4)//武将卡碎片
	 */
	//自动卖出
	if (((SGCardBatchSellLayer *)del)->getIsPressAuto())
	{
		if (selectedCard->count() == 0)
		{
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str18);
			return ;
		}
        
        int sellstars = ((SGCardBatchSellLayer *)del)->getAutoSellStars();
        int star4 = sellstars / 100;
        int star3 = (sellstars - star4*100) / 10;
        int star2 = sellstars - star4*100 - star3*10;
        
		int type = ((SGCardBatchSellLayer *)del)->getItemType();//根据layer中的entertype确定卡牌类型
		main::SellRoleItemAllAutoRequest *autoSellRequest = new main::SellRoleItemAllAutoRequest();
		autoSellRequest->set_type(type);
        
        if (star4 > 0)
        {
            autoSellRequest->add_stars(star4);
        }
        if (star3 > 0)
        {
            autoSellRequest->add_stars(star3);
        }
        if (star2 > 0)
        {
            autoSellRequest->add_stars(star2);
        }
        
		SGSocketClient::sharedSocketClient()->send(MSG_AUTOBATCHSELL, autoSellRequest);
	}
	else//手动选择
	{
		if (selectedCard->count() == 0)
		{
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str19);
			return ;
		}
		main::SellRoleItemAllRequest *sellResq = new main::SellRoleItemAllRequest();
		int type = ((SGCardBatchSellLayer *)del)->getItemType();
		
		sellResq->set_type(type);
		for (int i = 0; i < selectedCard->count(); i++)
		{
			int itemId = ((SGBaseMilitaryCard *)selectedCard->objectAtIndex(i))->getSsid();
			sellResq->add_itemid(itemId);
		}
		
		SGSocketClient::sharedSocketClient()->send(MSG_BATCHSELL, sellResq);
	}

}
void SGBatchSellBox_x::sellCancel()
{
    CCLOG("Cancel Comfirm");
	//是否是点击了自动卖出按钮,  取消后要把对应的选择的卡牌取消掉
	if (((SGCardBatchSellLayer *)del)->getIsPressAuto())
	{
		CCLOG("is Press Auto");
		((SGCardBatchSellLayer *)del)->setIsPressAuto(false);
		((SGCardBatchSellLayer *)del)->clearSelectedArray();
	}
	else
	{
		((SGCardBatchSellLayer *)del)->clearSelectedArray();
	}
	this->boxClose();
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}

}
void SGBatchSellBox_x::autoSellConfirmListener(CCObject *sender)
{
    CCLOG("Receive !");
	SGSocketRequest *request = (SGSocketRequest *)sender;
	if (request)
	{
		main::SellRoleItemAllAutoResponse *autoSellResponce = (main::SellRoleItemAllAutoResponse *)request->m_msg;
		if(autoSellResponce->state() == 0)
		{
			CCLOG("Failed?");
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str15);
		}
		else if (autoSellResponce->state() == 1)
		{
			CCLOG("Success!");
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str16);
		}
		else if (autoSellResponce->state() == 2)
		{
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str17);
		}
	}
	
    
	this->sellCancel();
	//((SGCardBatchSellLayer *)del)->backHandler();
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}

}
void SGBatchSellBox_x::sellConfirmListener(CCObject *obj)
{
    SGSocketRequest *request = (SGSocketRequest *)obj;
	if (request)
	{
		main::SellRoleItemAllResponse *sellRes = (main::SellRoleItemAllResponse *)request->m_msg;
		
		if(sellRes->state() == 0)
		{
			CCLOG("Failed?");
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str15);
		}
		else if (sellRes->state() == 1)
		{
			CCLOG("Success");
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str16);
		}
		else if (sellRes->state() == 2)
		{
			SG_SHOW_WINDOW(str_CardBatchSellLayer_str17);
		}
	}
	this->sellCancel();
	//((SGCardBatchSellLayer *)del)->backHandler();
	SGMainScene *mainScene = SGMainManager::shareMain()->getMainScene();
	SGMainLayer *mainLayer = (SGMainLayer *)mainScene->getChildByTag(sg_mainLayer);
	if (mainLayer) {
		mainLayer->setIsCanTouch(true,false);
	}

}











